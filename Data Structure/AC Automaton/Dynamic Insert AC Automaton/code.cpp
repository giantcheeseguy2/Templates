#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp> 
using namespace __gnu_pbds;
using namespace std;

#pragma GCC optimize("Ofast")
#pragma GCC optimization ("unroll-loops")
#pragma GCC target("avx,avx2,fma")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,tune=native")

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

vector<string> dict;
unordered_map<string, int> last;
bool removed[400000];

//assumes that all the dictionary words are unique
//Problem: Codeforces 710F
struct ac {
	vector<int> fail, e, words, prev;
	vector<vector<int>> g;

	void construct(vector<int> v){
		g.clear(), fail.clear(), e.clear(), words.clear(), prev.clear();
		words = v;
		g.pb(vector<int>(26, 0)), fail.pb(0), e.pb(-1), prev.pb(-1);
		for(int i = 0; i < v.size(); i++){
			int ind = v[i], m = dict[ind].length(), cur = 0;
			if(removed[ind]) continue;
			for(int j = 0; j < m; j++){
				int nxt = dict[ind][j] - 'a';
				if(!g[cur][nxt]) g.pb(vector<int>(26, 0)), fail.pb(0), e.pb(-1), prev.pb(-1), g[cur][nxt] = g.size() - 1;
				cur = g[cur][nxt];
			}
			e[cur] = ind;
		}
		queue<int> q;
		for(int i = 0; i < 26; i++) if(g[0][i]) q.push(g[0][i]);
		while(!q.empty()){
			int st = q.front();
			q.pop();
			for(int i = 0; i < 26; i++){
				if(g[st][i]){
					fail[g[st][i]] = g[fail[st]][i];
					prev[g[st][i]] = e[g[fail[st]][i]] == -1 ? prev[g[fail[st]][i]] : g[fail[st]][i];
					q.push(g[st][i]);
				} else {
					g[st][i] = g[fail[st]][i];
				}
			}
		}
	}
	
	void query(string &s, int &ans){
		if(words.size() == 0) return;
		int m = s.length(), cur = 0, x = 0;
		for(int i = 0; i < m; i++){
			cur = x = g[cur][s[i] - 'a'];
			while(x != -1){
				if(e[x] != -1 && !removed[e[x]]) ans++;
				x = prev[x];
			}
		}
	}

	void find(vector<int> &v){
		for(int i : words) v.pb(i);
	}
};

ac arr[21];
int start = 20;

void merge(){
	while(start + 1 <= 20 && arr[start].words.size() == arr[start + 1].words.size()){
		vector<int> words;
		arr[start].find(words), arr[start + 1].find(words);
		arr[start + 1].construct(words);
		start++;
	}
}

void insert(int x){
	arr[--start].construct(vector<int>{x});
	merge();
}

int query(string s){
	int ret = 0;
	for(int i = start; i < 20; i++) arr[i].query(s, ret);
	return ret;
}

int main(){
	setIO();
	int q;
	cin >> q;
	while(q--){
		int t;
		string s;
		cin >> t >> s;
		if(t == 1) dict.pb(s), insert(dict.size() - 1), last[s] = dict.size() - 1;
		else if(t == 2) removed[last[s]] = true;
		else cout << query(s) << endl;
	}
}