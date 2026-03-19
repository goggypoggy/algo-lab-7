#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstdint>

constexpr int64_t kInf = INT64_MIN;

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
    std::vector<bool> visited;
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
        if (G.d[i] == kInf) {
            std::cout << "* ";
        } else {
            std::cout << G.d[i] << ' ';
        }
    }
    std::cout << std::endl
    << "visited: ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << G.visited[i] << ' ';
    }
    std::cout << std::endl << std::endl;
}

void DFS_Visit(Graph& G, int node) {
    G.visited[node] = true;
    for (auto edge : G.edges[node]) {
        if (!G.visited[edge.to]) {
            DFS_Visit(G, edge.to);
        }
    }
}

void EdgeRelaxation(Graph& G, std::vector<Edge>& edges) {
    for (auto e : edges) {
        if (G.d[e.from] == kInf) {
            continue;
        }

        int64_t new_dist = G.d[e.from] + e.len;
        if (new_dist > G.d[e.to]) {
            G.d[e.to] = new_dist;
        }
    }
}

void BellmanFord(Graph& G, int S) {
    std::vector<Edge> edges;

    for (int i = 0; i < G.V; ++i) {
        G.d[i] = kInf;
    }
    G.d[S] = 0;

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

    auto prev_d = G.d;
    EdgeRelaxation(G, edges);
    // DebugOutput(G);

    if (prev_d != G.d) {
        for (int i = 0; i < G.V; ++i) {
            if (prev_d[i] != G.d[i] && !G.visited[i]) {
                DFS_Visit(G, i);
            }
        }
    }

    // DebugOutput(G);

    for (int i = 0; i < G.V; ++i) {
        if (G.visited[i]) {
            G.status[i] = '+';
        } else if (G.d[i] == kInf) {
            G.status[i] = '*';
        }
    }
}

int main(int, char**) {
    Graph G;

    std::cin >> G.V >> G.E;

    G.edges.resize(G.V);
    G.d.resize(G.V);
    G.status.resize(G.V);
    G.visited.resize(G.V);
    
    for (int i = 0; i < G.V; ++i) {
        G.edges[i] = {};
        G.d[i] = kInf;
        G.status[i] = '#';
        G.visited[i] = false;
    }
    
    for (int i = 0; i < G.E; ++i) {
        int from;
        int to;
        int64_t len;
        
        std::cin >> from >> to >> len;
        
        G.edges[from - 1].push_back({from - 1, to - 1, len});
    }
    
    BellmanFord(G, 0);

    int n = G.V - 1;

    if (G.status[n] == '#') {
        std::cout << G.d[n];
    } else if (G.status[n] == '+') {
        std::cout << ":)";
    } else if (G.status[n] == '*') {
        std::cout << ":(";
    }

    return 0;
}