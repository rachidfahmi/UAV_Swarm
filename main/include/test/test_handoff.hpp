#ifndef TEST_HANDOFF_HPP
#define TEST_HANDOFF_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../atomics/handoff_coordinator.hpp"

using namespace cadmium;

struct TestHandoff : public Coupled {
    TestHandoff(const std::string& id) : Coupled(id) {
        auto hc    = addComponent<HandoffCoordinator>("handoff_coordinator");
        auto input = addComponent<lib::IEStream<LinkStateMsg>>(
            "link_state_input",
            "./inputs/test_handoff_input.txt"
        );
        addCoupling(input->out, hc->link_state_change);
    }
};

#endif
