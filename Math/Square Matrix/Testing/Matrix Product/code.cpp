#include <bits/stdc++.h>
using namespace std;

template<class T, int n>
struct Matrix { 

    array<array<T, n>, n> mat;

    Matrix(){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] = (i == j);
            }
        }
    }

    Matrix(T v){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] = v; 
            }
        } 
    }

    array<T, n> &operator[](int x){
        assert(x < n);
        assert(x >= 0);
        return mat[x];
    }

    const array<T, n> &operator[](int x) const {
        assert(x < n);
        assert(x >= 0);
        return mat[x];
    }

    Matrix operator*(const Matrix<T, n> &x){
        Matrix<T, n> ret(0);
        for(int i = 0; i < n; i++){
            for(int k = 0; k < n; k++){
                for(int j = 0; j < n; j++){
                    ret[i][j] += mat[i][k]*x[k][j];
                }
            }
        }
        return ret;
    }

    Matrix &operator*=(const Matrix<T, n> &x){
        (*this) = (*this) * x;
        return *this;
    }

    Matrix operator+(const Matrix<T, n> &x){
        Matrix<T, n> ret(0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                ret[i][j] = mat[i][j] + x[i][j];
            }
        }
        return ret;
    }

    Matrix &operator+=(const Matrix<T, n> &x){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] += x[i][j];
            }
        }
        return *this;
    }

    Matrix operator-(const Matrix<T, n> &x){
        Matrix<T, n> ret(0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                ret[i][j] = mat[i][j] - x[i][j];
            }
        }
        return ret;
    }

    Matrix &operator-=(const Matrix<T, n> &x){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] -= x[i][j];
            }
        }
        return *this;
    }

    template<class U>
    Matrix operator^(U x){
        Matrix<T, n> ret;
        Matrix<T, n> prod = *this;
        while(x){
            if(x%2 == 1) ret *= prod;
            prod *= prod;
            x /= 2;
        }
        return ret;
    }

    template<class U>
    Matrix &operator^=(U x){
        Matrix<T, n> ret;
        while(x){
            if(x%2 == 1) ret *= (*this);
            (*this) *= (*this);
            x /= 2;
        }
        mat = ret.mat;
        return *this;
    }

    friend bool operator==(const Matrix<T, n> &a, const Matrix<T, n> &b){
        if(a.n != b.n) return false;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(a[i][j] != b[i][j]) return false;
            }
        }
        return true;
    }

    friend bool operator!=(const Matrix<T, n> &a, const Matrix<T, n> &b){
        return !(a == b);
    }

    friend ostream& operator<<(ostream &out, Matrix<T, n> &x){
        out << "[";
        for(int i = 0; i < x.n; i++){
            if(i) cout << "\n";
            out << "[";
            for(int j = 0; j < x.n; j++){
                if(j) out << ", ";
                cout << x[i][j];
            }
            out << "]";
        }
        out << "]\n";
        return out;
    }
};

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

    string to_string(const Modular&x) { return to_string(x.val); }
};

struct Mod { 
    using type = int;
    using cast = long long;
    const static type mod = 998244353; 
};

using mint = Modular<Mod>;

Matrix<mint, 1024> a(0), b(0), ans(0);

int main(){
    int n, m, k;
    cin >> n >> m >> k;
    for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) cin >> a[i][j];
    for(int i = 0; i < m; i++) for(int j = 0; j < k; j++) cin >> b[i][j];
    ans = a * b;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < k; j++){
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }
}