//here take a cat
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <utility>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <vector>
#include <random>
#include <chrono>
#include <queue>
#include <set>

#define ll long long
#define lb long double
#define pii pair<int, int>
#define pb push_back
#define mp make_pair
#define ins insert
#define cont continue
#define pow2(n) (1 << (n))

#define LC(n) (((n) << 1) + 1)
#define RC(n) (((n) << 1) + 2)
//#define add(a, b) (((a)%mod + (b)%mod)%mod)
#define mul(a, b) (((a)%mod * (b)%mod)%mod)
#define init(arr, val) memset(arr, val, sizeof(arr))
#define bckt(arr, val, sz) memset(arr, val, sizeof(arr[0]) * (sz))
#define uid(a, b) uniform_int_distribution<long long>(a, b)(rng)
#define tern(a, b, c) ((a) ? (b) : (c))
#define feq(a, b) (fabs(a - b) < eps)

#define moo printf
#define oom scanf
#define mool puts("") 
#define orz assert
#define fll fflush(stdout)
#define sz(x) (int)(x.size())

const lb eps = 1e-9;
const ll mod = 1e9 + 7, ll_max = (ll)1e18;
const int MX = 1e4 +10, int_max = 0x3f3f3f3f;

using namespace std;
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int D = 20;

int main(){
	cin.tie(0) -> sync_with_stdio(0);
	int n = 5, q = 5;
	cout << n << " " << q << "\n";
	for(int i = 1; i<=n; i++){
		cout << uid(0, D) << " ";
	}
	cout << endl;
	for(int i = 1; i<=q; i++){
		int op = uid(0, 4);
		cout << op << " ";
		if(op == 0){
			cout << uid(0, n) << " " << uid(0, D) << "\n";
			n++;
		}else if(op == 1){
			cout << uid(0, n-1) << "\n";
			n--;
		}else if(op == 3){
			int l = uid(0, n-1), r = uid(l+1, n);
			cout << l << " " << r << " " << uid(0, D) << " " << uid(0, D) << "\n";
		}else{
			int l = uid(0, n-1), r = uid(l+1, n);
			cout << l << " " << r << "\n";
		}
	}
	return 0;
}	