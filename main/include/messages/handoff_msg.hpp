#ifndef HANDOFF_MSG_HPP
#define HANDOFF_MSG_HPP

#include <iostream>

struct HandoffMsg {
    int target_uav_id;
    HandoffMsg(int id = -1) : target_uav_id(id) {}
};

inline std::ostream& operator<<(std::ostream& out, const HandoffMsg& h) {
    out << "target_uav:" << h.target_uav_id;
    return out;
}

inline std::istream& operator>>(std::istream& in, HandoffMsg& h) {
    in >> h.target_uav_id;
    return in;
}

#endif
