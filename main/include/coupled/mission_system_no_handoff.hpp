#ifndef MISSION_SYSTEM_NO_HANDOFF_HPP
#define MISSION_SYSTEM_NO_HANDOFF_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "../atomics/mission_task_generator.hpp"
#include "../coupled/communication_cluster_no_handoff.hpp"
#include "../atomics/ground_station_aggregator.hpp"

using namespace cadmium;

struct MissionSystemNoHandoff : public Coupled {

    MissionSystemNoHandoff(const std::string& id) : Coupled(id) {
        auto mtg = addComponent<MissionTaskGenerator>("mission_task_generator");
        auto cc  = addComponent<CommunicationClusterNoHandoff>("comm_cluster");
        auto gsa = addComponent<GroundStationAggregator>("ground_station_aggregator");

        addCoupling(mtg->task_assigned,  cc->task_in);
        addCoupling(cc->rx_packet,       gsa->mission_data);
        addCoupling(gsa->task_completed, mtg->task_completed);
    }
};

#endif
