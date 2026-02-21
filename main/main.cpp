#include "include/test/test_network_link.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto m = std::make_shared<TestNetworkLink>("test_network_link");
    auto rc = RootCoordinator(m);
    rc.setLogger<STDOUTLogger>(";");
    rc.start(); rc.simulate(40.0); rc.stop();
}
