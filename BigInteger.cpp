#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class BigInteger;
BigInteger long_mul(const BigInteger& x, const BigInteger& y);
BigInteger karatsuba(const BigInteger& x, const BigInteger& y);
BigInteger long_div(const BigInteger& x, const BigInteger& y, BigInteger& rem);
BigInteger pow(const BigInteger& a, const BigInteger& b);
BigInteger pow_mod(const BigInteger& a, const BigInteger& b,
                   const BigInteger& m);

class BigInteger {
 public:
  const unsigned long long base = 1000 * 1000 * 1000;
  const int base_digits = 9;

  // most significant digit determines the sign,
  // MSB of [0, boundary) means positive
  // [boundary, base) means negative
  // sign extension appends 0 or (base - 1) to MSD
  const unsigned long long boundary = base / 2ll;
  std::vector<unsigned long long> digits;

  // need to maintain a vector size of 1 at all times
  BigInteger() : digits(1, 0) {}
  ~BigInteger() { digits.clear(); }
  BigInteger(const long long&);
  BigInteger(const std::string&);
  BigInteger(const BigInteger&);
  BigInteger(const std::vector<unsigned long long>&);

  bool is_zero() const;
  bool is_positive() const;
  void print() const;
  std::string to_string() const;

  BigInteger& append_sign();
  BigInteger& sign_extend(std::size_t num_digits);
  BigInteger& truncate(std::size_t num_digits);
  BigInteger& shift_left(std::size_t);
  BigInteger& trim();
  BigInteger& negate();
  BigInteger& operator=(const BigInteger&);
  unsigned long long& operator[](std::size_t i);

  BigInteger abs() const;
  BigInteger upper_half(int) const;
  BigInteger lower_half(int) const;

  BigInteger operator-() const;
  BigInteger operator+(const BigInteger&) const;
  BigInteger operator-(const BigInteger&) const;
  BigInteger operator*(const BigInteger&)const;
  BigInteger operator/(const BigInteger&) const;
  BigInteger operator%(const BigInteger&) const;

  bool operator<(const BigInteger&) const;
  bool operator>(const BigInteger&) const;
  bool operator<=(const BigInteger&) const;
  bool operator>=(const BigInteger&) const;
  bool operator==(const BigInteger&) const;
  bool operator!=(const BigInteger&) const;

  BigInteger& operator++();
  BigInteger& operator--();
  BigInteger operator++(int);
  BigInteger operator--(int);

  friend std::istream& operator>>(std::istream&, BigInteger&);
  friend std::ostream& operator<<(std::ostream&, BigInteger);
};

static BigInteger _dummy_mod(0);

bool BigInteger::is_zero() const {
  return digits.size() == 1 && digits.back() == 0;
}

bool BigInteger::is_positive() const {
  assert(digits.size() > 0);
  return digits.back() == 0;
}

BigInteger& BigInteger::trim() {
  if (digits.size() > 0 && digits.back() == 0) {
    while (digits.size() > 0 && digits.back() == 0) {
      digits.pop_back();
    }
    digits.push_back(0);
    return *this;
  }
  if (digits.size() > 0 && digits.back() == base - 1) {
    while (digits.size() > 0 && digits.back() == base - 1) {
      digits.pop_back();
    }
    digits.push_back(base - 1);
    return *this;
  }
  return *this;
}

BigInteger& BigInteger::sign_extend(std::size_t num_digits) {
  if (num_digits <= digits.size()) {
    return *this;
  }
  unsigned long long sign = digits.back();
  std::size_t n = digits.size();
  digits.resize(num_digits);
  std::fill(digits.begin() + n, digits.end(), sign);
  return *this;
}

BigInteger& BigInteger::truncate(std::size_t num_digits) {
  if (num_digits >= digits.size()) {
    return *this;
  }
  digits.resize(num_digits);
  return *this;
}

