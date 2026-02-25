#ifndef POS_UPDATE_MSG_HPP
#define POS_UPDATE_MSG_HPP

#include <iostream>

struct PosUpdate {
    float x = 0.0f;
    float y = 0.0f;
};

inline std::ostream& operator<<(std::ostream& out, const PosUpdate& p) {
    out << p.x << " " << p.y;
    return out;
}

inline std::istream& operator>>(std::istream& in, PosUpdate& p) {
    in >> p.x >> p.y;
    return in;
}

#endif
