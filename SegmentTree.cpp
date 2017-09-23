// based on http://codeforces.com/blog/entry/18051
#include <vector>
#include <cstdint>
#include <limits>

template <class E>
struct Combine {
    virtual E identity() const = 0; 
    virtual E combine(const E &left, const E &right) = 0;
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
    E *delay;
    Combine<E> *combine;
    I height;
    I n;
public:
    SegmentTree(const std::vector<E> &elements, Combine<E> *combine) {
        n = elements.size();
        I np = n;
        height = 0;
        while (np > 0) {
            np >>= 1;
            height++;
        }
        delay = new E [n]();
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
    void modify_node(I i, E value, I length) {
        tree[i] += value * length;
        if (i < n)  {
            delay[i] = value;
        }
    }
    void push(I l, I r) {
        I hp = height;
        I length = ((I) 1) << (height - 1);
        for (l += n, r += n - 1; hp > 0; hp--, length >>= 1) {
            for (I i = l >> hp; i <= r >> hp; i++) {
                if (delay[i] != 0) {
                    modify_node(i << 1, delay[i], length);
                    modify_node((i << 1)|1, delay[i], length);
                    delay[i] = 0;
                }
            }
        }
    }
    void pull(I l, I r) {
        I length = 2;
        for (l += n, r += n - 1; l > 1; length <<= 1) {
            l >>= 1; r >>= 1;
            for (I i = r; i >= l; i++) {
                if (delay[i] == 0) {
                    tree[i] = combine->combine(tree[i << 1], tree[(i << 1)|1]);
                } else {
                    tree[i] = delay[i] * length;
                }
            }
        }
    }
    void modify(I l, I r, const E &value) {
        if (value == 0) return;
        push(l, l + 1);
        push(r - 1, r);
        I tl = l, tr = r, length = 1;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1, length <<= 1) {
            if (l & 1) modify_node(l++, value, length);
            if (r & 1) modify_node(--r, value, length);
        }
        pull(tl, tl + 1);
        pull(tr - 1, tr);
    }
    E query(I l, I r) {
        push(l, l + 1);
        push(r - 1, r);
        E resl = combine->identity();
        E resr = combine->identity();
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l&1) resl = combine->combine(tree[l++], resl);
            if (r&1) resr = combine->combine(resr, tree[--r]);
        }
        return combine->combine(resl, resr);
    }
};
