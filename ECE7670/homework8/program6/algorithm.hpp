//
// Created by dmortensen on 11/13/23.
//
#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include"polynomialT.h"
#include"polynomialT.cc"

namespace alg
{
    enum inputtype{BCH, none};
    template <class T>
    polynomialT<T> berlekampmassy(const T* s, int n, inputtype optimizefor=none)
    {
        int L = 0;
        polynomialT<T> c{1};
        polynomialT<T> p{1};
        polynomialT<T> x{1};
        int loopincrement = optimizefor==none?1:2;

        int l{1};
        T dm{1};
        for(int k = 0; k < n; k+=loopincrement)
        {
            T d = s[k];
            for(int j = 1; j <= c.getdegree(); j++)
            {
                d+= c[j]*s[k - j];
            }
            if(d == 0)  l++;
            else
            {
                if (c.getdegree() >= p.getdegree() + l) //2*L > k)
                {
                    c = c - (x << l)*p*(d/dm);
                    l++;
                }
                else
                {
                    auto t{c};
                    c = c - (x << l)*p*(d/dm);
                    L = k - L;
                    p = t;
                    dm = d;
                    l = 1;
                }
            }
            l += (optimizefor==BCH)?1:0;
        }
        return c;
    }



    template <class T>
    void berlekampmassy(const T* s, int n, T* c, int& L, inputtype optimizefor=none)
    {

        //preallocate placeholders and temporary variables
        auto out = c;
        T* p = new T[n]();
        T* t = new T[n]();
        T* temp;
        int loopincrement = optimizefor==none?1:2;

        // initialize memory
        c[0] = 1;
        p[0] = 1;
        L = 0;
        for(int i = 1; i < n; i ++)
        {
            c[i] = 0;
        }

        //initialize other variables
        int l{1};
        T dm{1};

        // loop over each input sequence character and update
        for(int k = 0; k < n; k+=loopincrement)
        {

            // compute discriminant
            T d = s[k];
            for(int j = 1; j <= L; j++)
            {
                d += c[j]*s[k - j];
            }

            // update LFSR polynomial
            if(d == 0)  l++;
            else
            {
                auto ddm = (d/dm);

                if (2*L >= k + 1)
                {
                    // update when no size change is needed
                    for(int j = l; j <= L; j++)
                    {
                        c[j] -= p[j - l]*ddm;
                    }
                    l++;
                }
                else
                {
                    // update when the length of the LFSR must increase
                    for(int j = 0; j <= L + l ; j++)
                    {
                        t[j] = (j < l)?c[j]:(c[j] - p[j - l]*ddm);
                    }
                    temp = p;
                    p = c;
                    c = t;
                    t = temp;
                    L = k - L + 1;
                    dm = d;
                    l = 1;
                }
            }
            l += (optimizefor==BCH)?1:0;
        }

        // package data for export
        L++;
        for(int i = 0; i < L; i ++)
        {
            out[i] = c[i];
        }
    }
}
#endif