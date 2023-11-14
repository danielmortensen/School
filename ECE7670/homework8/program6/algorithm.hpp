//
// Created by dmortensen on 11/13/23.
//
#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include"polynomialT.h"
#include"polynomialT.cc"
#include<vector>
namespace alg {
    enum inputtype {
        BCH, none
    };

    template<class T>
    polynomialT<T> berlekampmassy(const T *s, int n, inputtype optimizefor = none) {
        int L = 0;
        polynomialT<T> c{1};
        polynomialT<T> p{1};
        polynomialT<T> x{1};
        int loopincrement = optimizefor == none ? 1 : 2;

        int l{1};
        T dm{1};
        for (int k = 0; k < n; k += loopincrement) {
            T d = s[k];
            for (int j = 1; j <= c.getdegree(); j++) {
                d += c[j] * s[k - j];
            }
            if (d == 0) l++;
            else {
                if (c.getdegree() >= p.getdegree() + l) //2*L > k)
                {
                    c = c - (x << l) * p * (d / dm);
                    l++;
                } else {
                    auto t{c};
                    c = c - (x << l) * p * (d / dm);
                    L = k - L;
                    p = t;
                    dm = d;
                    l = 1;
                }
            }
            l += (optimizefor == BCH) ? 1 : 0;
        }
        return c;
    }


