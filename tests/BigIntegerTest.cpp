#include <bits/stdc++.h>

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "BigInteger.cpp"
using namespace std;
void testBigInteger();

BigInteger make_a_BigInteger(int size) {
  vector<uint64_t> d(size);
  for (auto& e : d) {
    e = rand() % (1000 * 1000 * 1000);
  }
  d.push_back((rand() % 2) * (1000 * 1000 * 1000 - 1));
  return BigInteger(d);
}

void long_mul_karatsuba() {
  for (int i = 0; i < 1000; i++) {
    BigInteger a(make_a_BigInteger(200));
    BigInteger b(make_a_BigInteger(200));
    assert(karatsuba(a, b) == long_mul(a, b));
  }
}

void long_mul_karatsuba_benchmark() {
  vector<BigInteger> as;
  vector<BigInteger> bs;
  for (int k = 10; k < 100; k += 10) {
    as.clear();
    bs.clear();
    for (int i = 0; i < 1000; i++) {
      as.push_back(make_a_BigInteger(k));
      bs.push_back(make_a_BigInteger(k));
    }

    cout << "Starting " << k << " digit karatsuba\n";
    clock_t start = clock();
    for (int i = 0; i < 1000; i++) {
      karatsuba(as[i], bs[i]);
    }
    cout << clock() - start << '\n';

    cout << "Starting " << k << " digit long_mul\n";
    start = clock();
    for (int i = 0; i < 1000; i++) {
      long_mul(as[i], bs[i]);
    }
    cout << clock() - start << '\n';
  }
}

void long_div_benchmark() {
  vector<BigInteger> as;
  vector<BigInteger> bs;
  BigInteger c;
  as.clear();
  bs.clear();
  for (int i = 0; i < 100; i++) {
    as.push_back(make_a_BigInteger(100));
    bs.push_back(make_a_BigInteger(25));
  }

  cout << "Starting division benchmark\n";
  clock_t start = clock();
  for (int i = 0; i < 100; i++) {
    long_div(as[i], bs[i], c);
  }
  cout << clock() - start << '\n';
}

