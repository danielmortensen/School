//
// Created by dmortensen on 11/9/23.
//

#ifndef ModAr_H
#define ModAr_H
#include<iostream>

template <int M>
class ModAr{
private:
    void v(const int in){
        int val = 0;
        while((in + val)< 0)
        {
            val += m_;
        }
        v_ = (val + in);
    }
protected:
    int v_;
    int m_{M};
    static int showmod_;
    static void gcd(const int m, const int b, int &x, int &y, int &g)
    {
        // Compute the inverse of b by finding the gcd(m_,b)
        int u[3],v1[3],t[3];
        int *up, *vp, *tp, *temp;
        int q;
        u[0] = 1;  u[1] = 0;  u[2] = m;
        v1[0] = 0;  v1[1] = 1;  v1[2] = b;

        up = u;  vp = v1;  tp = t;
        while(vp[2]) {
            q = up[2]/vp[2];
            tp[0] = up[0]-vp[0]*q;   // t = u - v_*q
            tp[1] = up[1]-vp[1]*q;
            tp[2] = up[2]-vp[2]*q;
            temp = up;
            up = vp;                // u = v_;
            vp = tp;                // v_ = t;
            tp = temp;
        }
        x = up[0];  y=up[1];  g=up[2];
    }
public:

    // default constructor
    ModAr(int inv = 0) { v(inv % m_);}

    // copy constructor
    ModAr(const ModAr& in) { v(in.v());}

    // destructor
    ~ModAr()  = default;

    // replacements for getter/setter functions
    int v() const { return v_;}
    int m() const { return m_;}
    int showmod() const {return showmod_;}
    void showmod(const int in) {
        if (in != 0 && in != 1) {
            std::cerr << "showmod must be either 0 or 1";
            exit(-1);
        } else {
            showmod_ = in;
        }
    }

    //instead of toint, define an explicit conversion
    explicit operator int() const {return v_;}

    ModAr& operator+=(const ModAr &a) {
        v((v_ + a.v_) % M);
        return *this;
    }
    ModAr& operator-=(const ModAr &a) {
        v_ = (v_ - a.v_) % M;
        if(v_ < 0) v_ += M;
        return *this;
    }
    ModAr& operator*=(const ModAr &a) {
        v((v_ * a.v_) % M);
        return *this;
    }
    ModAr& operator/=(const ModAr &a)
    {
        if(a.v_ == 0) {
            std::cout << "Division by zero not allowed\n";
            exit(-1);
        }
        int x,y,g;
        gcd(M, a.v_, x, y, g);
        if(g != 1) {
            std::cerr << "Error: Attempting to divide by noninvertible number\n";
            exit(-1);
        }
        v((v_ * y) % M);
        return *this;
    }

    // unary -
    ModAr operator-() const { return ModAr(M - v_); }

    ModAr operator+(const ModAr &b) const {return ModAr(v_ + b.v_);}
    ModAr operator-(const ModAr &b) const {return ModAr(v_ - b.v_);}
    ModAr operator*(const ModAr &b) const {return ModAr(v_ * b.v_);}
    ModAr operator/(const ModAr &b) const
    {
        // assert(m_ == b.m_);
        // mod operation is done in constructor
        if(b.v_ == 0) {
            std::cerr<< "Division by zero not allowed\n";
            exit(-1);
        }
        int x,y,g;
        gcd(M, b.v(), x, y, g);
        if(g != 1) {
            std::cerr << "Error: Attempting to divide by noninvertible number\n";
            exit(-1);
        }

        return ModAr(v_ * y);
    }
    ModAr operator^(const int e) const {
        /* Returns a^e.  Caution: the precedence of this operator does not follow
           conventional mathematics.  Must use with parenthesis, as in a*(b^3).
           const int e is changed to int e because there is no
           way the variable location the user supplies for e is
           going to be modified.  The function is more efficient
           if it can modify its own copy of e.
         */
        int ehere = e;
        int p=1;

        // checking if e==0 is faster than checking i<=e
        // if p==0, there is no need to go further
        for(; ehere&&p; ehere--)
            p = (p * v_) % M;  // mod at each step just to keep growth in check

        // (this is slower than it needs to be)
        return ModAr(p,M);
    }
    bool operator==(const ModAr &b) const {return (v_ == b.v_);}
    bool operator!=(const ModAr &b) const {return (v_ != b.v_);}
    friend std::ostream& operator <<(std::ostream &os, const ModAr &a) {
        if (a.showmod() == 1)
            return os << a.v() << " (mod " << a.m() << ")";
        return os << a.v();
    }
};

// define inline functions
template<int M> inline ModAr<M>
operator+(const int a, const ModAr<M> b) { return b+a;}

template<int M> inline ModAr<M>
operator-(const int a, const ModAr<M> b) { return ModAr<M>(a)-b;}

template<int M> inline ModAr<M>
operator*(const int a, const ModAr<M> b) { return b*a;}

template<int M> inline ModAr<M>
operator/(const int a, const ModAr<M> b) { return ModAr<M>(a)/b;}

template<int M> inline ModAr<M>
operator==(const int a, const ModAr<M> b) { return b==a;}

template<int M> inline ModAr<M>
operator!=(const int a, const ModAr<M> b) { return b!=a;}


// define static variables
template<int M>
int ModAr<M>::showmod_ = 0;

// define integer-ModAr operators



#endif //ModAr_H
