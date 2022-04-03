template<class T>
struct Matrix { 
    int n;
    vector<vector<T>> mat;
    T MOD = -1;

    Matrix(int _n, T mod){
        n = _n;
        MOD = mod;
        mat.assign(n, vector<T>(n, 0));
        for(int i = 0; i < n; i++) mat[i][i] = 1; 
    }

    Matrix(int _n, T v, T mod){
        n = _n;
        MOD = mod;
        mat.assign(n, vector<T>(n, v));
    } 

    vector<T>& operator [] (int x){
        assert(x < n);
        assert(x >= 0);
        return mat[x];
    }

    const vector<T>& operator [] (int x) const {
        assert(x < n);
        assert(x >= 0);
        return mat[x];
    }

    Matrix& operator=(const Matrix<T>& x){
        if(this == &x) return *this;
        n = x.n;
        mat = x.mat;
        MOD = x.MOD;
        return *this;
    }

    Matrix operator*(const Matrix<T> &x){
        Matrix<T> ret(n, 0, MOD);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    if(MOD > 0) (ret[i][j] += (long long)mat[i][k]*x[k][j]) %= MOD;
                    else ret[i][j] += (long long)mat[i][k]*x[k][j];
                }
            }
        }
        return ret;
    }

    Matrix operator*=(const Matrix<T> &x){
        Matrix<T> ret(n, 0, MOD);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    if(MOD > 0) (ret[i][j] += (long long)mat[i][k]*x[k][j]%MOD) %= MOD;
                    else ret[i][j] += (long long)mat[i][k]*x[k][j];
                }
            }
        }
        mat = ret.mat;
        return *this;
    }

    Matrix operator+(const Matrix<T> &x){
        Matrix<T> ret(n, 0, MOD);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                ret[i][j] = mat[i][j] + x[i][j];
                if(MOD > 0) ret[i][j] %= MOD;
            }
        }
        return ret;
    }

    Matrix &operator+=(const Matrix<T> &x){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] += x[i][j];
                if(MOD > 0) mat[i][j] %= MOD;
            }
        }
        return *this;
    }

    Matrix operator-(const Matrix<T> &x){
        Matrix<T> ret(n, 0, MOD);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                ret[i][j] = mat[i][j] - x[i][j];
                if(MOD > 0) ret[i][j] += MOD, ret[i][j] %= MOD;
            }
        }
        return ret;
    }

    Matrix &operator-=(const Matrix<T> &x){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] -= x[i][j];
                if(MOD > 0) mat[i][j] += MOD, mat[i][j] %= MOD;
            }
        }
        return *this;
    }

    Matrix operator^(long long x){
        Matrix<T> ret(n, MOD);
        Matrix<T> prod(n, 0, MOD);
        prod = *this;
        while(x){
            if(x%2 == 1) ret *= prod;
            prod *= prod;
            x /= 2;
        }
        return ret;
    }

    Matrix &operator^=(long long x){
        Matrix<T> ret(n, MOD);
        while(x){
            if(x%2 == 1) ret *= (*this);
            (*this) *= (*this);
            x /= 2;
        }
        mat = ret.mat;
        return *this;
    }

    friend bool operator==(const Matrix<T> &a, const Matrix<T> &b){
        return a.n == b.n && a.MOD == b.MOD && a.mat == b.mat;
    }

    friend bool operator!=(const Matrix<T> &a, const Matrix<T> &b){
        return !(a == b);
    }

    friend ostream& operator<<(ostream &out, Matrix<T> &v){
        out << "[";
        for(int i = 0; i < v.n; i++){
            if(i) cout << "\n";
            out << "[";
            for(int j = 0; j < v.n; j++){
                if(j) out << ", ";
                cout << v[i][j];
            }
            out << "]";
        }
        out << "]\n";
        return out;
    }
};