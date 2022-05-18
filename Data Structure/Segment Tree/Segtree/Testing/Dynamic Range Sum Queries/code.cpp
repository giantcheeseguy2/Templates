//https://cses.fi/problemset/task/1648
#include <bits/stdc++.h>
using namespace std;

struct Segtree {

    using T = long long;
    using L = long long;

    static T merge(T a, T b){
        return a + b;
    }

    static void apply(T &a, L v, int x){
        a = v;
    }

    int n;
    vector<T> seg;

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

    void update(int x, L v, int l, int r, int cur){
        if(l == x && x == r){
            apply(seg[cur], v, x);
            return;
        }
        int mid = (l + r)/2;
        if(x <= mid) update(x, v, l, mid, cur*2 + 1);
        else update(x, v, mid + 1, r, cur*2 + 2);
        seg[cur] = merge(seg[cur*2 + 1], seg[cur*2 + 2]);
    }

    T query(int l, int r, int ul, int ur, int cur){
        if(l <= ul && ur <= r) return seg[cur];
        int mid = (ul + ur)/2;
        if(r <= mid) return query(l, r, ul, mid, cur*2 + 1);
        if(l > mid) return query(l, r, mid + 1, ur, cur*2 + 2);
        return merge(query(l, r, ul, mid, cur*2 + 1), query(l, r, mid + 1, ur, cur*2 + 2));
    }

    Segtree(){

    }

    Segtree(const vector<T> &v){
        n = v.size();
        int sz = 1;
        while(sz < n) sz *= 2;
        seg.resize(2*sz);
        build(v, 0, n - 1, 0);
    } 

    Segtree(int _n, T v){
        n = _n;
        int sz = 1;
        while(sz < n) sz *= 2;
        seg.resize(2*sz);
        vector<T> tmp(n, v);
        build(tmp, 0, n - 1, 0);
    }

    T query(int l, int r){
        return query(l, r, 0, n - 1, 0);
    }

    void update(int x, L v){
        update(x, v, 0, n - 1, 0);
    }

};

int main(){
    int n, q;
    cin >> n >> q;
    vector<long long> v(n);
    for(int i = 0; i < n; i++) cin >> v[i];
    Segtree seg(v);
    while(q--){
        int t, a, b;
        cin >> t >> a >> b;
        if(t == 1) seg.update(a - 1, b);
        else cout << seg.query(a - 1, b - 1) << endl;
    }
}