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
    int    affected_uav_id;
    int    relay_target_id;

    explicit HandoffCoordinatorState()
        : sigma(std::numeric_limits<double>::infinity()),
          issuing(false),
          affected_uav_id(-1),
          relay_target_id(-1) {}
};

inline std::ostream& operator<<(std::ostream& out,
                                const HandoffCoordinatorState& s) {
    out << "issuing:" << (s.issuing ? "true" : "false")
        << " affected:" << s.affected_uav_id
        << " relay:" << s.relay_target_id
        << " sigma:" << s.sigma;
    return out;
}

class HandoffCoordinator : public Atomic<HandoffCoordinatorState> {
public:
    Port<LinkStateMsg> link_state_change;
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
                s.affected_uav_id = msg.uav_id;
                s.relay_target_id = (msg.uav_id + 1) % 3;  // next UAV as relay
                s.issuing         = true;
                s.sigma           = 1.0;
            }
        }
    }

    void internalTransition(HandoffCoordinatorState& s) const override {
        s.issuing         = false;
        s.affected_uav_id = -1;
        s.relay_target_id = -1;
        s.sigma           = std::numeric_limits<double>::infinity();
    }

    void output(const HandoffCoordinatorState& s) const override {
        handoff_in->addMessage(
            HandoffMsg(s.affected_uav_id, s.relay_target_id));
    }

    [[nodiscard]] double timeAdvance(
        const HandoffCoordinatorState& s) const override {
        return s.sigma;
    }
};

#endif
