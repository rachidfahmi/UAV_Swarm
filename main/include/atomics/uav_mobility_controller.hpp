#ifndef UAV_MOBILITY_CONTROLLER_HPP
#define UAV_MOBILITY_CONTROLLER_HPP

#include <iostream>
#include <limits>
#include <cadmium/modeling/devs/atomic.hpp>
#include "../messages/pos_update_msg.hpp"

using namespace cadmium;

struct UAVMobilityControllerState {
    double sigma;
    double x, y;

    explicit UAVMobilityControllerState(double x = 0.0, double y = 0.0)
        : sigma(0.0), x(x), y(y) {}
};

inline std::ostream& operator<<(std::ostream& out, const UAVMobilityControllerState& s) {
    out << "pos:(" << s.x << "," << s.y << ") sigma:" << s.sigma;
    return out;
}

class UAVMobilityController : public Atomic<UAVMobilityControllerState> {
public:
    Port<PosUpdateMsg> pos_update;   // ← was Port<PosUpdate>

    UAVMobilityController(const std::string& id, double x = 0.0, double y = 0.0)
        : Atomic<UAVMobilityControllerState>(id, UAVMobilityControllerState(x, y)) {
        pos_update = addOutPort<PosUpdateMsg>("pos_update");
    }

    void externalTransition(UAVMobilityControllerState& s, double e) const override {}

    void internalTransition(UAVMobilityControllerState& s) const override {
        s.sigma = 5.0;
    }

    void output(const UAVMobilityControllerState& s) const override {
        pos_update->addMessage(PosUpdateMsg(s.x, s.y));   // ← was PosUpdate
    }

    [[nodiscard]] double timeAdvance(const UAVMobilityControllerState& s) const override {
        return s.sigma;
    }
};

#endif
