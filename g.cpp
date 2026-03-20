#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>

constexpr uint64_t kInf = -1;

struct Node {
    uint64_t id = 0;
    uint64_t d = 0;
};

struct Graph {
    int V = 0;
    int E = 0;

    std::vector<uint64_t> d;
    std::vector<bool> visited;
};

struct closer {
    bool operator()(const Node& lhs, const Node& rhs) const {
        if (lhs.d == rhs.d) {
            return lhs.id < rhs.id;
        }
        return lhs.d < rhs.d;
    }
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
    std::cout << std::endl
    << "visited: ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << G.visited[i] << ' ';
    }
    std::cout << std::endl << std::endl;
}

void Dijkstra(Graph& G, uint64_t min, std::vector<uint64_t>& edges) {
    std::set<Node, closer> nodes;

    nodes.insert({0, 0});

    while (!nodes.empty()) {
        Node node = *(nodes.begin());
        nodes.erase(nodes.begin());
        
        G.visited[node.id] = true;

        for (auto edge : edges) {
            uint64_t to = (node.id + edge) % min;
            uint64_t new_dist = node.d + edge;

            if (new_dist < G.d[to]) {
                nodes.erase({to, G.d[to]});
                G.d[to] = new_dist;
                nodes.insert({to, G.d[to]});
            }
        }
    }
}

int main(int, char**) {
    Graph G;
    
    uint64_t h;
    std::vector<uint64_t> edges;

    std::cin
    >> h;
    --h;

    for (int i = 0; i < 3; ++i) {
        uint64_t edge;
        std::cin >> edge;

        edges.push_back(edge);
    }

    uint64_t min = edges[0];
    for (int i = 1; i < 3; ++i) {
        min = std::min(min, edges[i]);
    }

    // std::cout << min << std::endl;
    // for (auto e : edges) {
    //     std::cout << e << ' ';
    // }
    // std::cout << std::endl;

    G.V = min;

    G.d.resize(G.V);
    G.visited.resize(G.V);

    for (uint64_t i = 0; i < G.V; ++i) {
        G.d[i] = kInf;
        G.visited[i] = false;
    }
    G.d[0] = 0;
    
    // DebugOutput(G);

    Dijkstra(G, min, edges);

    // DebugOutput(G);

    uint64_t count = 0;

    for (uint64_t i = 0; i < G.V; ++i) {
        if (G.d[i] <= h) {
            ++count;
            count += (h - G.d[i]) / min;
        }
    }

    std::cout << count;

    return 0;
}