#ifndef UAV_MOBILITY_CONTROLLER_HPP
#define UAV_MOBILITY_CONTROLLER_HPP

#include <iostream>
#include <cmath>
#include <cadmium/modeling/devs/atomic.hpp>
#include "../messages/pos_update_msg.hpp"

using namespace cadmium;

struct UAVMobilityControllerState {
    double sigma;
    float  x;
    float  y;
    float  vx;   // velocity in x (m per step)
    float  vy;   // velocity in y (m per step)

    explicit UAVMobilityControllerState(
        float x0 = 10.0f, float y0 = 0.0f,
        float vx = 10.0f, float vy = 0.0f
    ) : sigma(5.0), x(x0), y(y0), vx(vx), vy(vy) {}
};

inline std::ostream& operator<<(std::ostream& out, const UAVMobilityControllerState& s) {
    out << "pos:(" << s.x << "," << s.y << ") sigma:" << s.sigma;
    return out;
}

class UAVMobilityController : public Atomic<UAVMobilityControllerState> {
public:
    Port<PosUpdate> pos_update;

    // x0, y0 = start position; vx, vy = displacement per 5s step
    UAVMobilityController(const std::string& id,
                          float x0 = 10.0f, float y0 = 0.0f,
                          float vx = 10.0f, float vy = 0.0f)
        : Atomic<UAVMobilityControllerState>(
            id, UAVMobilityControllerState(x0, y0, vx, vy)) {
        pos_update = addOutPort<PosUpdate>("pos_update");
    }

    // No inputs
    void externalTransition(UAVMobilityControllerState& s, double e) const override {
        s.sigma -= e;
    }

    // Advance position every 5s, emit update, repeat
    void internalTransition(UAVMobilityControllerState& s) const override {
        s.x    += s.vx;
        s.y    += s.vy;
        s.sigma = 5.0;
    }

    void output(const UAVMobilityControllerState& s) const override {
        pos_update->addMessage(PosUpdate(s.x, s.y));
    }

    [[nodiscard]] double timeAdvance(const UAVMobilityControllerState& s) const override {
        return s.sigma;
    }
};

#endif