BigInteger& BigInteger::negate() {
  assert(digits.size() > 0 &&
         (digits.back() == 0 || digits.back() == (base - 1)));
  if (is_zero()) {
    return *this;
  }
  for (auto& d : digits) {
    d = base - 1 - d;
  }
  for (auto& d : digits) {
    if (d != base - 1) {
      d += 1;
      break;
    } else {
      d = 0;
    }
  }
  trim();
  return *this;
}

BigInteger& BigInteger::append_sign() {
  if (digits.size() == 0) {
    digits.push_back(0);
    return *this;
  }
  if (digits.back() < boundary && digits.back() != 0) {
    digits.push_back(0);
  }
  if (digits.back() >= boundary && digits.back() != base - 1) {
    digits.push_back(base - 1);
  }
  return *this;
}

void BigInteger::print() const {
  for (auto e : digits) {
    std::cout << e << ' ';
  }
  std::cout << '\n';
}

BigInteger::BigInteger(const long long& v) {
  if (v == 0) {
    digits.push_back(0);
    return;
  }
  bool neg = false;
  long long val = v;
  if (val < 0) {
    neg = true;
    val = -val;
  }
  while (val) {
    digits.push_back(val % base);
    val /= base;
  }
  digits.push_back(0);
  if (neg == true) {
    negate();
  }
}

BigInteger::BigInteger(const std::string& s) {
  int pos = 0;
  bool negative = false;
  for (; pos < s.size() && (s[pos] == '-' || s[pos] == '+'); pos++) {
    if (s[pos] == '-') {
      negative = true;
    }
  }
  for (int i = s.size() - 1; i >= pos; i -= base_digits) {
    long long x = 0;
    for (int j = std::max(pos, i - base_digits + 1); j <= i; j++) {
      x = x * 10 + s[j] - '0';
    }
    digits.push_back(x);
  }
  digits.push_back(0);
  trim();
  if (negative) {
    negate();
  }
}

BigInteger::BigInteger(const BigInteger& v) { digits = v.digits; }

BigInteger::BigInteger(const std::vector<unsigned long long>& v) { digits = v; }

BigInteger& BigInteger::operator=(const BigInteger& v) {
  digits = v.digits;
  return *this;
}

unsigned long long& BigInteger::operator[](std::size_t i) { return digits[i]; }

BigInteger BigInteger::operator+(const BigInteger& v) const {
  BigInteger res(v);
  res.sign_extend(std::max(digits.size(), v.digits.size()));
  unsigned long long carry = 0;
  for (std::size_t i = 0; i < std::max(digits.size(), v.digits.size()); i++) {
    res.digits[i] += digits[std::min(i, (digits.size() - 1))] + carry;
    carry = res.digits[i] / base;
    res.digits[i] %= base;
  }
  res.append_sign();
  res.trim();
  return res;
}

BigInteger BigInteger::operator-(const BigInteger& v) const {
  return (*this) + BigInteger(v).negate();
}

BigInteger BigInteger::operator*(const BigInteger& v) const {
  return karatsuba(*this, v);
}

BigInteger BigInteger::operator/(const BigInteger& v) const {
  return long_div(*this, v, _dummy_mod);
}

BigInteger BigInteger::operator%(const BigInteger& v) const {
  BigInteger mod;
  long_div(*this, v, mod);
  return mod;
}

BigInteger BigInteger::operator-() const { return BigInteger(*this).negate(); }

// prefix increment
BigInteger& BigInteger::operator++() {
  *this = *this + BigInteger(1);
  return *this;
}

// postfix increment
BigInteger BigInteger::operator++(int post) {
  BigInteger res = *this;
  ++*this;
  return res;
}

// prefix decrement
BigInteger& BigInteger::operator--() {
  *this = *this + BigInteger(-1);
  return *this;
}

// postfix decrement
BigInteger BigInteger::operator--(int post) {
  BigInteger res = *this;
  --*this;
  return res;
}

BigInteger BigInteger::abs() const {
  BigInteger res(*this);
  if (!res.is_positive()) {
    res.negate();
  }
  return res;
}

