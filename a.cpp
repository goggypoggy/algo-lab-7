#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>

constexpr int kInf = -1;

struct Edge {
    int dest = kInf;
    int len = 0;
};

struct Node {
    int id = 0;
    int d = 0;
};

struct Graph {
    int V = 0;
    int E = 0;

    std::vector<std::vector<Edge>> edges;
    std::vector<int> d;
    std::vector<int> prev;
    std::vector<bool> visited;
};

struct closer {
    bool operator()(const Node& lhs, const Node& rhs) const {
        if (lhs.d == kInf) {
            return false;
        }
        if (rhs.d == kInf) {
            return true;
        }
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
    std::cout << std::endl
    << "prev:    ";
    for (int i = 0; i < G.V; ++i) {
        std::cout << G.prev[i] << ' ';
    }
    std::cout << std::endl << std::endl;
}

bool Dijkstra(Graph& G, int S, int F) {
    std::set<Node, closer> nodes;

    nodes.insert({S, 0});
    G.d[S] = 0;

    while (!nodes.empty()) {
        Node node = *(nodes.begin());
        nodes.erase(nodes.begin());
        
        G.visited[node.id] = true;

        // DebugOutput(G);

        if (node.id == F) {
            return true;
        }

        for (auto edge : G.edges[node.id]) {
            int new_dist = node.d + edge.len;
            if (G.d[edge.dest] == kInf || new_dist < G.d[edge.dest]) {
                nodes.erase({edge.dest, G.d[edge.dest]});
                G.d[edge.dest] = new_dist;
                G.prev[edge.dest] = node.id;
                nodes.insert({edge.dest, G.d[edge.dest]});
            }
        }
    }

    return false;
}

void OutputPath(Graph& G, int from, int to) {
    std::vector<int> path;
    
    int x = to;
    while (x != from) {
        path.push_back(x);
        x = G.prev[x];
    } 

    path.push_back(from);

    std::reverse(path.begin(), path.end());
    for (int i = 0; i < path.size(); ++i) {
        std::cout << path[i] + 1 << (i == path.size() - 1 ? "" : " ");
    }
}

int main(int, char**) {
    Graph G;
    int S;
    int F;

    std::cin >> G.V >> G.E >> S >> F;
    --S;
    --F;

    G.edges.resize(G.V);
    G.d.resize(G.V);
    G.prev.resize(G.V);
    G.visited.resize(G.V);

    for (int i = 0; i < G.V; ++i) {
        G.edges[i] = {};
        G.d[i] = kInf;
        G.prev[i] = -1;
        G.visited[i] = false;
    }

    for (int i = 0; i < G.E; ++i) {
        int from;
        int to;
        int len;

        std::cin >> from >> to >> len;

        G.edges[from - 1].push_back({to - 1, len});
        G.edges[to - 1].push_back({from - 1, len});
    }

    if (Dijkstra(G, S, F)) {
        std::cout << G.d[F] << std::endl;
        OutputPath(G, S, F);
    } else {
        std::cout << "-1";
    }

    return 0;
}