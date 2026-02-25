#ifndef TEST_MTG_HPP
#define TEST_MTG_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>

#include "../atomics/mission_task_generator.hpp"

struct TestMTG : public cadmium::Coupled {
    TestMTG(const std::string& id) : cadmium::Coupled(id) {

        auto mtg = addComponent<MissionTaskGenerator>("mission_task_generator");

        auto input = addComponent<cadmium::lib::IEStream<int>>(
            "task_completed_input",
            "./inputs/test_mtg_input.txt"
        );

        addCoupling(input->out, mtg->task_completed);
    }
};

#endif