#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "BigInteger.cpp"
#include "Flow.cpp"
#include "IO.cpp"
#include "Matrix.cpp"
#include "SegmentTree.cpp"
#include "ShortestPath.cpp"
#include "Sudoku.cpp"
#include "Union.cpp"
#include "intm.cpp"

using namespace std;

template <class I, class E>
void testSegmentTreeHelper(vector<E> &elements, I n) {
  for (I i = 0; i < n; i++) {
    elements.push_back(i + 1);
  }
  SegmentTree<I, E> t1(elements, MyMax<E>());
  assert(t1.query(0, 100) == 100);
  assert(t1.query(0, 50) == 50);
  assert(t1.query(50, 100) == 100);

  SegmentTree<I, E> t2(elements, MySum<E>());

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

void testBigInteger() {
  BigInteger zero(0);
  // zero has a size of 1
  assert(zero.digits.size() == 1);
  assert(zero.digits.back() == 0);
  string sp1(1000, '1');
  string sp2(1000, '2');
  string sp3(1000, '3');
  BigInteger ones(sp1);
  BigInteger twos(sp2);
  BigInteger threes(sp3);
  BigInteger negative_ones('-' + sp1);
  BigInteger negative_twos('-' + sp2);
  BigInteger negative_threes('-' + sp3);

  assert(zero == -zero);

  // adding zero with all other numbers
  assert(zero + zero == zero);
  assert(zero + negative_ones == negative_ones);
  assert(negative_ones + zero == negative_ones);
  assert(zero + ones == ones);
  assert(ones + zero == ones);

  // subtracting zero with all other numbers
  assert(zero - zero == zero);
  assert(zero - negative_ones == ones);
  assert(negative_ones - zero == negative_ones);
  assert(zero - ones == negative_ones);
  assert(ones - zero == ones);

  // times zero with all other numbers
  assert(zero * zero == zero);
  assert(zero * negative_ones == zero);
  assert(negative_ones * zero == zero);
  assert(zero * ones == zero);
  assert(ones * zero == zero);

  // division
  assert(zero / negative_ones == zero);
  assert(zero / ones == zero);

  // modulo
  assert(zero % negative_ones == zero);
  assert(zero % ones == zero);

  try {
    BigInteger(100) / 0;
    throw "Expected BigInteger Division by Zero Error.";
  } catch (std::domain_error e) {
    // expected an divide by zero exception
  } catch (const char *e) {
    cout << e << '\n';
  }

  assert(ones + twos == threes);
  assert(twos + ones == threes);
  assert(ones + negative_twos == negative_ones);
  assert(negative_twos + ones == negative_ones);
  assert(negative_ones + negative_twos == negative_threes);
  assert(negative_twos + negative_ones == negative_threes);

  assert(ones - twos == negative_ones);
  assert(twos - ones == ones);
  assert(ones - negative_twos == threes);
  assert(negative_twos - ones == negative_threes);
  assert(negative_ones - negative_twos == ones);
  assert(negative_twos - negative_ones == negative_ones);

  assert(ones * 2 == twos);
  assert(BigInteger(2) * ones == twos);
  assert(ones * -2 == negative_twos);
  assert(negative_twos * -1 == twos);
  assert(BigInteger(-1) * negative_twos == twos);
  assert(negative_twos * -1 == twos);

  assert(ones / twos == 0);
  assert(twos / ones == 2);
  assert(ones / negative_twos == 0);
  assert(negative_twos / ones == -2);
  assert(negative_ones / negative_twos == 0);
  assert(negative_twos / negative_ones == 2);

  assert(ones % twos == ones);
  assert(twos % ones == 0);
  assert(ones % negative_twos == negative_ones);
  assert(negative_twos % ones == 0);
  assert(negative_ones % negative_twos == negative_ones);
  assert(negative_twos % negative_ones == 0);

  // small numbers
  BigInteger a(11);
  BigInteger b(11);
  assert(a + b == BigInteger(22));
  assert(a - b == BigInteger(0));
  assert(a * b == BigInteger(121));
  assert(a / b == BigInteger(1));

  for (int i = 0; i < 1000; i++) {
    long long x = 0;
    long long y = 0;
    for (int j = 0; j < 100; j++) {
      x += rand() % 1000000000000000;
      y += rand() % 100;
    }
    BigInteger xy = BigInteger(x) / BigInteger(y);
    assert(xy == x / y);
  }

  // test some overflows
  assert(BigInteger(1000 * 1000 * 1000 - 1) + BigInteger(1) ==
         BigInteger(1000 * 1000 * 1000));
  assert(BigInteger(1) + BigInteger(1000 * 1000 * 1000 - 1) ==
         BigInteger(1000 * 1000 * 1000));
  assert(BigInteger(-1000 * 1000 * 1000 + 1) + BigInteger(-1) ==
         BigInteger(-1000 * 1000 * 1000));
  assert(BigInteger(-1) + BigInteger(-1000 * 1000 * 1000 + 1) ==
         BigInteger(-1000 * 1000 * 1000));
  assert(BigInteger(-1) + BigInteger(-(zero.base - 1)) ==
         BigInteger(-zero.base));
  assert(BigInteger(-(zero.base - 1)) + BigInteger(-1) ==
         BigInteger(-zero.base));
  assert(BigInteger(1) + BigInteger((zero.base - 1)) == BigInteger(zero.base));
  assert(BigInteger((zero.base - 1)) + BigInteger(1) == BigInteger(zero.base));

  BigInteger c("12312312312312313123123");
  BigInteger d("78787873241614257823");
  BigInteger e = c * d;
  BigInteger f = c - d;
  BigInteger g = d - c;
  assert(e == BigInteger("970060901773629064105172807606746864941229"));
  assert(f == BigInteger("12233524439070698865300"));
  assert(g == BigInteger("-12233524439070698865300"));
  BigInteger h = BigInteger("1");
  BigInteger i = BigInteger("3");
  BigInteger j = h - i;
  assert(j == -2);
  BigInteger k = BigInteger(1000000001);
  assert(k == 1000000001);
  BigInteger l = BigInteger(-1000000001);
  assert(l == -1000000001);
  a = j;
  assert(a == -2);

  assert(g < f);
  assert(f < e);
  assert(-e < -f);
  assert(l < f);
  assert(BigInteger(-1) - BigInteger(-1) == BigInteger(0));

  assert(BigInteger(0) > -1ll);
  assert(BigInteger(0) <= 0ll);
  assert(BigInteger(0) <= 1ll);
  assert(BigInteger(0) >= 0ll);
  assert(BigInteger(0) >= -1ll);
  assert(BigInteger(0) == 0ll);
  assert(BigInteger(0) != 1ll);

  BigInteger z = BigInteger(100);
  assert(++z == 101);
  assert(z == 101);
  assert(z++ == 101);
  assert(z == 102);

  assert(--z == 101);
  assert(z == 101);
  assert(z++ == 101);
  assert(z == 102);

  ifstream fin("BigIntegero.txt");
  string s, sa, sb;
  while (fin >> sa >> sb) {
    BigInteger a(sa);
    BigInteger b(sb);
    fin >> s;
    assert(a + b == BigInteger(s));
    fin >> s;
    assert(a - b == BigInteger(s));
    fin >> s;
    assert(a * b == BigInteger(s));
    fin >> s;
    assert(a.abs() / b.abs() == BigInteger(s));
    fin >> s;
    assert(a % b == BigInteger(s));
  }
  // test division by 0;
  for (int i = 0; i < 62; i++) {
    assert(pow(BigInteger(2), i) == BigInteger(1ll << i));
  }

  // deep copy test
  BigInteger m(1);
  assert(m == 1);
  BigInteger n(2);
  assert(n == 2);
  m = n;
  assert(m == 2);
  n = 12;
  assert(m == 2);
  assert(n == 12);
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

void checkboard(char *soln) {
  assert(soln != nullptr);
  for (int j = 0; j < 9; j++) {
    int sum = 0;
    for (int i = 0; i < 9; i++) {
      sum += soln[j * 9 + i];
    }
    assert(sum == 45);
  }
  for (int i = 0; i < 9; i++) {
    int sum = 0;
    for (int j = 0; j < 9; j++) {
      sum += soln[j * 9 + i];
    }
    assert(sum == 45);
  }
  for (int k = 0; k < 9; k++) {
    int row_start = k / 3 * 3;
    int col_start = k % 3 * 3;
    int sum = 0;
    for (int j = row_start; j < row_start + 3; j++) {
      for (int i = col_start; i < col_start + 3; i++) {
        sum += soln[j * 9 + i];
      }
    }
    assert(sum == 45);
  }
}

void testSudoku() {
  freopen("Sudoku.txt", "r", stdin);
  char board[81];
  string line;
  for (int j = 0; j < 9; j++) {
    cin >> line;
    for (int i = 0; i < 9; i++) {
      board[j * 9 + i] = line[i] - '0';
    }
  }
  char *soln = sudoku::solve(board);
  checkboard(soln);
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
  Matrix<int, intm> x(0, 0);
  cin >> x;
  cout << x << endl;
}

int main() {
  testUnion();
  testSegmentTree();
  testFlow();
  testShortestPath();
  testBigInteger();
  testIO();
  testSudoku();
  testMatrix();
  testintm();
  testintmMatrix();
  return 0;
}
