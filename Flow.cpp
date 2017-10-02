#include <vector>
#include <queue>
#include <iostream>

// https://github.com/jaehyunp/stanfordacm/blob/master/code/Dinic.c://github.com/jaehyunp/stanfordacm/blob/master/code/Dinic.cc
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
    std::vector<I> depth, pt;
    Flow(I n) : n(n), Edges(0), Graph(n), depth(n), pt(n) {}
    void add_edge(I u, I v, E cap = 1) {
        Edges.push_back(Edge<I, E>(u, v, cap));
        Graph[u].push_back(Edges.size() - 1);
        Edges.push_back(Edge<I, E>(v, u, cap));
        Graph[v].push_back(Edges.size() - 1);
    }
    bool bfs(I s, I t) {
        std::queue<I> q({s});
        std::fill(depth.begin(), depth.end(), n + 1);
        depth[s] = 0;
        while (!q.empty()) {
            I u = q.front();
            q.pop();
            if (u == t) break;
            for (I next : Graph[u]) {
                Edge<I, E> &edge = Edges[next];
                if (edge.flow < edge.capacity && depth[edge.v] > depth[edge.u] + 1) {
                    depth[edge.v] = depth[edge.u] + 1;
                    q.push(edge.v);
                }
            }
        }
        return depth[t] < n + 1;
    }
    E dfs(I u, I t, E flow = -1) {
        if (u == t || flow == 0) return flow;
        for (I &i = pt[u]; i < Graph[u].size(); i++) {
            Edge<I, E> &for_edge = Edges[Graph[u][i]];
            Edge<I, E> &rev_edge = Edges[Graph[u][i]^1];
            if (depth[for_edge.v] == depth[for_edge.u] + 1) {
                E amount = for_edge.capacity - for_edge.flow;
                if (flow > -1 && amount > flow) {
                    amount = flow;
                }
                if (E pushed = dfs(for_edge.v, t, amount)){
                    for_edge.flow += pushed;
                    rev_edge.flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    E max_flow(I s, I t) {
        E total = 0;
        while (bfs(s, t)) {
            std::fill(pt.begin(), pt.end(), 0);
            while (E flo = dfs(s, t)) {
                total += flo;
            }
        }
        return total;
    }
};
