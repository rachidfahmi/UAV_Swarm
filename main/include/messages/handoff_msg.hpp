#ifndef HANDOFF_MSG_HPP
#define HANDOFF_MSG_HPP

#include <iostream>

struct HandoffMsg {
    int target_uav_id;

    HandoffMsg() : target_uav_id(0) {}
    HandoffMsg(int id) : target_uav_id(id) {}
};

std::ostream& operator<<(std::ostream& out, const HandoffMsg& h) {
    out << "target_uav:" << h.target_uav_id;
    return out;
}

std::istream& operator>>(std::istream& in, HandoffMsg& h) {
    in >> h.target_uav_id;
    return in;
}

#endif
