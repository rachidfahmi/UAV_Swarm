#ifndef LINK_STATE_MSG_HPP
#define LINK_STATE_MSG_HPP

#include <iostream>
#include <string>

enum class LinkState { Available, Degraded, Disconnected };

inline std::ostream& operator<<(std::ostream& out, const LinkState& s) {
    switch (s) {
        case LinkState::Available:    out << "Available";    break;
        case LinkState::Degraded:     out << "Degraded";     break;
        case LinkState::Disconnected: out << "Disconnected"; break;
    }
    return out;
}

inline std::istream& operator>>(std::istream& in, LinkState& s) {
    std::string token;
    in >> token;
    if      (token == "Available")    s = LinkState::Available;
    else if (token == "Degraded")     s = LinkState::Degraded;
    else if (token == "Disconnected") s = LinkState::Disconnected;
    else in.setstate(std::ios::failbit);
    return in;
}

// ── NEW ──────────────────────────────────────────────────────────────────────
struct LinkStateMsg {
    int       uav_id;
    LinkState state;
    LinkStateMsg(int id = 0, LinkState s = LinkState::Available)
        : uav_id(id), state(s) {}
};

inline std::ostream& operator<<(std::ostream& out, const LinkStateMsg& m) {
    out << m.uav_id << "," << m.state;
    return out;
}

inline std::istream& operator>>(std::istream& in, LinkStateMsg& m) {
    char comma;
    in >> m.uav_id >> comma >> m.state;
    return in;
}
// ─────────────────────────────────────────────────────────────────────────────

#endif
