#ifndef TEST_UAV_COMM_HPP
#define TEST_UAV_COMM_HPP

#include <limits>                                        // ← add this
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../atomics/uav_comm_node.hpp"

using namespace cadmium;

struct TestUAVComm : public Coupled {
    TestUAVComm(const std::string& id) : Coupled(id) {
        auto cn         = addComponent<UAVCommNode>("uav_comm_node");
        auto pkt_input  = addComponent<lib::IEStream<int>>(
            "packet_input",
            "./inputs/test_uav_comm_packets.txt"
        );
        auto hoff_input = addComponent<lib::IEStream<HandoffMsg>>(
            "handoff_input",
            "./inputs/test_uav_comm_handoff.txt"
        );
        addCoupling(pkt_input->out,  cn->data_packet);
        addCoupling(hoff_input->out, cn->handoff_in);
    }
};

#endif
