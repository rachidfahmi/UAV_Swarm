#ifndef HANDOFF_MSG_HPP
#define HANDOFF_MSG_HPP

#include <iostream>
#include <string>

struct HandoffMsg {
    int target_uav_id = 0;
};

inline std::ostream& operator<<(std::ostream& out, const HandoffMsg& h) {
    out << "target_uav:" << h.target_uav_id;
    return out;
}

inline std::istream& operator>>(std::istream& in, HandoffMsg& h) {
    std::string tok;
    in >> tok;
    if (!in) return in;

    auto pos = tok.find(':');
    if (pos != std::string::npos)
        h.target_uav_id = std::stoi(tok.substr(pos + 1));
    else
        h.target_uav_id = std::stoi(tok);

    return in;
}

#endif
