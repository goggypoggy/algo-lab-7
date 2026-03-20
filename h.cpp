#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>

using flt = double;

struct Point {
    flt x = 0;
    flt y = 0;
};

struct Edge;

template<typename edge_type>
struct graph_type {
    std::vector<Point> v;

    std::vector<std::deque<edge_type>> e;
    std::vector<std::deque<edge_type>> e_incoming;

    graph_type(int V) {
        v.resize(V, {0, 0});
        e.resize(V, {});
        e_incoming.resize(V, {});
    }
};

using Graph = graph_type<Edge>;

bool RightOf(const Point& X, const Edge& e);
bool LeftOf(const Point& X, const Edge& e);

struct Edge {
    Graph* Gptr;
    int from_i;
    int to_i;
    flt lenSq = 0;

    bool operator==(const Edge& other) const {
        return from_i == other.from_i && to_i == other.to_i;
    }

    Edge Sym() const {
        return {Gptr, to_i, from_i};
    }

    Point from() const {
        return Gptr->v[from_i];
    }

    Point to() const {
        return Gptr->v[to_i];
    }

    flt len2() {
        flt x_delta = to().x - from().x;
        flt y_delta = to().y - from().y;

        return x_delta * x_delta + y_delta * y_delta;
    }

    flt len() {
        return std::sqrt(len2());
    }

    Edge Dprev() const {
        for (auto it = Gptr->e_incoming[to_i].cbegin(); it != Gptr->e_incoming[to_i].cend() - 1; ++it) {
            if (*(it + 1) == *this) {
                return *it;
            }
        }
        return Gptr->e_incoming[to_i].back();
    }

    Edge Lnext() const {
        return Dprev().Sym();
    }

    Edge Dnext() const {
        for (auto it = Gptr->e_incoming[to_i].crbegin(); it != Gptr->e_incoming[to_i].crend() - 1; ++it) {
            if (*(it + 1) == *this) {
                return *it;
            }
        }
        return *Gptr->e_incoming[to_i].begin();
    }

    Edge Rprev() const {
        return Dnext().Sym();
    }

    Edge Onext() const {
        for (auto it = Gptr->e[from_i].cbegin(); it != Gptr->e[from_i].cend() - 1; ++it) {
            if (*(it + 1) == *this) {
                return *it;
            }
        }
        return Gptr->e[from_i].back();
    }

    Edge Oprev() const {
        for (auto it = Gptr->e[from_i].crbegin(); it != Gptr->e[from_i].crend() - 1; ++it) {
            if (*(it + 1) == *this) {
                return *it;
            }
        }
        return *Gptr->e[from_i].begin();
    }
};

void Output(Graph& G) {
    printf("vertices:\n");
    for (int i = 0; i < G.v.size(); ++i) {
        printf("%i: %f %f\n", i, G.v[i].x, G.v[i].y);
    }
    printf("\noutgoing edges:\n");
    for (int i = 0; i < G.e.size(); ++i) {
        printf("%i -> x:\n", i);
        for (int j = 0; j < G.e[i].size(); ++j) {
            printf("\t%i -> %i\n", i, G.e[i][j].to_i);
        }
    }
    printf("\nincoming edges:\n");
    for (int i = 0; i < G.e_incoming.size(); ++i) {
        printf("x -> %i:\n", i);
        for (int j = 0; j < G.e_incoming[i].size(); ++j) {
            printf("\t%i -> %i\n", G.e_incoming[i][j].from_i, i);
        }
    }
}


flt Det3x3(
    flt a0, flt a1, flt a2,
    flt b0, flt b1, flt b2,
    flt c0, flt c1, flt c2
) {
    return
          a0 * (b1 * c2 - c1 * c2)
        - b0 * (a1 * c2 - c1 * a2)
        + c0 * (a1 * b2 - b1 * b2);
}

flt Det4x4(
    flt a0, flt a1, flt a2, flt a3,
    flt b0, flt b1, flt b2, flt b3,
    flt c0, flt c1, flt c2, flt c3,
    flt d0, flt d1, flt d2, flt d3
) {
    return
          a0 * Det3x3(
            b1, b2, b3,
            c1, c2, c3,
            d1, d2, d3
        )
        - b0 * Det3x3(
            a1, a2, a3,
            c1, c2, c3,
            d1, d2, d3
        )
        + c0 * Det3x3(
            a1, a2, a3,
            b1, b2, b3,
            d1, d2, d3
        )
        - d0 * Det3x3(
            a1, a2, a3,
            b1, b2, b3,
            c1, c2, c3
        );
}


