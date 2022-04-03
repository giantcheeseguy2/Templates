template<class T>
struct Matrix {
    int n;
    vector<vector<T>> mat;
    T MOD = - 1;

    Matrix(int _n){
        n = _n;
        mat.assign(n, vector<T>(n, 0));
        for(int i = 0; i < n; i++) mat[i][i] = 1; 
    }

    Matrix(int _n, T v){
        n = _n;
        mat.assign(n, vector<T>(n, v));
    }

    T& operator[][] (int x, int y){
        return mat[x][y];
    }

    Matrix<T> operator * (const Matrix<T> &x){
        Matrix<T> ret(n, 0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    ret[i][j] += (long long)mat[i][k]*x[k][j];
                    if(MOD != -1) ret[i][j] %= MOD;
                }
            }
        }
    }

    Matrix<T> &operator *= (const Matrix<T> &x){
        Matrix<T> ret(n, 0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    ret[i][j] += (long long)mat[i][k]*x[k][j];
                    if(MOD != -1) ret[i][j] %= MOD;
                }
            }
        }
        return this = ret;
    }

    Matrix<T> operator + (const Matrix<T> &x){
        Matrix<T> ret(n, 0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                ret[i][j] = mat[i][j] + x[i][j];
                if(MOD != -1) ret[i][j] %= mod;
            }
        }
        return ret;
    }

    Matrix<T> &operator += (const Matrix<T> &x){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] += x[i][j];
                if(MOD != -1) mat[i][j] %= mod;
            }
        }
        return this;
    }

    Matrix<T> operator - (const Matrix<T> &x){
        Matrix<T> ret(n, 0);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                ret[i][j] = mat[i][j] - x[i][j];
                if(MOD != -1) ret[i][j] += mod, ret[i][j] %= mod;
            }
        }
        return ret;
    }

    Matrix<T> &operator -= (const Matrix<T> &x){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                mat[i][j] -= x[i][j];
                if(MOD != -1) mat[i][j] += mod, mat[i][j] %= mod;
            }
        }
        return this;
    }
};