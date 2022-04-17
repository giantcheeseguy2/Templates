namespace FFT {

    using D = double;
    using cint = complex<D>;
    const D PI = acos(-1);

    const int MX = 1 << 18;

    cint dp[MX + 1];
    int rev[MX + 1];

    static void build(){
        int ind = 0;
        dp[0] = 1;
        for(int i = 1; i <= MX; i++){
            double a, b;
            sincos(PI*i/MX, &a, &b);
            dp[i] = cint(b*b - a*a, 2*a*b);
        }
    }

    static void fft(vector<cint> &v, int sign){
        int n = v.size(), lg = 31 - __builtin_clz(n);
        //swaps each position with its bitwise inverse. 
        //ensures correct ordering after some of the even and odd indices are swapped
        rev[0] = 0;
        for(int i = 1; i < n; i++){
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (lg - 1));
            if(i < rev[i]) swap(v[i], v[rev[i]]);
        }
        for(int i = 2; i <= n; i *= 2){
            //compute each segment of length i
            for(int j = 0; j < n; j += i){
                for(int k = 0; k < i/2; k++){
                    //formula F(x) = E(x^2) + x*O(x^2)
                    //dp[MX/i*k] is the ith root of unity ^ k (e^2*pi*sqrt(-1)*k*sign/i)
                    cint x = dp[MX/i*k]*v[j + i/2 + k];
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
        assert(n <= MX);
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
        //extracts the answer from the final polynomial. The values must be rounded due to double precision
        vector<T> ret(a.size() + b.size() - 1);
        for(int i = 0; i < a.size() + b.size() - 1; i++){
            ret[i] = (T)(aa[i].real() + 0.5);
        }
        return ret;
    }
}