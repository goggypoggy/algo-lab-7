#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstdint>

constexpr int64_t kInf = INT64_MAX;

struct Edge {
    int from = 0;
    int to = 0;

    int64_t len = 0;
};

struct Node {
    int id = 0;
    int64_t d = 0;
};

struct Graph {
    int V = 0;
    int E = 0;

    std::vector<std::vector<Edge>> edges;
    std::vector<int64_t> d;
    std::vector<char> status;
};

void DebugOutput(Graph& G) {
    std::cout
    << "id:      ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << i << ' ';
    }
    std::cout << std::endl
    << "d:       ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << G.d[i] << ' ';
    }
    std::cout << std::endl << std::endl;
}

void BellmanFord(Graph& G, int S) {
    std::vector<Edge> edges;
    edges.resize(G.E);

    for (int i = 0; i < G.V; ++i) {
        G.d[i] = kInf;
    }
    G.d[S] = 0;

    for (int i = 0; i < G.V; ++i) {
        for (auto e : G.edges[i]) {
            edges.push_back(e);
        }
    }

    DebugOutput(G);

    for (int i = 0; i < G.V; ++i) {
        for (auto e : edges) {
            if (G.d[e.from] == kInf) {
                continue;
            }

            int new_dist = G.d[e.from] + e.len;
            if (G.d[e.to] == kInf || new_dist < G.d[e.to]) {
                G.d[e.to] = new_dist;
            }
        }
        
    }
}

int main(int, char**) {
    Graph G;
    int S;

    std::cin >> G.V >> G.E >> S;
    --S;

    G.edges.resize(G.V);
    G.d.resize(G.V);
    
    for (int i = 0; i < G.V; ++i) {
        G.edges[i] = {};
        G.d[i] = kInf;
    }
    
    for (int i = 0; i < G.E; ++i) {
        int from;
        int to;
        int len;
        
        std::cin >> from >> to >> len;
        
        G.edges[from - 1].push_back({from - 1, to - 1, len});
    }
    
    BellmanFord(G, S);

    return 0;
}