#pragma once

#include <array>
#include <cmath>
#include <istream>
#include <vector>
#include "geom.h"

using namespace Geom;

struct Lattice {
    std::vector<point> vs;
    std::vector<std::vector<int>> fs;
    int rot_ord;
    double scale;
    point vx, vy;
};

inline Lattice read_lattice(std::istream &in, double scale = 1.0) {
    int cnt_v;
    in >> cnt_v;
    std::vector<point> vs(cnt_v);
    for (int i = 0; i < cnt_v; i++) {
        in >> vs[i];
        vs[i] = vs[i] * scale;
    }
    int cnt_f;
    in >> cnt_f;
    std::vector<std::vector<int>> fs(cnt_f);
    for (int i = 0; i < cnt_f; i++) {
        int f_sz;
        in >> f_sz;
        fs[i].resize(f_sz);
        for (int j = 0; j < f_sz; j++) {
            in >> fs[i][j];
        }
    }
    int rot_ord;
    in >> rot_ord;
    point vx = {scale, 0};
    point vy = {scale * cos(2 * M_PI / rot_ord), scale * sin(2 * M_PI / rot_ord)};
    return {.vs = vs,
            .fs = fs,
            .rot_ord = rot_ord,
            .scale = scale,
            .vx = vx,
            .vy = vy};
}

inline std::vector<point> lattice_polygon(const Lattice &lattice, int i, int j, int f) {
    std::vector<point> polygon;
    polygon.reserve(lattice.fs[f].size());
    for (int v: lattice.fs[f]) {
        polygon.push_back(lattice.vx * i + lattice.vy * j + lattice.vs[v]);
    }
    return polygon;
}

inline std::pair<double, double> lattice_coords(const Lattice &lattice, const point &p) {
    auto [a, c] = lattice.vx;
    auto [b, d] = lattice.vy;
    double det = a * d - b * c;
    return {point{d, -b} * p / det, point{-c, a} * p / det};
}

inline std::pair<int, int> localize(const Lattice &lattice, const point &p) {
    auto [c1, c2] = lattice_coords(lattice, p);
    int i = floor(c1 + eps);
    int j = floor(c2 + eps);
    return {i, j};
}
