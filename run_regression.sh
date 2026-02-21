#!/bin/bash
set -e
TESTS=(mtg sensor mobility gsa handoff uav_comm network_link)
mkdir -p regression_logs

for TEST in "${TESTS[@]}"; do
cat > main/main.cpp << MAIN
#include "include/test/test_${TEST}.hpp"
#include <cadmium/simulation/root_coordinator.hpp>
#include <cadmium/simulation/logger/stdout.hpp>
using namespace cadmium;
int main() {
    auto model = std::make_shared<Test$(echo $TEST | sed 's/_\(.\)/\U\1/g; s/^\(.\)/\U\1/')>("test_${TEST}");
    auto rc = RootCoordinator(model);
    rc.setLogger<STDOUTLogger>(";");
    rc.start();
    rc.simulate(100.0);
    rc.stop();
    return 0;
}
MAIN
    echo "=== Building $TEST ==="
    source build_sim.sh 2>&1 | tail -3
    echo "=== Running $TEST ==="
    ./bin/UAV_Swarm > regression_logs/${TEST}.log 2>&1
    echo "--- Output ---"
    cat regression_logs/${TEST}.log
    echo ""
done
echo "All regression tests complete. Logs in regression_logs/"
