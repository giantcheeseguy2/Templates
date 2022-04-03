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

    LazySeg(){

    } 

    LazySeg(const vector<T> &v, int _n){
        n = _n;
        seg.resize(4*n);
        tag.resize(4*n);
        build(v, 0, n - 1, 0);
    }

    LazySeg(T segVal, int _n){
        vector<T> v(_n, segVal);
        LazySeg(v, _n);
    }

    void build(const vector<T> &v, int l, int r, int cur){
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

    void push_down(int cur, int l, int r){
        if(tag[cur] == empty) return;
        int mid = (l + r)/2;
        apply(seg[cur*2 + 1], tag[cur*2 + 1], tag[cur], l, mid);
        apply(seg[cur*2 + 2], tag[cur*2 + 2], tag[cur], mid + 1, r);
        tag[cur] = empty;
    }

    void update(int l, int r, const L &v, int ul, int ur, int cur){
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

    void update(int l, int r, const L &v){
        update(l, r, v, 0, n - 1, 0);
    }

    T query(int l, int r){
        return query(l, r, 0, n - 1, 0);
    }

    friend ostream& operator << (ostream &out, LazySeg<Node> &v){
        out << "[";
        for(int i = 0; i < v.n; i++){
            if(i) cout << ", ";
            out << v.query(i, i);
        }
        cout << "]\n";
        return out;
    }
};

struct Node { 

    const int mod = 1e9 + 7;

    long long a, b, c;

    Node(){

    }

    Node(long long _a, long long _b, long long _c){
        a = _a;
        b = _b;
        c = _c;
    }

    Node &operator=(const Node &v){
        a = v.a;
        b = v.b;
        c = v.c;
        return *this;
    }

    Node &operator*=(const Node &v){
        Node ret = Node((((v.a*a)%mod) + ((v.b*b)%mod))%mod, (((v.b*a)%mod) + ((v.c*b)%mod))%mod, (((v.b*b)%mod) + ((v.c*c)%mod))%mod);
        a = ret.a;
        b = ret.b;
        c = ret.c;
        return *this;
    }

    Node operator+(const Node &v){
        return Node((a + v.a)%mod, (b + v.b)%mod, (c + v.c)%mod);
    }

    Node operator^(int x){
        Node ret(1, 0, 1);
        Node prod(1, 1, 0);
        while(x){
            if(x%2 == 1) ret *= prod;
            prod *= prod;
            x /= 2;
        }
        return ret;
    }

    friend bool operator==(const Node &a, const Node &b){
        return a.a == b.a && a.b == b.b && a.c == b.c;
    }
};

struct LazyNode {

    using T =  Node;
    using L =  Node;

    const L empty =  Node(1, 0, 1);

    void apply(T &a, L &b, L c, int l, int r){
        a *= c;
        b *= c;
    }

    T merge(T a, T b){
        return a + b;
    }
};

int main(){
    int n, q;
    cin >> n >> q;
    vector<Node> v(n);
    Node fib(1, 1, 0);
    for(int i = 0; i < n; i++){
        int x;
        cin >> x;
        v[i] = fib ^ x;
    }
    LazySeg<LazyNode> seg(v, n);
    while(q--){
        int t, a, b;
        cin >> t >> a >> b;
        a--, b--;
        if(t == 1){
            int x;
            cin >> x;
            seg.update(a, b, fib ^ x);
        } else {
            cout << seg.query(a, b).b << "\n";
        }
    }
}