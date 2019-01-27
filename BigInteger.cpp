#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

class BigInteger {
 public:
  const int base = 1000000000;
  const int base_digits = 9;
  std::vector<int> z;
  int sign;
  BigInteger() : sign(1), z(0){};
  ~BigInteger() { z.clear(); }
  BigInteger(const int);
  BigInteger(const long long);
  BigInteger(const std::string&);
  BigInteger(const BigInteger&);

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

BigInteger::BigInteger(const int v) {
  sign = 1;
  int val = v;
  if (v < 0) {
    sign = -1;
    val = -val;
  }
  if (val == 0) {
    z.push_back(0);
  }
  while (val) {
    z.push_back(val % base);
    val /= base;
  }
  trim();
}

BigInteger::BigInteger(const long long v) {
  sign = 1;
  long long val = v;
  if (v < 0) {
    sign = -1;
    val = -val;
  }
  if (val == 0) {
    z.push_back(0);
  }
  while (val) {
    z.push_back(val % base);
    val /= base;
  }
  trim();
}

BigInteger::BigInteger(const std::string& s) {
  sign = 1;
  int pos = 0;
  for (; pos < static_cast<int>(s.size()) && (s[pos] == '-' || s[pos] == '+');
       pos++) {
    if (s[pos] == '-') sign = -sign;
  }
  for (int i = s.size() - 1; i >= pos; i -= base_digits) {
    int x = 0;
    for (int j = std::max(pos, i - base_digits + 1); j <= i; j++)
      x = x * 10 + s[j] - '0';
    z.push_back(x);
  }
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
    return res;
  }
  return *this - (-v);
}

BigInteger BigInteger::operator-(const BigInteger& v) const {
  if (sign == v.sign) {
    if (abs() >= v.abs()) {
      BigInteger res = *this;
      for (int i = 0, carry = 0; i < v.z.size() || carry; i++) {
        res.z[i] -= carry + (i < v.z.size() ? v.z[i] : 0);
        carry = res.z[i] < 0;
        if (carry) res.z[i] += base;
      }
      res.trim();
      return res;
    }
    return -(v - *this);
  }
  return *this + (-v);
}

BigInteger BigInteger::operator*(const BigInteger& v) const {
  BigInteger res = *this;
  res.z.resize(this->z.size() + v.z.size());
  std::fill(res.z.begin(), res.z.end(), 0);
  for (int i = 0; i < z.size(); i++) {
    for (int j = 0, carry = 0; j < v.z.size() || carry > 0; j++) {
      long long s = (long long)res.z[i + j] + (long long)carry +
                    (long long)z[i] * ((j < v.z.size()) ? v.z[j] : 0);
      res.z[i + j] = s % base;
      carry = s / base;
    }
  }
  res.trim();
  return res;
}

BigInteger BigInteger::operator/(const BigInteger& v) const {
  if (v == 0) {
    throw std::overflow_error("BigInteger Division by Zero.");
  }
  BigInteger res, cur;
  for (int i = static_cast<int>(this->z.size()) - 1; i >= 0; i--) {
    cur.z.insert(cur.z.begin(), this->z[i]);
    int x = 0, lo = 0, hi = base + 1;
    while (lo < hi) {
      int mid = lo + (hi - lo) / 2;
      if (v * BigInteger(mid) < cur) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    cur = cur - BigInteger(lo - 1) * v;
    res.z.insert(res.z.begin(), lo - 1);
  }
  res.trim();
  return res;
}

BigInteger BigInteger::operator-() const {
  BigInteger res = *this;
  res.sign = -sign;
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
    if (z[i] != v.z[i]) return z[i] * sign < v.z[i] * sign;
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
