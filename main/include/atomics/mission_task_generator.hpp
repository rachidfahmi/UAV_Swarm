#ifndef MISSION_TASK_GENERATOR_HPP
#define MISSION_TASK_GENERATOR_HPP

#include <iostream>
#include <limits>
#include <cadmium/modeling/devs/atomic.hpp>

using namespace cadmium;

struct MissionTaskGeneratorState {
    double sigma;
    explicit MissionTaskGeneratorState() : sigma(60.0) {}
};

inline std::ostream& operator<<(std::ostream& out, const MissionTaskGeneratorState& s) {
    out << "sigma:" << s.sigma;
    return out;
}

class MissionTaskGenerator : public Atomic<MissionTaskGeneratorState> {
public:
    Port<int> task_completed;
    Port<int> task_assigned;

    MissionTaskGenerator(const std::string& id)
        : Atomic<MissionTaskGeneratorState>(id, MissionTaskGeneratorState()) {
        task_completed = addInPort<int>("task_completed");
        task_assigned  = addOutPort<int>("task_assigned");
    }

    // external: input arrives — ignore it, just keep sigma ticking
    void externalTransition(MissionTaskGeneratorState& s, double e) const override {
        s.sigma -= e;
    }

    // internal: timer fired — emit task_assigned, reset timer
    void internalTransition(MissionTaskGeneratorState& s) const override {
        s.sigma = 60.0;
    }

    void output(const MissionTaskGeneratorState& s) const override {
        task_assigned->addMessage(1);
    }

    [[nodiscard]] double timeAdvance(const MissionTaskGeneratorState& s) const override {
        return s.sigma;
    }
};

#endif
