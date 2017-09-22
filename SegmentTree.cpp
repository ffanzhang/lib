// based on http://codeforces.com/blog/entry/18051
#include <bits/stdc++.h>

template <class E>
struct Combine {
    virtual E identity() const {
        return std::numeric_limits<E>::max();
    };
    virtual E combine(const E &left, const E &right) {
        return std::min(left, right);
    }
};

template <class E>
struct MyMax : public Combine<E> {
    E identity () const {
        return std::numeric_limits<E>::lowest();
    }
    E combine(const E &left, const E &right) {
        return std::max(left, right);
    }
};

template <class E>
struct MySum : public Combine<E> {
    E identity () const {
        return (E) 0;
    }
    E combine(const E &left, const E &right) {
        return left + right;
    }
};


template <class I, class E>
class SegmentTree {
    std::vector<E> tree;
    Combine<E> *combine;
    I n;
public:
    SegmentTree(const std::vector<E> &elements, Combine<E> *combine) {
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
    ~SegmentTree() {
        tree.clear();
    }
    void modify(I i, const E &value) {
        for (tree[i += n] = value; i >>= 1;) {
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
