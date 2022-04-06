#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,bmi2,popcnt,lzcnt")
#include <bits/stdc++.h>
using namespace std;

template <class Node>
struct LazySeg : public Node {
 
    using T = typename Node::T;
    using L = typename Node::L;
    using Node::merge;
    using Node::apply_seg;
    using Node::apply_tag;
    using Node::segEmpty;
    using Node::tagEmpty;
 
    private:
        int n, size, log;
        vector<T> seg;
        vector<L> tag;
        vector<pair<int, int>> range;
 
        void apply(int x, L v, int l, int r){
            seg[x] = apply_seg(seg[x], v, l, r);
            if(x < size) tag[x] = apply_tag(tag[x], v, l, r);
        }
 
        void push_down(int x){
            if(tag[x] == tagEmpty) return;
            apply(x << 1, tag[x], range[x << 1].first, range[x << 1].second);
            apply(x << 1 | 1, tag[x], range[x << 1 | 1].first, range[x << 1 | 1].second);
            tag[x] = tagEmpty;
        }
 
        void comb(int x){
            range[x].first = range[x << 1].first;
            range[x].second = range[x << 1 | 1].second;
            seg[x] = merge(seg[x << 1], seg[x << 1 | 1]);
        }
 
        void push_down_point(int x){
            for(int i = log; i >= 1 + __builtin_ctz(x); i--) push_down(x >> i);
        }
 
        void update_point(int x){
            for(int i = 1 + __builtin_ctz(x); i <= log; i++) comb(x >> i);
        }
 
    public:
        LazySeg(){
 
        }
 
        LazySeg(const vector<T> &v){
            n = v.size();
            log = 0;
            while((1 << log) < n) log++;
            size = (1 << log);
            tag.assign(size, tagEmpty);
            seg.assign(2*size, segEmpty);
            range.resize(2*size);
            for(int i = 0; i < n; i++){
                seg[size + i] = v[i];
            }
            for(int i = 0; i < size; i++) range[i + size] ={i, i};
            for(int i = size - 1; i >= 1; i--) comb(i);
        } 
 
        void update(int l, int r, L v){
            l += size, r += size + 1;
            push_down_point(l);
            push_down_point(r);
            for(int ll = l, rr = r; ll < rr; ll >>= 1, rr >>= 1){
                if(ll & 1) apply(ll, v, range[ll].first, range[ll].second), ll++;
                if(rr & 1) --rr, apply(rr, v, range[rr].first, range[rr].second);
            }
            update_point(l);
            update_point(r);
        }
 
        int query(int l, int r){
            int ret = 0;
            l += size, r += size + 1;
            push_down_point(l);
            push_down_point(r);
            for(; l < r; l >>= 1, r >>= 1){
                if(l & 1) (ret += seg[l++].b) %= 1000000007;
                if(r & 1) (ret += seg[--r].b) %= 1000000007;
            }
            return ret;
        }
 
        friend ostream& operator << (ostream &out, LazySeg<Node> &v){
            out << "[";
            for(int i = 0; i < v.n; i++){
                if(i) cout << ", ";
                out << v.query(i, i);
            }
            out << "]\n";
            return out;
        }
};

const int mod = 1e9 + 7;
 
struct Node {  
 
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
 
    using T = Node;
    using L = Node;
 
    const L tagEmpty = Node(1, 0, 1);
    const T segEmpty = Node(0, 0, 0);
 
    T apply_seg(T a, L b, int l, int r){
        a *= b;
        return a;
    }
 
    L apply_tag(L a, L b, int l, int r){
        a *= b;
        return a;
    }
 
    T merge(T a, T b){
        return a + b;
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
    vector<Node> v(n);
    Node fib(1, 1, 0);
    for(int i = 0; i < n; i++){
        int x;
        scan(x);
        v[i] = fib ^ x;
    }
    LazySeg<LazyNode> seg(v);
    while(q--){
        int t, a, b;
        scan(t), scan(a), scan(b);
        a--, b--;
        if(t == 1){
            int x;
            scan(x);
            seg.update(a, b, fib ^ x);
        } else {
            cout << seg.query(a, b) << "\n";
        }
    }
}