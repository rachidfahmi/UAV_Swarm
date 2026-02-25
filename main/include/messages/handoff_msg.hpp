#ifndef HANDOFF_MSG_HPP
#define HANDOFF_MSG_HPP

#include <iostream>

struct HandoffMsg {
    int affected_uav_id;   // which UAV must switch to relay mode
    int relay_target_id;   // which UAV to relay through

    HandoffMsg(int affected = -1, int relay = -1)
        : affected_uav_id(affected), relay_target_id(relay) {}
};

inline std::ostream& operator<<(std::ostream& out, const HandoffMsg& h) {
    out << "affected:" << h.affected_uav_id
        << " relay:" << h.relay_target_id;
    return out;
}

inline std::istream& operator>>(std::istream& in, HandoffMsg& h) {
    in >> h.affected_uav_id >> h.relay_target_id;
    return in;
}

#endif
