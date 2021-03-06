#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>

#include "BigIntegerTest.cpp"
#include "Flow.cpp"
#include "IO.cpp"
#include "Matrix.cpp"
#include "SegmentTree.cpp"
#include "ShortestPath.cpp"
#include "Union.cpp"
#include "intm.cpp"

using namespace std;

template <class I, class E>
void testSegmentTreeHelper(vector<E> &elements, I n) {
  for (I i = 0; i < n; i++) {
    elements.push_back(i + 1);
  }
  unique_ptr<MyMax<E>> mymax(new MyMax<E>());
  SegmentTree<I, E> t1(elements, *mymax);
  assert(t1.query(0, 100) == 100);
  assert(t1.query(0, 50) == 50);
  assert(t1.query(50, 100) == 100);

  unique_ptr<MySum<E>> mysum(new MySum<E>());
  SegmentTree<I, E> t2(elements, *mysum);

  assert(t2.query(0, 100) == 5050);
  assert(t2.query(0, 50) == (50 * 51) / 2);
  for (int i = 1; i <= 100; i++) {
    assert(t2.query(0, i) == i * (i + 1) / 2);
  }
}

void testSegmentTree() {
  std::vector<int> elements0;
  std::vector<long long> elements1;
  std::vector<__int128> elements2;
  testSegmentTreeHelper(elements0, 100);
  testSegmentTreeHelper(elements1, 100);
  testSegmentTreeHelper(elements2, 100);

  SegmentTreeLazy<int, long long> st1(20);

  st1.modify(0, 10, 1);
  st1.modify(3, 6, 1);
  st1.modify(4, 7, 1);
  st1.modify(4, 8, 1);
  st1.modify(1, 8, 1);
  st1.modify(5, 6, 1);

  assert(st1.query(1, 2) == 2);
  assert(st1.query(5, 6) == 6);
  assert(st1.query(9, 10) == 1);
  assert(st1.query(4, 5) == 5);
  assert(st1.query(9, 10) == 1);
  assert(st1.query(6, 7) == 4);
  assert(st1.query(7, 8) == 3);
}

template <class I, class T>
void testUnionHelper(Union<T> *u, I n) {
  assert(n > 9);
  assert(u->components() == n);
  u->merge(1, 2);
  assert(u->is_connected(1, 2) == true);
  u->merge(3, 4);
  assert(u->is_connected(3, 4) == true);
  assert(u->is_connected(1, 3) == false);
  u->merge(1, 3);
  assert(u->is_connected(1, 4) == true);
  assert(u->is_connected(1, 5) == false);
  for (I i = 0; i < n - 1; i++) {
    u->merge(i, i + 1);
  }
  assert(u->is_connected(0, n - 1) == true);
  assert(u->is_connected(n / 2, 0) == true);
  assert(u->components() == 1);
  vector<int> v;
}

void testUnion() {
  int n = 10 + rand() % 100;

  Union<char> *u = new Union<char>(n);
  Union<int> *v = new UnionByRank<int>(n);
  Union<long long> *w = new UnionBySize<long long>(n);
  Union<__int128> *x = new Union<__int128>(n);

  testUnionHelper(u, n);
  testUnionHelper(v, n);
  testUnionHelper(w, n);
  testUnionHelper(x, n);
}

void testFlow() {
  Flow<int, int> flo(10);
  flo.add_edge(0, 1, 3);
  flo.add_edge(1, 2, 3);
  flo.add_edge(0, 2, 3);
  flo.add_edge(0, 4, 3);
  flo.add_edge(4, 2, 3);
  assert(flo.max_flow(0, 2) == 9);
}

void testShortestPath() {
  Dijkstra<int, int> d(10);
  d.add_edge(1, 2, 5);
  d.add_edge(2, 3, 7);
  assert(d.shortest_path(1, 3) == 12);
  d.add_edge(1, 2, 4);
  d.add_edge(1, 3, 7);
  d.add_edge(2, 3, 1);
  assert(d.shortest_path(1, 3) == 5);
}

std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

