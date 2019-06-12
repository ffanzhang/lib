#include <sstream>

// assuming all numbers use the same modulo
struct intm {
  long long m = 1000 * 1000 * 1000 + 7;
  long long n;
  intm(long long n) : n(n) {}
  intm(long long n, long long m) : n(n), m(m) {}
  intm(const intm& other) : n(other.n), m(other.m) {}
  intm pow(long long p) const {
    if (p == 0) {
      return intm(1ll, m);
    } else if (p == 1ll) {
      return intm(n, m);
    }
    intm res = pow(p / 2ll);
    res = res * res;
    if (p % 2ll == 1ll) {
      return intm(n, m) * res;
    }
    return res;
  }
  intm inverse() const { return pow(m - 2ll); }
  intm& operator=(const intm& other) {
    this->n = other.n;
    this->m = other.m;
    return *this;
  }
  // negation
  intm operator-() const { return intm((m - n) % m, m); }
  intm operator+(const intm& other) const { return intm((n + other.n) % m, m); }
  intm operator-(const intm& other) const {
    return intm(((n - other.n) + m) % m, m);
  }
  intm operator*(const intm& other) const { return intm((n * other.n) % m, m); }
  intm operator/(const intm& other) const {
    return intm((n * other.inverse().n) % m, m);
  }
  intm operator%(intm& other) const { return intm((n % other.n), m); }
  intm& operator+=(const intm& other) {
    this->n += other.n;
    this->n %= m;
    return *this;
  }
  intm& operator-=(const intm& other) {
    this->n -= other.n;
    this->n += m;
    this->n %= m;
    return *this;
  }
  intm& operator*=(const intm& other) {
    this->n *= other.n;
    this->n %= m;
    return *this;
  }
  intm& operator/=(const intm& other) {
    intm o(other);
    o = o.inverse();
    this->n *= o.n;
    this->n %= m;
    return *this;
  }
  // prefix
  intm& operator++() {
    n++;
    n %= m;
    return *this;
  }
  // postfix
  intm operator++(int post) {
    intm res(n, m);
    ++(*this);
    return res;
  }
  intm& operator--() {
    n--;
    n = (n + m) % m;
    return *this;
  }
  intm operator--(int post) {
    intm res(n, m);
    --(*this);
    return res;
  }
  bool operator<(const intm& other) { return n < other.n; }
  bool operator<=(const intm& other) { return n <= other.n; }
  bool operator>(const intm& other) { return n > other.n; }
  bool operator>=(const intm& other) { return n >= other.n; }
  bool operator==(const intm& other) { return n == other.n; }
  bool operator!=(const intm& other) { return n != other.n; }
  friend std::istream& operator>>(std::istream&, intm&);
  friend std::ostream& operator<<(std::ostream&, intm&);
};

std::istream& operator>>(std::istream& stream, intm& v) {
  long long num;
  stream >> num;
  v.n = num;
  return stream;
}

std::ostream& operator<<(std::ostream& stream, intm& v) {
  stream << v.n;
  return stream;
}
