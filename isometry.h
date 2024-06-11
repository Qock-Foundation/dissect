#pragma once

#include <array>
#include <cmath>
#include <vector>
#include "figure.h"
#include "geom.h"

using namespace Geom;

struct Isometry {
    point origin;
    int sym;
    double rot_angle;
    point dv;
};

const double max_trans = 100;

std::array<int, 4> get_max_shifts(const Figure &figure) {
    int max_shift = (int) round(max_trans / figure.lattice.scale);
    int max_shift_l = std::min(max_shift, figure.inner_x);
    int max_shift_r = std::min(max_shift, figure.n - figure.inner_x);
    int max_shift_d = std::min(max_shift, figure.inner_y);
    int max_shift_u = std::min(max_shift, figure.n - figure.inner_y);
    return {max_shift_l, max_shift_r, max_shift_d, max_shift_u};
}

inline int cnt_isometries(const Figure &figure) {
    auto [max_shift_l, max_shift_r, max_shift_d, max_shift_u] = get_max_shifts(figure);
    return 2 * figure.lattice.rot_ord * (max_shift_l + max_shift_r + 1) * (max_shift_d + max_shift_u + 1);
}

inline int encode_isometry(const Isometry &op, const Figure &figure) {
    auto [max_shift_l, max_shift_r, max_shift_d, max_shift_u] = get_max_shifts(figure);
    int c_rot = figure.lattice.rot_ord, c_dx = max_shift_l + max_shift_r + 1, c_dy = max_shift_d + max_shift_u + 1;
    int rot = (int) round(op.rot_angle * c_rot / (2 * M_PI));
    rot = (rot % c_rot + c_rot) % c_rot;
    auto [dx, dy] = localize(figure.lattice, op.dv);
    return ((op.sym * c_rot + rot) * c_dx + dx + max_shift_l) * c_dy + dy + max_shift_d;
}

inline Isometry decode_isometry(int is, const Figure &figure) {
    auto [max_shift_l, max_shift_r, max_shift_d, max_shift_u] = get_max_shifts(figure);
    int c_rot = figure.lattice.rot_ord, c_dx = max_shift_l + max_shift_r + 1, c_dy = max_shift_d + max_shift_u + 1;
    int dy = is % c_dy - max_shift_d;
    is /= c_dy;
    int dx = is % c_dx - max_shift_l;
    is /= c_dx;
    int rot = is % c_rot;
    is /= c_rot;
    int sym = is;
    return {.origin = figure.lattice.vx * figure.inner_x + figure.lattice.vy * figure.inner_y,
            .sym = sym,
            .rot_angle = 2 * M_PI * rot / c_rot,
            .dv = figure.lattice.vx * dx + figure.lattice.vy * dy};
}

inline point apply_isometry(const point &p, const Isometry &op) {
    auto q = p;
    if (op.sym) q.y = 2 * op.origin.y - q.y;
    q = op.origin + rotate(q - op.origin, op.rot_angle);
    q = q + op.dv;
    return q;
}

inline std::vector<std::vector<int>> build_isometry_graph(const Figure &figure) {
    int cnt_f = (int) figure.lattice.fs.size();
    int cnt_is = cnt_isometries(figure);
    std::vector<std::vector<int>> g(figure.s, std::vector<int>(cnt_is, -1));
    for (int i = 0; i < figure.n; i++) {
        for (int j = 0; j < figure.n; j++) {
            for (int f = 0; f < cnt_f; f++) {
                if (!figure.ord[i][j][f]) continue;
                for (int is = 0; is < cnt_is; is++) {
                    auto op = decode_isometry(is, figure);
                    auto polygon = lattice_polygon(figure.lattice, i, j, f);
                    for (auto &vertex: polygon) {
                        vertex = apply_isometry(vertex, op);
                    }
                    auto center = center_mass(polygon);
                    auto [i2, j2] = localize(figure.lattice, center);
                    if (i2 < 0 || j2 < 0 || i2 >= figure.n || j2 >= figure.n) {
                        continue;
                    }
                    for (int f2 = 0; f2 < cnt_f; f2++) {
                        if (coincide(polygon, lattice_polygon(figure.lattice, i2, j2, f2))) {
                            g[figure.ord[i][j][f] - 1][is] = figure.ord[i2][j2][f2] - 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    return g;
}