// checks if ABC is a counter-clockwise directed triangle
/*
|xA yA 1|
|xB yB 1| > 0
|xC yC 1|
*/
bool CCW(const Point& A, const Point& B, const Point& C) {
    return
        Det3x3(
            A.x, A.y, 1,
            B.x, B.y, 1,
            C.x, C.y, 1
        ) > static_cast<flt>(0);
}

bool RightOf(const Point& X, const Edge& e) {
    return CCW(X, e.to(), e.from());
}

bool LeftOf(const Point& X, const Edge& e) {
    return CCW(X, e.from(), e.to());
}

bool Valid(const Edge& e, const Edge& basel) {
    return RightOf(e.to(), basel);
}

bool InCircle(
    const Point& A,
    const Point& B,
    const Point& C,
    const Point& D
) {
    return Det4x4(
        A.x, A.y, A.x * A.x + A.y + A.y, 1,
        B.x, B.y, B.x * B.x + B.y + B.y, 1,
        C.x, C.y, C.x * C.x + C.y + C.y, 1,
        D.x, D.y, D.x * D.x + D.y + D.y, 1
    ) > static_cast<flt>(0);
}

Edge AddEdge(Graph& G, int a, int b) {
    Edge e = {&G, a, b};

    auto start_p = e.from();
    auto end_p = e.to();

    int i;
    for (i = 0; i < G.e[a].size(); ++i) {
        if (LeftOf(end_p, G.e[a][i])) {
            break;
        }
    }
    G.e[a].insert(G.e[a].cbegin() + i, e);

    int j;
    for (j = 0; j < G.e_incoming[b].size(); ++j) {
        if (LeftOf(start_p, G.e_incoming[b][j])) {
            break;
        }
    }
    G.e_incoming[b].insert(G.e_incoming[b].cbegin() + j, e);

    return e;
};

Edge MakeEdge(Graph& G, int a, int b) {
    AddEdge(G, b, a);

    return AddEdge(G, a, b);
}

void DeleteEdge(Edge e) {
    std::erase(e.Gptr->e_incoming[e.to_i], e);
    std::erase(e.Gptr->e[e.from_i], e);
}

// [first; last)
// returns [le, re]
std::pair<Edge, Edge> BuildDeloneOnSub(Graph& G, int first, int last) {
    // printf("> DeloneOnSub(%i, %i)\n", first, last);

    if (last - first == 2) {
        Edge e = MakeEdge(G, first, first + 1);
        return {e, e.Sym()};
    }
    if (last - first == 3) {
        Point& s1 = G.v[first];
        Point& s2 = G.v[first + 1];
        Point& s3 = G.v[first + 2];

        Edge a = MakeEdge(G, first, first + 1);
        Edge b = MakeEdge(G, first + 1, first + 2);
        if (CCW(s1, s2, s3)) {
            Edge c = MakeEdge(G, b.to_i, a.from_i);
            return {a, b.Sym()};
        }
        if (CCW(s1, s3, s2)) {
            Edge c = MakeEdge(G, b.to_i, a.from_i);
            return {c.Sym(), c};
        }
        return {a, b.Sym()};
    }

    int mid = (first + last) / 2;

    auto ld = BuildDeloneOnSub(G, first, mid);
    auto ldo = ld.first;
    auto ldi = ld.first;
    
    auto rd = BuildDeloneOnSub(G, mid, last);
    auto rdi = rd.first;
    auto rdo = rd.second;
    
    while (1) {
        if (LeftOf(rdi.from(), ldi)) {
            ldi = ldi.Lnext();
        } else if (RightOf(ldi.from(), rdi)) {
            rdi = rdi.Rprev();
        } else {
            break;
        }
    }

    Edge basel = MakeEdge(G, rdi.from_i, ldi.from_i);

    if (ldi.from_i == ldo.from_i) {
        ldo = basel.Sym();
    }
    if (rdi.from_i == rdo.from_i) {
        rdo = basel;
    }

    while (1) {
        Edge lcand = basel.Sym().Onext();

        if (Valid(lcand, basel)) {
            while (InCircle(
                basel.to(), basel.from(), lcand.to(), lcand.Onext().to()
            )) {
                Edge t = lcand.Onext();
                DeleteEdge(lcand);
                lcand = t;
            }
        }

        Edge rcand = basel.Oprev();

        if (Valid(rcand, basel)) {
            while (InCircle(
                basel.to(), basel.from(), rcand.to(), rcand.Oprev().to()
            )) {
                Edge t = rcand.Oprev();
                DeleteEdge(rcand);
                rcand = t;
            }
        }

        if (!Valid(lcand, basel) && !Valid(rcand, basel)) {
            break;
        }

        if (!Valid(lcand, basel) ||
            (
                Valid(rcand, basel) &&
                InCircle(lcand.to(), lcand.from(), rcand.from(), rcand.to())
            )
        ) {
            basel = MakeEdge(G, rcand.to_i, basel.to_i);
        } else {
            basel = MakeEdge(G, basel.from_i, lcand.to_i);
        }
    }

    return {ldo, rdo};
}

