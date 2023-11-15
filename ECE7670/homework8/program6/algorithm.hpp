//
// Created by dmortensen on 11/13/23.
//
#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include"polynomialT.h"
#include"polynomialT.cc"
#include<vector>

template<uint32_t N>
using GF2Rou = numsys::GF2<numsys::rouToDeg<N>()>;
using cint32 = const int32_t&;

namespace alg {

    enum decodemethod{
        ReedSolomon, BCH
    };

    template<uint32_t nSymbol = 2, class T>
    polynomialT<T> berlekampmassy(const T *s, int n) {
        int L = 0;
        polynomialT<T> c{1};
        polynomialT<T> p{1};
        polynomialT<T> x{1};
        int loopincrement = (nSymbol == 1) ? 2 : 1;

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
            l += (nSymbol == 1) ? 1 : 0;
        }
        return c;
    }

    template<uint32_t nSymbol=2, class T>
    void berlekampmassy(const T *s, int n, T *c, int &L) {

        //preallocate placeholders and temporary variables
        auto out = c;
        T *p = new T[n]();
        T *t = new T[n]();
        T *temp;
        int loopincrement = (nSymbol == 1) ? 2 : 1;

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
            l += (nSymbol == 1) ? 1 : 0;
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

    template<uint32_t D>
    constexpr GF2Rou<D>beta()
    {
        constexpr uint32_t gf = numsys::rouToDeg<D>();
        constexpr uint32_t p = ((1 << gf) - 1)/D;
        return numsys::GF2<gf>{p,numsys::exponent};
    }

    template<uint32_t N, uint32_t D = 1> // D is the number of bits per symbol
    void computesyndrome(uint32_t* data, const int& nData, const int& nErrorCapacity, GF2Rou<N>* s, const uint32_t& b=1)
    {
        auto base{beta<N>()};
        auto currBeta{beta<N>()^b};
        const uint32_t mask{(1 << D) - 1};
        const uint32_t nSymbolPerPacket = 32/D;
        const uint32_t nPacket = ((nData/32) + (((nData/32)*32) != nData)) + 1;
        uint32_t iBeta = 0;
        for(int iSyndrome = 0; iSyndrome < nErrorCapacity*2; iSyndrome++)
        {
            for(int iPacket= 0; iPacket < nPacket; iPacket++)
            {
                auto working = data[iPacket];
                for(int iSymbol= 0; iSymbol < nSymbolPerPacket; iSymbol++)
                {
                    auto sample = GF2Rou<N>{working&mask};
                    if(sample == 0) continue;
                    else  s[iSyndrome] += sample*(currBeta^(iBeta));
                    working >>= D;
                    iBeta++;
                }
            }
            currBeta*=base;
        }
    }

    template<uint32_t N>
    void forney(GF2Rou<N>* errLocPoly, cint32 nErrLocPoly, GF2Rou<N>* s, cint32 nS, GF2Rou<N>* errLoc, cint32 nErrLoc, GF2Rou<N>* e, cint32 b=1)
    {
        // compute polynomial multiplication, error locator poly < syndrome poly
        // 2t is the number of syndrome values
        GF2Rou<N> errCorrPoly[nS];
        for(int k = 0; k < nS; k++)
        {
            for(int i = std::max(0,k - nS + 1); i < std::min(nErrLocPoly,k + 1); i++)
            {
                errCorrPoly[k%nS] += (errLocPoly[i]*s[k-i]);
            }
        }

        // compute derivative of the error locator polynomial
        auto dErrLocPoly = new GF2Rou<N>[nErrLocPoly - 1]();
        for(int i = 0; i < nErrLocPoly - 1; i+=2)  dErrLocPoly[i] = errLocPoly[i+1];

        // compute error values
        GF2Rou<N> xDErrLoc;
        GF2Rou<N> xErrEval;
        for(int i = 0; i < nErrLoc; i++)
        {
            xDErrLoc = 0;
            for(int j = 0; j < nErrLocPoly; j+=2) xDErrLoc += (dErrLocPoly[j]*(errLoc[i]^(-j)));

            xErrEval = 0;
            for(int j = 0; j < nS; j++)
            {
                xErrEval += errCorrPoly[j]*(errLoc[i]^(-j));

            }
            e[i] = -(xErrEval/(xDErrLoc*((errLoc[i])^(b-1))));
        }
    }


    template<uint32_t nCode, uint32_t nSymbol>
    void decode(uint32_t* data, const uint32_t& nDataSymbol, const int nErrorCapacity, const uint32_t b=1)
    {
        // preallocate and initialize
        int nSyndrome = nErrorCapacity*2;
        GF2Rou<nCode>::set(numsys::viewmode,numsys::power);

        // compute syndrome values
        GF2Rou<nCode> s[nSyndrome]={0};
        computesyndrome<nCode,nSymbol>(data,nDataSymbol, nErrorCapacity, s);

        // compute error-locator polynomial
        GF2Rou<nCode> errLocPoly[nSyndrome] = {0};
        int nErrLocPoly{0};
        berlekampmassy<nSymbol>(s,nSyndrome,errLocPoly,nErrLocPoly);

        // compute error locations
        GF2Rou<nCode> errLoc[nSyndrome] = {}; int nErrLoc{0};
        chiensearch(errLocPoly,nErrorCapacity*2,errLoc,nErrLoc);
        for(auto& d : errLoc){d = d^(-1);}

        // compute error values
        GF2Rou<nCode> corrected[nErrLoc]{0};
        forney<nCode>(errLocPoly, nErrLocPoly, s, nSyndrome, errLoc, nErrLoc, corrected);

        // apply corrections
        const uint32_t nBitPerSymbol{numsys::rouToDeg<nCode>()};
        const uint32_t symbolPerPacket{32/nBitPerSymbol};
        uint32_t loc, packetLoc, bitLoc;
        for(int iErr = 0; iErr < nErrLoc; iErr++)
        {
            loc = errLoc[iErr].exp();
            packetLoc = loc/symbolPerPacket;
            bitLoc = (loc - packetLoc*symbolPerPacket)*nBitPerSymbol;
            data[packetLoc]^=(corrected[iErr].value << bitLoc);
        }
    }
}

#endif