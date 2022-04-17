//https://judge.yosupo.jp/problem/convolution_mod
#include <bits/stdc++.h>
using namespace std;

template<class mod>
struct Modular {

    using T = typename mod::type;
    using C = typename mod::cast;

    template<class U>
    static T norm(const U &x){
        T ret;
        if(-mod::mod <= x && x <= mod::mod) ret = (T)x;
        else ret = (T)(x%mod::mod);
        if(ret < 0) ret += mod::mod;
        return ret;
    }

    template<class U>
    static T fpow(T a, U b){
        T ret = 1;
        while(b){
            if(b%2 == 1) ret = norm((C)ret*a);
            a = norm((C)a*a);
            b /= 2;
        }
        return ret;
    }

    T val;

    Modular(){
        val = 0;
    }

    template<class U>
    Modular(const U &x){
        val = norm(x);
    }

    const T &operator()() const { return val; }

    template<class U>
    explicit operator U() const { return static_cast<U>(val); } 

    Modular operator-(){ return Modular<mod>(-val); }

    Modular &operator+=(const Modular &x){
        val = norm((C)val + x.val);
        return *this;
    }

    Modular &operator-=(const Modular &x){
        val = norm((C)val - x.val + mod::mod);
        return *this;
    }

    Modular &operator*=(const Modular &x){
        val = norm((C)val*x.val);
        return *this;
    }

    Modular &operator/=(const Modular &x){
        val = norm((C)val*fpow(x.val, mod::mod - 2));
        return *this;
    }

    Modular &operator%=(const Modular &x){
        val = val%x.val;
        return *this;
    }

    template<class U>
    Modular &operator^=(const U &x){
        assert(("be careful when raising to a modded power", typeid(x) != typeid(*this)));
        val = fpow(val, x);
        return *this;
    }

    Modular &operator++(){ return *this += 1; }

    Modular &operator--(){ return *this -= 1; }

    Modular operator++(int){
        Modular<mod> ret = *this;
        ++*this;
        return ret;
    }

    Modular operator--(int){
        Modular<mod> ret = *this;
        --*this;
        return ret;
    }

    friend Modular operator+(const Modular &a, const Modular &b){ return Modular(a.val) += b; }

    friend Modular operator-(const Modular &a, const Modular &b){ return Modular(a.val) -= b; }

    friend Modular operator*(const Modular &a, const Modular &b){ return Modular(a.val) *= b; }

    friend Modular operator/(const Modular &a, const Modular &b){ return Modular(a.val) /= b; }

    friend Modular operator%(const Modular &a, const Modular &b){ return Modular(a.val) %= b; }

    template<class U>
    friend Modular operator^(const Modular &a, const U &b){ return Modular(a.val) ^= b; }

    friend bool operator<(const Modular &a, const Modular &b){ return a.val < b.val; }

    friend bool operator<=(const Modular &a, const Modular &b){ return a.val <= b.val; }

    friend bool operator>(const Modular &a, const Modular &b){ return a.val > b.val; }

    friend bool operator>=(const Modular &a, const Modular &b){ return a.val >= b.val; }

    friend bool operator==(const Modular &a, const Modular &b){ return a.val == b.val; }

    friend bool operator!=(const Modular &a, const Modular &b){ return a.val != b.val; }

    friend ostream &operator<<(ostream &out, Modular x){ return out << x.val; }

    friend istream &operator>>(istream &in, Modular &x){ 
        in >> x.val;
        x.val = norm(x.val);
        return in;
    }

    string to_string(const Modular&x) { return to_string(x.val); }
};

struct Mod { 
    using type = int;
    using cast = long long;
    const static type mod = 998244353;
};

using mint = Modular<Mod>;

//requires mint
namespace NTT {
    /**
     *  Useful mods MOD should be >= N*MAX^2:
     *      998244353 (9e8)
     *      2748779069441 (2e12)
     *      4179340454199820289 (4e18)
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
    static vector<T> mult(const vector<T> &a, const vector<T> &b, T mod = 0){
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
        vector<T> ret(n);
        for(int i = 0; i < n; i++){
            ret[i] = aa[i]();
        }
        return ret;
    }
}

int main(){
    int n, m;
    cin >> n >> m;
    vector<int> v1(n), v2(m);
    for(int i = 0; i < n; i++) cin >> v1[i];
    for(int i = 0; i < m; i++) cin >> v2[i];
    vector<int> ans = NTT::mult(v1, v2);
    for(int i = 0; i < n + m - 1; i++) cout << ans[i] << " ";
    cout << "\n";
}