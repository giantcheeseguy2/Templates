struct LazyNode {

    using T = int;
    using L = int;

    const L tagEmpty = 0;
    const T segEmpty = 0;

    T apply_seg(T a, L b, int l, int r){
        return a + b;
    }

    L apply_tag(L a, L b, int l, int r){
        return a + b;
    }

    T merge(T a, T b){
        return a + b;
    }
};