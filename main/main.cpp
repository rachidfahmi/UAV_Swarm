#include "include/test/test_mobile_uav_node.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>

using namespace cadmium;

int main() {
    auto model = std::make_shared<TestMobileUAVNode>("test_mobile_uav_node");
    auto rc = RootCoordinator(model);
    rc.setLogger<STDOUTLogger>(";");
    rc.start();
    rc.simulate(30.0);
    rc.stop();
    return 0;
}
