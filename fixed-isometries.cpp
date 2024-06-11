#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "check-fixed.h"
#include "figure.h"
#include "isometry.h"
#include "lattice.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "usage: " << argv[0] << " lattice figure" << endl;
        return 1;
    }
    string lattice_path(argv[1]);
    string figure_path(argv[2]);
    ifstream fin_lattice(lattice_path);
    ifstream fin_figure(figure_path);
    auto lattice = read_lattice(fin_lattice);
    auto figure = read_figure(fin_figure, lattice);

//    point origin = {5 * 1.5, 5 * sqrt(3) / 2};
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI / 3, .dv = {-0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = -M_PI / 3, .dv = {-0.5, -sqrt(3) / 2}}
//    };

//    point origin = {7, 2 * sqrt(3)};
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {1, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = 4 * M_PI / 3, .dv = {-1, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = 4 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI / 3, .dv = {0, sqrt(3)}},
//            {.origin = origin, .sym = 1, .rot_angle = 5 * M_PI / 3, .dv = {0.5, sqrt(3) / 2}}
//    };

//    point origin = {7 * 1.5, 7 * sqrt(3) / 2};
//    double scale = 2.0 / 3;
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = 0, .dv = {0.5 * scale, sqrt(3) / 2 * scale}},
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {0.5 * scale, -sqrt(3) / 2 * scale}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI, .dv = {-0.5 * scale, sqrt(3) / 2 * scale}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI, .dv = {-0.5 * scale, -sqrt(3) / 2 * scale}}
//    };

//    point origin = {11 * 1.5, 11 * sqrt(3) / 2};
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {1, sqrt(3)}},
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = -M_PI / 3, .dv = {-1, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = -M_PI / 3, .dv = {-2, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI / 3, .dv = {1, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI / 3, .dv = {2, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI, .dv = {-0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI, .dv = {-1, -sqrt(3)}}
//    };

//    point origin = {11 * 1.5, 11 * sqrt(3) / 2};
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {1, sqrt(3)}},
//            {.origin = origin, .sym = 1, .rot_angle = 0, .dv = {0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = 0, .dv = {1, -sqrt(3)}},
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI, .dv = {-0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI, .dv = {-1, sqrt(3)}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI, .dv = {-0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI, .dv = {-1, -sqrt(3)}}
//    };

//    point origin = {11 * 1.5, 11 * sqrt(3) / 2};
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = 0, .dv = {0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = 0, .dv = {0, sqrt(3)}},
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {0, -sqrt(3)}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI, .dv = {0, sqrt(3)}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI, .dv = {-0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI, .dv = {0, -sqrt(3)}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI, .dv = {-0.5, -sqrt(3) / 2}}
//    };

//    point origin = {11 * 1.5, 11 * sqrt(3) / 2};
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = 2 * M_PI /  3, .dv = {1, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = 2 * M_PI / 3, .dv = {0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {1, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI / 3, .dv = {-0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI / 3, .dv = {-1, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = -M_PI / 3, .dv = {-0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = -M_PI / 3, .dv = {-1, 0}}
//    };

//    point origin = {11 * 1.5, 11 * sqrt(3) / 2};
//    vector<Isometry> ops = {
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI /  3, .dv = {1, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = 2 * M_PI / 3, .dv = {0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = -2 * M_PI / 3, .dv = {1, 0}},
//            {.origin = origin, .sym = 0, .rot_angle = -2 * M_PI / 3, .dv = {0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = M_PI / 3, .dv = {-0.5, sqrt(3) / 2}},
//            {.origin = origin, .sym = 1, .rot_angle = M_PI / 3, .dv = {-1, 0}},
//            {.origin = origin, .sym = 1, .rot_angle = -M_PI / 3, .dv = {-0.5, -sqrt(3) / 2}},
//            {.origin = origin, .sym = 0, .rot_angle = -M_PI / 3, .dv = {-1, 0}}
//    };

    point origin = {11 * 1.5, 11 * sqrt(3) / 2};
    vector<Isometry> ops = {
            {.origin = origin, .sym = 1, .rot_angle = M_PI / 3, .dv = {0, 0}},
            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {0, 0}},
            {.origin = origin, .sym = 1, .rot_angle = M_PI, .dv = {0, 0}},
            {.origin = origin, .sym = 0, .rot_angle = 4 * M_PI / 3, .dv = {0, 0}},
            {.origin = origin, .sym = 1, .rot_angle = 5 * M_PI / 3, .dv = {0, 0}},
            {.origin = origin, .sym = 0, .rot_angle = 2 * M_PI / 3, .dv = {1, 0}},
            {.origin = origin, .sym = 0, .rot_angle = 4 * M_PI / 3, .dv = {-0.5, sqrt(3) / 2}},
            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {-0.5, sqrt(3) / 2}},
            {.origin = origin, .sym = 0, .rot_angle = 0, .dv = {1, 0}},
            {.origin = origin, .sym = 1, .rot_angle = M_PI, .dv = {-1, 0}}
    };

    {
        auto g = build_isometry_graph(figure);
        vector<int> iss;
        iss.reserve(ops.size());
        for (const auto &op: ops) {
            iss.push_back(encode_isometry(op, figure));
        }
        vector<double> part0(figure.s);
        int cnt = (int) ops.size() + 1;
        cout << "running fractional check..." << endl;
        bool ok = check_fractional(g, iss, cnt, part0);
        if (ok) {
            cout << "FOUND FRACTIONAL" << endl << setprecision(4) << fixed;
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
        } else {
            cout << "NOT FOUND FRACTIONAL" << endl;
        }
    }

    for (int factor = 1; factor <= 4; factor++) {
        {
            cout << "factor = " << factor << endl;
            ifstream fin_lattice2("lattices/tr6");
            auto lattice2 = read_lattice(fin_lattice2, 1.0 / factor);
            cout << "converting to lattice" << endl;
            auto figure2 = convert_to_lattice(figure, lattice2);
            cout << "building graph" << endl;
            auto g = build_isometry_graph(figure2);
            vector<int> iss;
            iss.reserve(ops.size());
            for (const auto &op: ops) {
                iss.push_back(encode_isometry(op, figure2));
            }
            cout << "running discrete check..." << endl;
            bool ok = check_discrete(g, iss);
            cout << (ok ? "OK!" : "NOT OK") << endl;
            if (ok) break;
        }
    }
}
