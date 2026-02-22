#ifndef COMMUNICATION_CLUSTER_NO_HANDOFF_HPP
#define COMMUNICATION_CLUSTER_NO_HANDOFF_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "../coupled/mobile_uav_node.hpp"
#include "../atomics/network_link_model.hpp"

using namespace cadmium;

// Exp 2: No HandoffCoordinator — UAV0 disconnects, stays disconnected
struct CommunicationClusterNoHandoff : public Coupled {
    Port<int> task_in;
    Port<int> rx_packet;

    CommunicationClusterNoHandoff(const std::string& id) : Coupled(id) {
        task_in   = addInPort<int>("task_in");
        rx_packet = addOutPort<int>("rx_packet");

        auto uav0 = addComponent<MobileUAVNode>("uav0", 0, 30.0, 0.0, 10.0, 0.0);
        auto uav1 = addComponent<MobileUAVNode>("uav1", 1, 30.0, 0.0,  0.0, 0.0);
        auto uav2 = addComponent<MobileUAVNode>("uav2", 2, 40.0, 0.0,  0.0, 0.0);

        auto nl0  = addComponent<NetworkLinkModel>("nl0", 0,  50.0, 100.0);
        auto nl1  = addComponent<NetworkLinkModel>("nl1", 1, 100.0, 200.0);
        auto nl2  = addComponent<NetworkLinkModel>("nl2", 2, 100.0, 200.0);
        // No HandoffCoordinator

        addCoupling(task_in, uav0->task_in);
        addCoupling(task_in, uav1->task_in);
        addCoupling(task_in, uav2->task_in);

        addCoupling(uav0->tx_out,     nl0->tx_in);
        addCoupling(uav1->tx_out,     nl1->tx_in);
        addCoupling(uav2->tx_out,     nl2->tx_in);
        addCoupling(uav0->pos_update, nl0->pos_update);
        addCoupling(uav1->pos_update, nl1->pos_update);
        addCoupling(uav2->pos_update, nl2->pos_update);
        // No HC couplings — link_state_change events go nowhere

        addCoupling(nl0->rx_packet, rx_packet);
        addCoupling(nl1->rx_packet, rx_packet);
        addCoupling(nl2->rx_packet, rx_packet);
    }
};

#endif
