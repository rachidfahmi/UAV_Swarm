#include "include/test/test_sensor.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>

using namespace cadmium;

int main() {
    auto model = std::make_shared<TestSensor>("test_sensor");
    auto rootCoordinator = RootCoordinator(model);
    rootCoordinator.setLogger<STDOUTLogger>(";");
    rootCoordinator.start();
    rootCoordinator.simulate(20.0);
    rootCoordinator.stop();
    return 0;
}
