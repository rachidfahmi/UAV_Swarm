#ifndef POS_UPDATE_MSG_HPP
#define POS_UPDATE_MSG_HPP

#include <iostream>

struct PosUpdate {
    float x;
    float y;

    PosUpdate() : x(0.0f), y(0.0f) {}
    PosUpdate(float x, float y) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& out, const PosUpdate& p) {
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

std::istream& operator>>(std::istream& in, PosUpdate& p) {
    in >> p.x >> p.y;
    return in;
}

#endif
