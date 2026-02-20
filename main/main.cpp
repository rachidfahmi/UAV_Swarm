#include "include/test/test_uav_comm.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>

using namespace cadmium;

int main() {
    auto model = std::make_shared<TestUAVComm>("test_uav_comm");
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(25.0);
    rootCoordinator.stop();
    return 0;
}
