#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MAX = (1 << 23);
ll bit[MAX+1];

// 1 indexed for both modify and query
ll query(ll x) {
    ll res = 0;
    while (x > 0) {
        res += bit[x];
        x -= x & -x;
    }
    return res;
}

void modify(ll x, ll val) {
    while (x <= MAX) {
        bit[x] += val;
        x += x & -x;
    }
}

const char* test_data = R"__(
10 4
+ 7 23
? 8
+ 3 17
? 8
)__";

const char* test_out = R"__(23
40
)__";


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    stringstream s_in(test_data);
    stringstream s_out;

    string op;
    ll n, q;
    ll x, delta;
    s_in >> n >> q;
    for (int i = 0; i < q; i++) {
        s_in >> op;
        if (op[0] == '+') {
            s_in >> x >> delta;
            x++;
            modify(x, delta);
        } else {
            s_in >> x;
            if (x == 0) {
                s_out << 0 << '\n';
            } else {
                ll ans = query(x);
                s_out << ans << '\n';
            }
        }
    }
    assert(strcmp(s_out.str().c_str(), test_out) == 0);
    return 0;
}

// kattis: 
// fenwick: use fast io!!

