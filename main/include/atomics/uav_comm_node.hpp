#ifndef UAV_COMM_NODE_HPP
#define UAV_COMM_NODE_HPP

#include <iostream>
#include <limits>
#include <cadmium/modeling/devs/atomic.hpp>
#include "../messages/handoff_msg.hpp"

using namespace cadmium;

enum class CommMode { Direct, Relay };

struct UAVCommNodeState {
    double   sigma;
    bool     transmitting;
    CommMode mode;
    int      relay_target_id;

    explicit UAVCommNodeState()
        : sigma(std::numeric_limits<double>::infinity()),
          transmitting(false),
          mode(CommMode::Direct),
          relay_target_id(-1) {}
};

inline std::ostream& operator<<(std::ostream& out, const UAVCommNodeState& s) {
    out << "transmitting:" << (s.transmitting ? "true" : "false")
        << " mode:" << (s.mode == CommMode::Direct ? "direct" : "relay")
        << " relay_target:" << s.relay_target_id
        << " sigma:" << s.sigma;
    return out;
}

class UAVCommNode : public Atomic<UAVCommNodeState> {
public:
    Port<int>        data_packet;
    Port<HandoffMsg> handoff_in;
    Port<int>        tx_out;

    UAVCommNode(const std::string& id)
        : Atomic<UAVCommNodeState>(id, UAVCommNodeState()) {
        data_packet = addInPort<int>("data_packet");
        handoff_in  = addInPort<HandoffMsg>("handoff_in");
        tx_out      = addOutPort<int>("tx_out");
    }

    void externalTransition(UAVCommNodeState& s, double e) const override {
        s.sigma -= e;

        // handoff_in always updates mode, never interrupts transmission
        if (!handoff_in->empty()) {
            auto msg = handoff_in->getBag().back();
            s.mode            = CommMode::Relay;
            s.relay_target_id = msg.target_uav_id;
        }

        // data_packet: start transmitting only if idle; drop if busy
        if (!data_packet->empty()) {
            if (!s.transmitting) {
                s.transmitting = true;
                s.sigma        = 1.0;   // T_tx = 1s
            }
            // else: drop silently
        }
    }

    void internalTransition(UAVCommNodeState& s) const override {
        s.transmitting = false;
        s.sigma        = std::numeric_limits<double>::infinity();
    }

    void output(const UAVCommNodeState& s) const override {
        // send 1 if direct, 2 if relay (encodes destination)
        tx_out->addMessage(s.mode == CommMode::Direct ? 1 : 2);
    }

    [[nodiscard]] double timeAdvance(const UAVCommNodeState& s) const override {
        return s.sigma;
    }
};

#endif
