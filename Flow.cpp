#include <vector>
#include <queue>
#include <iostream>
#include <set>
#include <cstring>
#include <cstdio>

// based on the following
// https://github.com/jaehyunp/stanfordacm/blob/master/code/Dinic.cc

template <class I, class E>
struct Edge {
    I u, v;
    E capacity, flow;
    Edge(I u = 0, I v = 0, E capacity = 1, E flow = 0) :
        u(u), v(v), capacity(capacity), flow(flow) {}
};

template <class I, class E>
class Flow {
public:
    I n;
    std::vector<Edge<I, E>> Edges;
    std::vector<std::vector<I>> Graph;
    I* level;
    I* last_visited;
    E INF;
    Flow(I n) : n(n), Edges(0), Graph(n) {
        Edges.reserve(2 * n - 2);
        memset(&INF, 0x3f, sizeof(E));
        level = new I [n];
        last_visited = new I [n];
    }
    void add_edge(I u, I v, E cap = 1) {
        if (u != v) {
            Edges.push_back(Edge<I, E>(u, v, cap));
            Graph[u].push_back(Edges.size() - 1);
            Edges.push_back(Edge<I, E>(v, u, 0));
            Graph[v].push_back(Edges.size() - 1);
        }
    }
    ~Flow() {
        Edges.clear();
        Graph.clear();
        delete last_visited;
        delete level;
    }
    bool bfs(I s, I t) {
        memset(level, 0x3f, sizeof(I) * n);
        std::queue<I> q({s});
        level[s] = 0;
        while (!q.empty()) {
            I u = q.front(); q.pop();
            if (u == t) break;
            for (I& next : Graph[u]) {
                Edge<I, E>& edge = Edges[next];
                if (edge.flow < edge.capacity && level[edge.v] > level[edge.u] + 1) {
                    level[edge.v] = level[edge.u] + 1;
                    q.push(edge.v);
                }
            }
        }
        return level[t] < n;
    }
    E dfs(I u, I t, E flow) {
        if (u == t || flow == 0) return flow;
        for (I& i = last_visited[u]; i < Graph[u].size(); i++) {
            Edge<I, E>& for_edge = Edges[Graph[u][i]];
            Edge<I, E>& rev_edge = Edges[Graph[u][i]^1];
            if (level[for_edge.v] == level[for_edge.u] + 1 && for_edge.capacity > for_edge.flow) {
                if (E pushed = dfs(for_edge.v, t, std::min(flow, for_edge.capacity - for_edge.flow))) {
                    for_edge.flow += pushed, rev_edge.flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    E max_flow(I s, I t) {
        E total = 0;
        while (bfs(s, t)) {
            memset(last_visited, 0, sizeof(I) * n);
            while (E flo = dfs(s, t, INF)) {
                total += flo;
            }
        }
        return total;
    }
};