bool BigInteger::operator<(const BigInteger& v) const {
  int sign = is_positive() * 2 - 1;
  int vsign = v.is_positive() * 2 - 1;
  if (sign != vsign) return sign < vsign;
  if (digits.size() != v.digits.size())
    return digits.size() * sign < v.digits.size() * vsign;
  for (int i = digits.size() - 1; i >= 0; i--)
    if (digits[i] != v.digits[i]) return digits[i] < v.digits[i];
  return false;
}

bool BigInteger::operator>(const BigInteger& v) const { return v < *this; }

bool BigInteger::operator<=(const BigInteger& v) const { return !(v < *this); }

bool BigInteger::operator>=(const BigInteger& v) const { return !(*this < v); }

bool BigInteger::operator==(const BigInteger& v) const {
  return !(*this < v) && !(v < *this);
}

bool BigInteger::operator!=(const BigInteger& v) const {
  return *this < v || v < *this;
}

std::string BigInteger::to_string() const {
  BigInteger v(*this);
  std::ostringstream oss;
  oss << v;
  return oss.str();
}

std::istream& operator>>(std::istream& stream, BigInteger& v) {
  std::string s;
  stream >> s;
  v = BigInteger(s);
  return stream;
}

std::ostream& operator<<(std::ostream& stream, BigInteger v) {
  if (!v.is_positive()) {
    stream << '-';
    v.negate();
  }
  while (v.digits.size() > 0 && v.digits.back() == 0) {
    v.digits.pop_back();
  }
  stream << (v.digits.empty() ? 0 : v.digits.back());
  for (int i = static_cast<int>(v.digits.size()) - 2; i >= 0; --i)
    stream << std::setw(v.base_digits) << std::setfill('0') << v.digits[i];
  return stream;
}

BigInteger pow(const BigInteger& a, const BigInteger& b) {
  if (b == BigInteger(0)) return BigInteger(1);
  if (b == BigInteger(1)) return a;
  BigInteger tmp = pow(a, b / BigInteger(2));
  tmp = tmp * tmp;
  if ((b % BigInteger(2)) == 0) return tmp;
  return tmp * a;
}

BigInteger pow_mod(const BigInteger& a, const BigInteger& b,
                   const BigInteger& m) {
  if (b == BigInteger(0)) return BigInteger(1);
  if (b == BigInteger(1)) return a % m;
  BigInteger tmp = pow(a, b / BigInteger(2));
  tmp = (tmp * tmp) % m;
  if ((b % BigInteger(2)) == 0) return tmp;
  return (tmp * a) % m;
}

BigInteger long_mul(const BigInteger& x, const BigInteger& y) {
  if (x.is_zero() || y.is_zero()) {
    return BigInteger(0);
  }
  BigInteger c;
  int valid_digits = x.digits.size() + y.digits.size() - 1;
  c.sign_extend(valid_digits);
  for (std::size_t i = 0; i < valid_digits; i++) {
    unsigned long long carry = 0;
    for (std::size_t j = 0; j + i < valid_digits; j++) {
      c.digits[i + j] += carry + x.digits[std::min(i, x.digits.size() - 1)] *
                                     y.digits[std::min(j, y.digits.size() - 1)];
      carry = c.digits[i + j] / c.base;
      c.digits[i + j] %= c.base;
    }
  }
  c.append_sign();
  c.trim();
  return c;
}

BigInteger BigInteger::lower_half(int m) const {
  if (is_zero()) {
    return BigInteger(0);
  }
  std::vector<unsigned long long> new_digits(
      digits.begin(), std::min(digits.begin() + m, digits.end()));
  new_digits.push_back(0);
  return BigInteger(new_digits).trim();
}

BigInteger BigInteger::upper_half(int m) const {
  if (is_zero()) {
    return BigInteger(0);
  }
  if (m >= digits.size()) {
    return BigInteger(0);
  }
  std::vector<unsigned long long> new_digits(digits.begin() + m, digits.end());
  new_digits.push_back(0);
  return BigInteger(new_digits).trim();
}

