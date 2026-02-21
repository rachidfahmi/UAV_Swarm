#ifndef COMMUNICATION_CLUSTER_HPP
#define COMMUNICATION_CLUSTER_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "../coupled/mobile_uav_node.hpp"
#include "../atomics/network_link_model.hpp"
#include "../atomics/handoff_coordinator.hpp"

using namespace cadmium;

struct CommunicationCluster : public Coupled {
    Port<int> task_in;
    Port<int> rx_packet;

    CommunicationCluster(const std::string& id) : Coupled(id) {

        task_in   = addInPort<int>("task_in");
        rx_packet = addOutPort<int>("rx_packet");

        // ── Components ───────────────────────────────────────────────────────
        // UAV0: starts (30,0) vx=10 — will cross d1=50 at t=20, d2=100 at t=45
        // UAV1: static (30,0)       — always Available
        // UAV2: static (40,0)       — always Available
        auto uav0 = addComponent<MobileUAVNode>("uav0", 0, 30.0, 0.0, 10.0, 0.0);
        auto uav1 = addComponent<MobileUAVNode>("uav1", 1, 30.0, 0.0,  0.0, 0.0);
        auto uav2 = addComponent<MobileUAVNode>("uav2", 2, 40.0, 0.0,  0.0, 0.0);

        // NL0: tighter thresholds d1=50, d2=100 so UAV0 disconnects early
        auto nl0  = addComponent<NetworkLinkModel>("nl0", 0,  50.0, 100.0);
        auto nl1  = addComponent<NetworkLinkModel>("nl1", 1, 100.0, 200.0);
        auto nl2  = addComponent<NetworkLinkModel>("nl2", 2, 100.0, 200.0);

        auto hc   = addComponent<HandoffCoordinator>("handoff_coordinator");

        // ── EIC ──────────────────────────────────────────────────────────────
        addCoupling(task_in, uav0->task_in);
        addCoupling(task_in, uav1->task_in);
        addCoupling(task_in, uav2->task_in);

        // ── IC ───────────────────────────────────────────────────────────────
        // tx_out → network links
        addCoupling(uav0->tx_out, nl0->tx_in);
        addCoupling(uav1->tx_out, nl1->tx_in);
        addCoupling(uav2->tx_out, nl2->tx_in);

        // pos_update → network links
        addCoupling(uav0->pos_update, nl0->pos_update);
        addCoupling(uav1->pos_update, nl1->pos_update);
        addCoupling(uav2->pos_update, nl2->pos_update);

        // link_state_change → handoff coordinator
        addCoupling(nl0->link_state_change, hc->link_state_change);
        addCoupling(nl1->link_state_change, hc->link_state_change);
        addCoupling(nl2->link_state_change, hc->link_state_change);

        // handoff broadcasts to all UAVs (each checks affected_uav_id)
        addCoupling(hc->handoff_in, uav0->handoff_in);
        addCoupling(hc->handoff_in, uav1->handoff_in);
        addCoupling(hc->handoff_in, uav2->handoff_in);

        // ── EOC ──────────────────────────────────────────────────────────────
        addCoupling(nl0->rx_packet, rx_packet);
        addCoupling(nl1->rx_packet, rx_packet);
        addCoupling(nl2->rx_packet, rx_packet);
    }
};

#endif
