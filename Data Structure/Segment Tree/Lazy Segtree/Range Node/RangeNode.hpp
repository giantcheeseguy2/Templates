struct RangeNode {

    using T = long long;
    using L = long long;

    const L empty = 0;

    void apply(T &a, L &b, L c, int l, int r){
        a += c;
        b += c;
    }

    T merge(T a, T b){
        return a + b;
    }
};