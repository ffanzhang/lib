#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

class BigInteger {
 public:
  const long long base = 1000000000;
  const int base_digits = 9;
  std::vector<int> z;
  int sign;
  BigInteger() : sign(1), z(1, 0){};
  ~BigInteger() { z.clear(); }
  BigInteger(const int);
  BigInteger(const long long);
  BigInteger(const std::string&);
  BigInteger(const BigInteger&);

  bool is_zero() const;
  void trim();
  void print();

  BigInteger abs() const;
  BigInteger operator-() const;
  BigInteger& operator=(const BigInteger&);
  BigInteger operator+(const BigInteger&) const;
  BigInteger operator-(const BigInteger&) const;
  BigInteger operator*(const BigInteger&)const;
  BigInteger operator/(const BigInteger&) const;

  // prefix increment
  BigInteger& operator++();
  // postfix increment
  BigInteger operator++(int);
  // prefix decrement
  BigInteger& operator--();
  // postfix decrement
  BigInteger operator--(int);

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
  return z.size() == 0 || (z.size() == 1 && z.back() == 0);
}

void BigInteger::trim() {
  while (z.size() > 1 && z.back() == 0) {
    z.pop_back();
  }
}
void BigInteger::print() {
  if (sign == -1) std::cout << '-';
  std::cout << (z.empty() ? 0 : z.back());
  for (int i = static_cast<int>(z.size()) - 2; i >= 0; i--) {
    std::cout << std::setw(base_digits) << std::setfill('0') << z[i];
  }
}

BigInteger::BigInteger(const int v) : sign(1), z(1, 0) {
  if (v == 0) {
    return;
  }
  int val = v;
  if (v < 0) {
    sign = -1;
    val = -val;
  }
  z.clear();
  while (val) {
    z.push_back(val % base);
    val /= base;
  }
  trim();
  sign = is_zero() ? 1 : sign;
}

BigInteger::BigInteger(const long long v) : sign(1), z(1, 0) {
  if (v == 0) {
    return;
  }
  long long val = v;
  if (v < 0) {
    sign = -1;
    val = -val;
  }
  z.clear();
  while (val) {
    z.push_back(val % base);
    val /= base;
  }
  trim();
  sign = is_zero() ? 1 : sign;
}

BigInteger::BigInteger(const std::string& s) : sign(1), z(1, 0) {
  int pos = 0;
  for (; pos < static_cast<int>(s.size()) && (s[pos] == '-' || s[pos] == '+');
       pos++) {
    if (s[pos] == '-') sign = -sign;
  }
  z.clear();
  for (int i = s.size() - 1; i >= pos; i -= base_digits) {
    int x = 0;
    for (int j = std::max(pos, i - base_digits + 1); j <= i; j++)
      x = x * 10 + s[j] - '0';
    z.push_back(x);
  }
  trim();
  sign = is_zero() ? 1 : sign;
}

BigInteger::BigInteger(const BigInteger& v) {
  sign = v.sign;
  z.clear();
  std::copy(v.z.begin(), v.z.end(), std::back_inserter(z));
}

BigInteger& BigInteger::operator=(const BigInteger& v) {
  sign = v.sign;
  z.clear();
  std::copy(v.z.begin(), v.z.end(), std::back_inserter(z));
  return *this;
}

BigInteger BigInteger::operator+(const BigInteger& v) const {
  if (v.is_zero()) {
    return *this;
  }
  if (is_zero()) {
    return v;
  }
  if (sign == v.sign) {
    BigInteger res = v;
    for (int i = 0, carry = 0; i < std::max(z.size(), v.z.size()) || carry;
         i++) {
      if (i == res.z.size()) res.z.push_back(0);
      res.z[i] += carry + (i < z.size() ? z[i] : 0);
      carry = res.z[i] >= base;
      if (carry) res.z[i] -= base;
    }
    res.trim();
    res.sign = res.is_zero() ? 1 : res.sign;
    return res;
  }
  return *this - (-v);
}

BigInteger BigInteger::operator-(const BigInteger& v) const {
  BigInteger res;
  if (sign == v.sign) {
    if (abs() >= v.abs()) {
      res = *this;
      for (int i = 0, carry = 0; i < v.z.size() || carry; i++) {
        res.z[i] -= carry + (i < v.z.size() ? v.z[i] : 0);
        carry = res.z[i] < 0;
        if (carry) res.z[i] += base;
      }
    } else {
      res = -(v - *this);
    }
  } else {
    res = *this + (-v);
  }
  res.trim();
  res.sign = res.is_zero() ? 1 : res.sign;
  return res;
}

