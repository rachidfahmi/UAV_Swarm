#ifndef TEST_MOBILITY_HPP
#define TEST_MOBILITY_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "../atomics/uav_mobility_controller.hpp"

using namespace cadmium;

// No IEStream needed — model has no inputs
struct TestMobility : public Coupled {
    TestMobility(const std::string& id) : Coupled(id) {
        // Start at (10,0), move +10m in x each step
        // Crossings: d1=50 at t=20, d2=100 at t=45
        addComponent<UAVMobilityController>(
            "uav_mobility_controller",
            10.0f, 0.0f,   // start pos
            10.0f, 0.0f    // velocity per step
        );
    }
};

#endif
