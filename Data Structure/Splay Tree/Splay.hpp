struct Splay {

    using T = long long;
    using L = long long;

    static L empty(){ return 0; }

    static T comb(T a, T b){
        return a + b;
    }

    static void apply_val(T &x, L v, int sz){
        x += v*sz;
    }

    static void apply_tag(L &x, L v, int sz){
        x += v;
    }

    struct node {
        int par, sz, ind, rev;
        array<int, 2> child;
        T val, sum;
        L tag;

        node(int p, int i, T v){
            par = p;
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
    vector<node> splay;

    Splay(){
        n = 0;
    }

    void pull(int ind){
        if(ind == -1) return;
        splay[ind].sz = 1;
        splay[ind].sum = splay[ind].val;
        for(int i = 0; i <= 1; i++){
            int nxt = splay[ind].child[i];
            if(nxt != -1){
                splay[ind].sum = comb(splay[ind].sum, splay[nxt].sum);
                splay[ind].sz += splay[nxt].sz;
            }
        }
    }

    void push(int ind){
        if(ind == -1) return;
        if(splay[ind].rev == 0 && splay[ind].tag == empty()) return;
        if(splay[ind].rev) swap(splay[ind].child[0], splay[ind].child[1]);
        for(int i = 0; i <= 1; i++){
            int nxt = splay[ind].child[i];
            if(nxt != -1){
                int sz = splay[nxt].sz;
                apply_val(splay[nxt].val, splay[ind].tag, 1);
                apply_val(splay[nxt].sum, splay[ind].tag, sz);
                apply_tag(splay[nxt].tag, splay[ind].tag, sz);
                splay[nxt].rev ^= splay[ind].rev;
            }
        }
        splay[ind].rev = 0;
        splay[ind].tag = empty();
    }

    void reserve(int _n){
        splay.reserve(_n);
    }

    int newnode(T v){
        splay.push_back(node(-1, n, v));
        return n++;
    }

    //attaches a, to b's side
    void attach(int a, int b, int side){
        splay[b].child[side] = a;
        if(a != -1) splay[a].par = b;
    }

    int side(int ind){
        assert(splay[ind].par != -1);
        return splay[splay[ind].par].child[1] == ind;
    }

    //rotates ind with its parent
    void balance(int ind){
        if(splay[ind].par == -1) return;
        int s = side(ind), p = splay[ind].par;
        push(p);
        push(ind);
        if(splay[p].par != -1) attach(ind, splay[p].par, side(p));
        else splay[ind].par = -1;
        attach(splay[ind].child[!s], p, s);
        splay[ind].child[!s] = -1;
        attach(p, ind, !s);
        pull(p);
        pull(ind);
    }

    //finds the node at index x in rt's tree
    int getnode(int rt, int x){
        assert(0 <= x && x < splay[rt].sz);
        while(true){
            push(rt);
            int nxt = (splay[rt].l() == -1 ? 0 : splay[splay[rt].l()].sz);
            if(x == nxt) return rt;
            if(x <= nxt) rt = splay[rt].l();
            else x -= nxt + 1, rt = splay[rt].r();
        }
    }

    //moves ind to root
    int reroot(int ind, int tar = -1){
        while(splay[ind].par != tar){
            if(splay[splay[ind].par].par != tar){
                balance((side(ind) == side(splay[ind].par) ? splay[ind].par : ind));
            }
            balance(ind);
        }
        return ind;
    }

    //merges two trees a and b
    int merge(int a, int b){
        if(a == -1) return b;
        if(b == -1) return a;
        int last = reroot(getnode(a, splay[a].sz - 1));
        push(last);
        splay[b].par = last;
        splay[last].child[1] = b;
        pull(last);
        return last;
    }

    //splits rt into [0, k], (k, sz)
    pair<int, int> split(int rt, int k){
        if(k == splay[rt].sz - 1) return pair<int, int>{rt, -1};
        if(k == -1) return pair<int, int>{-1, rt};   
        int a = reroot(getnode(rt, k)), b = splay[a].child[1]; 
        push(a);
        splay[a].child[1] = -1;
        splay[b].par = -1;
        pull(a);
        return pair<int, int>{a, b};
    }

    //inserts v at index x in rt's tree
    void insert(int &rt, int x, T v){
        if(rt == -1){
            rt = newnode(v);
            return;
        }
        assert(0 <= x && x <= splay[rt].sz);
        pair<int, int> nw = split(rt, x - 1);
        int cur = newnode(v);
        rt = merge(nw.first, merge(cur, nw.second));
        rt = reroot(cur);
    }

    //removes the value at index in rt's tree
    void erase(int &rt, int x){
        assert(0 <= x && x < splay[rt].sz);
        rt = reroot(getnode(rt, x));
        if(splay[rt].child[0] != -1) splay[splay[rt].child[0]].par = -1;
        if(splay[rt].child[1] != -1) splay[splay[rt].child[1]].par = -1;
        rt = merge(splay[rt].child[0], splay[rt].child[1]);
    }

    //rotates l...r k, l_i = l_(i - k)
    void rotate(int &rt, int l, int r, int k){
        if(l == r) return;
        k %= r - l + 1;
        if(k == 0) return;
        pair<int, int> a = split(rt, l - 1);
        pair<int, int> b = split(a.second, r - l);
        pair<int, int> cur = split(b.first, k - 1);
        rt = merge(a.first, merge(cur.second, merge(cur.first, b.second)));
    }

    void update(int &rt, int l, int r, L v){
        assert(0 <= l && r < splay[rt].sz);
        if(0 == l && r == splay[rt].sz - 1){
            apply_val(splay[rt].val, v, 1);
            apply_val(splay[rt].sum, v, splay[rt].sz);
            apply_tag(splay[rt].tag, v, splay[rt].sz);
        } else if(l == 0){
            rt = reroot(getnode(rt, r + 1));
            int lc = splay[rt].l();
            apply_val(splay[lc].val, v, 1);
            apply_val(splay[lc].sum, v, splay[lc].sz);
            apply_tag(splay[lc].tag, v, splay[lc].sz);
            rt = reroot(lc);
        } else if(r == splay[rt].sz - 1){
            rt = reroot(getnode(rt, l - 1));
            int rc = splay[rt].r();
            apply_val(splay[rc].val, v, 1);
            apply_val(splay[rc].sum, v, splay[rc].sz);
            apply_tag(splay[rc].tag, v, splay[rc].sz);
            rt = reroot(rc);
        } else {
            rt = reroot(getnode(rt, l - 1));
            reroot(getnode(rt, r + 1), rt);
            int c = splay[splay[rt].r()].l();
            apply_val(splay[c].val, v, 1);
            apply_val(splay[c].sum, v, splay[c].sz);
            apply_tag(splay[c].tag, v, splay[c].sz);
            rt = reroot(c);
        }
    }

    void reverse(int &rt, int l, int r){
        assert(0 <= l && r < splay[rt].sz);
        if(0 == l && r == splay[rt].sz - 1){
            splay[rt].rev ^= 1;
        } else if(l == 0){
            rt = reroot(getnode(rt, r + 1));
            int lc = splay[rt].l();
            splay[lc].rev ^= 1;
            rt = reroot(lc);
        } else if(r == splay[rt].sz - 1){
            rt = reroot(getnode(rt, l - 1));
            int rc = splay[rt].r();
            splay[rc].rev ^= 1;
            rt = reroot(rc);
        } else {
            rt = reroot(getnode(rt, l - 1));
            reroot(getnode(rt, r + 1), rt);
            int c = splay[splay[rt].r()].l();
            splay[c].rev ^= 1;
            rt = reroot(c);
        }
    }

    T query(int &rt, int l, int r){
        assert(0 <= l && r < splay[rt].sz);
        if(l == r){
            rt = reroot(getnode(rt, l));
            return splay[rt].val;
        }
        if(0 == l && r == splay[rt].sz - 1){
            return splay[rt].sum;
        } else if(l == 0){
            rt = reroot(getnode(rt, r + 1));
            return splay[splay[rt].l()].sum;
        } else if(r == splay[rt].sz - 1){
            rt = reroot(getnode(rt, l - 1));
            return splay[splay[rt].r()].sum;
        } else {
            rt = reroot(getnode(rt, l - 1));
            reroot(getnode(rt, r + 1), rt);
            return splay[splay[splay[rt].r()].l()].sum;
        }
    }
};