#pragma once

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#define SAT_SUCCESS 10
#define SAT_FAIL 20

bool satisfiable(const std::vector<std::vector<int>> &cnf, std::vector<bool> &sol) {
    int n = (int) sol.size();
    int m = (int) cnf.size();
    int fds[2];
    int fds2[2];
    if (pipe(fds) || pipe(fds2)) {
        perror("pipe");
        exit(1);
    }
    int child = fork();
    if (child == 0) {
        close(fds[1]);
        close(fds2[0]);
        dup2(fds[0], 0);
        dup2(fds2[1], 1);
        execlp(KISSAT_EXECUTABLE, "kissat", nullptr);
        exit(-1);
    } else if (child > 0) {
        close(fds[0]);
        close(fds2[1]);
        FILE *fout = fdopen(fds[1], "w");
        fprintf(fout, "p cnf %d %d\n", n, m);
        for (int i = 0; i < m; i++) {
            for (int x: cnf[i]) {
                fprintf(fout, "%d ", x);
            }
            fprintf(fout, " 0\n");
        }
        fclose(fout);
        FILE *fin = fdopen(fds2[0], "r");
        char *line = nullptr;
        ssize_t len;
        size_t cap = 0;
        while ((len = getline(&line, &cap, fin)) > 0) {
            std::string s(line, len);
            if (len > 0 && s[0] == 'v') {
                std::stringstream ss;
                ss << s.substr(1);
                int x;
                while (ss >> x) {
                    if (x > 0) {
                        sol[x - 1] = true;
                    } else if (x < 0) {
                        sol[-x - 1] = false;
                    }
                }
            }
            free(line);
            line = nullptr;
        }
        free(line);
        int status;
        waitpid(child, &status, 0);
        status = WEXITSTATUS(status);
        if (status == SAT_SUCCESS) {
            return true;
        } else if (status == SAT_FAIL) {
            return false;
        } else {
            throw std::runtime_error("kissat exited with code " + std::to_string(status));
        }
    } else {
        perror("fork");
        exit(1);
    }
}
