struct Treap {

    using T = long long;
    using L = long long;

    static L empty(){ return 0; }

    static T comb(T a, T b){
        return a + b;
    }

    static void apply_val(T &x, L v, int sz){
        x += v;
    }

    static void apply_tag(L &x, L v, int sz){
        x += v;
    }

    struct node {
        array<int, 2> child;
        int sz;
        bool rev;
        T val, sum;
        L tag;

        node(T v){
            ind = i;
            val = sum = v;
            sz = 1;
            child = {-1, -1};
            tag = empty();
            rev = 0;
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
        treap.push_back(node(v));
        return n++;
    }

    //splits rt's tree into [0, k) [k, sz)
    pair<int, int> split(int rt, int k){
        if(rt == -1) return pair<int, int>{-1, -1};
        push(rt);
        int nxt = (treap[rt].child[0] == -1 ? 0 : treap[treap[rt].child[0]].sz);
        pair<int, int> ret;
        if(nxt < k){
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
        if(rand()%(treap[a].sz + treap[b].sz) < treap[b].sz){}
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