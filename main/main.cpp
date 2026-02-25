#include "include/coupled/mission_system_no_handoff.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<MissionSystemNoHandoff>("mission_no_handoff");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(200.0); rc.stop();
}
