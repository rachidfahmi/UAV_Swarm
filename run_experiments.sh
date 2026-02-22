#!/bin/bash

echo "=== Building ==="
source build_sim.sh

echo ""
echo "=== Experiment 1: Atomic Model Tests ==="

# 1a - MissionTaskGenerator
cat > main/main.cpp << 'MAINCPP'
#include "include/test/test_mission_task_generator.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestMissionTaskGenerator>("test_mtg");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(200.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp1a_mission_task_generator.log
echo "1a done -> logs/exp1a_mission_task_generator.log"

# 1b - UAVSensorModule
cat > main/main.cpp << 'MAINCPP'
#include "include/test/test_sensor_module.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestSensorModule>("test_sensor");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(30.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp1b_uav_sensor_module.log
echo "1b done -> logs/exp1b_uav_sensor_module.log"

# 1c - NetworkLinkModel
cat > main/main.cpp << 'MAINCPP'
#include "include/test/test_network_link.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestNetworkLink>("test_nl");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(50.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp1c_network_link_model.log
echo "1c done -> logs/exp1c_network_link_model.log"

# 1d - GroundStationAggregator
cat > main/main.cpp << 'MAINCPP'
#include "include/test/test_ground_station.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestGroundStation>("test_gsa");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(30.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp1d_ground_station_aggregator.log
echo "1d done -> logs/exp1d_ground_station_aggregator.log"

# 1e - HandoffCoordinator
cat > main/main.cpp << 'MAINCPP'
#include "include/test/test_handoff_coordinator.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestHandoffCoordinator>("test_hc");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(30.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp1e_handoff_coordinator.log
echo "1e done -> logs/exp1e_handoff_coordinator.log"

# 1f - UAVCommNode
cat > main/main.cpp << 'MAINCPP'
#include "include/test/test_comm_node.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestCommNode>("test_comm");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(30.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp1f_uav_comm_node.log
echo "1f done -> logs/exp1f_uav_comm_node.log"

# 1g - UAVMobilityController
cat > main/main.cpp << 'MAINCPP'
#include "include/test/test_mobility.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestMobility>("test_mobility");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(30.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp1g_uav_mobility_controller.log
echo "1g done -> logs/exp1g_uav_mobility_controller.log"

echo ""
echo "=== Experiment 2: Baseline — No Handoff ==="
cat > main/main.cpp << 'MAINCPP'
#include "include/coupled/mission_system_no_handoff.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<MissionSystemNoHandoff>("mission_system_no_handoff");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(200.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp2_baseline_no_handoff.log
echo "2 done -> logs/exp2_baseline_no_handoff.log"

echo ""
echo "=== Experiment 3: With Handoff ==="
cat > main/main.cpp << 'MAINCPP'
#include "include/coupled/mission_system.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<MissionSystem>("mission_system");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(200.0); rc.stop();
}
MAINCPP
cmake --build build --target UAV_Swarm -j4 2>/dev/null
./bin/UAV_Swarm > logs/exp3_with_handoff.log
echo "3 done -> logs/exp3_with_handoff.log"

echo ""
echo "=== All logs saved in ./logs/ ==="
ls -lh logs/
