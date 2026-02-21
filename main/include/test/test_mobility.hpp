#ifndef TEST_MOBILITY_HPP
#define TEST_MOBILITY_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include "../atomics/uav_mobility_controller.hpp"

using namespace cadmium;

struct TestMobility : public Coupled {
    TestMobility(const std::string& id) : Coupled(id) {
        // (id, x, y) — 3 args only
        addComponent<UAVMobilityController>("uav_mobility_controller", 60.0, 0.0);
    }
};

#endif
