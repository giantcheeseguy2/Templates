template<class T, T mod>
struct MInt {

    T val;

    MInt(){
        val = 0;
    }

    template<class U>
    static T norm(const U &x){
        T ret;
        if(-mod <= x && x <= mod) ret = static_cast<T>(x);
        else ret = static_cast<T>(x%mod);
        if(ret < 0) ret += mod;
        return ret;
    }

    const T &operator()()
};