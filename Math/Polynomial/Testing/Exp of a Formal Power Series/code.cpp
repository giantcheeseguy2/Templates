#include <bits/stdc++.h>
using namespace std;

template<class mod>
struct Modular {

    using T = typename mod::type;
    using C = typename mod::cast;

    template<class U>
    static T fpow(T a, U b){
        T ret = 1;
        while(b){
            if(b%2 == 1) ret = (C)ret*a%mod::mod;
            a = (C)a*a%mod::mod;
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
        val = x%mod::mod;
        if(val < 0) val += mod::mod;
    }

    const T Mod() const { return mod::mod; }

    const T &operator()() const { return val; }

    template<class U>
    explicit operator U() const { return static_cast<U>(val); } 

    Modular operator-(){ return Modular<mod>(-val); }

    Modular &operator+=(const Modular &x){
        val = (val + x.val)%mod::mod;
        return *this;
    }

    Modular &operator-=(const Modular &x){
        val = (val - x.val + mod::mod)%mod::mod;
        return *this;
    }

    Modular &operator*=(const Modular &x){
        val = (C)val*x.val%mod::mod;
        return *this;
    }

    Modular &operator/=(const Modular &x){
        val = (C)val*fpow(x.val, mod::mod - 2)%mod::mod;
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
        Modular ret = *this;
        ++*this;
        return ret;
    }

    Modular operator--(int){
        Modular ret = *this;
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

    bool hasSqrt(){
        return val == 0 || fpow(val, (mod::mod - 1)/2) == 1;
    }

    Modular sqrt(){
        if(val == 0) return 0;
        if(val == 1) return 1;
        assert(hasSqrt());
        Modular r = 1, c = 1, a = 1, b = 1, aa, bb;
        while(((r*r - val) ^ ((mod::mod - 1)/2)) == 1) r++;
        Modular mult = r*r - val;
        T x = (mod::mod + 1)/2;
        while(x){
            if(x%2 == 1){
                aa = c*b*mult + a*r;
                bb = c*a + r*b;
                a = aa, b = bb;
            }
            aa = c*c*mult + r*r;
            bb = 2*c*r;
            r = aa, c = bb;
            x /= 2;
        }
        if(a >= (mod::mod + 1)/2) a *= -1;
        return a;
    }

    friend bool operator<(const Modular &a, const Modular &b){ return a.val < b.val; }

    friend bool operator<=(const Modular &a, const Modular &b){ return a.val <= b.val; }

    friend bool operator>(const Modular &a, const Modular &b){ return a.val > b.val; }

    friend bool operator>=(const Modular &a, const Modular &b){ return a.val >= b.val; }

    friend bool operator==(const Modular &a, const Modular &b){ return a.val == b.val; }

    friend bool operator!=(const Modular &a, const Modular &b){ return a.val != b.val; }

    friend ostream &operator<<(ostream &out, Modular x){ return out << x.val; }

    friend istream &operator>>(istream &in, Modular &x){ 
        in >> x.val;
        x.val = x.val%mod::mod;
        if(x.val < 0) x.val += mod::mod;
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

template<class T>
struct Polynomial {

    using D = double;
    using cint = complex<D>;
    const D PI = acos(-1);
    
    vector<T> poly;

    Polynomial(){
        poly.clear();
    }

    Polynomial(int n){
        poly.resize(n);
    }

    Polynomial(const vector<T> &v){
        poly = v;
    }

    void resize(int n){
        if(poly.size() != n) poly.resize(n);
    }

    const int size() const {
        return poly.size();
    }

    const typename vector<T>::const_iterator begin() const {
        return poly.begin();
    }

    const typename vector<T>::const_iterator end() const {
        return poly.end();
    }

    const vector<T> &operator()() const { return poly; }

    T &operator[](int x){
        assert(x >= 0);
        if(x >= poly.size()) resize(x + 1);
        assert(x < poly.size());
        return poly[x];
    }

    const T &operator[](int x) const {
        assert(x >= 0);
        assert(x < poly.size());
        return poly[x];
    }

    template<class U>
    void invert(vector<U> &v){
        int n = v.size(), lg = 31 - __builtin_clz(n);
        vector<int> rev(n);
        rev[0] = 0;
        for(int i = 1; i < n; i++){
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (lg - 1));
            if(i < rev[i]) swap(v[i], v[rev[i]]);
        }
    }

    void fft(vector<cint> &v, int sign){
        invert(v);
        int n = v.size();
        vector<cint> root(n);
        root[1] = cint(1, 0);
        for (int i = 2; i < n; i *= 2) {
            D ang = PI/i;
            cint mult = {cos(ang), sin(ang)};
            for (int j = i/2; j < i; j++) {
                root[j*2] = root[j];
                root[j*2 + 1] = root[j]*mult;
            }
        }
        for(int i = 2; i <= n; i *= 2){
            for(int j = 0; j < n; j += i){
                for(int k = 0; k < i/2; k++){
                    cint x = root[k + i/2]*v[j + i/2 + k];
                    v[j + k + i/2] = v[j + k] - x;
                    v[j + k] = v[j + k] + x;
                }
            }
        }
        if(sign == -1){
            for(cint &i : v) i /= n;
            reverse(v.begin() + 1, v.end());
        }
    }

    void ntt(vector<T> &v, int sign, T proot){
        invert(v);
        int n = v.size();
        for(int i = 2; i <= n; i *= 2){
            T mult = proot ^ ((Mod::mod - 1)/i);
            for(int j = 0; j < n; j += i){
                T root = 1;
                for(int k = 0; k < i/2; k++){
                    T a = v[j + k], b = root*v[j + i/2 + k];
                    v[j + k] = a + b;
                    v[j + k + i/2] = a - b;
                    root *= mult;
                }
            }
        }
        if(sign == -1){
            T div = (T)1/n;
            for(T &i : v) i *= div;
            reverse(v.begin() + 1, v.end());
        }
    }

    void fftmult(const Polynomial &x){
        int n = 1, _n = poly.size() + x.size() - 1;
        while(n < poly.size() + x.size()) n *= 2;
        vector<cint> a(poly.begin(), poly.end()), b(x.begin(), x.end());
        a.resize(n), b.resize(n);
        fft(a, 1), fft(b, 1);
        for(int i = 0; i < n; i++) a[i] *= b[i];
        fft(a, -1);
        resize(_n);
        for(int i = 0; i < _n; i++) poly[i] = (T)(a[i].real() + 0.5);
    }

    void nttmult(const Polynomial &x){
        int n = 1, _n = poly.size() + x.size() - 1;
        while(n < poly.size() + x.size()) n *= 2;
        T proot = 3;
        while(__gcd(proot(), proot.Mod()) != 1) proot++;
        vector<T> b(x.begin(), x.end());
        poly.resize(n), b.resize(n);
        ntt(poly, 1, proot), ntt(b, 1, proot);
        for(int i = 0; i < n; i++) poly[i] *= b[i];
        ntt(poly, -1, proot);
        resize(_n);
    }

    Polynomial &operator*=(const Polynomial &x){
        if constexpr (is_integral_v<T>) fftmult(x);     
        else nttmult(x);
        return *this;
    }

    friend Polynomial operator*(const Polynomial &a, const Polynomial &b) { return Polynomial(a.poly) *= b; }

    template<class U>
    Polynomial &operator*=(const U &x){
        for(int i = 0; i < size(); i++) poly[i] *= x;
        return *this;
    }

    template<class U>
    friend Polynomial operator*(const Polynomial &a, const U &b) { return Polynomial(a.poly) *= b; }

    Polynomial &operator-=(const Polynomial &x){
        resize(max(x.size(), size()));
        for(int i = 0; i < x.size(); i++) poly[i] -= x[i];
        return *this;
    }

    friend Polynomial operator-(const Polynomial &a, const Polynomial &b) { return Polynomial(a.poly) -= b; }

    Polynomial &operator+=(const Polynomial &x){
        resize(max(x.size(), size()));
        for(int i = 0; i < x.size(); i++) poly[i] += x[i];
        return *this;
    }

    friend Polynomial operator+(const Polynomial &a, const Polynomial &b) { return Polynomial(a.poly) += b; }

    template<class U>
    Polynomial &operator%=(const U &x){
        if(size() > x) resize(x);
        return *this;
    }

    template<class U>
    friend Polynomial operator%(const Polynomial &a, const U &b) { return Polynomial(a.poly) %= b; }

    Polynomial &operator=(const Polynomial &x){
        poly = x.poly;
        return *this;
    }

    friend bool operator==(const Polynomial &a, const Polynomial &b){ return a.poly == b.poly; }

    friend bool operator!=(const Polynomial &a, const Polynomial &b){ return a.poly != b.poly; }

    Polynomial inv(){
        assert(poly[0] != 0);
        Polynomial ret(vector<T> { (T)1/poly[0] });
        int n = poly.size();
        for(int i = 2; i < 2*n; i *= 2){
            ret *= Polynomial(vector<T> {2}) - (ret*(*this%i))%i;
            ret %= i;
        }
        return ret%n;
    }

    Polynomial inv(int n){
        assert(poly[0] != 0);
        Polynomial ret(vector<T> { (T)1/poly[0] });
        for(int i = 2; i < 2*n; i *= 2){
            ret *= Polynomial(vector<T> {2}) - (ret*(*this%i))%i;
            ret %= i;
        }
        return ret%n;
    }

    bool hasSqrt(){
        int ind = 0;
        while(ind < poly.size() && poly[ind] == 0) ind++;
        if(ind == poly.size()) return true;
        return ind%2 == 0 && (ind == poly.size() || poly[ind].hasSqrt());
    }

    Polynomial sqrt(){
        assert(hasSqrt());
        int ind = 0, n = size();
        while(ind < n && poly[ind] == 0) ind++;
        if(ind == n) return *this;
        Polynomial mult = Polynomial(vector<T>(begin() + ind, end()));
        Polynomial ans = Polynomial(vector<T>{mult[0].sqrt()});
        for(int i = 2; i + ind/2 < 2*n; i *= 2){
            ans += (ans.inv(i) * mult)%i;
            ans *= (T)1/2;
        }
        Polynomial ret;
        for(int i = ind/2; i < n; i++) ret[i] = ans[i - ind/2];
        return ret;
    }

    Polynomial integral(){
        Polynomial ret;
        ret.resize(size() + 1);
        for(int i = 1; i <= size(); i++) ret[i] = poly[i - 1]/i;
        return ret;
    }

    Polynomial derivative(){
        Polynomial ret;
        ret.resize(size() - 1);
        for(int i = 1; i < size(); i++) ret[i - 1] = i*poly[i];
        return ret;
    }

    Polynomial ln(int n){
        Polynomial ret = (inv(n + 1) * derivative())%(n + 1);
        return ret.integral()%n;
    }

    Polynomial exp(int n){
        Polynomial ret(vector<T> {1});
        for(int i = 2; i < 2*n; i *= 2){
            ret *= Polynomial(vector<T> {1}) - ret.ln(i) + (*this)%i;
            ret %= i;
        }
        return ret%n;
    }

    template<class U>
    Polynomial &operator^=(U x){
        int n = size();
        (*this) = ln(n);
        (*this) *= x;
        (*this) = exp(n);
        return *this;
    }
};

int main(){
    int n;
    cin >> n;
    Polynomial<mint> p;
    for(int i = 0; i < n; i++) cin >> p[i];
    p = p.exp(n);
    for(int i = 0; i < p.size(); i++) cout << p[i] << " ";
    cout << endl;
}