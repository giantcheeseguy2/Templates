//https://cses.fi/problemset/task/1722
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

    array<T, n>& operator[](int x){
        assert(x < n);
        assert(x >= 0);
        return mat[x];
    }

    const array<T, n>& operator[](int x) const {
        assert(x < n);
        assert(x >= 0);
        return mat[x];
    }

    Matrix operator*(const Matrix<T, n> &x){
        Matrix<T, n> ret(0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    ret[i][j] += mat[i][k]*x[k][j];
                }
            }
        }
        return ret;
    }

    Matrix &operator*=(const Matrix<T, n> &x){
        Matrix<T, n> ret(0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    ret[i][j] += mat[i][k]*x[k][j];
                }
            }
        }
        mat = ret.mat;
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

    Matrix operator^(long long x){
        Matrix<T, n> ret;
        Matrix<T, n> prod = *this;
        while(x){
            if(x%2 == 1) ret *= prod;
            prod *= prod;
            x /= 2;
        }
        return ret;
    }

    Matrix &operator^=(long long x){
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

int main(){
    long long n;
    cin >> n;
    Matrix<int, 4> mat(0);
    mat[0][0] = 1;
    mat[0][1] = 1;
    mat[1][0] = 1;
    cout << (mat ^ n)[0][1] << endl;
}