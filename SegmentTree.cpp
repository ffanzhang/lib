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
struct MyMin : public Combine<E> {
    E identity () const {
        return std::numeric_limits<E>::max();
    }
    E combine(const E &left, const E &right) {
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

struct Element {
    // length, leftValue, leftFrequency, rightValue, rightFrequency,
    // middleValue, MiddleFrequency;
    int l, lv, lf, rv, rf, mv, mf;
    Element(int l = 0, int lv = 0, int lf = 0, int rv = 0, int rf = 0, int mv = 0, int mf = 0) : l(l), lv(lv), lf(lf), rv(rv), rf(rf), mv(mv), mf(mf) {}
};

// computes the frequency from a list of sorted elements
// UVA 11235
struct Frequency : public Combine<Element> {
    Element identity() const {
        return Element(0, 0, 0, 0, 0, 0, 0);
    }
    Element combine(const Element &left, const Element &right) {
        if (left.l == 0) {
            return right;
        }
        if (right.l == 0) {
            return left;
        }
        // combined length
        int l = left.l + right.l;
        int lv = left.lv;
        int rv = right.rv;

        int lf = left.lf;
        // case where left value fills left chunk
        if (left.lf == left.l && right.lv == left.lv) {
            lf = left.l + right.lf;
        }
        int rf = right.rf;
        // case where right value fills right chunk
        if (right.l == right.rf && right.rv == left.rv) {
            rf = right.l + left.rf;
        }
        int newf = 0;
        int newv = 0;
        // concatenates middle value
        if (left.rv == right.lv) {
            newf = left.rf + right.lf;
            newv = left.rv;
        }
        int mf = left.mf;
        int mv = left.mv;
        // determine whether to keep left middle value or
        // right middle value
        if (right.mf > left.mf) {
            mf = right.mf;
            mv = right.mv;
        }
        // determine whether to keep new concatenated middle value
        // or one of the old middle values
        if (newf > mf) {
            mf = newf;
            mv = newv;
        }
        return Element(l, lv, lf, rv, rf, mv, mf);
    }
};

// UVA 11297
template <class I, class E>
class SegmentTree {
    std::vector<E> tree;
    Combine<E> *combine;
    I n;
public:
    SegmentTree() {}
    SegmentTree(const std::vector<E> &elements, Combine<E> *combine) {
        n = elements.size();
        tree.resize(2 * n);
        this->combine = combine;
        for (I i = n; i < 2 * n; i++) {
            tree[i] = elements[i - n];
        }
        for (I i = n - 1; i > 0; i--) {
            tree[i] = combine->combine(tree[i<<1], tree[(i<<1)|1]);
        }
    }
    ~SegmentTree() {
        tree.clear();
    }
    void modify(I i, const E &value) {
        for (tree[i += n] = value; i >>= 1;) {
            tree[i] = combine->combine(tree[i<<1], tree[(i<<1)|1]);
        }
    }
    E query(I l, I r) {
        E resl = combine->identity();
        E resr = combine->identity();
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l&1) resl = combine->combine(resl, tree[l++]);
            if (r&1) resr = combine->combine(tree[--r], resr);
        }
        return combine->combine(resl, resr);
    }
};

template <class I, class E>
class SegmentTreeLazy {
    std::vector<E> tree;
    E *delay;
    Combine<E> *combine;
    I height;
    I n;
public:
    SegmentTree(I n) : n(n), height(0) {
        I np = n;
        while (np > 0) {
            np >>= 1;
            height++;
        }
        delay = new E [n]();
        tree.resize(2 * n);
        this->combine = new MyMin<E>();
    }
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
            delay[i] += value;
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
            for (I i = r; i >>= l; i++) {
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
            if (l&1) modify_node(l++, value, length);
            if (r&1) modify_node(--r, value, length);
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
            if (l&1) resl = combine->combine(resl, tree[l++]);
            if (r&1) resr = combine->combine(tree[--r], resr);
        }
        return combine->combine(resl, resr);
    }
};
