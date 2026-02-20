#include "include/test/test_handoff.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>

using namespace cadmium;

int main() {
    auto model = std::make_shared<TestHandoff>("test_handoff");
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(30.0);
    rootCoordinator.stop();
    return 0;
}
