#ifndef LINK_STATE_MSG_HPP
#define LINK_STATE_MSG_HPP

#include <iostream>
#include <string>

enum class LinkState { Available=0, Degraded=1, Disconnected=2 };

inline std::ostream& operator<<(std::ostream& out, const LinkState& s) {
    switch (s) {
        case LinkState::Available:    out << "Available";    break;
        case LinkState::Degraded:     out << "Degraded";     break;
        case LinkState::Disconnected: out << "Disconnected"; break;
    }
    return out;
}

inline std::istream& operator>>(std::istream& in, LinkState& s) {
    std::string tok;
    in >> tok;
    if (!in) return in;

    if      (tok == "0" || tok == "Available")    s = LinkState::Available;
    else if (tok == "1" || tok == "Degraded")     s = LinkState::Degraded;
    else if (tok == "2" || tok == "Disconnected") s = LinkState::Disconnected;
    else in.setstate(std::ios::failbit);

    return in;
}

#endif
