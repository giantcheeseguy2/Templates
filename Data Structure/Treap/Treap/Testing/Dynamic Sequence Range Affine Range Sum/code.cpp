//https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
#include <bits/stdc++.h>
using namespace std;

template<class mod>
struct Modular {

    using T = typename mod::type;
    using C = typename mod::cast;

    template<class U>
    static T fpow(T a, U b){
        T ret = 1;
        while(b){
            if(b%2 == 1) ret = (C)ret*a%mod::mod;
            a = (C)a*a%mod::mod;
            b /= 2;
        }
        return ret;
    }

    T val;

    Modular(){
        val = 0;
    }

    template<class U>
    Modular(const U &x){
        val = x%mod::mod;
        if(val < 0) val += mod::mod;
    }

    const T Mod() const { return mod::mod; }

    const T &operator()() const { return val; }

    template<class U>
    explicit operator U() const { return static_cast<U>(val); } 

    Modular operator-(){ return Modular<mod>(-val); }

    Modular &operator+=(const Modular &x){
        val = (val + x.val)%mod::mod;
        return *this;
    }

    Modular &operator-=(const Modular &x){
        val = (val - x.val + mod::mod)%mod::mod;
        return *this;
    }

    Modular &operator*=(const Modular &x){
        val = (C)val*x.val%mod::mod;
        return *this;
    }

    Modular &operator/=(const Modular &x){
        val = (C)val*fpow(x.val, mod::mod - 2)%mod::mod;
        return *this;
    }

    Modular &operator%=(const Modular &x){
        val = val%x.val;
        return *this;
    }

    template<class U>
    Modular &operator^=(const U &x){
        assert(("be careful when raising to a modded power", typeid(x) != typeid(*this)));
        val = fpow(val, x);
        return *this;
    }

    Modular &operator++(){ return *this += 1; }

    Modular &operator--(){ return *this -= 1; }

    Modular operator++(int){
        Modular ret = *this;
        ++*this;
        return ret;
    }

    Modular operator--(int){
        Modular ret = *this;
        --*this;
        return ret;
    }

    friend Modular operator+(const Modular &a, const Modular &b){ return Modular(a.val) += b; }

    friend Modular operator-(const Modular &a, const Modular &b){ return Modular(a.val) -= b; }

    friend Modular operator*(const Modular &a, const Modular &b){ return Modular(a.val) *= b; }

    friend Modular operator/(const Modular &a, const Modular &b){ return Modular(a.val) /= b; }

    friend Modular operator%(const Modular &a, const Modular &b){ return Modular(a.val) %= b; }

    template<class U>
    friend Modular operator^(const Modular &a, const U &b){ return Modular(a.val) ^= b; }

    bool hasSqrt(){
        return val == 0 || fpow(val, (mod::mod - 1)/2) == 1;
    }

    Modular sqrt(){
        if(val == 0) return 0;
        if(val == 1) return 1;
        assert(hasSqrt());
        Modular r = 1, c = 1, a = 1, b = 1, aa, bb;
        while(((r*r - val) ^ ((mod::mod - 1)/2)) == 1) r++;
        Modular mult = r*r - val;
        T x = (mod::mod + 1)/2;
        while(x){
            if(x%2 == 1){
                aa = c*b*mult + a*r;
                bb = c*a + r*b;
                a = aa, b = bb;
            }
            aa = c*c*mult + r*r;
            bb = 2*c*r;
            r = aa, c = bb;
            x /= 2;
        }
        if(a >= (mod::mod + 1)/2) a *= -1;
        return a;
    }

    friend bool operator<(const Modular &a, const Modular &b){ return a.val < b.val; }

    friend bool operator<=(const Modular &a, const Modular &b){ return a.val <= b.val; }

    friend bool operator>(const Modular &a, const Modular &b){ return a.val > b.val; }

    friend bool operator>=(const Modular &a, const Modular &b){ return a.val >= b.val; }

    friend bool operator==(const Modular &a, const Modular &b){ return a.val == b.val; }

    friend bool operator!=(const Modular &a, const Modular &b){ return a.val != b.val; }

    friend ostream &operator<<(ostream &out, Modular x){ return out << x.val; }

    friend istream &operator>>(istream &in, Modular &x){ 
        in >> x.val;
        x.val = x.val%mod::mod;
        if(x.val < 0) x.val += mod::mod;
        return in;
    }

    string to_string(const Modular&x) { return to_string(x.val); }
};

struct Mod { 
    using type = int;
    using cast = long long;
    const static type mod = 998244353;
};

using mint = Modular<Mod>;

struct Treap {

    using T = mint;
    using L = pair<mint, mint>;

    static L empty(){ return {1, 0}; }

    static T comb(T a, T b){
        return a + b;
    }

    static void apply_val(T &x, L v, int sz){
        x = v.first*x + v.second*sz;
    }

    static void apply_tag(L &x, L v, int sz){
        x = {v.first*x.first, v.first*x.second + v.second};
    }

    struct node {
        array<int, 2> child;
        int ind, sz, weight, rev;
        T val, sum;
        L tag;

