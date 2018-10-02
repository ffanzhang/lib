// based on http://codeforces.com/blog/entry/18051
#include <cstdint>
#include <limits>
#include <vector>

template <class E>
struct Merger {
  virtual E identity() const = 0;
  virtual E merge(const E& left, const E& right) const = 0;
};

template <class E>
struct MyMin : public Merger<E> {
  E identity() const { return std::numeric_limits<E>::max(); }
  E merge(const E& left, const E& right) const { return std::min(left, right); }
};

template <class E>
struct MyMax : public Merger<E> {
  E identity() const { return std::numeric_limits<E>::lowest(); }
  E merge(const E& left, const E& right) const { return std::max(left, right); }
};

template <class E>
struct MySum : public Merger<E> {
  E identity() const { return (E)0; }
  E merge(const E& left, const E& right) const { return left + right; }
};

struct Element {
  // length, leftValue, leftFrequency, rightValue, rightFrequency,
  // middleValue, MiddleFrequency;
  int l, lv, lf, rv, rf, mv, mf;
  Element(int l = 0, int lv = 0, int lf = 0, int rv = 0, int rf = 0, int mv = 0,
          int mf = 0)
      : l(l), lv(lv), lf(lf), rv(rv), rf(rf), mv(mv), mf(mf) {}
};

// computes the frequency from a list of sorted elements
// UVA 11235
struct Frequency : public Merger<Element> {
  Element identity() const { return Element(0, 0, 0, 0, 0, 0, 0); }
  Element merge(const Element& left, const Element& right) const {
    if (left.l == 0) {
      return right;
    }
    if (right.l == 0) {
      return left;
    }
    // mergerd length
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
  const Merger<E>* merger;
  I n;

 public:
  SegmentTree() {}
  SegmentTree(const std::vector<E>& elements, const Merger<E>& merger) {
    n = elements.size();
    tree.resize(2 * n);
    this->merger = &merger;
    for (I i = n; i < 2 * n; i++) {
      tree[i] = elements[i - n];
    }
    for (I i = n - 1; i > 0; i--) {
      tree[i] = this->merger->merge(tree[i << 1], tree[(i << 1) | 1]);
    }
  }
  ~SegmentTree() { tree.clear(); }
  void modify(I i, const E& value) {
    for (tree[i += n] = value; i >>= 1;) {
      tree[i] = merger->merge(tree[i << 1], tree[(i << 1) | 1]);
    }
  }
  // left inclusive, right exclusive
  E query(I l, I r) {
    E resl = merger->identity();
    E resr = merger->identity();
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1) resl = merger->merge(resl, tree[l++]);
      if (r & 1) resr = merger->merge(tree[--r], resr);
    }
    return merger->merge(resl, resr);
  }
};

// UVA 11402
// ranges are left inclusive, right exclusive
template <class I, class E>
class SegmentTreeLazy {
  std::vector<E> tree;
  E* pending;
  const Merger<E>* merger;
  I height;
  I n;

 public:
  SegmentTreeLazy() {}
  SegmentTreeLazy(I n) : n(n), height(0) {
    I np = n;
    while (np > 0) {
      np >>= 1;
      height++;
    }
    pending = new E[n]();
    tree.resize(2 * n);
    this->merger = new MySum<E>();
  }
  SegmentTreeLazy(const std::vector<E>& elements, const Merger<E>& merger) {
    n = elements.size();
    I np = n;
    height = 0;
    while (np > 0) {
      np >>= 1;
      height++;
    }
    pending = new E[n]();
    tree.resize(2 * n);
    this->merger = &merger;
    for (I i = n; i < 2 * n; i++) {
      tree[i] = elements[i - n];
    }
    for (I i = n - 1; i > 0; i--) {
      tree[i] = merger->merge(tree[i << 1], tree[(i << 1) | 1]);
    }
  }
  ~SegmentTreeLazy() {
    delete pending;
    tree.clear();
  }
  // this version specifies change of value
  void modify_node(I i, const E& value, I length) {
    tree[i] += value * length;
    if (i < n) {
      pending[i] += value;
    }
  }
  // propagate pending changes
  void push(I l, I r) {
    I hp = height;
    I length = ((I)1) << (height - 1);
    for (l += n, r += n - 1; hp > 0; hp--, length >>= 1) {
      for (I i = l >> hp; i <= r >> hp; i++) {
        if (pending[i] != 0) {
          modify_node(i << 1, pending[i], length);
          modify_node((i << 1) | 1, pending[i], length);
          pending[i] = 0;
        }
      }
    }
  }
  void pull(I l, I r) {
    for (l += n, r += n - 1; l > 1;) {
      l >>= 1;
      r >>= 1;
      for (I i = r; i >= l; i--) {
        if (pending[i] == 0) {
          tree[i] = merger->merge(tree[i << 1], tree[(i << 1) | 1]);
        }
      }
    }
  }
  void modify(I l, I r, const E& value) {
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
    E resl = merger->identity();
    E resr = merger->identity();
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1) resl = merger->merge(resl, tree[l++]);
      if (r & 1) resr = merger->merge(tree[--r], resr);
    }
    return merger->merge(resl, resr);
  }
};
