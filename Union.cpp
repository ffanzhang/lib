// union find with path compression and naive merge
template <class E>
struct Union {
  E *parent;
  E count;
  Union(E n) {
    count = n;
    parent = new E[n];
    for (E i = 0; i < n; i++) {
      parent[i] = i;
    }
  }
  ~Union() { delete parent; }
  inline E components() { return count; }
  inline E find(E u) {
    return parent[u] = (parent[u] == u ? u : find(parent[u]));
  }
  inline bool is_connected(E u, E v) { return find(u) == find(v); }
  virtual void merge(E u, E v) {
    u = find(u);
    v = find(v);
    if (u == v) return;
    parent[u] = v;
    count--;
  }
};

// with rank compression
template <class E>
class UnionByRank : public Union<E> {
  E *rank;

 public:
  UnionByRank(E n) : Union<E>(n) { rank = new E[n](); }
  ~UnionByRank() { delete rank; }
  void merge(E u, E v) {
    u = this->find(u);
    v = this->find(v);
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
template <class E>
class UnionBySize : public Union<E> {
  E *size;

 public:
  UnionBySize(E n) : Union<E>(n) {
    size = new E[n]();
    for (E i = 0; i < n; i++) {
      size[i] = 1;
    }
  }
  ~UnionBySize() { delete size; }
  void merge(E u, E v) {
    u = this->find(u);
    v = this->find(v);
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
