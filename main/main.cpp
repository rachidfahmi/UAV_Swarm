#include "include/coupled/mission_system.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>

using namespace cadmium;

int main() {
    auto model = std::make_shared<MissionSystem>("mission_system");
    auto rc = RootCoordinator(model);
    rc.setLogger<STDOUTLogger>(";");
    rc.start();
    rc.simulate(200.0);
    rc.stop();
    return 0;
}
