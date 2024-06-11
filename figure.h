#pragma once

#include <cmath>
#include <istream>
#include <vector>
#include "geom.h"
#include "lattice.h"

using namespace Geom;

struct Figure {
    Lattice lattice;
    int n, s;
    std::vector<std::vector<std::vector<int>>> ord;
    int inner_x, inner_y;
};

inline Figure read_figure(std::istream &in, const Lattice &lattice) {
    int n;
    in >> n;
    int cnt_f = (int) lattice.fs.size();
    std::vector<std::vector<std::vector<int>>> ord(n, std::vector<std::vector<int>>(n, std::vector<int>(cnt_f)));
    int s = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int f = 0; f < cnt_f; f++) {
                int x;
                in >> x;
                if (x) ord[i][j][f] = ++s;
            }
        }
    }
    std::vector<point> centers;
    std::vector<std::pair<int, int>> inner;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int f = 0; f < cnt_f; f++) {
                if (ord[i][j][f]) {
                    auto polygon = lattice_polygon(lattice, i, j, f);
                    centers.push_back(center_mass(polygon));
                    inner.emplace_back(i, j);
                }
            }
        }
    }
    point center = center_mass(centers);
    int inner_x = -1, inner_y = -1;
    double min_dist = lattice.scale;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            point v = lattice.vx * i + lattice.vy * j;
            for (int f = 0; f < cnt_f; f++) {
                double dist = sqrt((v - center) * (v - center));
                if (ord[i][j][f] && dist < min_dist) {
                    min_dist = dist;
                    inner_x = i;
                    inner_y = j;
                }
            }
        }
    }
//    int num = rand() % inner.size();
//    int inner_x = inner[num].first;
//    int inner_y = inner[num].second;
    return {.lattice = lattice,
            .n = n,
            .s = s,
            .ord = ord,
            .inner_x = inner_x,
            .inner_y = inner_y};
}

inline Figure convert_to_lattice(const Figure &figure, const Lattice &lattice) {
    int scale_factor = (int) round(figure.lattice.scale / lattice.scale);
    int n = figure.n * scale_factor;
    int cnt_f = (int) lattice.fs.size();
    int cnt_f2 = (int) figure.lattice.fs.size();
    std::vector<std::vector<std::vector<int>>> ord(n, std::vector<std::vector<int>>(n, std::vector<int>(cnt_f)));
    int s = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int f = 0; f < cnt_f; f++) {
                auto polygon = lattice_polygon(lattice, i, j, f);
                int i2 = i / scale_factor, j2 = j / scale_factor;
                point center = center_mass(polygon);
                for (int f2 = 0; f2 < cnt_f2; f2++) {
                    auto polygon2 = lattice_polygon(figure.lattice, i2, j2, f2);
                    if (figure.ord[i2][j2][f2] && point_inside_polygon(center, polygon2)) {
                        ord[i][j][f] = ++s;
                        break;
                    }
                }
            }
        }
    }
    int inner_x = figure.inner_x * scale_factor;
    int inner_y = figure.inner_y * scale_factor;
    return {.lattice = lattice,
            .n = n,
            .s = s,
            .ord = ord,
            .inner_x = inner_x,
            .inner_y = inner_y};
}
