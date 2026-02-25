#ifndef TEST_MTG_HPP
#define TEST_MTG_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../atomics/mission_task_generator.hpp"

using namespace cadmium;

struct TestMTG : public Coupled {
    TestMTG(const std::string& id) : Coupled(id) {

        auto mtg = addComponent<MissionTaskGenerator>("mission_task_generator");

        // Feed task_completed signals at t=45 and t=90
        // to confirm they are ignored and don't shift the schedule
        auto input = addComponent<lib::IEStream<int>>(
            "task_completed_input",
            "./inputs/test_mtg_input.txt"
        );

        addCoupling(input->out, mtg->task_completed);
    }
};

#endif
