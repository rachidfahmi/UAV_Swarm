#include <limits>
#include "include/test/test_mtg.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
#include <cadmium/simulation/logger/csv.hpp>

using namespace cadmium;

int main() {
    auto model = std::make_shared<TestMTG>("test_mtg");
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(200.0);
    rootCoordinator.stop();
    return 0;
}
