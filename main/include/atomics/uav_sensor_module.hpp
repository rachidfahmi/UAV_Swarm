#ifndef UAV_SENSOR_MODULE_HPP
#define UAV_SENSOR_MODULE_HPP

#include <iostream>
#include <limits>
#include <cadmium/modeling/devs/atomic.hpp>

using namespace cadmium;

struct UAVSensorModuleState {
    double sigma;
    bool   sensing;

    explicit UAVSensorModuleState()
        : sigma(std::numeric_limits<double>::infinity()), sensing(false) {}
};

inline std::ostream& operator<<(std::ostream& out, const UAVSensorModuleState& s) {
    out << "sensing:" << (s.sensing ? "true" : "false") << " sigma:" << s.sigma;
    return out;
}

class UAVSensorModule : public Atomic<UAVSensorModuleState> {
public:
    Port<int> task_in;
    Port<int> data_packet;

    UAVSensorModule(const std::string& id)
        : Atomic<UAVSensorModuleState>(id, UAVSensorModuleState()) {
        task_in     = addInPort<int>("task_in");
        data_packet = addOutPort<int>("data_packet");
    }

    void externalTransition(UAVSensorModuleState& s, double e) const override {
        if (s.sensing) {
            // mid-sensing: ignore all inputs, just tick sigma down
            s.sigma -= e;
            return;
        }
        // idle: accept task_in and start sensing
        if (!task_in->empty()) {
            s.sensing = true;
            s.sigma   = 5.0;
        }
    }

    void internalTransition(UAVSensorModuleState& s) const override {
        // sensing complete → back to idle
        s.sensing = false;
        s.sigma   = std::numeric_limits<double>::infinity();
    }

    void output(const UAVSensorModuleState& s) const override {
        data_packet->addMessage(1);
    }

    [[nodiscard]] double timeAdvance(const UAVSensorModuleState& s) const override {
        return s.sigma;
    }
};

#endif
