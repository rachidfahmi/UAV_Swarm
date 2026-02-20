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

    explicit HandoffCoordinatorState()
        : sigma(std::numeric_limits<double>::infinity()),
          issuing(false) {}
};

inline std::ostream& operator<<(std::ostream& out, const HandoffCoordinatorState& s) {
    out << "issuing:" << (s.issuing ? "true" : "false")
        << " sigma:" << s.sigma;
    return out;
}

class HandoffCoordinator : public Atomic<HandoffCoordinatorState> {
public:
    Port<LinkState>   link_state_change;
    Port<HandoffMsg>  handoff_in;

    HandoffCoordinator(const std::string& id)
        : Atomic<HandoffCoordinatorState>(id, HandoffCoordinatorState()) {
        link_state_change = addInPort<LinkState>("link_state_change");
        handoff_in        = addOutPort<HandoffMsg>("handoff_in");
    }

    void externalTransition(HandoffCoordinatorState& s, double e) const override {
        s.sigma -= e;
        if (!link_state_change->empty()) {
            auto state = link_state_change->getBag().back();
            if (state == LinkState::Disconnected) {
                s.issuing = true;
                s.sigma   = 1.0;   // T_handoff = 1s
            }
            // Available or Degraded → stay monitoring, no change
        }
    }

    void internalTransition(HandoffCoordinatorState& s) const override {
        // handoff issued → back to monitoring
        s.issuing = false;
        s.sigma   = std::numeric_limits<double>::infinity();
    }

    void output(const HandoffCoordinatorState& s) const override {
        handoff_in->addMessage(HandoffMsg(0));  // target_uav_id=0 in atomic test
    }

    [[nodiscard]] double timeAdvance(const HandoffCoordinatorState& s) const override {
        return s.sigma;
    }
};

#endif
