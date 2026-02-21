#ifndef TEST_COMM_CLUSTER_HPP
#define TEST_COMM_CLUSTER_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../coupled/communication_cluster.hpp"

using namespace cadmium;

struct TestCommCluster : public Coupled {
    Port<int> rx_out;

    TestCommCluster(const std::string& id) : Coupled(id) {
        rx_out = addOutPort<int>("rx_out");

        auto cluster    = addComponent<CommunicationCluster>("comm_cluster");
        auto task_input = addComponent<lib::IEStream<int>>(
            "task_input", "./inputs/test_l3_task.txt");

        addCoupling(task_input->out, cluster->task_in);
        addCoupling(cluster->rx_packet, rx_out);
    }
};

#endif
