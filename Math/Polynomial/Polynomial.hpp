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
        for(int i = 2; i + ind < 2*n; i *= 2){
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
        Polynomial ret = (inv(n + 1)*derivative())%(n + 1);
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
    Polynomial pow(U k, int n){
        for(int i = 0; i < min((int)poly.size(), n); i++){
            if(i*k >= n) return Polynomial(n);
            if(poly[i] == 0) continue;
            Polynomial ans = Polynomial(vector<T>(begin() + i, end()));
            ans.resize(n - k*i);
            T first = ans[0];
            ans *= (T)1/first;
            ans = ans.ln(n - k*i)*k;
            ans = ans.exp(n - k*i);
            ans *= first ^ k;
            Polynomial ret;
            for(int j = k*i; j < n; j++) ret[j] = ans[j - k*i];
            return ret;
        }
        return Polynomial(n);
    }
};