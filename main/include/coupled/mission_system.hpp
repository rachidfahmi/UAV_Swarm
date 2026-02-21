#ifndef MISSION_SYSTEM_HPP
#define MISSION_SYSTEM_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "../atomics/mission_task_generator.hpp"
#include "../coupled/communication_cluster.hpp"
#include "../atomics/ground_station_aggregator.hpp"

using namespace cadmium;

struct MissionSystem : public Coupled {

    MissionSystem(const std::string& id) : Coupled(id) {

        // ── Components ───────────────────────────────────────────────────────
        auto mtg = addComponent<MissionTaskGenerator>("mission_task_generator");
        auto cc  = addComponent<CommunicationCluster>("comm_cluster");
        auto gsa = addComponent<GroundStationAggregator>("ground_station_aggregator");

        // ── IC (no EIC/EOC — top model has no external ports) ────────────────
        addCoupling(mtg->task_assigned, cc->task_in);    // tasks → cluster
        addCoupling(cc->rx_packet,      gsa->mission_data); // packets → GSA
        addCoupling(gsa->task_completed, mtg->task_completed); // feedback → MTG
    }
};

#endif
