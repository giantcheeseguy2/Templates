#include <bits/stdc++.h>
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
inline int random(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

int main(int, char* argv[]) {

  int n = random(1, 3);
  int m = random(1, 3);
  int k = random(1, 3);
  int MOD = 998244353;

  printf("%d %d %d\n", n, m, k);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      printf("%d", random(0, MOD - 1));
      if (j + 1 != m) printf(" ");
    }
    printf("\n");
  }

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < k; j++) {
      printf("%d", random(0, MOD - 1));
      if (j + 1 != k) printf(" ");
    }
    printf("\n");
  }

  return 0;
}