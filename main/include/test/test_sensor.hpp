#ifndef TEST_SENSOR_HPP
#define TEST_SENSOR_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../atomics/uav_sensor_module.hpp"

using namespace cadmium;

struct TestSensor : public Coupled {
    TestSensor(const std::string& id) : Coupled(id) {
        auto sensor = addComponent<UAVSensorModule>("uav_sensor_module");
        auto input  = addComponent<lib::IEStream<int>>(
            "task_in_input",
            "./inputs/test_sensor_input.txt"
        );
        addCoupling(input->out, sensor->task_in);
    }
};

#endif
