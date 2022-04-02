#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp> 
using namespace __gnu_pbds;
using namespace std;

#define pb push_back
#define ff first
#define ss second

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll>   pll;

const int inf = 1000000000;
const int mod = 1000000007;
const double PI = acos(-1);

struct chash {
	const uint64_t C = ll(2e18*PI)+71;
	const int RANDOM = rand();
	ll operator()(ll x) const {
		return __builtin_bswap64((x^RANDOM)*C);
	}
};

template<class K> using sset =  tree<K, null_type, less<K>, rb_tree_tag, tree_order_statistics_node_update>;
template<class K, class V> using gphash = gp_hash_table<K, V, chash, equal_to<K>, direct_mask_range_hashing<>, linear_probe_fn<>, hash_standard_resize_policy< hash_exponential_size_policy<>, hash_load_check_resize_trigger<>, true> >;

inline ll ceil0(ll a, ll b) { 
    return a / b + ((a ^ b) > 0 && a % b); 
} 
inline ll floor0(ll a, ll b) { 
    return a / b - ((a ^ b) < 0 && a % b); 
} 

void setIO() {
	ios_base::sync_with_stdio(0); cin.tie(0);	
}

const int maxn = 1000000;

int n, g[maxn][26], e[maxn], fail[maxn], sz = 0;
string dict[maxn];

void construct(){
	memset(g, 0, sizeof(g));
	memset(e, 0, sizeof(e));
	memset(fail, 0, sizeof(fail));
	for(int i = 1; i <= n; i++){
		int m = (int)dict[i].size(), cur = 0;
		for(int j = 0; j < m; j++){
			int nxt = dict[i][j] - 'a';
			if(!g[cur][nxt]) g[cur][nxt] = ++sz;
			cur = g[cur][nxt];
		}
		e[cur] = i;
	}
	queue<int> q;
	for(int i = 0; i < 26; i++) if(g[0][i]) q.push(g[0][i]);
	while(!q.empty()){
		int st = q.front();
		q.pop();
		for(int i = 0; i < 26; i++){
			int nxt = g[st][i];
			if(nxt){
				fail[nxt] = g[fail[st]][i];
				q.push(nxt);
			} else {
				g[st][i] = g[fail[st]][i];
			}
		}
	}
}

int occ[maxn];

void query(string s){
	memset(occ, 0, sizeof(occ));
	int cur = 0, x = 0;
	for(int i = 0; i < (int)s.size(); i++){
		cur = x = g[cur][s[i] - 'a'];
		while(x){
			if(e[x]) occ[e[x]]++;
			x = fail[x];
		}
	}
}

int main(){
	setIO();	
	string s;
	cin >> s >> n;
	for(int i = 1; i <= n; i++) cin >> dict[i];
	construct();
	query(s);
	for(int i = 1; i <= n; i++) cout << occ[i] << endl;
}