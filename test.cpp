#include "SegmentTree.cpp"
#include "Union.cpp"
#include "Flow.cpp"
#include "ShortestPath.cpp"
#include "BigInteger.cpp"
#include "IO.cpp"
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <cassert>
using namespace std;

template <class I, class E>
void testSegmentTreeHelper(vector<E>& elements, I n) {
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

void testBigInteger() {
    BigInteger a(11);
    BigInteger b(11);
    assert(a + b == BigInteger(22));
    assert(a * b == BigInteger(121));
    assert(a - b == BigInteger(0));
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
    assert(k.z.size() == 2);
    BigInteger l = BigInteger(-1000000001);
    assert(l == -1000000001);
    assert(l.z.size() == 2);
    a = j;
    assert(a == -2);

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

int main() {
    testUnion();
    testSegmentTree();
    testFlow();
    testShortestPath();
    testBigInteger();
    testIO();
    return 0;
}