BigInteger BigInteger::operator*(const BigInteger& v) const {
  if (is_zero() || v.is_zero()) {
    return BigInteger(0);
  }
  BigInteger res = *this;
  res.z.resize(this->z.size() + v.z.size() + 2);
  std::fill(res.z.begin(), res.z.end(), 0);
  for (int i = 0; i < z.size(); i++) {
    for (long long j = 0, carry = 0LL; j < v.z.size() || carry > 0; j++) {
      long long s =
          static_cast<long long>(res.z[i + j]) + carry +
          static_cast<long long>(z[i]) *
              ((j < v.z.size()) ? static_cast<long long>(v.z[j]) : 0LL);
      res.z[i + j] = s % base;
      carry = s / base;
    }
  }
  res.sign *= v.sign;
  res.trim();
  res.sign = res.is_zero() ? 1 : res.sign;
  return res;
}

BigInteger BigInteger::operator/(const BigInteger& v) const {
  if (v.is_zero()) {
    throw std::overflow_error("BigInteger Division by Zero.");
  }
  if (v.abs() > abs() || is_zero()) {
    return BigInteger(0);
  }
  if (abs() == v.abs()) {
    return sign == v.sign ? BigInteger(1) : BigInteger(-1);
  }
  BigInteger res, cur, vv = v;
  cur.z.clear();
  vv.sign = 1;
  for (int i = static_cast<int>(this->z.size()) - 1; i >= 0; i--) {
    cur.z.insert(cur.z.begin(), this->z[i]);
    long long sol = -1LL, lo = 0LL, hi = base - 1;
    while (lo <= hi) {
      long long mid = lo + (hi - lo) / 2LL;
      BigInteger r = vv * BigInteger(mid);
      if (r < cur) {
        lo = mid + 1;
      } else if (r > cur) {
        hi = mid - 1;
      } else {
        sol = mid;
        break;
      }
    }
    if (sol == -1LL) {
      sol = lo;
    }
    while ((vv * BigInteger(sol)) < cur) {
      sol++;
    }
    while ((vv * BigInteger(sol)) > cur && sol > 0) {
      sol--;
    }
    cur = cur - vv * BigInteger(std::min(base - 1LL, std::max(0LL, sol)));
    res.z.insert(res.z.begin(), std::min(base - 1LL, std::max(0LL, sol)));
    cur.trim();
    res.trim();
  }
  res.sign = sign * v.sign;
  res.trim();
  res.sign = res.is_zero() ? 1 : res.sign;
  return res;
}

BigInteger BigInteger::operator-() const {
  BigInteger res = *this;
  res.sign = -sign;
  res.sign = res.is_zero() ? 1 : res.sign;
  return res;
}

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
  BigInteger res = *this;
  res.sign *= res.sign;
  return res;
}

bool BigInteger::operator<(const BigInteger& v) const {
  if (sign != v.sign) return sign < v.sign;
  if (z.size() != v.z.size()) return z.size() * sign < v.z.size() * v.sign;
  for (int i = z.size() - 1; i >= 0; i--)
    if (z[i] != v.z[i]) return z[i] * sign < v.z[i] * v.sign;
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

std::istream& operator>>(std::istream& stream, BigInteger& v) {
  std::string s;
  stream >> s;
  v = BigInteger(s);
  return stream;
}

std::ostream& operator<<(std::ostream& stream, BigInteger& v) {
  if (v.sign == -1) stream << '-';
  stream << (v.z.empty() ? 0 : v.z.back());
  for (int i = static_cast<int>(v.z.size()) - 2; i >= 0; --i)
    stream << std::setw(v.base_digits) << std::setfill('0') << v.z[i];
  return stream;
}

std::ostream& operator<<(std::ostream& stream, BigInteger v) {
  if (v.sign == -1) stream << '-';
  stream << (v.z.empty() ? 0 : v.z.back());
  for (int i = static_cast<int>(v.z.size()) - 2; i >= 0; --i)
    stream << std::setw(v.base_digits) << std::setfill('0') << v.z[i];
  return stream;
}