        node(int i, T v){
            ind = i;
            val = sum = v;
            sz = 1;
            child = {-1, -1};
            tag = empty();
            rev = 0;
            weight = rand();
        }

        int l(){ return child[0]; }
        int r(){ return child[1]; }
    };
    
    int n;
    vector<node> treap;

    Treap(){
        n = 0;
    }

    void pull(int ind){
        if(ind == -1) return;
        treap[ind].sz = 1;
        treap[ind].sum = treap[ind].val;
        for(int i = 0; i <= 1; i++){
            int nxt = treap[ind].child[i];
            if(nxt != -1){
                treap[ind].sum = comb(treap[ind].sum, treap[nxt].sum);
                treap[ind].sz += treap[nxt].sz;
            }
        }
    }

    void push(int ind){
        if(ind == -1) return;
        if(treap[ind].rev == 0 && treap[ind].tag == empty()) return;
        if(treap[ind].rev) swap(treap[ind].child[0], treap[ind].child[1]);
        for(int i = 0; i <= 1; i++){
            int nxt = treap[ind].child[i];
            if(nxt != -1){
                int sz = treap[nxt].sz;
                apply_val(treap[nxt].val, treap[ind].tag, 1);
                apply_val(treap[nxt].sum, treap[ind].tag, sz);
                apply_tag(treap[nxt].tag, treap[ind].tag, sz);
                treap[nxt].rev ^= treap[ind].rev;
            }
        }
        treap[ind].rev = 0;
        treap[ind].tag = empty();
    }

    void reserve(int _n){
        treap.reserve(_n);
    }

    int newnode(T v){
        treap.push_back(node(n, v));
        return n++;
    }

    //splits rt's tree into [0, k) [k, sz)
    pair<int, int> split(int rt, int k){
        if(rt == -1) return pair<int, int>{-1, -1};
        push(rt);
        int nxt = (treap[rt].child[0] == -1 ? 0 : treap[treap[rt].child[0]].sz);
        pair<int, int> ret;
        if(nxt == k){
            ret = {treap[rt].child[0], rt};
            treap[rt].child[0] = -1;
        } else if(nxt < k){
            ret = split(treap[rt].r(), k - nxt - 1);
            treap[rt].child[1] = ret.first;
            ret.first = rt;
        } else {
            ret = split(treap[rt].l(), k);
            treap[rt].child[0] = ret.second;
            ret.second = rt;
        }
        pull(rt);
        return ret;
    }

    int merge(int a, int b){
        if(a == -1) return b;
        if(b == -1) return a;
        if(treap[a].weight < treap[b].weight){
            push(a);
            treap[a].child[1] = merge(treap[a].child[1], b);
            pull(a);
            return a;
        } else {
            push(b);
            treap[b].child[0] = merge(a, treap[b].child[0]);
            pull(b);
            return b;
        }
    }

    void insert(int &rt, int x, T v){
        if(rt == -1){
            rt = newnode(v);
            return;
        }
        pair<int, int> nw = split(rt, x);
        rt = merge(nw.first, merge(newnode(v), nw.second));
    }

    void erase(int &rt, int x){
        pair<int, int> a = split(rt, x);
        pair<int, int> b = split(a.second, 1);
        rt = merge(a.first, b.second);
    }

    void reverse(int &rt, int l, int r){
        pair<int, int> a = split(rt, l);
        pair<int, int> b = split(a.second, r - l + 1);
        treap[b.first].rev ^= 1;
        rt = merge(a.first, merge(b.first, b.second));
    }

    void update(int &rt, int l, int r, L v){
        pair<int, int> a = split(rt, l);
        pair<int, int> b = split(a.second, r - l + 1);
        apply_val(treap[b.first].val, v, 1);
        apply_val(treap[b.first].sum, v, r - l + 1);
        apply_tag(treap[b.first].tag, v, r - l + 1);
        rt = merge(a.first, merge(b.first, b.second)); 
    }

    T query(int &rt, int l, int r){
        pair<int, int> a = split(rt, l);
        pair<int, int> b = split(a.second, r - l + 1); 
        T ret = treap[b.first].sum;
        rt = merge(a.first, merge(b.first, b.second));
        return ret;
    }
};

int main(){
    int n, q;
    cin >> n >> q;
    Treap treap;
    treap.reserve(n + q);
    int rt = -1;
    for(int i = 0; i < n; i++){
        int x;
        cin >> x;
        treap.insert(rt, i, x);
    }
    while(q--){
        int t;
        cin >> t;
        if(t == 0){
            int a, b;
            cin >> a >> b;
            treap.insert(rt, a, b);
        } else if(t == 1){
            int x;
            cin >> x;
            treap.erase(rt, x);
        } else if(t == 2){
            int l, r;
            cin >> l >> r;
            treap.reverse(rt, l, r - 1);
        } else if(t == 3){
            int l, r, b, c;
            cin >> l >> r >> b >> c;
            treap.update(rt, l, r - 1, {b, c});
        } else {
            int l, r;
            cin >> l >> r;
            cout << treap.query(rt, l, r - 1) << "\n";
        }
    }
}