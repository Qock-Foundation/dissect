#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "figure.h"
#include "isometry.h"
#include "lattice.h"
#include "sat.h"

using namespace std;
using namespace Geom;

int main(int argc, char **argv) {
    if (argc != 4) {
        cerr << "usage: " << argv[0] << " lattice figure k" << endl;
        return 1;
    }
    std::string lattice_path(argv[1]);
    std::string figure_path(argv[2]);
    int k = atoi(argv[3]);

    // Reading lattice and figure
    ifstream fin_lattice(lattice_path);
    ifstream fin_figure(figure_path);
    auto lattice = read_lattice(fin_lattice);
    auto figure = read_figure(fin_figure, lattice);

    // Building CNF
    int s = figure.s;
    int cnt_f = (int) lattice.fs.size();
    int cnt_is = cnt_isometries(figure);
    auto g = build_isometry_graph(figure);
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
        vector<bool> used1(s), used2(s);
        for (int i = 0; i < s; i++) {
            int j = g[i][is];
            if (j == -1) continue;
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
    // Optional: additional constraints for tr6
    if (cnt_f == 12) {
        for (int i = 0; i < s; i += 6) {
            for (int c = 0; c < k; c++) {
                vector<int> xs(6);
                for (int num = 0; num < 6; num++) {
                    xs[num] = 1 + (i + num) * k + c;
                }
                for (int num = 0; num < 6; num++) {
                    cnf.push_back({-xs[num], xs[(num + 3) % 6], xs[(num + 2) % 6]});
                    cnf.push_back({-xs[num], xs[(num + 3) % 6], xs[(num + 4) % 6]});
                }
            }
        }
    }

    // Running SAT-solver
    vector<bool> sol(s * k + (k - 1) * cnt_is);
    while (satisfiable(cnf, sol)) {
        // Output
        vector<int> col(s);
        for (int i = 0; i < s; i++) {
            int ind = find(sol.begin() + i * k, sol.begin() + (i + 1) * k, true) - sol.begin();
            col[i] = ind - i * k + 1;
        }
        cout << figure.n << endl;
        for (int i = 0; i < figure.n; i++) {
            for (int j = 0; j < figure.n; j++) {
                for (int id: figure.ord[i][j]) {
                    if (id == 0) cout << 0 << ' ';
                    else cout << col[id - 1] << ' ';
                }
                cout << ' ';
            }
            cout << endl;
        }

        // Forbidding it
        int c0 = -1;
        for (int f = 0; f < cnt_f; f++) {
            if (int id = figure.ord[figure.inner_x][figure.inner_y][f]; id != 0) {
                c0 = col[id - 1];
                break;
            }
        }
        for (int is = 0; is < cnt_is; is++) {
            bool ok = true;
            vector<int> target;
            for (int i = 0; i < s; i++) {
                if (col[i] != c0) continue;
                int j = g[i][is];
                if (j == -1) {
                    ok = false;
                    break;
                }
                target.push_back(j);
            }
            if (!ok) continue;
            for (int c = 0; c < k; c++) {
                cnf.emplace_back();
                for (int j: target) {
                    cnf.back().push_back(-(1 + j * k + c));
                }
            }
        }
    }
}