void testIO() {
  freopen("IO.txt", "r", stdin);
  freopen("IOo.txt", "w+", stdout);
  vector<int> v(8);
  for (int i = 0; i < 8; i++) {
    io::read_int(v[i]);
  }
  assert(v[0] == 1);
  assert(v[1] == 2);
  assert(v[2] == 3);
  assert(v[3] == 5);
  assert(v[4] == 6);
  assert(v[5] == 123);
  assert(v[6] == -2);
  assert(v[7] == -8);
  vector<string> s(5);
  for (int i = 0; i < 5; i++) {
    io::read_string(s[i]);
  }
  assert(s[0] == "hello");
  assert(s[1] == "world");
  assert(s[2] == "i'm");
  assert(s[3] == "reading");
  assert(s[4] == "strings");
  vector<double> d(4);
  for (int i = 0; i < 4; i++) {
    io::read_double(d[i]);
  }
  assert(abs(d[0] - 3.14) < 1e-9);
  assert(abs(d[1] - 2.78) < 1e-9);
  assert(abs(d[2] - 1.222222) < 1e-9);
  assert(abs(d[3] - -2.87) < 1e-9);

  io::write_string(s[0]);
  io::write_char(' ');
  io::write_string(s[1]);
  io::write_char(' ');
  io::write_int(2018);
  io::write_char(' ');
  io::write_int(-2018);
  io::write_char(' ');
  io::write_int(314332424);
  io::write_char(' ');
  io::write_double(34.334, 10);
}

void testMatrix() {
  // use fib matrix to test multiplication and power
  vector<vector<int>> data = {{1, 1}, {1, 0}};
  Matrix<int, int> m(data);
  auto r0 = m.pow(0);
  assert(r0.get(0, 0) == 1);
  assert(r0.get(1, 1) == 1);
  auto r1 = m.pow(1);
  assert(r1[0][0] == 1);
  auto r2 = m.pow(2);
  assert(r2[0][0] == 2);
  auto r3 = m.pow(5);
  assert(r3[0][0] == 8);
  auto r4 = m.pow(29);
  assert(r4[0][0] == 832040);
  vector<vector<int>> data2 = {{2, 4}, {3, 8}};
  Matrix<int, int> b(data2);
  auto c = m + b;
  auto d = m - b;
  assert(c[0][0] == 3);
  assert(c[0][1] == 5);
  assert(c[1][0] == 4);
  assert(c[1][1] == 8);
  assert(d[0][0] == -1);
  assert(d[0][1] == -3);
  assert(d[1][0] == -2);
  assert(d[1][1] == -8);
}

void testintm() {
  intm x(3, 11);
  intm y(7, 11);
  assert((x + y) == intm(10, 11));
  assert((x - y) == intm(7, 11));
  assert((x * y) == intm(10, 11));
  assert((x / y) == intm(2, 11));
  x += intm(2, 11);
  assert(x == intm(5, 11));
  x += intm(8, 11);
  assert(x == intm(2, 11));
  x -= intm(3, 11);
  assert(x == intm(10, 11));
  x -= intm(12, 11);
  assert(x == intm(9, 11));
  x *= intm(8, 11);
  assert(x == intm(6, 11));
  x /= intm(2, 11);
  assert(x++ == intm(3, 11));
  assert(x == intm(4, 11));
  assert(--x == intm(3, 11));
  assert(x-- == intm(3, 11));
  assert(x == intm(2, 11));
  assert(--x == intm(1, 11));
}

void testintmMatrix() {
  vector<vector<intm>> data = {{1, 1}, {1, 0}};
  Matrix<int, intm> m(data);
  intm z(123, 123);
  auto r0 = m.pow(0);
  assert(r0[0][0] == intm(1, 1000000007));
  assert(r0[1][1] == intm(1, 1000000007));
  auto r1 = m.pow(29);
  assert(r1[0][0] == intm(832040, 1000000007));
  auto r2 = m.pow(199);
  assert(r2[0][0] == intm(349361645, 1000000007));
  auto r3 = m.pow(399);
  assert(r3[0][0] == intm(967250938, 1000000007));
}

int main(int argc, char *argv[]) {
  if (argc > 1 && string(argv[1]) == "big_int") {
    testBigInteger();
    return 0;
  }
  if (argc > 1 && string(argv[1]) == "wo_big_int") {
    testUnion();
    testSegmentTree();
    testFlow();
    testShortestPath();
    testMatrix();
    testintm();
    testintmMatrix();
    testIO();
    return 0;
  }

  testUnion();
  testSegmentTree();
  testFlow();
  testShortestPath();
  testMatrix();
  testintm();
  testintmMatrix();
  testBigInteger();
  testIO();
  return 0;
}
