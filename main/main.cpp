#include "include/test/test_gsa.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>

using namespace cadmium;

int main() {
    auto model = std::make_shared<TestGSA>("test_gsa");
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(70.0);
    rootCoordinator.stop();
    return 0;
}
