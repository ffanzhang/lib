#include "SegmentTree.cpp"
#include "Union.cpp"
#include <cassert>
using namespace std;

void testSegmentTree() {
    std::vector<int> elements;
    for (int i = 0; i < 100; i++) {
        elements.push_back(i + 1);
    }
    Combine<int> *c = new MyMax<int>();
    SegmentTree<int, int> t1(elements, c);
    assert(t1.query(0, 100) == 100);
    assert(t1.query(0, 50) == 50);
    assert(t1.query(50, 100) == 100);

    Combine<int> *d = new Combine<int>();
    SegmentTree<int, int> t2(elements, d);
    assert(t2.query(0, 100) == 1);
    assert(t2.query(0, 50) == 1);
    assert(t2.query(50, 100) == 51);

    SegmentTree<int, int> t3(elements, (new MySum<int>()));
    assert(t3.query(0, 100) == 5050);
    assert(t3.query(0, 50) == (50 * 51) / 2);
    for (int i = 1; i <= 100; i++) {
        assert(t3.query(0, i) == i * (i + 1) / 2);
    }
}

void testUnionHelper(Union<long long> *u, long long n) {
    assert(u->components() == n);
    for (long long i = 0; i < n - 1; i++) {
        u->merge(i, i + 1);
    }
    assert(u->is_connected(0, n - 1) == true);
    assert(u->is_connected(n / 2, 0) == true);
    assert(u->components() == 1);
}

void testUnion() {
    long long n = 100;
    Union<long long> *u = new Union<long long>(n);
    Union<long long> *v = new UnionByRank<long long>(n);
    Union<long long> *w = new UnionBySize<long long>(n);
    testUnionHelper(u, n);
    testUnionHelper(v, n);
    testUnionHelper(w, n);

    Union<int> *x = new Union<int>(10);
    x->merge(1, 2);
    x->merge(3, 4);
    assert(x->is_connected(1, 3) == false);
    x->merge(1, 3);
    assert(x->is_connected(1, 4) == true);
    assert(x->is_connected(1, 5) == false);
}

int main() {
    testUnion();
    testSegmentTree();
    return 0;
}
