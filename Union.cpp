#include <bits/stdc++.h>
#include <cassert>

// union find with path compression and naive merge
template <class T>
class Union {
public:
    std::vector<T> parent;
    int count = 0;
    Union(T n) {
        count = n;
        parent.resize(n);
        for (T i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    ~Union() {
        parent.clear();
    }
    inline T components() {
        return count;
    }
    inline T find(T u) {
        return parent[u] = (parent[u] == u ? u : find(parent[u]));
    }
    inline bool is_connected(T u, T v) {
        return find(u) == find(v);
    }
    virtual void merge(T u, T v) {
        u = find(u); v = find(v);
        if (u == v) return;
        parent[u] = v;
        count--;
    }
};

// with rank compression 
template <class T>
class UnionByRank : public Union<T> {
    std::vector<T> rank;
public:
    UnionByRank(T n) : Union<T>(n) {
        rank.resize(n);
        std::fill(rank.begin(), rank.end(), 0);
    }
    ~UnionByRank() {
        rank.clear();
    }
    void merge(T u, T v) {
        u = this->find(u); v = this->find(v);
        if (u == v) return;
        if (rank[u] < rank[v]) {
            this->parent[u] = v;
        } else {
            this->parent[v] = u;
        }
        if (rank[u] == rank[v]) {
            rank[u]++;
        }
        this->count--;
    }
};

// with size compression
template <class T>
class UnionBySize : public Union<T> {
    std::vector<T> size;
public:
    UnionBySize(T n) : Union<T>(n) {
        size.resize(n);
        std::fill(size.begin(), size.end(), 1);
    }
    ~UnionBySize() {
        size.clear();
    }
    void merge(T u, T v) {
        u = this->find(u); v = this->find(v);
        if (u == v) return;
        if (size[u] < size[v]) {
            this->parent[u] = v;
            size[v] += size[u];
        } else {
            this->parent[v] = u;
            size[u] += size[v];
        }
        this->count--;
    }
};

void test(Union<long long> *u, long long n) {
    assert(u->components() == n);
    for (long long i = 0; i < n - 1; i++) {
        u->merge(i, i + 1);
    }
    assert(u->is_connected(0, n - 1) == true);
    assert(u->is_connected(n / 2, 0) == true);
    assert(u->components() == 1);

}

int main() {
    long long n = 100;
    Union<long long> *u = new Union<long long>(n);
    Union<long long> *v = new UnionByRank<long long>(n);
    Union<long long> *w = new UnionBySize<long long>(n);
    test(u, n);
    test(v, n);
    test(w, n);
    return 0;
}
