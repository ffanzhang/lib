#include <cassert>
#include <vector>

template <class I, class E>
class Matrix {
  std::vector<std::vector<E>> data;
  E mod = -1;

 public:
  Matrix();
  ~Matrix() { data.clear(); }
  Matrix(I row, I col, E mod = -1) {
    this->mod = mod;
    data.resize(row);
    fill(data.begin(), data.end(), std::vector<E>(col, E(0)));
  }
  Matrix(const std::vector<std::vector<E>>& data, E mod = -1) {
    this->data = data;
    this->mod = mod;
  }
  Matrix(const Matrix& other) {
    // std::vector has an internal copy operator that copies inner
    // std::vectors recursively
    data = other.data;
    mod = other.mod;
  }
  void setmod(E mod) { this->mod = mod; }
  E getmod() { return this->mod; }
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
  void set(I row, I col, E e) { data[row][col] = e; }
  E get(I row, I col) { return data[row][col]; }
  std::vector<E>& operator[](I i) { return data[i]; }
  Matrix operator+(const Matrix& other) const {
    assert(data.size() > 0 && data.size() == other.data.size());
    assert(data[0].size() == other.data[0].size());
    Matrix res(*this);
    for (I j = 0; j < data.size(); j++) {
      for (I i = 0; i < data[j].size(); i++) {
        res.data[j][i] += other.data[j][i];
        if (mod > 0) {
          res.data[j][i] %= mod;
        }
      }
    }
    return res;
  }
  Matrix operator-(const Matrix& other) const {
    assert(data.size() > 0 && data.size() == other.data.size());
    assert(data[0].size() == other.data[0].size());
    Matrix res(*this);
    for (I j = 0; j < data.size(); j++) {
      for (I i = 0; i < data[j].size(); i++) {
        res.data[j][i] -= other.data[j][i];
        if (mod > 0) {
          res.data[j][i] = (res.data[j][i] + mod) % mod;
        }
      }
    }
    return res;
  }
  Matrix operator*(const Matrix& other) const {
    assert(data.size() > 0);
    assert(other.data.size() > 0);
    assert(data.size() == other.data[0].size());
    Matrix res(data.size(), other.data[0].size(), this->mod);
    for (I j = 0; j < data.size(); j++) {
      for (I i = 0; i < other.data[0].size(); i++) {
        for (I k = 0; k < other.data.size(); k++) {
          if (mod > 0) {
            res.data[j][i] =
                (res.data[j][i] + data[j][k] * other.data[k][i] % mod) % mod;
          } else {
            res.data[j][i] += data[j][k] * other.data[k][i];
          }
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
  Matrix pow(E n) {
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
};
