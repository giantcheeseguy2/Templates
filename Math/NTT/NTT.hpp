//requires mint
namespace NTT {
    /**
     *  Useful mods MOD should be >= N*MAX^2:
     *      998244353 (9e8)
     *      2748779069441 (2e12)
     *      4179340454199820289 (4e18)
     *  More mods: https://pastebin.com/T4YynxWM
     */

    //Returns the bitwise reverse of x using only the first mx bits
    static int rev(int x, int mx){
        int ret = 0;
        for(int i = 0; i < mx; i++){
            if(x & (1 << i)) ret |= 1 << (mx - 1 - i);
        }
        return ret;
    }

    static void ntt(vector<mint> &v, int sign, mint proot){
        int n = v.size(), lg = 0;
        while((1 << lg) < n) lg++;
        //swaps each position with its bitwise inverse. 
        //ensures correct ordering after some of the even and odd indices are swapped
        for(int i = 0; i < n; i++){
            if(i < rev(i, lg)) swap(v[i], v[rev(i, lg)]);
        }
        for(int i = 2; i <= n; i *= 2){
            //compute each segment of length i
            //finds primitive root of the mod
            mint mult = proot ^ ((Mod::mod - 1)/i);
            for(int j = 0; j < n; j += i){
                //nth root of unity computed using the primitive root
                mint root = 1;
                for(int k = 0; k < i/2; k++){
                    mint a = v[j + k], b = root*v[j + i/2 + k];
                    //computes the left half of the segment
                    v[j + k] = a + b;
                    //computes the right half of the segment
                    v[j + k + i/2] = a - b;
                    //increases the current root of unity
                    root *= mult;
                }
            }
        }
        //If we are computing invDFT, we must divide every value by n
        if(sign == -1){
            for(mint &i : v) i /= n;
            //since we didn't make the power negative for the NTT, we should reverse the answer
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
        //computes a value for mod that can be used as a primitive root
        mint proot = 2;
        while(__gcd(proot(), Mod::mod - 1) != 1) proot++;
        //constructs two copies of polynomials a and b, but using complex numbers instead
        vector<mint> aa(a.begin(), a.end()), bb(b.begin(), b.end());
        //resize the polynomials to have a size with a power of 2
        aa.resize(n), bb.resize(n);
        //computes DFT(a) and DFT(b)
        ntt(aa, 1, proot), ntt(bb, 1, proot);
        //a = DFT(a) * DFT(b)
        for(int i = 0; i < n; i++) aa[i] *= bb[i];
        //returns a to a polynomial, a = invDFT(a)
        ntt(aa, -1, proot);
        //extracts the answer from the final polynomial. The values must be rounded due to double precision
        vector<T> ret(a.size() + b.size() - 1);
        for(int i = 0; i < a.size() + b.size() - 1; i++){
            ret[i] = aa[i]();
        }
        return ret;
    }
}