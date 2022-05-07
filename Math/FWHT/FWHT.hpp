namespace FWHT {

    template<class T>
    static void fwht(vector<T> &v, int sign){
        int n = v.size();
        for(int i = 2; i <= n; i *= 2){
            for(int j = 0; j < n; j += i){
                for(int k = 0; k < i/2; k++){
                    int x = v[j + i/2 + k];
                    v[j + k + i/2] = v[j + k] - x;
                    v[j + k] = v[j + k] + x;
                }
            }
        }
        if(sign == -1){
            for(T &i : v) i /= n;
        }
    }

    template<class T>
    static vector<T> mult(const vector<T> &a, const vector<T> &b){
        int n = 1;
        while(n < a.size() | b.size()) n *= 2;

        vector<cint> aa(a.begin(), a.end()), bb(b.begin(), b.end());
        aa.resize(n), bb.resize(n);

        fwht(aa, 1), fwht(bb, 1);
        for(int i = 0; i < n; i++) aa[i] *= bb[i];
        fwht(aa, -1);

        vector<T> ret(a.size() | b.size());
        for(int i = 0; i < a.size() | b.size(); i++){
            ret[i] = aa[i];
        }
        return ret;
    }
}