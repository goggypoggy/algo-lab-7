#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstdint>

struct Edge {
    int from = 0;
    int to = 0;

    float r = 0;
    float c = 0;
};

struct Graph {
    int V = 0;
    int E = 0;

    std::vector<std::vector<Edge>> edges;
    std::vector<float> s;
};

void DebugOutput(Graph& G) {
    std::cout
    << "id:      ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << i << ' ';
    }
    std::cout << std::endl
    << "s:       ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << G.s[i] << ' ';
    }
    std::cout << std::endl << std::endl;
}

void EdgeRelaxation(Graph& G, std::vector<Edge>& edges) {
    auto prev_s = G.s;
    for (auto e : edges) {
        if (prev_s[e.from] == -INFINITY) {
            continue;
        }

        float new_sum = prev_s[e.from] * e.r - e.c;
        if (new_sum > prev_s[e.to]) {
            G.s[e.to] = new_sum;
        }
    }
}

bool BellmanFord(Graph& G, int S, float start_sum) {
    std::vector<Edge> edges;

    for (int i = 0; i < G.V; ++i) {
        for (auto e : G.edges[i]) {
            edges.push_back(e);
        }
    }

    // DebugOutput(G);
    
    for (int i = 0; i < G.V - 1; ++i) {
        EdgeRelaxation(G, edges);
        // DebugOutput(G);
    }

    // std::cout << "\nV-th relaxation:\n";

    auto prev_s = G.s;
    EdgeRelaxation(G, edges);
    // DebugOutput(G);

    for (int i = 0; i < G.V; ++i) {
        if (prev_s[i] < G.s[i]) {
            return true;
        }
    }

    return false;
}

int main(int, char**) {
    Graph G;
    int S;
    float start_sum;

    std::cin >> G.V >> G.E >> S >> start_sum;
    --S;

    G.edges.resize(G.V);
    G.s.resize(G.V);
    
    for (int i = 0; i < G.V; ++i) {
        G.edges[i] = {};
        G.s[i] = -INFINITY;
    }
    G.s[S] = start_sum;
    
    for (int i = 0; i < G.E; ++i) {
        int from;
        int to;
        float r_ab, c_ab, r_ba, c_ba;
        
        std::cin >> from >> to >> r_ab >> c_ab >> r_ba >> c_ba;
        
        G.edges[from - 1].push_back({from - 1, to - 1, r_ab, c_ab});
        G.edges[to - 1].push_back({to - 1, from - 1, r_ba, c_ba});
    }
    
    if (BellmanFord(G, S, start_sum)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }

    return 0;
}