// lefter :)
struct lefter {
    bool operator()(const Point& lhs, const Point& rhs) const {
        if (lhs.x == rhs.x) {
            return lhs.y < rhs.y;
        }
        return lhs.x < rhs.x;
    }
};

void BuildDelone(Graph& G) {
    std::sort(G.v.begin(), G.v.end(), lefter());

    if (G.v.size() <= 1) {
        return;
    }

    BuildDeloneOnSub(G, 0, G.v.size());
}

struct shorter {
    bool operator()(const Edge& lhs, const Edge& rhs) const {
        if (lhs.lenSq == rhs.lenSq) {
            if (lhs.from_i == rhs.from_i) {
                return lhs.to_i < rhs.to_i;
            }
            return lhs.from_i < rhs.from_i;
        }
        return lhs.lenSq < rhs.lenSq;
    }
};

// Union-Find (Disjoint Set Union)
struct DSU {
    std::vector<int> parent, rank;

    DSU(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]); // path compression
        return parent[x];
    }

    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (rank[x] < rank[y]) std::swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
        return true;
    }
};

std::vector<Edge> BuildMST(Graph& G) {
    int V = G.v.size();

    std::vector<Edge> edge_list;

    for (int i = 0; i < G.v.size(); ++i) {
        for (int j = 0; j < G.e[i].size(); ++j) {
            edge_list.push_back(G.e[i][j]);
        }
    }

    for (auto& e : edge_list) {
        e.lenSq = e.len2();
    }

    std::sort(edge_list.begin(), edge_list.end(), shorter());

    DSU dsu(V);
    std::vector<Edge> mst_edges;

    for (const Edge& edge : edge_list) {
        if (dsu.unite(edge.from_i, edge.to_i)) {
            mst_edges.push_back(edge);
            if (mst_edges.size() == V - 1)
                break;
        }
    }

    return mst_edges;
}

flt WeightOfMST(std::vector<Edge>& edges) {
    flt weight = 0;

    for (auto e : edges) {
        weight += sqrt(e.lenSq);
    }

    return weight;
}

int main(int, char**) {
    int N;
    std::cin >> N;

    Graph T(N);

    for (int i = 0; i < N; ++i) {
        std::cin >> T.v[i].x >> T.v[i].y;
    }

    BuildDelone(T);

    Output(T);

    // std::vector<Edge> mst_edges = BuildMST(T);

    // std::cout << WeightOfMST(mst_edges);

    // std::cout << WeightOfMST(mst_edges) << std::endl;

    // for (auto e : mst_edges) {
    //     std::cout << e.from_i + 1 << ' ' << e.to_i + 1 << std::endl;
    // }

    // Output(T);


    // auto test = T.e[2][0];
    // printf("\n\ntest: %i -> %i\n", test.from_i, test.to_i);
    // test = test.Oprev();
    // printf("\n\ntest: %i -> %i\n", test.from_i, test.to_i);

    // Graph G = BuildMST(T);

    return 0;
}