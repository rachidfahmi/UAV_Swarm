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
    double vx, vy;   // velocity — position steps per tick

    explicit UAVMobilityControllerState(
        double x=0, double y=0, double vx=0, double vy=0)
        : sigma(5.0), x(x), y(y), vx(vx), vy(vy) {}
};

inline std::ostream& operator<<(std::ostream& out,
                                const UAVMobilityControllerState& s) {
    out << "pos:(" << s.x << "," << s.y << ") sigma:" << s.sigma;
    return out;
}

class UAVMobilityController : public Atomic<UAVMobilityControllerState> {
public:
    Port<PosUpdateMsg> pos_update;

    UAVMobilityController(const std::string& id,
                          double x=0, double y=0,
                          double vx=0, double vy=0)
        : Atomic<UAVMobilityControllerState>(
            id, UAVMobilityControllerState(x, y, vx, vy)) {
        pos_update = addOutPort<PosUpdateMsg>("pos_update");
    }

    void externalTransition(UAVMobilityControllerState& s, double e) const override {}

    void internalTransition(UAVMobilityControllerState& s) const override {
        s.x    += s.vx;   // step position each tick
        s.y    += s.vy;
        s.sigma = 5.0;
    }

    void output(const UAVMobilityControllerState& s) const override {
        pos_update->addMessage(PosUpdateMsg(s.x, s.y));
    }

    [[nodiscard]] double timeAdvance(
        const UAVMobilityControllerState& s) const override {
        return s.sigma;
    }
};

#endif
