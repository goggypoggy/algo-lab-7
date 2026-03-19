#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstdint>

constexpr int64_t kInf = -1;

struct Graph {
    int V = 0;

    std::vector<std::vector<int64_t>> d;
};

void DebugOutput(Graph& G) {
    std::cout
    << "id:      ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << i << ' ';
    }
    std::cout << std::endl
    << "d:\n";
    for (int i = 0; i < G.V; ++i) {
        for (int j = 0; j < G.V; ++j) {
            if (G.d[i][j] == kInf) {
                std::cout << "* ";
            } else {
                std::cout << G.d[i][j] << ' ';
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void FloydWarshall(Graph& G) {
    for (int i = 0; i < G.V; ++i) {
        for (int u = 0; u < G.V; ++u) {
            for (int v = 0; v < G.V; ++v) {
                if (G.d[u][i] == kInf || G.d[i][v] == kInf) {
                    continue;
                }
                int64_t new_dist = G.d[u][i] + G.d[i][v];
                if (G.d[u][v] == kInf || new_dist < G.d[u][v]) {
                    G.d[u][v] = new_dist;
                }
            }
        }
        // DebugOutput(G);
    }
}

int main(int, char**) {
    Graph G;

    std::cin >> G.V;

    G.d.resize(G.V);
    
    for (int i = 0; i < G.V; ++i) {
        G.d[i] = {};
        G.d[i].resize(G.V, kInf);
        G.d[i][i] = 0;
    }
    
    for (int from = 0; from < G.V; ++from) {
        for (int to = 0; to < G.V; ++to) {
            int64_t len;
            std::cin >> len;
            G.d[from][to] = len;
        }
    }
    
    FloydWarshall(G);
    // DebugOutput(G);

    int64_t rad = INT64_MAX;
    int64_t diam = -1;

    for (int i = 0; i < G.V; ++i) {
        int64_t ecc = G.d[i][0];
        for (int j = 1; j < G.V; ++j) {
            ecc = std::max(ecc, G.d[i][j]);
        }

        rad = std::min(rad, ecc);
        diam = std::max(diam, ecc);
    }

    std::cout << diam << std::endl << rad;

    return 0;
}