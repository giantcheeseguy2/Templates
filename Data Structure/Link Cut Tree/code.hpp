struct LinkCut {

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
        int par, ppar, sz, rev;
        array<int, 2> child;
        T val, sum;
        L tag;

        node(){

        }

        node(int p, int pp, T v){
            par = p;
            ppar = pp;
            val = sum = v;
            sz = 1;
            child = {-1, -1};
            tag = empty();
            rev = 0;
        }

        int l(){ return child[0]; }
        int r(){ return child[1]; }
    };

    vector<node> lct;

    LinkCut(){

    }

    void pull(int ind){
        if(ind == -1) return;
        lct[ind].sz = 1;
        lct[ind].sum = lct[ind].val;
        for(int i = 0; i <= 1; i++){
            int nxt = lct[ind].child[i];
            if(nxt != -1){
                lct[ind].sum = comb(lct[ind].sum, lct[nxt].sum);
                lct[ind].sz += lct[nxt].sz;
            }
        }
    }

    void push(int ind){
        if(ind == -1) return;
        if(lct[ind].rev == 0 && lct[ind].tag == empty()) return;
        if(lct[ind].rev) swap(lct[ind].child[0], lct[ind].child[1]);
        for(int i = 0; i <= 1; i++){
            int nxt = lct[ind].child[i];
            if(nxt != -1){
                int sz = lct[nxt].sz;
                apply_val(lct[nxt].val, lct[ind].tag, 1);
                apply_val(lct[nxt].sum, lct[ind].tag, sz);
                apply_tag(lct[nxt].tag, lct[ind].tag, sz);
                lct[nxt].rev ^= lct[ind].rev;
            }
        }
        lct[ind].rev = 0;
        lct[ind].tag = empty();
    }

    void resize(int n){
        lct.resize(n);
    }

    void newnode(int ind, T v){
        if(ind >= (int)lct.size()) lct.resize(ind + 1);
        lct[ind] = node(-1, -1, v);
    }

    //attaches a, to b's side
    void attach(int a, int b, int side){
        lct[b].child[side] = a;
        if(a != -1) lct[a].par = b;
    }

    //detachs ind's preferred child
    void detach(int ind){
        if(lct[ind].r() != -1){
            lct[lct[ind].r()].ppar = ind;
            lct[lct[ind].r()].par = -1;
            lct[ind].child[1] = -1;
        }
    }

    int side(int ind){
        assert(lct[ind].par != -1);
        return lct[lct[ind].par].child[1] == ind;
    }

    //rotates ind with its parent
    void balance(int ind){
        if(lct[ind].par == -1) return;
        int s = side(ind), p = lct[ind].par;
        if(lct[p].par != -1) attach(ind, lct[p].par, side(p));
        else lct[ind].par = -1;
        attach(lct[ind].child[!s], p, s);
        lct[ind].child[!s] = -1;
        attach(p, ind, !s);
        lct[ind].ppar = lct[p].ppar;
        lct[p].ppar = -1;
        pull(p);
        pull(ind);
    }

    //moves ind to root
    int splay(int ind){
        int cur = ind;
        vector<int> v;
        while(cur != -1) v.push_back(cur), cur = lct[cur].par;
        reverse(v.begin(), v.end());
        for(int i : v) push(i);
        while(lct[ind].par != -1){
            if(lct[lct[ind].par].par != -1){
                balance((side(ind) == side(lct[ind].par) ? lct[ind].par : ind));
            }
            balance(ind);
        }
        return ind;
    }

    //puts ind on its tree's preferred path from root
    int access(int ind){
        splay(ind);
        detach(ind);
        pull(ind);
        int cur = ind;
        while(lct[cur].ppar != -1){
            int p = lct[cur].ppar;
            splay(p);
            detach(p);
            attach(cur, p, 1);
            pull(p);
            lct[cur].ppar = -1;
            cur = p;
        }
        return splay(ind);
    }

    //finds the root of ind's tree
    int findroot(int ind){
        access(ind);
        while(lct[ind].l() != -1) push(ind = lct[ind].l());
        return splay(ind);
    }

    //sets ind as root of tree
    void reroot(int ind){
        access(ind); 
        lct[ind].rev ^= 1;
    }

    //makes the parent of a to be b
    void link(int a, int b){
        reroot(a);
        lct[a].ppar = b;
    }

    //cuts a's edge to b
    void cut(int a, int b){
        reroot(a);
        access(b);
        lct[b].child[0] = -1;
        lct[a].par = -1;
        pull(b);
    }

    //query the path from a to b
    T query(int a, int b){
        reroot(a);
        access(b);
        return lct[b].sum;
    }

    //update the path from a to b
    void update(int a, int b, L v){
        reroot(a);
        access(b);
        apply_tag(lct[b].tag, v, lct[b].sz);
        apply_val(lct[b].sum, v, lct[b].sz);
        apply_val(lct[b].val, v, 1);
    }

    //finds lca of a and b
    int lca(int a, int b){
        access(a);
        int ret = -1;
        splay(b);
        detach(b);
        while(lct[b].ppar != -1){
            int p = lct[b].ppar;
            ret = p;
            splay(p);
            detach(p);
            attach(b, p, 1);
            pull(p);
            lct[b].ppar = -1;
            splay(b);
        }
        return (ret == -1 ? b : ret);
    }
};