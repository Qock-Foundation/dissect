#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include "geom.h"
#include "glpk.h"
#include "sat.h"

using namespace Geom;

bool check_discrete(const std::vector<std::vector<int>> &g, std::vector<int> &iss) {
    int n = (int) g.size();
    int k = (int) iss.size() + 1;
    std::vector<std::vector<int>> sets;
    for (int i = 0; i < n; i++) {
        std::vector<int> cur_set = {i};
        cur_set.reserve(k);
        for (int is: iss) {
            cur_set.push_back(g[i][is]);
        }
        std::set<int> elems(cur_set.begin(), cur_set.end());
        elems.erase(-1);
        if (elems.size() == k) {
            sets.push_back(cur_set);
        }
    }
    int m = (int) sets.size();
    std::vector<std::vector<int>> cnf;
    for (int i = 0; i < n; i++) {
        cnf.emplace_back();
        for (int j = 0; j < m; j++) {
            if (find(sets[j].begin(), sets[j].end(), i) != sets[j].end()) {
                cnf.back().push_back(1 + j);
            }
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = i + 1; j < m; j++) {
            bool intersect = false;
            for (int elem: sets[i]) {
                if (find(sets[j].begin(), sets[j].end(), elem) != sets[j].end()) {
                    intersect = true;
                    break;
                }
            }
            if (intersect) {
                cnf.push_back({-(1 + i), -(1 + j)});
            }
        }
    }
    std::vector<bool> sol(m);
    return satisfiable(cnf, sol);
}

bool check_fractional(const std::vector<std::vector<int>> &g, std::vector<int> &iss, int cnt,
                      std::vector<double> &part0) {
    int n = (int) g.size();
    int k = (int) iss.size() + 1;
    std::vector<std::vector<int>> sets;
    std::vector<bool> used(g.size());
    for (int i = 0; i < n; i++) {
        std::vector<int> cur_set = {i};
        cur_set.reserve(k);
        for (int is: iss) {
            cur_set.push_back(g[i][is]);
        }
        if (std::find(cur_set.begin(), cur_set.end(), -1) == cur_set.end()) {
            sets.push_back(cur_set);
            for (int elem: cur_set) {
                used[elem] = true;
            }
        }
    }
    int m = (int) sets.size();
    if (std::count(used.begin(), used.end(), true) < cnt * k) {
        return false;
    }

    // Init
    glp_prob *lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MAX);

    // Adding rows
    glp_add_rows(lp, n);
    for (int i = 0; i < n; i++) {
        glp_set_row_bnds(lp, 1 + i, GLP_UP, 1.0, 1.0);
    }

    // Adding columns
    glp_add_cols(lp, m);
    for (int j = 0; j < m; j++) {
        glp_set_col_kind(lp, 1 + j, GLP_CV);
        glp_set_col_bnds(lp, 1 + j, GLP_LO, 0.0, 0.0);
        glp_set_obj_coef(lp, 1 + j, 1.0);
    }

    // Building matrix
    std::vector<int> is, js;
    std::vector<double> vs;
    for (int j = 0; j < m; j++) {
        std::map<int, int> cnt_occ;
        for (int i: sets[j]) cnt_occ[i]++;
        for (auto [i, occ]: cnt_occ) {
            is.push_back(1 + i);
            js.push_back(1 + j);
            vs.push_back((double) occ);
        }
    }
    glp_load_matrix(lp, (int) vs.size(), is.data() - 1, js.data() - 1, vs.data() - 1);

    // Solving
    glp_smcp parm;
    glp_init_smcp(&parm);
    parm.msg_lev = GLP_MSG_OFF;
    parm.presolve = GLP_ON;
    int status = glp_simplex(lp, &parm);
    double ans = glp_get_obj_val(lp);
    for (int j = 0; j < m; j++) {
        part0[sets[j][0]] = glp_get_col_prim(lp, 1 + j);
    }

    glp_delete_prob(lp);
    return status == 0 && ans > cnt - eps;
}
