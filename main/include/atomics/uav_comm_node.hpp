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
    int      pending_packet;

    explicit UAVCommNodeState()
        : sigma(std::numeric_limits<double>::infinity()),
          transmitting(false),
          mode(CommMode::Direct),
          relay_target_id(-1),
          pending_packet(0) {}
};

inline std::ostream& operator<<(std::ostream& out,
                                const UAVCommNodeState& s) {
    out << "transmitting:" << (s.transmitting ? "true" : "false")
        << " mode:" << (s.mode == CommMode::Direct ? "direct" : "relay")
        << " relay_target:" << s.relay_target_id
        << " pending:" << s.pending_packet
        << " sigma:" << s.sigma;
    return out;
}

class UAVCommNode : public Atomic<UAVCommNodeState> {
    int uav_id_;   // own identity — only accept handoffs addressed to us
public:
    Port<int>        data_packet;
    Port<HandoffMsg> handoff_in;
    Port<int>        tx_out;

    UAVCommNode(const std::string& id, int uav_id = 0)
        : Atomic<UAVCommNodeState>(id, UAVCommNodeState()),
          uav_id_(uav_id) {
        data_packet = addInPort<int>("data_packet");
        handoff_in  = addInPort<HandoffMsg>("handoff_in");
        tx_out      = addOutPort<int>("tx_out");
    }

    void externalTransition(UAVCommNodeState& s, double e) const override {
        s.sigma -= e;

        // Only apply handoff if it's addressed to this UAV
        if (!handoff_in->empty()) {
            auto msg = handoff_in->getBag().back();
            if (msg.affected_uav_id == uav_id_) {
                s.mode            = CommMode::Relay;
                s.relay_target_id = msg.relay_target_id;
            }
        }

        // data_packet: start if idle, drop if busy
        if (!data_packet->empty()) {
            if (!s.transmitting) {
                s.pending_packet = data_packet->getBag().back();
                s.transmitting   = true;
                s.sigma          = 1.0;
            }
        }
    }

    void internalTransition(UAVCommNodeState& s) const override {
        s.transmitting = false;
        s.sigma        = std::numeric_limits<double>::infinity();
    }

    void output(const UAVCommNodeState& s) const override {
        tx_out->addMessage(s.pending_packet);
    }

    [[nodiscard]] double timeAdvance(
        const UAVCommNodeState& s) const override {
        return s.sigma;
    }
};

#endif
