template<class mod>
struct Modular {

    using T = typename mod::type;
    using C = typename mod::cast;

    template<class U>
    static T norm(const U &x){
        T ret;
        if(-mod::mod <= x && x < mod::mod) ret = static_cast<T>(x);
        else ret = static_cast<T>(x%mod::mod);
        if(ret < 0) ret += mod::mod;
        return ret;
    }

    template<class U>
    static T fpow(U a, U b){
        T ret = 1;
        T prod = norm(a);
        while(b){
            if(b%2 == 1) ret = norm((C)ret*prod);
            prod = norm((C)prod*prod);
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
        val = norm((C)val - x.val);
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

    friend bool operator<(const Modular &a, const Modular &b){ return a.val < b.val; }

    friend bool operator<=(const Modular &a, const Modular &b){ return a.val <= b.val; }

    friend bool operator>(const Modular &a, const Modular &b){ return a.val > b.val; }

    friend bool operator>=(const Modular &a, const Modular &b){ return a.val >= b.val; }

    friend bool operator==(const Modular &a, const Modular &b){ return a.val == b.val; }

    friend bool operator!=(const Modular &a, const Modular &b){ return a.val != b.val; }

    friend ostream &operator<<(ostream &out, Modular &x){ return out << x.val; }

    friend istream &operator>>(istream &in, Modular &x){ 
        in >> x.val;
        x.val = norm(x.val);
        return in;
    }

    string to_string(const Modular x) { return to_string(x.val); }
};

struct Mod { 
    using type = int;
    using cast = long long;
    const static type mod = 1000000007; 
};

using mint = Modular<Mod>;