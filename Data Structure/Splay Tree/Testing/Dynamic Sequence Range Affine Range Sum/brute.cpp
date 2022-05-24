#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <utility>
#include <cassert>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include <set>
#include <map>
 
#define ll long long
#define lb long double
#define sz(vec) ((int)(vec.size()))
#define all(x) x.begin(), x.end()
#define pb push_back
#define mp make_pair
#define kill(x, s) {int COND = x; if(COND){ cout << s << "\n"; return ; }}
 
const lb eps = 1e-9;
//const ll mod = 1e18, ll_max = 1e18;
const ll mod = (1 << (23)) * 119 +1, ll_max = 1e18;
const int MX = 1000000000, int_max = 0x3f3f3f3f;
 
struct {
	template<class T>
		operator T() {
			T x; std::cin >> x; return x;
		}
} in;
 
using namespace std;
 
#define lc c[0]
#define rc c[1]
#define ile(u) (tr[tr[u].par].lc != u)
struct splay_tree{
	struct value{
		ll val; int sz;
		ll aggr;
		value(){val = sz = 0; aggr = 0ll;}
		value(ll c, ll b, int s){val = c, aggr = b, sz = s; }
 
	};
	struct tag{
		ll add, mul, rev;
		tag(ll a, ll b, ll c){ add = a, mul = b, rev = c; }
		tag(){ add = 0, mul = 1, rev = 0;}
		tag operator * (const tag& b) const{
			if(isid()) return b;
			if(b.isid()) return *this;
			return tag( (1ll*mul*b.add%mod + add)%mod, 1ll*mul*b.mul%mod, rev^b.rev);
		}
		value operator + (const value& b) const{
			if(isid()) return b;
			return value((1ll*b.val*mul%mod +add)%mod, (1ll*b.aggr*mul%mod +1ll*add*b.sz%mod)%mod, b.sz);
		}
		bool isid() const{ return (add == 0) && (mul == 1) && (rev == 0); }
	} ;
	struct node{
		int par;
		int c[2];
		value v;
		tag t;
		node(){ par =c[0] = c[1] = 0; }
	} ;
	vector<node> tr;
	void pull(int u){
		tr[u].v.aggr = (tr[tr[u].lc].v.aggr + tr[tr[u].rc].v.aggr + tr[u].v.val) %mod;
		tr[u].v.sz = tr[tr[u].lc].v.sz + tr[tr[u].rc].v.sz + 1;
	}
	void apply(int u, tag& t){
		if(!u) return ;
		tr[u].t = t * tr[u].t;
		tr[u].v = t + tr[u].v;
	}
	void push(int u){
		if((!u) || tr[u].t.isid()) return ;
		if(tr[u].t.rev){
			swap(tr[u].lc, tr[u].rc);
		}
		apply(tr[u].lc, tr[u].t);
		apply(tr[u].rc, tr[u].t);
		tr[u].t = tag();
	}
	splay_tree(){
		node zero = node();
		zero.v = value();
		zero.t = tag();
		tr.pb(zero);
	}
 
	void pr(int u){
		if(!u) return ;
		push(u);
		pr(tr[u].lc);
		//if(tr[u].v.val)
		cout << tr[u].v.val << " "; 
		pr(tr[u].rc);
	}
 
	void rotate(int u, int dir){
 
		int r = u;
		int g = tr[u].c[dir];
		int a = tr[tr[u].c[dir]].c[dir^1];
		//order of pushing: tr[u].par, u, g
		//changes to c
		push(tr[u].par);
		push(u);
		push(g);
		if(tr[r].par){
			if(!ile(r)) tr[tr[r].par].lc = g;
			else tr[tr[r].par].rc = g;
		}
		assert(r && g);
		tr[g].c[dir^1] = r; 
		tr[r].c[dir] = a;
		//changes to par
		tr[g].par = tr[r].par;
		tr[r].par = g;
		if(a) tr[a].par = r;
		//order of pulling: r, g, tr[g].par	
		pull(r);
		pull(g);
		if(tr[g].par) pull(tr[g].par);
	}
	void splay(int u){
		while(tr[u].par){
			int p = tr[u].par, gp = tr[p].par;
			if(gp == 0){
				//zig
				rotate(p, ile(u));
			}else if(ile(p)^ile(u)){
				//zig zag
				rotate(p, ile(u));
				rotate(gp, ile(u));
			}else{
				//zig zig
				rotate(gp, ile(p));
				rotate(p, ile(u));
			}
 
		}		
		assert(tr[0].lc == tr[0].rc && tr[0].rc == 0);
	}
 
