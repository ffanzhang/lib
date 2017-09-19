# union find with path compression and linking by rank
class Union:
    _parent = None
    _rank = None
    _count = 0
    def __init__(self, n):
        self._parent = [i for i in range(n)] 
        self._rank = [0 for i in range(n)]
        self._count = n

    def components(self):
        return self._count

    def find(self, u):
        self._parent[u] = (u if self._parent[u] == u else self.find(self._parent[u]))
        return self._parent[u]

    def connect(self, u, v):
        u = self.find(u)
        v = self.find(v)
        if u == v:
            return
        if self._rank[u] < self._rank[v]:
            self._parent[u] = v
        else:
            self._parent[v] = u
        if self._rank[u] == self._rank[v]:
            self._rank[u] += 1

        self._count -= 1

    def is_connected(self, u, v):
        return self.find(u) == self.find(v)

if __name__ == "__main__":
    n = 100
    u = Union(n)
    for i in range(0, n - 1):
        u.connect(i, i + 1)

    assert u.is_connected(0, n - 1) == True
