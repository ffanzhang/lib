// based on http://codeforces.com/blog/entry/18051
//
#include <bits/stdc++.h>
using namespace std;

template <class E>
struct Combine {
    virtual E identity() {
        return std::numeric_limits<E>::max();
    };
    virtual E combine(E left, E right) {
        return std::min(left, right);
    }
};

template <class E>
struct MyMax : public Combine<E> {
    E identity () {
        return std::numeric_limits<E>::lowest();
    }
    E combine(E left, E right) {
        return std::max(left, right);
    }
};

template <class I, class E>
class SegmentTree {
    std::vector<E> tree;
    Combine<E> *combine;
    I n;
public:
    SegmentTree(vector<E> &elements, Combine<E> *combine) {
        n = elements.size();
        tree.resize(2 * n);
        this->combine = combine;
        for (I i = n; i < 2 * n; i++) {
            tree[i] = elements[i - n];
        }
        for (I i = n - 1; i > 0; i--) {
            tree[i] = combine->combine(tree[i << 1], tree[(i << 1)|1]);
        }
    }
    E query(I l, I r) {
        E resl = combine->identity();
        E resr = combine->identity();
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l&1) resl = combine->combine(tree[l++], resl);
            if (r&1) resr = combine->combine(resr, tree[--r]);
        }
        return combine->combine(resl, resr);
    }

};

int main() {
    vector<int> elements;
    for (int i = 0; i < 100; i++) {
        elements.push_back(i + 1);
    }
    Combine<int> *c = new MyMax<int>();
    SegmentTree<int, int> t1(elements, c);
    assert(t1.query(0, 100) == 100);

    Combine<int> *d = new Combine<int>();
    SegmentTree<int, int> t2(elements, d);
    assert(t2.query(0, 100) == 1);

    return 0;
}