	int find(int& rt, int key){
		int u = rt;
		int ans = 0;
		while(u != 0){
			push(u);
			if(key == tr[tr[u].lc].v.sz){
				ans = u;
				break ;
			}
			if(key < tr[tr[u].lc].v.sz+1){
				u = tr[u].lc;
			}else{
				key -= tr[tr[u].lc].v.sz + 1;
				u = tr[u].rc;
			}
		}
		if(ans){
			splay(ans);
			rt = ans;
		}
		return ans; //leftmose occurance?
	}
 
 
	int insert(int& rt, int ind, int key){
		//if(find(rt, key)) return 0;
		int u = rt;
		int v;
		while(1){
			push(u);
			int dir = ind >= tr[tr[u].lc].v.sz+1;
			if((tr[u].c[dir] == 0 || rt == 0)){
				node nv = node();
				nv.par = u;
				nv.v = value(key, key, 1);
				nv.t = tag();
				if(u) tr[u].c[dir] = sz(tr);
				v = sz(tr);
				tr.pb(nv);
				break ;
			}
			ind -= dir*(tr[tr[u].lc].v.sz + 1);
			u = tr[u].c[dir];
		}
		//for(int j = tr[v].par; j; j = tr[j].par){
			//pull(j);
		//}
		if(v){
			splay(v);
			rt = v;
		}
		return v; 
	}
 
	int erase(int& rt, int key){
		if(find(rt, key) == 0) return 0;
		push(rt);
		int l = tr[rt].lc, r = tr[rt].rc;
		tr[l].par = tr[r].par = 0; //f
		int v = r;
		if(v){
			while(1){
				push(v);
				if(tr[v].lc == 0) break ;
				v = tr[v].lc;
			}
			splay(v);
			assert(tr[v].lc == 0);
			tr[v].lc = l;
		}
		rt = v ? v : l;
		tr[l].par = v;
		//pull(v);
		return 1;
	}
 
	void update(int& rt, int l, int r, tag t){
		find(rt, r+1);
		push(rt);
 
		int r1 = tr[rt].c[0];
		int p = tr[r1].par;
		tr[r1].par = 0;
		int r2 = find(r1, l-1);
		push(rt);
		int r3 = tr[r2].c[1];
		assert(r3);
		//push(r2);
 
		apply(r3, t);
		tr[r2].par = p;
		tr[p].c[0] = r2;
		assert(p == rt);
	}	
 
	ll sum(int& rt, int l, int r){
		ll L = 0, R = 0;
		find(rt, l);
		
		push(rt);
		L = tr[tr[rt].lc].v.aggr;
		find(rt, r+1);
 
		push(rt);
		R = tr[tr[rt].lc].v.aggr;
		return (R  + mod - L)%mod;
	}
 
};
#undef lc
#undef rc
#undef ile

void solve(){
	splay_tree bbst = splay_tree();
	int rt = 0;
	int n = in, q = in;
	bbst.insert(rt, 0, 0);
	bbst.insert(rt, 0, 0);
	for(int i = 1; i<=n; i++){
		int a; cin >> a;
		bbst.insert(rt, i, a);
	}
	for(int i = 1; i<=q; i++){
		int op = in;
		if(op == 0){
			int ind, a;
			cin >> ind >> a;
			ind++;
			bbst.insert(rt, ind, a);
		}else if(op == 1){
			int ind;
			cin >> ind;
			ind++;
			bbst.erase(rt, ind);
		}else if(op == 2){
			int l, r;
			cin >> l >> r;
			l++;
			bbst.update(rt, l, r, splay_tree::tag(0, 1, 1));
		}else if(op == 3){
			int l, r, m, a;
			cin >> l >> r >> m >> a;
			l++;
			bbst.update(rt, l, r, splay_tree::tag(a, m, 0));
		}else{
			int l, r;
			cin >> l >> r;
			l++;
			cout << bbst.sum(rt, l, r) << "\n";
		}
	}
}


int main(){
	cin.tie(0) -> sync_with_stdio(0);

	int T = 1;
	//cin >> T;
	for(int i = 1; i<=T; i++){
		solve();
	}
	return 0;
}