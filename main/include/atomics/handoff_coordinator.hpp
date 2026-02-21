#ifndef HANDOFF_COORDINATOR_HPP
#define HANDOFF_COORDINATOR_HPP

#include <iostream>
#include <limits>
#include <cadmium/modeling/devs/atomic.hpp>
#include "../messages/link_state_msg.hpp"
#include "../messages/handoff_msg.hpp"

using namespace cadmium;

struct HandoffCoordinatorState {
    double sigma;
    bool   issuing;
    int    pending_uav_id;    // ← which UAV needs the handoff

    explicit HandoffCoordinatorState()
        : sigma(std::numeric_limits<double>::infinity()),
          issuing(false),
          pending_uav_id(-1) {}
};

inline std::ostream& operator<<(std::ostream& out, const HandoffCoordinatorState& s) {
    out << "issuing:" << (s.issuing ? "true" : "false")
        << " pending_uav:" << s.pending_uav_id
        << " sigma:" << s.sigma;
    return out;
}

class HandoffCoordinator : public Atomic<HandoffCoordinatorState> {
public:
    Port<LinkStateMsg> link_state_change;   // ← was Port<LinkState>
    Port<HandoffMsg>   handoff_in;

    HandoffCoordinator(const std::string& id)
        : Atomic<HandoffCoordinatorState>(id, HandoffCoordinatorState()) {
        link_state_change = addInPort<LinkStateMsg>("link_state_change");
        handoff_in        = addOutPort<HandoffMsg>("handoff_in");
    }

    void externalTransition(HandoffCoordinatorState& s, double e) const override {
        s.sigma -= e;
        if (!link_state_change->empty()) {
            auto msg = link_state_change->getBag().back();
            if (msg.state == LinkState::Disconnected) {
                s.issuing       = true;
                s.pending_uav_id = msg.uav_id;   // ← store which UAV
                s.sigma         = 1.0;
            }
            // Available or Degraded → stay monitoring
        }
    }

    void internalTransition(HandoffCoordinatorState& s) const override {
        s.issuing        = false;
        s.pending_uav_id = -1;
        s.sigma          = std::numeric_limits<double>::infinity();
    }

    void output(const HandoffCoordinatorState& s) const override {
        handoff_in->addMessage(HandoffMsg(s.pending_uav_id));  // ← correct UAV
    }

    [[nodiscard]] double timeAdvance(const HandoffCoordinatorState& s) const override {
        return s.sigma;
    }
};

#endif
