namespace FFT {

    using D = double;
    using cint = complex<D>;
    const D PI = acos(-1);

    static void fft(vector<cint> &v, int sign){
        int n = v.size(), lg = 31 - __builtin_clz(n);
        //swaps each position with its bitwise inverse. 
        //ensures correct ordering after some of the even and odd indices are swapped
        vector<int> rev(n);
        rev[0] = 0;
        for(int i = 1; i < n; i++){
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (lg - 1));
            if(i < rev[i]) swap(v[i], v[rev[i]]);
        }
        //precomputes the nth roots of unity
        vector<cint> root(n);
        root[1] = cint(1, 0);
        for (int i = 1; i < lg; i++) {
            D ang = PI/(1 << i);
            cint mult = {cos(ang), sin(ang)};
            for (int j = (1 << (i - 1)); j < (1 << i); j++) {
                root[j*2] = root[j];
                root[j*2 + 1] = root[j]*mult;
            }
        }
        for(int i = 2; i <= n; i *= 2){
            //compute each segment of length i
            for(int j = 0; j < n; j += i){
                for(int k = 0; k < i/2; k++){
                    //formula F(x) = E(x^2) + x*O(x^2)
                    //root[k + i/2] is the ith root of unity ^ k (e^(2*pi*sqrt(-1)*k*sign/i))
                    cint x = root[k + i/2]*v[j + i/2 + k];
                    //computes the right half of the segment
                    v[j + k + i/2] = v[j + k] - x;
                    //computes the left half of the segment
                    v[j + k] = v[j + k] + x;
                }
            }
        }
        //If we are computing invDFT, we must divide every value by n
        if(sign == -1){
            for(cint &i : v) i /= n;
            //We also have to reverse the coefficients except for the constant for inverse
            reverse(v.begin() + 1, v.end());
        }
    }

    //DFT(A * B) = DFT(A) * DFT(B)
    //A * B = invDFT(DFT(A) * DFT(B))
    template<class T>
    static vector<T> mult(const vector<T> &a, const vector<T> &b){
        //resizes the answer to a power of 2
        int n = 1;
        while(n < a.size() + b.size()) n *= 2;
        //constructs two copies of polynomials a and b, but using complex numbers instead
        vector<cint> aa(a.begin(), a.end()), bb(b.begin(), b.end());
        //resize the polynomials to have a size with a power of 2
        aa.resize(n), bb.resize(n);
        //computes DFT(a) and DFT(b)
        fft(aa, 1), fft(bb, 1);
        //a = DFT(a) * DFT(b)
        for(int i = 0; i < n; i++) aa[i] *= bb[i];
        //returns a to a polynomial, a = invDFT(a)
        fft(aa, -1);
        //values must be rounded due to double precision
        vector<T> ret(a.size() + b.size() - 1);
        for(int i = 0; i < a.size() + b.size() - 1; i++){
            ret[i] = (T)(aa[i].real() + 0.5);
        }
        return ret;
    }
}