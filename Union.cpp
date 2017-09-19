#include <bits/stdc++.h>
#include <cassert>

// union find with path compression and linking by rank
template <class T>
class Union {
    std::vector<T> parent;
    std::vector<T> rank;
    T count;
public:
    Union(T n) {
        parent.resize(n);
        rank.resize(n);
        count = n;
        std::fill(rank.begin(), rank.end(), 0);
        for (T i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    ~Union() {
        parent.clear();
        rank.clear();
    }
    T components() {
        return count;
    }
    inline T find(T u) {
        return parent[u] = (parent[u] == u ? u : find(parent[u]));
    }
    void connect(T u, T v) {
        u = find(u), v = find(v);
        if (u == v) return;
        if (rank[u] < rank[v]) {
            parent[u] = v;
        } else {
            parent[v] = u;
        }
        if (rank[u] == rank[v]) {
            rank[u]++;
        }
        count--;
    }
    inline bool is_connected(T u, T v) {
        return find(u) == find(v);
    }
};

int main() {
    long long n = 100;
    Union<long long> u(100);
    for (long long i = 0; i < n - 1; i++) {
        u.connect(i, i + 1);
    }
    assert(u.is_connected(0, n - 1) == true);
    assert(u.components() == 1);
    return 0;
}
