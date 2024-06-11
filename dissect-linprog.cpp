#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "check-fixed.h"
#include "figure.h"
#include "isometry.h"
#include "lattice.h"

using namespace std;
using namespace Geom;

void isometry_search(const vector<vector<int>> &g, vector<int> &iss, const Figure &figure,
                     const vector<Lattice> &smaller_lattices, int k) {
    int s = figure.s;
    int cnt_is = cnt_isometries(figure);
    int sz = s / k;
    vector<double> part0(s);
    if (!check_fractional(g, iss, sz, part0)) {
        return;
    }
    if (iss.size() == 2) cerr << iss[0] << ' ' << iss[1] << endl;
    if (iss.size() == k - 1) {
        cout << "candidate: " << endl;
        for (int is: iss) {
            auto op = decode_isometry(is, figure);
            cout << "sym = " << op.sym << ", rot = " << op.rot_angle << ", dv = " << op.dv << ", origin = " << op.origin
                 << endl;
        }
        cout << endl;
        for (int i = 0; i < figure.n; i++) {
            for (int j = 0; j < figure.n; j++) {
                for (int id: figure.ord[i][j]) {
                    if (id == 0) cout << 0 << ' ';
                    else cout << part0[id - 1] << ' ';
                }
                cout << ' ';
            }
            cout << endl;
        }
        if (check_discrete(g, iss)) {
            cout << "OK" << endl;
            return;
        }
        cout << "NOT OK, trying smaller lattices" << endl;
        for (const auto &smaller_lattice: smaller_lattices) {
            auto figure_new = convert_to_lattice(figure, smaller_lattice);
            auto g_new = build_isometry_graph(figure_new);
            vector<int> iss_new;
            iss_new.reserve(iss.size());
            for (int is: iss) {
                auto op = decode_isometry(is, figure);
                iss_new.push_back(encode_isometry(op, figure_new));
            }
            if (check_discrete(g_new, iss_new)) {
                cout << "OK!" << endl;
                break;
            } else {
                cout << "NOT OK" << endl;
            }
        }
        return;
    }
    int next_is = iss.empty() ? 0 : iss.back() + 1;
    for (; next_is < cnt_is; next_is++) {
        iss.push_back(next_is);
        isometry_search(g, iss, figure, smaller_lattices, k);
        iss.pop_back();
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "usage: " << argv[0] << " figure k" << endl;
        return 1;
    }
    string lattice_path("lattices/tr");
    string figure_path(argv[1]);
    int k = atoi(argv[2]);

    // Reading lattice and figure
    ifstream fin_lattice(lattice_path);
    ifstream fin_figure(figure_path);
    auto lattice = read_lattice(fin_lattice);
    auto figure = read_figure(fin_figure, lattice);
    vector<Lattice> smaller_lattices;
    for (int factor = 1; factor <= 2; factor++) {
        ifstream fin2("lattices/tr6");
        auto lattice2 = read_lattice(fin2, 1.0 / factor);
        smaller_lattices.push_back(lattice2);
    }

    // Solving
    auto g = build_isometry_graph(figure);
    vector<int> iss;
    isometry_search(g, iss, figure, smaller_lattices, k);
}
