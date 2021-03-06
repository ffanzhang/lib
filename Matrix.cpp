#include <cassert>
#include <sstream>
#include <vector>

template <class I, class E>
class Matrix {
  std::vector<std::vector<E>> data;

 public:
  Matrix(){};
  ~Matrix() { data.clear(); }
  Matrix(I row, I col) {
    data.resize(row);
    fill(data.begin(), data.end(), std::vector<E>(col, E(0)));
  }
  Matrix(const std::vector<std::vector<E>>& data) { this->data = data; }
  Matrix(const Matrix& other) {
    // std::vector has an internal copy operator that copies inner
    // std::vectors recursively
    data = other.data;
  }
  void set0() {
    fill(data.begin(), data.end(), std::vector<E>(data[0].size(), E(0)));
  }
  void seteye() {
    set0();
    assert(data.size() > 0 && data.size() == data[0].size());
    for (I i = 0; i < data.size(); i++) {
      data[i][i] = E(1);
    }
  }
  void set(I row, I col, E e) { this->data[row][col] = e; }
  E get(I row, I col) { return data[row][col]; }
  std::vector<E>& operator[](I i) { return data[i]; }
  Matrix operator+(const Matrix& other) const {
    assert(data.size() > 0 && data.size() == other.data.size());
    assert(data[0].size() == other.data[0].size());
    Matrix res(*this);
    for (I j = 0; j < data.size(); j++) {
      for (I i = 0; i < data[j].size(); i++) {
        res.data[j][i] = res.data[j][i] + other.data[j][i];
      }
    }
    return res;
  }
  Matrix& operator=(const Matrix& other) {
    data = other.data;
    return *this;
  }
  Matrix operator-(const Matrix& other) const {
    assert(data.size() > 0 && data.size() == other.data.size());
    assert(data[0].size() == other.data[0].size());
    Matrix res(*this);
    for (I j = 0; j < data.size(); j++) {
      for (I i = 0; i < data[j].size(); i++) {
        res.data[j][i] -= other.data[j][i];
      }
    }
    return res;
  }
  Matrix operator*(const Matrix& other) const {
    assert(data.size() > 0);
    assert(other.data.size() > 0);
    assert(data.size() == other.data[0].size());
    Matrix res(data.size(), other.data[0].size());
    for (I j = 0; j < data.size(); j++) {
      for (I i = 0; i < other.data[0].size(); i++) {
        for (I k = 0; k < other.data.size(); k++) {
          res.data[j][i] += data[j][k] * other.data[k][i];
        }
      }
    }
    return res;
  }
  Matrix operator%(const E& M) const {
    Matrix res(*this);
    for (auto& row : res) {
      for (auto& e : row) {
        e %= M;
      }
    }
    return res;
  }
  Matrix pow(I n) {
    assert(data.size() > 0 && data.size() == data[0].size());
    Matrix res(*this);
    if (n == 0) {
      res.seteye();
      return res;
    }
    if (n == 1) {
      return res;
    }
    res = pow(n / 2);
    res = res * res;
    if (n % 2 == 1) {
      res = res * data;
    }
    return res;
  }
  void print() {
    std::cout << '\n';
    for (int j = 0; j < data.size(); j++) {
      for (int i = 0; i < data[j].size(); i++) {
        std::cout << data[j][i] << ' ';
      }
      std::cout << '\n';
    }
    std::cout << '\n';
  }
  template <class _I, class _E>
  friend std::istream& operator>>(std::istream&, Matrix<_I, _E>& m);

  template <class _I, class _E>
  friend std::ostream& operator<<(std::ostream&, const Matrix<_I, _E>& m);
};

template <class I, class E>
std::istream& operator>>(std::istream& stream, Matrix<I, E>& m) {
  int r, c;
  stream >> r >> c;
  m.data.resize(r);
  for (int j = 0; j < r; j++) {
    m.data[j].resize(c);
    for (int i = 0; i < c; i++) {
      stream >> m.data[j][i];
    }
  }
  return stream;
}

template <class I, class E>
std::ostream& operator<<(std::ostream& stream, const Matrix<I, E>& m) {
  stream << '\n';
  for (I j = 0; j < m.data.size(); j++) {
    for (I i = 0; i < m.data[j].size(); i++) {
      stream << m.data[j][i] << ' ';
    }
    stream << '\n';
  }
  stream << '\n';
  return stream;
}
