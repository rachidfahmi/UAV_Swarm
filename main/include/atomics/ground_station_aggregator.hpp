#ifndef GROUND_STATION_AGGREGATOR_HPP
#define GROUND_STATION_AGGREGATOR_HPP

#include <iostream>
#include <limits>
#include <cadmium/modeling/devs/atomic.hpp>

using namespace cadmium;

struct GroundStationAggregatorState {
    double sigma;
    int    received_count;
    bool   reporting;

    explicit GroundStationAggregatorState()
        : sigma(std::numeric_limits<double>::infinity()),
          received_count(0),
          reporting(false) {}
};

inline std::ostream& operator<<(std::ostream& out, const GroundStationAggregatorState& s) {
    out << "count:" << s.received_count
        << " reporting:" << (s.reporting ? "true" : "false")
        << " sigma:" << s.sigma;
    return out;
}

class GroundStationAggregator : public Atomic<GroundStationAggregatorState> {
public:
    Port<int> mission_data;
    Port<int> task_completed;

    GroundStationAggregator(const std::string& id)
        : Atomic<GroundStationAggregatorState>(id, GroundStationAggregatorState()) {
        mission_data   = addInPort<int>("mission_data");
        task_completed = addOutPort<int>("task_completed");
    }

    void externalTransition(GroundStationAggregatorState& s, double e) const override {
        s.sigma -= e;
        if (!mission_data->empty()) {
            s.received_count += mission_data->getBag().size();
            if (s.received_count >= 3) {
                s.reporting = true;
                s.sigma     = 0.0;   // Ta=0 → instant output
            }
        }
    }

    void internalTransition(GroundStationAggregatorState& s) const override {
        // fired after Ta=0 reporting state
        s.received_count = 0;
        s.reporting      = false;
        s.sigma          = std::numeric_limits<double>::infinity();
    }

    void output(const GroundStationAggregatorState& s) const override {
        task_completed->addMessage(1);
    }

    [[nodiscard]] double timeAdvance(const GroundStationAggregatorState& s) const override {
        return s.sigma;
    }
};

#endif
