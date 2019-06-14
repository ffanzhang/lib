#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class BigInteger;
BigInteger long_mul(const BigInteger& x, const BigInteger& y);
BigInteger long_div(const BigInteger& x, const BigInteger& y);

class BigInteger {
 public:
  const unsigned long long base = 1000 * 1000 * 1000;
  const int base_digits = 9;

  // most significant digit determines the sign,
  // MSB of [0, boundary) means positive
  // [boundary, base) means negative
  // sign extension appends 0 or (base - 1) to MSD
  const unsigned long long boundary = base / 2ll;
  std::vector<unsigned int> digits;

  // need to maintain a vector size of 1 at all times
  BigInteger() : digits(1, 0) {}
  ~BigInteger() { digits.clear(); }
  BigInteger(const long long&);
  BigInteger(const std::string&);
  BigInteger(const BigInteger&);

  bool is_zero() const;
  bool is_positive() const;
  void print() const;
  std::string to_string() const;

  // assum well formed input for internal calls and maintain invariance
  // before calling other functions
  BigInteger& append_sign();
  BigInteger& sign_extend(unsigned int num_digits);
  BigInteger& trim();
  BigInteger& negate();
  BigInteger& operator=(const BigInteger&);
  unsigned int& operator[](int i);

  // prefix increment
  BigInteger& operator++();
  // postfix increment
  BigInteger operator++(int);
  // prefix decrement
  BigInteger& operator--();
  // postfix decrement
  BigInteger operator--(int);

  BigInteger abs() const;
  BigInteger operator-() const;
  BigInteger operator+(const BigInteger&) const;
  BigInteger operator-(const BigInteger&) const;
  BigInteger operator*(const BigInteger&)const;
  BigInteger operator/(const BigInteger&) const;

  bool operator<(const BigInteger&) const;
  bool operator>(const BigInteger&) const;
  bool operator<=(const BigInteger&) const;
  bool operator>=(const BigInteger&) const;
  bool operator==(const BigInteger&) const;
  bool operator!=(const BigInteger&) const;

  bool operator<(const long long) const;
  bool operator>(const long long) const;
  bool operator<=(const long long) const;
  bool operator>=(const long long) const;
  bool operator==(const long long) const;
  bool operator!=(const long long) const;

  friend std::istream& operator>>(std::istream&, BigInteger&);
  friend std::ostream& operator<<(std::ostream&, BigInteger);
};

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
}

BigInteger& BigInteger::sign_extend(unsigned int num_digits) {
  assert(digits.size() > 0 &&
         (digits.back() == 0 || digits.back() == (base - 1)));
  while (digits.size() < num_digits) {
    digits.push_back(digits.back());
  }
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

BigInteger::BigInteger(const BigInteger& v) {
  digits.clear();
  digits = v.digits;
}

BigInteger& BigInteger::operator=(const BigInteger& v) {
  digits.clear();
  digits = v.digits;
  return *this;
}

unsigned int& BigInteger::operator[](int i) { return digits[i]; }

BigInteger BigInteger::operator+(const BigInteger& v) const {
  if (v.is_zero()) {
    return *this;
  }
  if (is_zero()) {
    return v;
  }
  BigInteger res(v);
  if (digits.size() > v.digits.size()) {
    res.sign_extend(digits.size());
  }
  long long carry = 0, i = 0;
  for (i = 0; i < digits.size(); i++) {
    res[i] += digits[i] + carry;
    carry = res[i] / base;
    res[i] = res[i] % base;
  }
  // sign extending without actually doing it
  for (; i < res.digits.size(); i++) {
    res[i] += digits.back() + carry;
    carry = res[i] / base;
    res[i] = res[i] % base;
  }
  res.append_sign();
  res.trim();
  return res;
}

BigInteger BigInteger::operator-(const BigInteger& v) const {
  return (*this) + BigInteger(v).negate();
}

BigInteger BigInteger::operator*(const BigInteger& v) const {
  return long_mul(*this, v);
}

BigInteger BigInteger::operator/(const BigInteger& v) const {
  return long_div(*this, v);
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

bool BigInteger::operator<(const long long v) const {
  return *this < BigInteger(v);
}

bool BigInteger::operator>(const long long v) const {
  return BigInteger(v) < *this;
}

bool BigInteger::operator<=(const long long v) const {
  return !(BigInteger(v) < *this);
}

bool BigInteger::operator>=(const long long v) const {
  return !(*this < BigInteger(v));
}

bool BigInteger::operator==(const long long v) const {
  return *this == BigInteger(v);
}

bool BigInteger::operator!=(const long long v) const {
  return *this != BigInteger(v);
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

/*
BigInteger pow(const BigInteger& a, const BigInteger& b) {
  if (b == BigInteger(0)) return BigInteger(1);
  BigInteger tmp = pow(a, b / BigInteger(2));
  if (b.z[0] % 2 == 0) return tmp * tmp;
  return tmp * tmp * a;
}
*/

BigInteger long_mul(const BigInteger& x, const BigInteger& y) {
  if (x.is_zero() || y.is_zero()) {
    return BigInteger(0);
  }
  BigInteger c;
  unsigned int op_size = std::max(x.digits.size(), y.digits.size()) * 2ll;
  int valid_digits = x.digits.size() + y.digits.size() - 1;

  c.sign_extend(op_size);
  for (long long i = 0; i < valid_digits; i++) {
    for (long long j = 0, carry = 0; j + i < valid_digits; j++) {
      long long s =
          static_cast<long long>(c.digits[i + j]) + carry +
          static_cast<long long>(
              x.digits[std::min(i, (long long)x.digits.size() - 1)]) *
              static_cast<long long>(
                  y.digits[std::min(j, (long long)y.digits.size() - 1)]);
      c.digits[i + j] = s % c.base;
      carry = s / c.base;
    }
  }
  while (c.digits.size() > valid_digits) {
    c.digits.pop_back();
  }
  c.append_sign();
  c.trim();
  return c;
}

BigInteger long_div(const BigInteger& x, const BigInteger& y) {
  if (y.is_zero()) {
    throw std::overflow_error("BigInteger Division by Zero.");
  }
  bool xsign = x.is_positive();
  bool ysign = y.is_positive();
  bool res_postive = (xsign - ysign) == 0;

  BigInteger a(x.abs());
  BigInteger b(y.abs());
  BigInteger tmp;
  BigInteger res;

  if (a.is_zero() || b > a) {
    return BigInteger(0);
  }
  if (a == b) {
    return res_postive ? BigInteger(1) : BigInteger(-1);
  }
  BigInteger test_int;
  for (int i = a.digits.size() - 1; i >= 0; i--) {
    tmp.digits.insert(tmp.digits.begin(), a.digits[i]);
    long long lo = 0ll, hi = a.base, cur_digit = 0ll;
    while (lo < hi) {
      long long mid = lo + (hi - lo) / 2ll;
      test_int = b * BigInteger(mid);
      if (test_int < tmp) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    // test_int should be >= tmp
    if (test_int == tmp) {
      cur_digit = lo;
    } else {
      cur_digit = std::max(0ll, lo - 1);
    }
    if (cur_digit > 0) {
      tmp = tmp - b * BigInteger(cur_digit);
    }
    res.digits.insert(res.digits.begin(), cur_digit);
    tmp.trim();
    res.trim();
  }
  res.trim();
  if (!res_postive) {
    res.negate();
  }
  // this version rounds to zero, python rounds to negative infinity
  return res;
}
