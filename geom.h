#pragma once

#include <cmath>
#include <istream>
#include <ostream>
#include <vector>

namespace Geom {
    const double eps = 1e-4;

    struct point {
        double x, y;
    };

    std::istream &operator>>(std::istream &in, point &p) {
        return in >> p.x >> p.y;
    }

    std::ostream &operator<<(std::ostream &out, point &p) {
        return out << p.x << ' ' << p.y;
    }

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
        return std::abs(a.x - b.x) < eps && std::abs(a.y - b.y) < eps;
    }

    bool operator!=(const point &a, const point &b) {
        return !(a == b);
    }

    double operator*(const point &a, const point &b) {
        return a.x * b.x + a.y * b.y;
    }

    double operator^(const point &a, const point &b) {
        return a.x * b.y - a.y * b.x;
    }

    double angle(const point &a, const point &b) {
        return atan2(a ^ b, a * b);
    }

    point rotate(const point &a, double angle) {
        return {cos(angle) * a.x - sin(angle) * a.y,
                sin(angle) * a.x + cos(angle) * a.y};
    }

    point center_mass(const std::vector<point> &polygon) {
        point c = {0, 0};
        for (const point &v: polygon) {
            c = c + v;
        }
        return c / (double) polygon.size();
    }

    bool point_inside_polygon(const point &p, const std::vector<point> &polygon) {
        size_t n = polygon.size();
        double total_angle = 0;
        for (size_t i = 0; i < n; i++) {
            size_t j = (i + 1) % n;
            total_angle += angle(polygon[j] - p, polygon[i] - p);
        }
        return std::abs(total_angle) > 1;
    }

    bool coincide(const std::vector<point> &polygon1, const std::vector<point> &polygon2) {
        if (polygon1.size() != polygon2.size()) return false;
        size_t n = polygon1.size();
        size_t delta = std::find(polygon1.begin(), polygon1.end(), polygon2[0]) - polygon1.begin();
        if (delta == n) return false;
        bool ok1 = true, ok2 = true;
        for (size_t i = 0; i < n; i++) {
            if (polygon2[i] != polygon1[(delta + i) % n]) {
                ok1 = false;
            }
            if (polygon2[i] != polygon1[(n + delta - i) % n]) {
                ok2 = false;
            }
        }
        return ok1 || ok2;
    }
}