void testBigInteger() {
  BigInteger zero(0);
  // zero has a size of 1
  assert(zero.digits.size() == 1);
  assert(zero.digits.back() == 0);
  string sp1(1000, '1');
  string sp2(1000, '2');
  string sp3(1000, '3');
  BigInteger ones(sp1);
  BigInteger twos(sp2);
  BigInteger threes(sp3);
  BigInteger negative_ones('-' + sp1);
  BigInteger negative_twos('-' + sp2);
  BigInteger negative_threes('-' + sp3);

  assert(zero == -zero);

  // adding zero with all other numbers
  assert(zero + zero == zero);
  assert(zero + negative_ones == negative_ones);
  assert(negative_ones + zero == negative_ones);
  assert(zero + ones == ones);
  assert(ones + zero == ones);

  // subtracting zero with all other numbers
  assert(zero - zero == zero);
  assert(zero - negative_ones == ones);
  assert(negative_ones - zero == negative_ones);
  assert(zero - ones == negative_ones);
  assert(ones - zero == ones);

  // times zero with all other numbers
  assert(long_mul(zero, zero) == 0);
  assert(long_mul(zero, negative_ones) == 0);
  assert(long_mul(negative_ones, zero) == 0);
  assert(long_mul(zero, ones) == 0);
  assert(long_mul(ones, zero) == 0);

  assert(karatsuba(zero, zero) == 0);
  assert(karatsuba(zero, negative_ones) == 0);
  assert(karatsuba(negative_ones, zero) == 0);
  assert(karatsuba(zero, ones) == 0);
  assert(karatsuba(ones, zero) == 0);

  // division
  assert(zero / negative_ones == zero);
  assert(zero / ones == zero);

  // modulo
  assert(zero % negative_ones == zero);
  assert(zero % ones == zero);

  try {
    BigInteger(100) / 0;
    throw "Expected BigInteger Division by Zero Error.";
  } catch (std::domain_error e) {
    // expected an divide by zero exception
  } catch (const char* e) {
    cout << e << '\n';
  }

  assert(ones + twos == threes);
  assert(twos + ones == threes);
  assert(ones + negative_twos == negative_ones);
  assert(negative_twos + ones == negative_ones);
  assert(negative_ones + negative_twos == negative_threes);
  assert(negative_twos + negative_ones == negative_threes);

  assert(ones - twos == negative_ones);
  assert(twos - ones == ones);
  assert(ones - negative_twos == threes);
  assert(negative_twos - ones == negative_threes);
  assert(negative_ones - negative_twos == ones);
  assert(negative_twos - negative_ones == negative_ones);

  assert(long_mul(BigInteger(2), ones) == twos);
  assert(long_mul(ones, -2) == negative_twos);
  assert(long_mul(negative_twos, -1) == twos);
  assert(long_mul(BigInteger(-1), negative_twos) == twos);
  assert(long_mul(negative_twos, -1) == twos);

  assert(karatsuba(BigInteger(2), ones) == twos);
  assert(karatsuba(ones, -2) == negative_twos);
  assert(karatsuba(negative_twos, -1) == twos);
  assert(karatsuba(BigInteger(-1), negative_twos) == twos);
  assert(karatsuba(negative_twos, -1) == twos);

  assert(ones / twos == 0);
  assert(twos / ones == 2);
  assert(ones / negative_twos == 0);
  assert(negative_twos / ones == -2);
  assert(negative_ones / negative_twos == 0);
  assert(negative_twos / negative_ones == 2);

  assert(ones % twos == ones);
  assert(twos % ones == 0);
  assert(ones % negative_twos == negative_ones);
  assert(negative_twos % ones == 0);
  assert(negative_ones % negative_twos == negative_ones);
  assert(negative_twos % negative_ones == 0);

  // small numbers
  BigInteger a(11);
  BigInteger b(11);
  assert(a + b == BigInteger(22));
  assert(a - b == BigInteger(0));
  assert(a * b == BigInteger(121));
  assert(a / b == BigInteger(1));

  for (int i = 0; i < 1000; i++) {
    long long x = 0;
    long long y = 0;
    for (int j = 0; j < 100; j++) {
      x += rand() % 1000000000000000;
      y += rand() % 100;
    }
    BigInteger xy = BigInteger(x) / BigInteger(y);
    assert(xy == x / y);
  }

  // test some overflows
  assert(BigInteger(1000 * 1000 * 1000 - 1) + BigInteger(1) ==
         BigInteger(1000 * 1000 * 1000));
  assert(BigInteger(1) + BigInteger(1000 * 1000 * 1000 - 1) ==
         BigInteger(1000 * 1000 * 1000));
  assert(BigInteger(-1000 * 1000 * 1000 + 1) + BigInteger(-1) ==
         BigInteger(-1000 * 1000 * 1000));
  assert(BigInteger(-1) + BigInteger(-1000 * 1000 * 1000 + 1) ==
         BigInteger(-1000 * 1000 * 1000));
  assert(BigInteger(-1) + BigInteger(-(zero.base - 1)) ==
         BigInteger(-zero.base));
  assert(BigInteger(-(zero.base - 1)) + BigInteger(-1) ==
         BigInteger(-zero.base));
  assert(BigInteger(1) + BigInteger((zero.base - 1)) == BigInteger(zero.base));
  assert(BigInteger((zero.base - 1)) + BigInteger(1) == BigInteger(zero.base));

  BigInteger c("12312312312312313123123");
  BigInteger d("78787873241614257823");
  BigInteger e = c * d;
  BigInteger f = c - d;
  BigInteger g = d - c;
  assert(e == BigInteger("970060901773629064105172807606746864941229"));
  assert(f == BigInteger("12233524439070698865300"));
  assert(g == BigInteger("-12233524439070698865300"));
  BigInteger h = BigInteger("1");
  BigInteger i = BigInteger("3");
  BigInteger j = h - i;
  assert(j == -2);
  BigInteger k = BigInteger(1000000001);
  assert(k == 1000000001);
  BigInteger l = BigInteger(-1000000001);
  assert(l == -1000000001);
  a = j;
  assert(a == -2);

  assert(g < f);
  assert(f < e);
  assert(-e < -f);
  assert(l < f);
  assert(BigInteger(-1) - BigInteger(-1) == BigInteger(0));

  assert(BigInteger(0) > -1ll);
  assert(BigInteger(0) <= 0ll);
  assert(BigInteger(0) <= 1ll);
  assert(BigInteger(0) >= 0ll);
  assert(BigInteger(0) >= -1ll);
  assert(BigInteger(0) == 0ll);
  assert(BigInteger(0) != 1ll);

  BigInteger z = BigInteger(100);
  assert(++z == 101);
  assert(z == 101);
  assert(z++ == 101);
  assert(z == 102);

  assert(--z == 101);
  assert(z == 101);
  assert(z++ == 101);
  assert(z == 102);

  ifstream fin("BigIntegero.txt");
  string s, sa, sb;
  while (fin >> sa >> sb) {
    BigInteger a(sa);
    BigInteger b(sb);
    fin >> s;
    assert(a + b == BigInteger(s));
    fin >> s;
    assert(a - b == BigInteger(s));
    fin >> s;
    assert(long_mul(a, b) == BigInteger(s));
    assert(karatsuba(a, b) == BigInteger(s));
    fin >> s;
    assert(a.abs() / b.abs() == BigInteger(s));
    fin >> s;
    assert(a % b == BigInteger(s));
  }
  for (int i = 0; i < 62; i++) {
    assert(pow(BigInteger(2), i) == BigInteger(1ll << i));
  }

  // deep copy test
  BigInteger m(1);
  assert(m == 1);
  BigInteger n(2);
  assert(n == 2);
  m = n;
  assert(m == 2);
  n = 12;
  assert(m == 2);
  assert(n == 12);
  long_mul_karatsuba();
  long_mul_karatsuba_benchmark();
  long_div_benchmark();
}