BigInteger& BigInteger::shift_left(std::size_t n) {
  if (is_zero()) {
    return *this;
  }
  std::vector<unsigned long long> new_digits(digits.size() + n);
  std::copy(digits.begin(), digits.end(), new_digits.begin() + n);
  digits = new_digits;
  return *this;
}

BigInteger karatsuba(const BigInteger& x, const BigInteger& y) {
  if (x.is_zero() || y.is_zero()) {
    return BigInteger(0);
  }
  if (x.digits.size() < 35 || y.digits.size() < 35) {
    return long_mul(x, y);
  }
  bool res_positive = (x.is_positive() == y.is_positive());
  BigInteger xx(x);
  BigInteger yy(y);
  if (!xx.is_positive()) {
    xx.negate();
  }
  if (!yy.is_positive()) {
    yy.negate();
  }
  int m = std::min(x.digits.size(), y.digits.size()) / 2;
  BigInteger x0 = xx.lower_half(m);
  BigInteger x1 = xx.upper_half(m);
  BigInteger y0 = yy.lower_half(m);
  BigInteger y1 = yy.upper_half(m);
  BigInteger z2 = karatsuba(x1, y1);
  BigInteger z0 = karatsuba(x0, y0);
  BigInteger z1 = karatsuba(x0 + x1, y0 + y1) - z2 - z0;

  BigInteger c = z2.shift_left(2 * m) + z1.shift_left(m) + z0;
  c.trim();
  if (!res_positive) {
    c.negate();
  }
  return c;
}

BigInteger long_div(const BigInteger& x, const BigInteger& y, BigInteger& rem) {
  if (y.is_zero()) {
    throw std::domain_error("BigInteger Division by Zero.");
  }
  bool xsign = x.is_positive();
  bool ysign = y.is_positive();
  bool res_postive = (xsign == ysign);

  BigInteger a(x.abs());
  BigInteger b(y.abs());
  BigInteger tmp;
  BigInteger res;

  if (a.is_zero() || b > a) {
    if (xsign != ysign && !a.is_zero()) {
      rem = b - a;
    } else {
      rem = a;
    }
    if (ysign == false) {
      rem.negate();
    }
    return BigInteger(0);
  }
  if (a == b) {
    rem = BigInteger(0);
    return res_postive ? BigInteger(1) : BigInteger(-1);
  }

  BigInteger test_int;
  for (int i = a.digits.size() - 1; i >= 0; i--) {
    tmp.digits.insert(tmp.digits.begin(), a.digits[i]);
    unsigned long long lo = 0ll, hi = a.base, cur_digit = 0ll;
    while (lo < hi) {
      unsigned long long mid = lo + (hi - lo) / 2ll;
      test_int = long_mul(b, mid);
      if (test_int < tmp) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    // test_int should be >= tmp
    test_int = b * BigInteger(lo);
    if (test_int == tmp) {
      cur_digit = lo;
    } else {
      cur_digit = std::max(0ull, lo - 1);
    }
    if (cur_digit > 0) {
      tmp = tmp - b * BigInteger(cur_digit);
    }
    res.digits.insert(res.digits.begin(), cur_digit);
    tmp.trim();
    res.trim();
  }
  tmp.trim();
  res.trim();
  if (ysign != xsign && tmp > 0) {
    rem = b - tmp;
  } else {
    rem = tmp;
  }
  if (!res_postive) {
    res.negate();
  }
  if (ysign == false) {
    rem.negate();
  }
  // this version rounds to zero, python rounds to negative infinity
  return res;
}

// kattis:
// simpleaddition: add
// wizardofodds: powers of 2
// pseudoprime: too slow, used intm.cpp instead
//
// codeforces:
// acmsguru 112, can also python
//
// euler13: add
// euler16: powers of 2
