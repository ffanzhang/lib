#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

namespace tsp {

#define MAX_N 15
#define INF 0x3f3f3f3f 
#define min(a, b) ((a < b) ? (a) : (b))

ll dp[(1<<MAX_N)][MAX_N+1];
ll c[MAX_N+1][MAX_N+1];
int masks[MAX_N+1][(1<<MAX_N)];
int sz[MAX_N+1];
int ids[MAX_N+1];

void init() {
    for (int mask = 2; mask < (1 << MAX_N); mask++) {
        int num = __builtin_popcount(mask);
        // mask must include the starting position
        if (mask & 1) {
            masks[num][sz[num]++] = mask;
        }
    }
}

// O(2^n * n^2)
ll solve(int n) {
    memset(dp, 0, sizeof(dp));
    // for path length from 2 to n
    for (int pl = 2; pl <= n; pl++) {
        // iterate through possible subsets of size pl
        for (int k = 0; k < sz[pl]; k++) { 
            int mask = masks[pl][k];
            dp[mask][0] = INF;
            int ct = 0;
            memset(ids, 0, sizeof(ids));
            // extract individual bits out of a mask 
            int mask_copy = mask;
            while (mask_copy) {
                int lsb = mask_copy & -mask_copy;
                mask_copy -= mask_copy & -mask_copy;
                int index = log2(lsb);
                ids[ct++] = index;
            }
            for (int j = 0; j < ct; j++) {
                dp[mask][ids[j]] = INF;
                if (ids[j] != 0) {
                    for (int i = 0; i < ct; i++) { 
                        if (ids[j] != ids[i]) {
                            dp[mask][ids[j]] = min(dp[mask][ids[j]], dp[mask - (1 << ids[j])][ids[i]] + c[ids[i]][ids[j]]);
                        }
                    }
                }
            }
        }
    }
    ll ans = INF;
    for (int i = 1; i < n; i++) {
        ans = min(ans, dp[(1<<n)-1][i] + c[i][0]);
    }
    return ans;
}

}; // namespace tsp

const char* test_data = R"__(
1
4
0 2 6 7
3 0 8 3 
5 9 0 1 
8 4 1 0
)__";

int main() {
    tsp::init();
    stringstream ss;
    ss << test_data;
    int n, t;
    ss >> t;
    while (t--) {
        ss >> n;
        for (int j = 0; j < n ; j++) {
            for (int i = 0; i < n; i++) {
                ss >> tsp::c[j][i];
            }
        }
        ll ans = tsp::solve(n);
        std::cout << ans;
        assert(ans == 11);
    }
    return 0;
}

// SPOJ
// PESADA04
