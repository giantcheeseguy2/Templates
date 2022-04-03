#include <bits/stdc++.h>
using namespace std;

template <class Base>
struct LazySeg : public Base {
    using T = typename Base::T
    using Base::merge;
    using Base::apply;
    using Base::empty;

    int n;
    vector<T> seg;
    vector<T> tag;

    void build(vector<T> &v, int l = 0, int r = n - 1, int cur = 0){
        if(l == r){
            seg[cur] = arr[l];
            return;
        }
        int mid = (l + r)/2;
        build(arr, l, mid, cur*2 + 1);
        build(arr, mid + 1, r, cur*2 + 2);
        seg[cur] = merge(seg[cur*2 + 1], seg[cur*2 + 2]);
    }

    void init(vector<T> &v, int n){
        seg.resize(4*_n);
        tag.assign(4*_n, empty);
        build(v);
    }

    void init(T segVal, int n){
        vector<T> v(n, segVal);
        init(v, n);
    }

    void push_down(int cur, int l, int r){
        if(tag[cur] == empty) return;
        int mid = (l + r)/2;
        apply(seg[cur*2 + 1], tag[cur*2 + 1], tag[cur], l, mid);
        apply(seg[cur*2 + 2], tag[cur*2 + 2], tag[cur], mid + 1, r);
        tag[cur] = empty;
    }

    void update(int l, int r, T v, int ul = 0, int ur = n - 1, int cur = 0){
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

    T query(int l, int r, int ul = 0, int ur = n - 1, int cur = 0){
        if(l <= ul && ur <= r) return seg[cur];
        int mid = (ul + ur)/2;
        push_down(cur, ul, ur);
        if(r <= mid) return query(l, r, ul, mid, cur*2 + 1);
        if(l > mid) return query(l, r, mid + 1, ur, cur*2 + 2);
        return merge(query(l, r, ul, mid, cur*2 + 1), query(l, r, mid + 1, ur, cur*2 + 2));
    }
};