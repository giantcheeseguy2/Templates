//https://judge.yosupo.jp/problem/range_affine_range_sum
#pragma GCC optimize("O2,unroll-loops")
#pragma GCC target("avx2,bmi2,popcnt,lzcnt")
#include <bits/stdc++.h>
using namespace std;

struct LazySeg {

    using T = long long;
    using L = pair<long long, long long>;

    const int MOD = 998244353;

    static constexpr L empty = {1, 0};

    static T merge(T a, T b){
        return (a + b)%998244353;
    }

    static void apply(T &a, L &b, L v, int l, int r){
        a = (v.first*a + v.second*(r - l + 1))%998244353;
        b = {(v.first*b.first)%998244353, (v.first*b.second + v.second)%998244353};
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

const int BUFSIZE = 20 << 20;
char Buf[BUFSIZE + 1], *buf = Buf;

template<class T>
void scan(T &x){
    int neg = 1;
    for(x = 0; *buf < '0' || *buf > '9'; buf++) if(*buf == '-') neg = -1;
    while(*buf >= '0' && *buf <= '9') x = x*10 + (*buf - '0'), buf++;
    x *= neg;
}

void setIO(){
    fread(Buf, 1, BUFSIZE, stdin);
}

int main(){
    setIO();
    int n, q;
    scan(n), scan(q);
    vector<long long> v(n);
    for(int i = 0; i < n; i++) scan(v[i]);
    LazySeg seg(v);
    while(q--){
        int t, l, r;
        scan(t), scan(l), scan(r);
        if(t == 0){
            int b, c;
            scan(b), scan(c);
            seg.update(l, r - 1, {b, c});
        } else {
            cout << seg.query(l, r - 1) << "\n";
        }
    }
}