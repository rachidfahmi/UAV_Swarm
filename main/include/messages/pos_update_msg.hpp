#ifndef POS_UPDATE_MSG_HPP
#define POS_UPDATE_MSG_HPP

#include <iostream>
#include <cmath>

struct PosUpdateMsg {
    double x, y;
    PosUpdateMsg(double x = 0.0, double y = 0.0) : x(x), y(y) {}
    double distance() const { return std::sqrt(x * x + y * y); }
};

inline std::ostream& operator<<(std::ostream& out, const PosUpdateMsg& m) {
    out << m.x << "," << m.y;
    return out;
}

inline std::istream& operator>>(std::istream& in, PosUpdateMsg& m) {
    char comma;
    in >> m.x >> comma >> m.y;
    if (comma != ',')                    // ← validate separator
        in.setstate(std::ios::failbit);
    return in;
}

#endif
