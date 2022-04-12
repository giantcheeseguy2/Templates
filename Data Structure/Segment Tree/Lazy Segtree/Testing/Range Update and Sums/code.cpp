//https://cses.fi/problemset/task/1735
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi2,popcnt,lzcnt")
#include <bits/stdc++.h>
using namespace std;

struct LazySeg {

    using T = long long;
    using L = pair<long long, long long>;

    static constexpr L empty = {0, 0};

    static T merge(T a, T b){
        return a + b;
    }
 
    void apply(T &a, L& b, L v, int l, int r){
        if(v.first){
            a = v.first*(r - l + 1);
            b.first = v.first;
            b.second = 0;
        }
        if(v.second){
            a += v.second*(r - l + 1);
            if(b.first) b.first += v.second;
            else b.second += v.second;
        }
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
    vector<long long> v(n);
    for(int i = 0; i < n; i++) cin >> v[i];
    LazySeg seg(v);
    while(q--){
        int t, l, r;
        cin >> t >> l >> r;
        l--, r--;
        if(t == 1){
            int x;
            cin >> x;
            seg.update(l, r, {0, x});
        } else if(t == 2){
            int x; 
            cin >> x;
            seg.update(l, r, {x, 0});
        } else {
            cout << seg.query(l, r) << "\n";
        }
    }
}