//https://judge.yosupo.jp/problem/range_affine_range_sum
#pragma GCC optimize("O2,unroll-loops")
#pragma GCC target("avx2,bmi2,popcnt,lzcnt")
#include <bits/stdc++.h>
using namespace std;

template<class mod>
struct Modular {

    using T = typename mod::type;
    using C = typename mod::cast;

    template<class U>
    static T norm(const U &x){
        T ret;
        if(-mod::mod <= x && x < mod::mod) ret = static_cast<T>(x);
        else ret = static_cast<T>(x%mod::mod);
        if(ret < 0) ret += mod::mod;
        return ret;
    }

    template<class U>
    static T fpow(U a, U b){
        T ret = 1;
        T prod = norm(a);
        while(b){
            if(b%2 == 1) ret = norm((C)ret*prod);
            prod = norm((C)prod*prod);
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
        val = norm(x);
    }

    const T &operator()() const { return val; }

    template<class U>
    explicit operator U() const { return static_cast<U>(val); } 

    Modular operator-(){ return Modular<mod>(-val); }

    Modular &operator+=(const Modular &x){
        val = norm((C)val + x.val);
        return *this;
    }

    Modular &operator-=(const Modular &x){
        val = norm((C)val - x.val);
        return *this;
    }

    Modular &operator*=(const Modular &x){
        val = norm((C)val*x.val);
        return *this;
    }

    Modular &operator/=(const Modular &x){
        val = norm((C)val*fpow(x.val, mod::mod - 2));
        return *this;
    }

    Modular &operator%=(const Modular &x){
        val = val%x.val;
        return *this;
    }

    Modular &operator++(){ return *this += 1; }

    Modular &operator--(){ return *this -= 1; }

    Modular operator++(int){
        Modular<mod> ret = *this;
        ++*this;
        return ret;
    }

    Modular operator--(int){
        Modular<mod> ret = *this;
        --*this;
        return ret;
    }

    friend Modular operator+(const Modular &a, const Modular &b){ return Modular(a.val) += b; }

    friend Modular operator-(const Modular &a, const Modular &b){ return Modular(a.val) -= b; }

    friend Modular operator*(const Modular &a, const Modular &b){ return Modular(a.val) *= b; }

    friend Modular operator/(const Modular &a, const Modular &b){ return Modular(a.val) /= b; }

    friend bool operator<(const Modular &a, const Modular &b){ return a.val < b.val; }

    friend bool operator<=(const Modular &a, const Modular &b){ return a.val <= b.val; }

    friend bool operator>(const Modular &a, const Modular &b){ return a.val > b.val; }

    friend bool operator>=(const Modular &a, const Modular &b){ return a.val >= b.val; }

    friend bool operator==(const Modular &a, const Modular &b){ return a.val == b.val; }

    friend bool operator!=(const Modular &a, const Modular &b){ return a.val != b.val; }

    friend ostream &operator<<(ostream &out, Modular x){ return out << x.val; }

    friend istream &operator>>(istream &in, Modular &x){ 
        in >> x.val;
        x.val = norm(x.val);
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

struct LazySeg {

    using T = mint;
    using L = pair<mint, mint>;

    const L empty = {1, 0};

    static T merge(T a, T b){
        return a + b;
    }

    static void apply(T &a, L &b, L v, int l, int r){
        a = v.first*a + v.second*(r - l + 1);
        b = {v.first*b.first, v.first*b.second + v.second};
    }

    private:
        int n;
        vector<T> seg;
        vector<L> tag;

        void push_down(int x, int l, int r){
            if(tag[x] == empty) return;
            int mid = (l + r)/2;
            apply(seg[x*2 + 1], tag[x*2 + 1], tag[x], l, mid);
            apply(seg[x*2 + 2], tag[x*2 + 2], tag[x], mid + 1, r);
            tag[x] = empty;
        }

        void build(const vector<T> &v, int l, int r, int cur){
            if(l == r){
                seg[cur] = v[l];
                return;
            }
            int mid = (l + r)/2;
            build(v, l, mid, cur*2 + 1);
            build(v, mid + 1, r, cur*2 + 2);
            seg[cur] = merge(seg[cur*2 + 1], seg[cur*2 + 2]);
        }

        void update(int l, int r, L v, int ul, int ur, int cur){
            if(l <= ul && ur <= r){
                apply(seg[cur], tag[cur], v, ul, ur);
                return;
            }
            push_down(cur, ul, ur);
            int mid = (ul + ur)/2;
            if(l <= mid) update(l, r, v, ul, mid, cur*2 + 1);
            if(r > mid) update(l, r, v, mid + 1, ur, cur*2 + 2);
            seg[cur] = merge(seg[cur*2 + 1], seg[cur*2 + 2]);
        }

        T query(int l, int r, int ul, int ur, int cur){
            if(l <= ul && ur <= r) return seg[cur];
            push_down(cur, ul, ur);
            int mid = (ul + ur)/2;
            if(r <= mid) return query(l, r, ul, mid, cur*2 + 1);
            if(l > mid) return query(l, r, mid + 1, ur, cur*2 + 2);
            return merge(query(l, r, ul, mid, cur*2 + 1), query(l, r, mid + 1, ur, cur*2 + 2));
        }

    public:
        LazySeg(){

        }

        LazySeg(const vector<T> &v){
            n = v.size();
            int sz = 1;
            while(sz < n) sz *= 2;
            tag.assign(2*sz, empty);
            seg.resize(2*sz);
            build(v, 0, n - 1, 0);
        } 

        T query(int l, int r){
            return query(l, r, 0, n - 1, 0);
        }

        void update(int l, int r, L v){
            update(l, r, v, 0, n - 1, 0);
        }

        friend ostream& operator << (ostream &out, LazySeg &v){
            out << "[";
            for(int i = 0; i < v.n; i++){
                if(i) cout << ", ";
                out << v.query(i, i);
            }
            cout << "]\n";
            return out;
        }
};

int main(){
    int n, q;
    cin >> n >> q;
    vector<mint> v(n);
    for(int i = 0; i < n; i++) cin >> v[i];
    LazySeg seg(v);
    while(q--){
        int t, l, r;
        cin >> t >> l >> r;
        if(t == 0){
            int b, c;
            cin >> b >> c;
            seg.update(l, r - 1, {b, c});
        } else {
            cout << seg.query(l, r - 1) << "\n";
        }
    }
}