    template<class T>
    void berlekampmassy(const T *s, int n, T *c, int &L, inputtype optimizefor = none) {

        //preallocate placeholders and temporary variables
        auto out = c;
        T *p = new T[n]();
        T *t = new T[n]();
        T *temp;
        int loopincrement = optimizefor == none ? 1 : 2;

        // initialize memory
        c[0] = 1;
        p[0] = 1;
        L = 0;
        for (int i = 1; i < n; i++) {
            c[i] = 0;
        }

        //initialize other variables
        int l{1};
        T dm{1};

        // loop over each input sequence character and update
        for (int k = 0; k < n; k += loopincrement) {

            // compute discrepancy
            T d = s[k];
            for (int j = 1; j <= L; j++) {
                d += c[j] * s[k - j];
            }

            // update LFSR polynomial
            if (d == 0) l++;
            else {
                auto ddm = (d / dm);

                if (2 * L >= k + 1) {
                    // update when no size change is needed
                    for (int j = l; j <= L; j++) {
                        c[j] -= p[j - l] * ddm;
                    }
                    l++;
                } else {
                    // update when the length of the LFSR must increase
                    for (int j = 0; j <= L + l; j++) {
                        t[j] = (j < l) ? c[j] : (c[j] - p[j - l] * ddm);
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
            l += (optimizefor == BCH) ? 1 : 0;
        }

        // package data for export
        L++;
        for (int i = 0; i < L; i++) {
            out[i] = c[i];
        }
    }
    template<uint32_t T>
    std::vector<numsys::GF2<T>> chiensearch(polynomialT<numsys::GF2<T>> error) {
        // compute number of options per coefficient
        auto val = T;
        int idx = 0;
        while (val != 0) {
            val >>= 1;
            idx++;
        }
        int nVal = (1 << (idx - 1));

        auto nDegree = error.getdegree();
        std::vector<numsys::GF2<T>> zeros;
        numsys::GF2<T> curr, base, f;
        int zeroIdx = 0;
        for (int i = 0; i < nVal; i++) {
            base = i; curr = 1; f = 0;
            for (int j = 0; j <= nDegree; j++) {
                f += error[j] * curr;
                curr *= base;
            }
            if (f == 0) {
                zeros.push_back(base);
                zeroIdx++;
                if (zeroIdx == nDegree) { break; }
            }
        }
        return zeros;
    }
    template<uint32_t T>
    void chiensearch(numsys::GF2<T>* error, int nError, numsys::GF2<T>* zeros, int& nZero) {

        // compute number of options per coefficient
        auto nVal = (1 << T);
//        int idx = 0;
//        while (val != 0) {
//            val >>= 1;
//            idx++;
//        }
//        int nVal = (1 << (idx - 1));

        auto nDegree = nError - 1;
        numsys::GF2<T> curr, base, f;
        int zeroIdx = 0;
        for (int i = 0; i < nVal; i++) {
            base = i; curr = 1; f = 0;
            for (int j = 0; j <= nDegree; j++) {
                f += error[j] * curr;
                curr *= base;
            }
            if (f == 0) {
                zeros[zeroIdx] = base;
                zeroIdx++;
                if (zeroIdx == nDegree) { break; }
            }
        }
        nZero = zeroIdx;
    }
    template<uint32_t N>
    constexpr uint32_t nToGf2()
    {
        uint32_t r = 1;
        uint32_t m = 0;
        uint32_t val = 1;
        while(r != 0)
        {
            val*=2; m++;
            r = ((val - 1)^N)%N;
        }
        return m;
    }
    template<uint32_t D>
    constexpr numsys::GF2<nToGf2<D>()>beta()
    {
        constexpr uint32_t gf = nToGf2<D>();
        constexpr uint32_t p = ((1 << gf) - 1)/D;
        return numsys::GF2<gf>{p,numsys::exponent};
    }

    template<uint32_t N>
    void computesyndrome(const uint32_t& data, const int& nErrorCapacity, numsys::GF2<nToGf2<N>()>* s, const uint32_t& b=1)
    {
        auto base{beta<N>()};
        auto currBeta{beta<N>()^b};
        for(int iSyndrome = 0; iSyndrome < nErrorCapacity*2; iSyndrome++)
        {
            s[iSyndrome] = (data >> (N - 1))%2;
            for(int iData = 1; iData < N; iData++)
            {
                auto val = (data >> (N - iData - 1))%2;
                if(val == 1) s[iSyndrome] += currBeta^iData;
            }
            currBeta*=base;
        }
    }
    template<class M>
    M evaluatepoly(const M& x, const M* p, const int& n)
    {
        M output{p[0]};
        auto tempx = x;
        for(int i = 1; i < n; i++)
        {
            output+=p[i]*tempx;
            tempx*=x;
        }
        return output;
    }

    template<uint32_t N>
    void forney(numsys::GF2<nToGf2<N>()>* c, const int& nC, numsys::GF2<nToGf2<N>()>* s, const int &nS, numsys::GF2<nToGf2<N>()>* errloc, const int& nErrloc, numsys::GF2<nToGf2<N>()>* e, const uint32_t& b=1)
    {
        // compute polynomial multiplication
        int nConv= std::min((nC - 1)*(nS - 1) + 1, nS);
        numsys::GF2<nToGf2<N>()> temp[nConv];
        for(int k = 0; k < nConv; k++)
        {
            temp[k] = 0;
            for(int i = std::max(0,k - nS + 1); i < std::min(nS,k); i++)
            {
                temp[k] += (s[i]*c[k-i]);
            }
        }

        numsys::GF2<nToGf2<N>()> den;
        for(int iErr = 0; iErr < nC; iErr++)
        {
            auto num = evaluatepoly((errloc[iErr]^(-1)), c,nC);
            den = c[1];
            for(int iC = 3; iC < nC - (nC - 1)%2; iC+=2)
            {
                den+=c[iC]*(errloc[iErr]^(-iC));
            }
            den *= ((errloc[iErr]^(b-1)));
            e[iErr] = num/den;
            std::cout<<e[iErr]<<std::endl;
        }
    }


    template<uint32_t N>
    void decode(uint32_t data, int nErrorCapacity, uint32_t b=1)
    {
        // preallocate and initialize
        constexpr uint32_t G = nToGf2<N>();
        int nSyndrome = nErrorCapacity*2;
        numsys::GF2<G>::set(numsys::viewmode,numsys::power);

        // compute syndrome values
        numsys::GF2<G> s[nSyndrome]={0};
        computesyndrome<N>(data,nErrorCapacity, s);
        
        // compute error-locator polynomial
        numsys::GF2<G> c[nSyndrome] = {0};
        int L{0};
        berlekampmassy(s,nSyndrome,c,L);

        // compute zeros for error-locator polynomial
        numsys::GF2<G> zeros[nSyndrome] = {};
        int nZero{0};
        chiensearch(c,nErrorCapacity*2,zeros,nZero);

        // error locations are the reciprocals of the zero values
        uint32_t errloc[nZero];
        for(int i = 0; i < nZero; i++)
        {
            errloc[i] = (zeros[i]^(-1)).exp();
        }

        // compute error values
        numsys::GF2<G> corrected[nZero]{0};
        forney<N>(c, L, s, nSyndrome,zeros,nZero,corrected);

    }
}

#endif