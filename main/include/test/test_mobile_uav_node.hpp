#ifndef TEST_MOBILE_UAV_NODE_HPP
#define TEST_MOBILE_UAV_NODE_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../coupled/mobile_uav_node.hpp"

using namespace cadmium;

struct TestMobileUAVNode : public Coupled {
    // Expose coupled outputs so the logger confirms boundary crossings
    Port<int>          tx_out;
    Port<PosUpdateMsg> pos_out;

    TestMobileUAVNode(const std::string& id) : Coupled(id) {
        tx_out  = addOutPort<int>("tx_out");
        pos_out = addOutPort<PosUpdateMsg>("pos_out");

        // UAV0: starts at (0,0), moves +15 in x per tick → crosses d1=100 at t=35
        auto uav = addComponent<MobileUAVNode>(
                       "mobile_uav_node", 0, 0.0, 0.0, 15.0, 0.0);

        auto task_input = addComponent<lib::IEStream<int>>(
            "task_input", "./inputs/test_l2_task.txt");
        auto hoff_input = addComponent<lib::IEStream<HandoffMsg>>(
            "handoff_input", "./inputs/test_l2_handoff.txt");

        addCoupling(task_input->out, uav->task_in);
        addCoupling(hoff_input->out, uav->handoff_in);

        // EOC — expose outputs at test boundary
        addCoupling(uav->tx_out,     tx_out);
        addCoupling(uav->pos_update, pos_out);
    }
};

#endif
