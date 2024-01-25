#include <algorithm>
#include <cassert>
#include <cstdint>
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
  const uint64_t base = 1000 * 1000 * 1000;
  const int base_digits = 9;

  // most significant digit determines the sign,
  // MSB of [0, boundary) means positive
  // [boundary, base) means negative
  // sign extension appends 0 or (base - 1) to MSD
  const uint64_t boundary = base / static_cast<uint64_t>(2);
  std::vector<uint64_t> digits;

  // need to maintain a vector size of 1 at all times
  BigInteger() : digits(1, 0) {}
  ~BigInteger() { digits.clear(); }
  BigInteger(const int64_t&);
  BigInteger(const std::string&);
  BigInteger(const BigInteger&);
  BigInteger(const std::vector<uint64_t>&);

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
  uint64_t& operator[](std::size_t i);

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
  uint64_t sign = digits.back();
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

BigInteger::BigInteger(const int64_t& v) {
  if (v == 0) {
    digits.push_back(0);
    return;
  }
  bool neg = false;
  int64_t val = v;
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
    int64_t x = 0;
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
  digits.assign(v.digits.begin(), v.digits.end());
}

BigInteger::BigInteger(const std::vector<uint64_t>& v) {
  digits.assign(v.begin(), v.end());
}

BigInteger& BigInteger::operator=(const BigInteger& v) {
  digits = v.digits;
  return *this;
}

uint64_t& BigInteger::operator[](std::size_t i) { return digits[i]; }

BigInteger BigInteger::operator+(const BigInteger& v) const {
  BigInteger res(v);
  res.sign_extend(std::max(digits.size(), v.digits.size()));
  uint64_t carry = 0;
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
    uint64_t carry = 0;
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
  std::vector<uint64_t> new_digits(digits.begin(),
                                   std::min(digits.begin() + m, digits.end()));
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
  std::vector<uint64_t> new_digits(digits.begin() + m, digits.end());
  new_digits.push_back(0);
  return BigInteger(new_digits).trim();
}

BigInteger& BigInteger::shift_left(std::size_t n) {
  if (is_zero()) {
    return *this;
  }
  std::vector<uint64_t> new_digits(digits.size() + n);
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

BigInteger compute_rem(bool xsign, bool ysign, const BigInteger& div,
                       const BigInteger& rem) {
  BigInteger new_rem = rem;
  if (xsign != ysign && !rem.is_zero()) {
    new_rem = div - rem;
  }
  return (ysign == false) ? new_rem.negate() : new_rem;
}

BigInteger long_div(const BigInteger& x, const BigInteger& y, BigInteger& rem) {
  if (y.is_zero()) {
    throw std::domain_error("BigInteger Division by Zero.");
  }
  bool xsign = x.is_positive();
  bool ysign = y.is_positive();
  bool res_positive = (xsign == ysign);

  BigInteger a(x.abs());
  BigInteger b(y.abs());

  if (a.is_zero() || b > a) {
    rem = compute_rem(xsign, ysign, b, a);
    return BigInteger(0);
  }

  if (a == b) {
    rem = BigInteger(0);
    return res_positive ? BigInteger(1) : BigInteger(-1);
  }

  if (a.digits.size() == 2 && b.digits.size() == 2) {
    uint64_t q = a.digits[0] / b.digits[0];
    rem = compute_rem(xsign, ysign, b, BigInteger(a.digits[0] % b.digits[0]));
    return res_positive ? BigInteger(q) : BigInteger(-q);
  }

  BigInteger res;
  BigInteger tmp;
  tmp.digits.clear();
  std::copy(a.digits.end() - b.digits.size() + 1, a.digits.end(),
            back_inserter(tmp.digits));

  BigInteger test_int;
  for (int i = a.digits.size() - b.digits.size(); i >= 0; i--) {
    tmp.digits.insert(tmp.digits.begin(), a.digits[i]);

    uint64_t lo = 0, hi = a.base, cur_digit = 0ll;
    if (tmp > b) {
      // these two blobs narrow down binary search range
      if (tmp.digits.size() >= 2 && tmp.digits.size() == b.digits.size()) {
        lo = tmp.digits[tmp.digits.size() - 2] /
             (b.digits[b.digits.size() - 2] + 1);
        hi = (tmp.digits[tmp.digits.size() - 2] + 1) /
             b.digits[b.digits.size() - 2];
      }
      if (tmp.digits.size() >= 3 && b.digits.size() >= 2 &&
          tmp.digits.size() > b.digits.size()) {
        lo = (tmp.digits[tmp.digits.size() - 2] * b.base +
              tmp.digits[tmp.digits.size() - 3]) /
             (b.digits[b.digits.size() - 2] + 1);
        hi = (tmp.digits[tmp.digits.size() - 2] * b.base +
              tmp.digits[tmp.digits.size() - 3] + 1) /
             (b.digits[b.digits.size() - 2]);
      }
      while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / static_cast<uint64_t>(2);
        test_int = b * mid;
        if (test_int < tmp) {
          lo = mid + 1;
        } else {
          hi = mid;
        }
      }
      if (b * lo > tmp) {
        cur_digit = std::max<uint64_t>(lo - static_cast<uint64_t>(1), 0);
      } else {
        cur_digit = lo;
      }
      if (cur_digit > 0) {
        tmp = tmp - b * BigInteger(cur_digit);
      }
    }
    res.digits.insert(res.digits.begin(), cur_digit);
    tmp.trim();
    res.trim();
  }
  tmp.trim();
  res.trim();
  rem = compute_rem(xsign, ysign, b, tmp);
  if (!res_positive) {
    res.negate();
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
