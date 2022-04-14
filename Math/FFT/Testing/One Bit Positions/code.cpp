//https://cses.fi/problemset/task/2112
#include <bits/stdc++.h>
using namespace std;

namespace FFT {

    using cint = complex<double>;
    const double PI = acos(-1);

    //Returns the bitwise reverse of x using only the first mx bits
    static int rev(int x, int mx){
        int ret = 0;
        for(int i = 0; i < mx; i++){
            if(x & (1 << i)) ret |= 1 << (mx - 1 - i);
        }
        return ret;
    }

    static void fft(vector<cint> &v, int sign){
        int n = v.size(), lg = 0;
        while((1 << lg) < n) lg++;
        //swaps each position with its bitwise inverse. 
        //ensures correct ordering after some of the even and odd indices are swapped
        for(int i = 0; i < n; i++){
            if(i < rev(i, lg)) swap(v[i], v[rev(i, lg)]);
        }
        for(int i = 2; i <= n; i *= 2){
            //compute each segment of length i
            cint mult(cos(sign*2*PI/i), sin(sign*2*PI/i));
            for(int j = 0; j < n; j += i){
                //nth root of unity (e^2*pi*i*k*sign/n)
                cint root(1, 0);
                for(int k = 0; k < i/2; k++){
                    cint a = v[j + k], b = root*v[j + i/2 + k];
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
        if(sign == -1) for(cint &i : v) i /= n;
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
        //extracts the answer from the final polynomial. The values must be rounded due to double precision
        vector<T> ret(n);
        for(int i = 0; i < n; i++){
            ret[i] = round(aa[i].real());
        }
        return ret;
    }
}

int main(){
    string s;
    cin >> s;
    int n = s.length();
    vector<int> a(n), b(n);
    for(int i = 0; i < n; i++){
        a[i] = s[i] - '0';
        b[n - i - 1] = s[i] - '0';
    }
    vector<int> ans;
    ans = FFT::mult(a, b);
    for(int i = n; i < 2*n - 1; i++) cout << ans[i] << " ";
    cout << "\n";
}