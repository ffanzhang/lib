#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

template <class I, class E>
struct ShortestPathNode {
  I index;
  E total_cost;
  ShortestPathNode(I index, E total_cost = 0x3f3f3f3f3f3f3f3f)
      : index(index), total_cost(total_cost) {}
  virtual bool operator<(const ShortestPathNode<I, E> &other) const = 0;
};

template <class I, class E>
struct Distance : public ShortestPathNode<I, E> {
  Distance(I index, E total_cost) : ShortestPathNode<I, E>(index, total_cost) {}
  bool operator<(const ShortestPathNode<I, E> &other) const {
    return this->total_cost >= other.total_cost;
  }
};

template <class I, class E>
struct ShortestPath {
  I n;
  ShortestPath(I n) : n(n) {}
  virtual void add_edge(I u, I v, E cost) = 0;
  virtual E shortest_path(I u, I v) = 0;
};

// SPOJ EZDIJKST :AC
template <class I, class E>
struct Dijkstra : public ShortestPath<I, E> {
  I n;
  std::vector<std::unordered_map<I, E>> graph;
  bool *visited;
  E *costs;
  Dijkstra(I n) : n(n), ShortestPath<I, E>(n), graph(n) {
    visited = new bool[n]();
    costs = new E[n]();
  }
  ~Dijkstra() {
    delete visited;
    delete costs;
  }
  void add_edge(I u, I v, E cost) {
    if (graph[u].find(v) == graph[u].end()) {
      graph[u][v] = cost;
    } else {
      graph[u][v] = std::min(graph[u][v], cost);
    }
  }
  E shortest_path(I u, I v) {
    memset(visited, 0, sizeof(bool) * n);
    memset(costs, 0x3f, sizeof(E) * n);
    std::priority_queue<Distance<I, E>> pq;
    pq.push(Distance<I, E>(u, 0));
    while (!pq.empty()) {
      auto cur_node = pq.top();
      pq.pop();
      if (visited[cur_node.index]) continue;
      visited[cur_node.index] = true;
      for (auto child : graph[cur_node.index]) {
        auto child_index = child.first;
        auto child_cost = child.second;
        if (!visited[child_index] &&
            costs[child_index] > cur_node.total_cost + child_cost) {
          costs[child_index] = cur_node.total_cost + child_cost;
          pq.push(Distance<I, E>(child_index, costs[child_index]));
        }
      }
    }
    return costs[v];
  }
};
