#include "SegmentTree.cpp"
#include "Union.cpp"
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <cassert>
using namespace std;

template <class I, class E>
void testSegmentTreeHelper(vector<E> &elements, I n) {
    for (I i = 0; i < n; i++) {
        elements.push_back(i + 1);
    }
    Combine<E> *a = new MyMax<E>();
    SegmentTree<I, E> t1(elements, a);
    assert(t1.query(0, 100) == 100);
    assert(t1.query(0, 50) == 50);
    assert(t1.query(50, 100) == 100);

    Combine<E> *b = new MySum<E>();
    SegmentTree<I, E> t2(elements, b);

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

int main() {
    testUnion();
    testSegmentTree();

    return 0;
}