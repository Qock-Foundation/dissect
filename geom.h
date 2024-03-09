#pragma once

#include <cmath>

namespace Geom {
    const double eps = 1e-3;

    struct point {
        double x, y;
    };

    point operator+(const point &a, const point &b) {
        return {a.x + b.x, a.y + b.y};
    }

    point operator-(const point &a, const point &b) {
        return {a.x - b.x, a.y - b.y};
    }

    point operator*(const point &a, double b) {
        return {a.x * b, a.y * b};
    }

    point operator/(const point &a, double b) {
        return {a.x / b, a.y / b};
    }

    bool operator==(const point &a, const point &b) {
        return abs(a.x - b.x) < eps && abs(a.y - b.y) < eps;
    }

    point rotate(const point &a, double angle) {
        return {cos(angle) * a.x - sin(angle) * a.y,
                sin(angle) * a.x + cos(angle) * a.y};
    }
}
