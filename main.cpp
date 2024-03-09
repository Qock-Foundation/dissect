#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "geom.h"
#include "sat.h"

using namespace std;
using namespace Geom;

struct isometry {
    int sym, rot, dx, dy;
};

int n, rot_ord;
point v1, v2, delta;

int encode_isometry(const isometry &op) {
    int c_rot = rot_ord, c_dx = n + 1, c_dy = n + 1;
    return ((op.sym * c_rot + op.rot) * c_dx + op.dx) * c_dy + op.dy;
}

isometry decode_isometry(int is) {
    int c_rot = rot_ord, c_dx = n + 1, c_dy = n + 1;
    int dy = is % c_dy;
    is /= c_dy;
    int dx = is % c_dx;
    is /= c_dx;
    int rot = is % c_rot;
    is /= c_rot;
    int sym = is;
    return {sym, rot, dx, dy};
}

point apply_isometry(const point &a, const isometry &op) {
    auto b = a;
    if (op.sym) b.y = 2 * delta.y - b.y;
    b = delta + rotate(b - delta, 2 * M_PI / rot_ord * op.rot);
    b = b + v1 * op.dx + v2 * op.dy - delta;
    return b;
}

vector<vector<int>> eq_cnf(const vector<int> &var) {
    vector<vector<int>> res;
    for (int x: var) {
        for (int y: var) {
            res.push_back({x, -y});
        }
    }
    return res;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "usage: " << argv[0] << " lattice_file k" << endl;
        return 1;
    }
    int k = atoi(argv[2]);
    srand(clock());

    // Reading lattice
    fstream fin(argv[1]);
    int cnt_v;
    fin >> cnt_v;
    vector<point> vs(cnt_v);
    for (int i = 0; i < cnt_v; i++) {
        fin >> vs[i].x >> vs[i].y;
    }
    int cnt_f;
    fin >> cnt_f;
    vector<vector<int>> fs(cnt_f);
    for (int i = 0; i < cnt_f; i++) {
        int f_sz;
        fin >> f_sz;
        fs[i].resize(f_sz);
        for (int j = 0; j < f_sz; j++) {
            fin >> fs[i][j];
        }
    }
    fin >> rot_ord;
    v1 = {1, 0};
    v2 = {cos(2 * M_PI / rot_ord), sin(2 * M_PI / rot_ord)};

    // Reading figure
    cin >> n;
    int cnt_is = 2 * rot_ord * (n + 1) * (n + 1);
    vector<vector<vector<int>>> fig(n, vector<vector<int>>(n, vector<int>(cnt_f)));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int f = 0; f < cnt_f; f++) {
                cin >> fig[i][j][f];
            }
        }
    }
    vector<pair<int, int>> used;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            bool found = false;
            for (int f = 0; f < cnt_f; f++) {
                if (fig[i][j][f]) {
                    found = true;
                    break;
                }
            }
            if (found) {
                used.emplace_back(i, j);
            }
        }
    }
    int pos = rand() % used.size();
    delta = v1 * used[pos].first + v2 * used[pos].second;
    vector<point> centers;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int f = 0; f < cnt_f; f++) {
                if (fig[i][j][f]) {
                    point cent = {0, 0};
                    for (int v: fs[f]) {
                        point cur = v1 * i + v2 * j + vs[v];
                        cent = cent + cur;
                    }
                    cent = cent / (double) fs[f].size();
                    centers.push_back(cent);
                }
            }
        }
    }

    // Building CNF
    int s = (int) centers.size();
    vector<vector<int>> cnf;
    for (int i = 0; i < s; i++) {
        cnf.emplace_back();
        for (int j = 0; j < k; j++) {
            cnf.back().push_back(1 + i * k + j);
        }
        for (int j1 = 0; j1 < k; j1++) {
            for (int j2 = 0; j2 < j1; j2++) {
                cnf.push_back({-(1 + i * k + j1), -(1 + i * k + j2)});
            }
        }
    }
    for (int c = 0; c < k - 1; c++) {
        cnf.emplace_back();
        for (int is = 0; is < cnt_is; is++) {
            cnf.back().push_back(1 + s * k + c * cnt_is + is);
        }
    }
    for (int is = 0; is < cnt_is; is++) {
        auto op = decode_isometry(is);
        vector<bool> used1(s), used2(s);
        for (int i = 0; i < s; i++) {
            point from = centers[i];
            point to = apply_isometry(from, op);
            int j = find(centers.begin(), centers.end(), to) - centers.begin();
            if (j == s) continue;
            used1[i] = used2[j] = true;
            for (int c = 0; c < k - 1; c++) {
                int id_from = 1 + i * k + (k - 1);
                int id_to = 1 + j * k + c;
                int id_is = 1 + s * k + c * cnt_is + is;
                cnf.push_back({-id_from, -id_is, id_to});
                cnf.push_back({-id_to, -id_is, id_from});
            }
        }
        for (int i = 0; i < s; i++) {
            if (!used1[i]) {
                for (int c = 0; c < k - 1; c++) {
                    int id_from = 1 + i * k + (k - 1);
                    int id_is = 1 + s * k + c * cnt_is + is;
                    cnf.push_back({-id_from, -id_is});
                }
            }
        }
        for (int j = 0; j < s; j++) {
            if (!used2[j]) {
                for (int c = 0; c < k - 1; c++) {
                    int id_to = 1 + j * k + c;
                    int id_is = 1 + s * k + c * cnt_is + is;
                    cnf.push_back({-id_to, -id_is});
                }
            }
        }
    }
    // Optional: additional constraints for triangular_med
    if (cnt_f == 12) {
        for (int i = 0; i < s; i += 6) {
            for (int c = 0; c < k; c++) {
                int x1 = 1 + (i + 0) * k + c;
                int y1 = 1 + (i + 2) * k + c;
                int z1 = 1 + (i + 4) * k + c;
                int x2 = 1 + (i + 1) * k + c;
                int y2 = 1 + (i + 3) * k + c;
                int z2 = 1 + (i + 5) * k + c;
                auto eq1 = eq_cnf({x1, y1, z1});
                auto eq2 = eq_cnf({x2, y2, z2});
                for (const auto &elem: eq1) cnf.push_back(elem);
                for (const auto &elem: eq2) cnf.push_back(elem);
            }
        }
    }

    // Running SAT-solver
    vector<bool> sol(s * k + (k - 1) * cnt_is);
    bool sat = satisfiable(cnf, sol);

    // Output
    if (sat) {
        vector<int> col(s);
        for (int i = 0; i < s; i++) {
            int ind = find(sol.begin() + i * k, sol.begin() + (i + 1) * k, true) - sol.begin();
            col[i] = ind - i * k + 1;
        }
        int id = 0;
        cout << n << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int f = 0; f < cnt_f; f++) {
                    if (fig[i][j][f] == 0) {
                        cout << 0 << ' ';
                    } else {
                        cout << col[id++] << ' ';
                    }
                }
                cout << ' ';
            }
            cout << endl;
        }
    } else {
        cout << n << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int f = 0; f < cnt_f; f++) {
                    cout << -fig[i][j][f] << ' ';
                }
                cout << ' ';
            }
            cout << endl;
        }
    }
}
