//https://judge.yosupo.jp/problem/range_affine_range_sum
#include <bits/stdc++.h>
using namespace std;

template <class Node>
struct LazySeg : public Node {
    using T = typename Node::T;
    using L = typename Node::L;
    using Node::merge;
    using Node::apply;
    using Node::empty;

    int n;
    vector<T> seg;
    vector<L> tag;

    void build(vector<T> &v, int l, int r, int cur){
        if(l == r){
            tag[cur] = empty;
            seg[cur] = v[l];
            return;
        }
        int mid = (l + r)/2;
        build(v, l, mid, cur*2 + 1);
        build(v, mid + 1, r, cur*2 + 2);
        seg[cur] = merge(seg[cur*2 + 1], seg[cur*2 + 2]);
        tag[cur] = empty;
    }

    void init(vector<T> &v, int _n){
        n = _n;
        seg.resize(4*n);
        tag.resize(4*n);
        build(v, 0, n - 1, 0);
    }

    void init(T segVal, int _n){
        vector<T> v(_n, segVal);
        init(v, _n);
    }

    void push_down(int cur, int l, int r){
        if(tag[cur] == empty) return;
        int mid = (l + r)/2;
        apply(seg[cur*2 + 1], tag[cur*2 + 1], tag[cur], l, mid);
        apply(seg[cur*2 + 2], tag[cur*2 + 2], tag[cur], mid + 1, r);
        tag[cur] = empty;
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
        int mid = (ul + ur)/2;
        push_down(cur, ul, ur);
        if(r <= mid) return query(l, r, ul, mid, cur*2 + 1);
        if(l > mid) return query(l, r, mid + 1, ur, cur*2 + 2);
        return merge(query(l, r, ul, mid, cur*2 + 1), query(l, r, mid + 1, ur, cur*2 + 2));
    }

    void update(int l, int r, L v){
        update(l, r, v, 0, n - 1, 0);
    }

    T query(int l, int r){
        return query(l, r, 0, n - 1, 0);
    }
};

struct RangeNode {

    const long long MOD = 998244353;

    using T = long long;
    using L = pair<long long, long long>;

    const L empty = {1, 0};

    void apply(T &a, L &b, L c, int l, int r){
        a = (c.first*a%MOD + c.second*(r - l + 1)%MOD)%MOD;
        b = {c.first*b.first%MOD, c.first*b.second%MOD + c.second%MOD};
    }

    T merge(T a, T b){
        return (a + b)%MOD;
    }
};

int main(){
    int n, q;
    cin >> n >> q;
    vector<long long> v(n);
    for(int i = 0; i < n; i++) cin >> v[i];
    LazySeg<RangeNode> seg;
    seg.init(v, n);
    while(q--){
        int t, l, r;
        cin >> t >> l >> r;
        r--;
        if(t == 0){
            int b, c;
            cin >> b >> c;
            seg.update(l, r, {b, c});
        } else {
            cout << seg.query(l, r) << endl;
        }
    }
}