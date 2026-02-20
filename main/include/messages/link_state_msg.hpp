#ifndef LINK_STATE_MSG_HPP
#define LINK_STATE_MSG_HPP

#include <iostream>

enum class LinkState {
    Available,
    Degraded,
    Disconnected
};

std::ostream& operator<<(std::ostream& out, const LinkState& s) {
    switch (s) {
        case LinkState::Available:     out << "Available";     break;
        case LinkState::Degraded:      out << "Degraded";      break;
        case LinkState::Disconnected:  out << "Disconnected";  break;
    }
    return out;
}

std::istream& operator>>(std::istream& in, LinkState& s) {
    int val;
    in >> val;
    s = static_cast<LinkState>(val);
    return in;
}

#endif
