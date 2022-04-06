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

        T query(int l, int r){
            T ret = segEmpty;
            l += size, r += size + 1;
            push_down_point(l);
            push_down_point(r);
            for(; l < r; l >>= 1, r >>= 1){
                if(l & 1) ret = merge(ret, seg[l++]);
                if(r & 1) ret = merge(ret, seg[--r]);
            }
            return ret;
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