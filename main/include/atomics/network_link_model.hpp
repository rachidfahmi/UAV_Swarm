#ifndef NETWORK_LINK_MODEL_HPP
#define NETWORK_LINK_MODEL_HPP

#include <iostream>
#include <limits>
#include <cadmium/modeling/devs/atomic.hpp>
#include "../messages/link_state_msg.hpp"
#include "../messages/pos_update_msg.hpp"

using namespace cadmium;

enum class NLPhase { Available, Degraded, Disconnected, Forwarding, Notify };

struct NetworkLinkModelState {
    double    sigma;
    NLPhase   phase;
    LinkState stable_state;   // last stable link state (return target for Forwarding)
    LinkState new_state;      // target state for Notify transition
    double    d1, d2;
    int       pending_packet;

    explicit NetworkLinkModelState(double d1 = 100.0, double d2 = 200.0)
        : sigma(std::numeric_limits<double>::infinity()),
          phase(NLPhase::Available),
          stable_state(LinkState::Available),
          new_state(LinkState::Available),
          d1(d1), d2(d2),
          pending_packet(0) {}
};

inline std::ostream& operator<<(std::ostream& out, const NetworkLinkModelState& s) {
    std::string ph;
    switch (s.phase) {
        case NLPhase::Available:    ph = "Available";    break;
        case NLPhase::Degraded:     ph = "Degraded";     break;
        case NLPhase::Disconnected: ph = "Disconnected"; break;
        case NLPhase::Forwarding:   ph = "Forwarding";   break;
        case NLPhase::Notify:       ph = "Notify";       break;
    }
    out << "phase:" << ph
        << " stable:" << s.stable_state
        << " sigma:" << s.sigma;
    return out;
}

class NetworkLinkModel : public Atomic<NetworkLinkModelState> {
public:
    Port<int>          tx_in;
    Port<PosUpdateMsg> pos_update;
    Port<int>          rx_packet;
    Port<LinkState>    link_state_change;

    NetworkLinkModel(const std::string& id, double d1 = 100.0, double d2 = 200.0)
        : Atomic<NetworkLinkModelState>(id, NetworkLinkModelState(d1, d2)) {
        tx_in             = addInPort<int>("tx_in");
        pos_update        = addInPort<PosUpdateMsg>("pos_update");
        rx_packet         = addOutPort<int>("rx_packet");
        link_state_change = addOutPort<LinkState>("link_state_change");
    }

    void externalTransition(NetworkLinkModelState& s, double e) const override {
        s.sigma -= e;

        // Transient states: ignore all external inputs
        if (s.phase == NLPhase::Forwarding || s.phase == NLPhase::Notify)
            return;

        // tx_in: forward if not Disconnected, drop silently if Disconnected
        if (!tx_in->empty()) {
            if (s.phase != NLPhase::Disconnected) {
                s.pending_packet = tx_in->getBag().back();
                s.phase          = NLPhase::Forwarding;
                s.sigma          = 0.0;
            }
            // Disconnected → drop, no state change
            return;
        }

        // pos_update: check thresholds, emit Notify if crossed
        if (!pos_update->empty()) {
            double d = pos_update->getBag().back().distance();

            if (s.phase == NLPhase::Available && d > s.d1) {
                s.new_state = LinkState::Degraded;
                s.phase     = NLPhase::Notify;
                s.sigma     = 0.0;
            } else if (s.phase == NLPhase::Degraded && d > s.d2) {
                s.new_state = LinkState::Disconnected;
                s.phase     = NLPhase::Notify;
                s.sigma     = 0.0;
            } else if (s.phase == NLPhase::Disconnected && d < s.d1) {
                s.new_state = LinkState::Available;
                s.phase     = NLPhase::Notify;
                s.sigma     = 0.0;
            }
            // else: no threshold crossed → stay, no output
        }
    }

    void internalTransition(NetworkLinkModelState& s) const override {
        if (s.phase == NLPhase::Forwarding) {
            // Return to the stable state we came from
            s.phase = phaseFrom(s.stable_state);
            s.sigma = std::numeric_limits<double>::infinity();
        } else if (s.phase == NLPhase::Notify) {
            // Commit transition to new stable state
            s.stable_state = s.new_state;
            s.phase        = phaseFrom(s.new_state);
            s.sigma        = std::numeric_limits<double>::infinity();
        }
    }

    void output(const NetworkLinkModelState& s) const override {
        if (s.phase == NLPhase::Forwarding)
            rx_packet->addMessage(s.pending_packet);
        else if (s.phase == NLPhase::Notify)
            link_state_change->addMessage(s.new_state);
    }

    [[nodiscard]] double timeAdvance(const NetworkLinkModelState& s) const override {
        return s.sigma;
    }

private:
    static NLPhase phaseFrom(LinkState ls) {
        switch (ls) {
            case LinkState::Available:    return NLPhase::Available;
            case LinkState::Degraded:     return NLPhase::Degraded;
            case LinkState::Disconnected: return NLPhase::Disconnected;
        }
        return NLPhase::Available;
    }
};

#endif
