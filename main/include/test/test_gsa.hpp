#ifndef TEST_GSA_HPP
#define TEST_GSA_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../atomics/ground_station_aggregator.hpp"

using namespace cadmium;

struct TestGSA : public Coupled {
    TestGSA(const std::string& id) : Coupled(id) {
        auto gsa   = addComponent<GroundStationAggregator>("ground_station_aggregator");
        auto input = addComponent<lib::IEStream<int>>(
            "mission_data_input",
            "./inputs/test_gsa_input.txt"
        );
        addCoupling(input->out, gsa->mission_data);
    }
};

#endif
