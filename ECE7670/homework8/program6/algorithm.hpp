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
    const uint32_t N_BIT_PER_PACKET = 32;
    template <class T, class Func>
    void xgcd(const T &a, const T &b, T &g, T &s, T &t, Func isDone)
    {
        std::cout<<a<<std::endl;
        std::cout<<b<<std::endl;
        T rim2{0};
        T rim1{a};
        T rim0{b};
        T sim2{0};
        T sim1{1};
        T sim0{0};
        T tim2{0};
        T tim1{0};
        T tim0{1};
        int i = 0;
        while(!isDone(rim0)) {

            //update values for next loop
            i = i+1;
            rim2 = rim1;
            rim1 = rim0;
            sim2 = sim1;
            sim1 = sim0;
            tim2 = tim1;
            tim1 = tim0;

            // compute next set of values
            auto qi = rim2/rim1;
            rim0 = rim2 - qi*rim1;
            sim0 = sim2 - qi*sim1;
            tim0 = tim2 - qi*tim1;
        }
        g = rim0; s = sim0; t = tim0;
    }

    template<class T>
    void sugiyama(const int nInput, const T input[], polynomialT<T>& g, polynomialT<T>& s, polynomialT<T>& t)
    {
        // instantiate our x^n polynomial
        auto coefA = new T[nInput + 1]{};
        coefA[nInput] = T{1};
        polynomialT<T> a(nInput, coefA);

        // instantiate our data polynomial
        polynomialT<T> b(nInput - 1, input);

        // compute the extended Euclidean algorithm
        auto temp = [&](polynomialT<T> in) ->bool{ return in.degree < (nInput/2);};
        xgcd(a,b,g,s,t,temp);
        g/=t[0];
        s/=t[0];
        t/=t[0];
    }
    template<class T>
    void sugiyama(const T* input, const int nInput, T* output, int& nOutput, T* g, int& nG, T* s, int& nS)
    {
        polynomialT<T> gPol;
        polynomialT<T> sPol;
        polynomialT<T> tPol;
        sugiyama(nInput, input,gPol, sPol, tPol);
        nOutput = tPol.getdegree() + 1;
        nG = gPol.getdegree() + 1;
        nS = sPol.getdegree() + 1;
        for(int i = 0; i <= gPol.getdegree(); i++){g[i] = gPol[i];}
        for(int i = 0; i <= sPol.getdegree(); i++){s[i] = sPol[i];}
        for(int i = 0; i <= tPol.getdegree(); i++){output[i] = tPol[i];}
    }
    template<class T>
    void sugiyama(const T* input, const int& nInput, T* output, int& nOutput)
    {
        int nG = 0;
        int nS = 0;
        T* g = new T[nInput];
        T* s = new T[nInput];
        sugiyama(input,nInput,output,nOutput,g,nG,s,nS);
        delete[] g;
        delete[] s;
    }



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
        const uint32_t nSymbolPerPacket = N_BIT_PER_PACKET/D;
        const uint32_t nPacket = (((nData*D)/N_BIT_PER_PACKET) + (((nData/N_BIT_PER_PACKET)*N_BIT_PER_PACKET) != nData));
        uint32_t iBeta;
        for(int iSyndrome = 0; iSyndrome < nErrorCapacity*2; iSyndrome++)
        {
            iBeta = 0;

            for(int iPacket= 0; iPacket < nPacket; iPacket++)
            {
                auto working = data[iPacket];
                for(int iSymbol= 0; iSymbol < nSymbolPerPacket; iSymbol++)
                {
                    auto sample = GF2Rou<N>{working&mask};
                    if(sample != 0) {s[iSyndrome] += sample*(currBeta^(iBeta)); }
                    working >>= D;
                    iBeta++;
                }
            }
            std::cout<<s[iSyndrome] <<std::endl;
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
        delete[] dErrLocPoly;
    }

    template<uint32_t nCode, uint32_t nSymbol>
    void decodeRs(uint32_t* data, const uint32_t& nDataSymbol, const int nErrorCapacity, const uint32_t b=1)
    {
        // preallocate and initialize
        int nSyndrome = nErrorCapacity*2;
        GF2Rou<nCode>::set(numsys::viewmode,numsys::power);

        // compute syndrome values
        auto s = new GF2Rou<nCode>[nSyndrome]();
        computesyndrome<nCode,nSymbol>(data,nDataSymbol, nErrorCapacity, s);

        // if there are no errors, terminate
        int nError{0};
        for(int i = 0; i < nSyndrome; i++)
        {
            nError += (s[i] != 0);
        }
        if(nError == 0) return;

        // compute error-locator polynomial
        auto errLocPoly = new GF2Rou<nCode>[nSyndrome]();
        auto errLocPoly2 = new GF2Rou<nCode>[nSyndrome]();
        int nErrLocPoly2{0};
        int nErrLocPoly{0};
        sugiyama(s,nSyndrome,errLocPoly,nErrLocPoly);
        berlekampmassy<nSymbol>(s,nSyndrome,errLocPoly2,nErrLocPoly2);
        for(int i = 0; i < nSyndrome; i++)
        {
            std::cout<< "sugiyama["<<i<<"] = " << errLocPoly[i] << " berlekamp[" << i << "] = " << errLocPoly2[i] <<std::endl;
            if(errLocPoly[i] != errLocPoly2[i])
            {
                std::cout<<"problem"<<std::endl;
            }
        }

        // compute error locations
        auto errLoc = new GF2Rou<nCode>[nSyndrome]();
        int nErrLoc{0};
        chiensearch(errLocPoly,nErrorCapacity*2,errLoc,nErrLoc);
        for(int i = 0; i < nErrLoc; i++){errLoc[i] = errLoc[i]^(-1);}

        // compute error values
        auto corrected = new GF2Rou<nCode>[nErrLoc]();
        forney<nCode>(errLocPoly, nErrLocPoly, s, nSyndrome, errLoc, nErrLoc, corrected);

        // apply corrections
        const uint32_t symbolPerPacket{N_BIT_PER_PACKET/nSymbol};
        uint32_t loc, packetLoc, bitLoc;
        for(int iErr = 0; iErr < nErrLoc; iErr++)
        {
            loc = errLoc[iErr].exp();
            packetLoc = loc/symbolPerPacket;
            bitLoc = (loc - packetLoc*symbolPerPacket)*nSymbol;
            data[packetLoc]^=(corrected[iErr].value << bitLoc);
        }
        delete[] s;
        delete[] corrected;
        delete[] errLoc;
        delete[] errLocPoly;
        delete[] errLocPoly2;
    }

    //toPacketIdx
//    template<uint32_t nCode, uint32_t nInput, uint32_t b=1>
    polynomialT<GF2Rou<15>>toRsGenerator()
    {
        const uint32_t nCode = 15;
        const uint32_t nInput = 5;
        const uint32_t b = 1;

        auto r = beta<nCode>();
        const GF2Rou<nCode> t[] = {0,1};
        const polynomialT<GF2Rou<nCode>> x{1,t};
        auto generator = polynomialT<GF2Rou<nCode>>(1);
        for(int i=0; i < (nCode-nInput); i++){generator*=(x + (r^(b + i)));}
    }

    template<uint32_t nBitPerSymbol>
    uint32_t toPacketIdx(uint32_t symbolIdx)
    {
        return (symbolIdx*nBitPerSymbol)/N_BIT_PER_PACKET;
    }

    template<uint32_t nBitPerSymbol>
    uint32_t toShiftIdx(uint32_t symbolIdx)
    {
        return ((symbolIdx*nBitPerSymbol)%N_BIT_PER_PACKET);
    }

    template<uint32_t nCode, uint32_t nInput>
    uint32_t toErrorCapacity()
    {
        return (nCode - nInput)/2;
    }
    template<class T>
    T* multpoly(T* first, int degFirst, T* second, int degSecond, T* output)
    {
        auto w0 = degFirst < degSecond?first:second;
        auto w1 = degFirst >= degSecond?first:second;
        auto nw0 = degFirst < degSecond?degFirst + 1:degSecond+1;
        auto nw1 = degFirst >= degSecond?degFirst+1:degSecond+1;
        for(int k = 0; k < nw1+ nw0 - 1; k++)
        {
            output[k] = T{0};
            for(int i = std::max(0,k - nw1 + 1); i < std::min(nw0,k+1); i++)
            {
                 output[k] += w0[i]*w1[k-i];
            }
        }
    }
    //toShiftIdx
    template<uint32_t nCodeSymbol>
    uint32_t iSymToValue(const uint32_t* input, uint32_t idx)
    {
        const uint32_t nBitPerSymbol{numsys::rouToDeg<nCodeSymbol>()};
        const int bitIdx = idx*nBitPerSymbol;
        const int iPacket = (bitIdx/N_BIT_PER_PACKET);
        const int iShift = bitIdx - (iPacket*N_BIT_PER_PACKET);
        const int thresh = N_BIT_PER_PACKET - nBitPerSymbol;
        const uint32_t mask ((1 << nBitPerSymbol) - 1);
        uint32_t data = (input[iPacket] >> iShift)&mask;
        if(iShift > thresh)
        {
            data |= (input[iPacket+1] & ((1 << (N_BIT_PER_PACKET - iShift)) - 1));
        }
        return data;
    }

    template<uint32_t nCodeSymbol>
    uint32_t storesymbol(uint32_t src, uint32_t* dest, uint32_t idx)
    {
        const uint32_t nBitPerSymbol{numsys::rouToDeg<nCodeSymbol>()};
        const int bitIdx = nBitPerSymbol*idx;
        const int iPacket = (bitIdx/N_BIT_PER_PACKET);
        const int iShift = bitIdx - (iPacket*N_BIT_PER_PACKET);
        const int thresh = N_BIT_PER_PACKET - nBitPerSymbol;
        dest[iPacket] |= (src << iShift);
        if(iShift > thresh)
        {
            dest[iPacket + 1] = src  >> (nBitPerSymbol - (N_BIT_PER_PACKET - iShift));
        }

    }
    template<class T>
    void addpoly(T* poly1, T* poly2, uint32_t nPoly, T*poly3=nullptr)
    {
        if(poly3 == nullptr) poly3 = poly1;
        for(int i = 0; i < nPoly; i++)
        {
            poly3[i] = poly1[i] + poly2[i];
        }

    }
    template<class T>
    void shiftpoly(T* poly1, int nPoly, int shift, T*poly2=nullptr)
    {
        if(poly2 == nullptr) poly2 = poly1;
        if(shift > 0)
        {
            const int base1 = nPoly + shift - 1;
            const int base2 = nPoly - 1;
            for(int i = base1; i >= shift; i--)
            {
                poly2[i] = poly1[i - shift];
            }
            for(int i = 0; i < shift; i++)
            {
                poly2[i] = 0;
            }
        }
        else if(shift == 0) return;
        else
        {
            std::cerr << "shiftpoly not implemented for negative shifts." << std::endl;
            exit(-1);
        }
    }

    template<class T>
    void multpoly(T* poly, int nPoly, T coef, T* result=nullptr)
    {
        if(result == nullptr) result = poly;
         for(int i = 0; i < nPoly; i++)
         {
             result[i] = poly[i]*coef;
         }
    }
    template<class T>
    void divpoly(T* poly, int nPoly, T coef, T* result=nullptr)
    {
        if(result==nullptr) result=poly;
        for(int i = 0; i < nPoly; i++)
        {
            result[i] = poly[i]/coef;
        }
    }
    template<class T>
    void subpoly(T* poly, T* poly2, int nPoly, T* result=nullptr)
    {
        if(result==nullptr) result = poly;
        for(int i = 0; i < nPoly; i++)
        {
            result[i] = poly[i] - poly2[i];
        }
    }
    template<class T> // assumes the modulus polynomial is monic
    void modpoly(T* poly, int nPoly, T* mod, int nMod, T* result=nullptr)
    {

        //format result memory
        if(result==nullptr) result=poly;
        else
        {
            for(int i = 0; i < nPoly; i++) result[i] = poly[i];
        }
        auto temp = new T[nPoly]();
        for(int i = nPoly - 1; i >= nMod - 1; i--)
        {
            multpoly(mod,nMod,result[i],temp);
            shiftpoly(temp,nMod,i - nMod + 1);
            subpoly(result,temp, i + 1);
        }

        //cleanup
        delete[] temp;
    }

    template<uint32_t nCode, uint32_t nInput, uint32_t b=1>
    void encodeRs(const uint32_t* message, const uint32_t& nMessage, uint32_t* codeword, uint32_t& nCodeword)
    {
        GF2Rou<nCode>::set(numsys::viewmode,numsys::power);

        // compute generator
        const uint32_t nBit{numsys::rouToDeg<nCode>()};
        auto r = beta<nCode>();
        auto generator1 = new GF2Rou<nCode>[nCode-nInput + 1](); generator1[0] = 1;
        auto generator2 = new GF2Rou<nCode>[nCode-nInput + 1];

        GF2Rou<nCode> x[2] = {1,1};
        for(int i=0; i < (nCode-nInput); i++){
            x[0] *= r;
            multpoly(x,1, generator1, i, generator2);
            auto temp = generator1;
            generator1=generator2;
            generator2 = temp;
        }

        // compute codeword
        GF2Rou<nCode> inpoly[nCode] = {0};
        GF2Rou<nCode> inter[nCode] = {0};
        GF2Rou<nCode> outpoly[nCode] = {0};
        nCodeword = 0;
        for(int i = 0; i < nMessage; i+=nInput)
        {
            // populate polynomial
            for(int j = i; j < i + nInput; j++)
            {
                inpoly[(j - i)] = iSymToValue<nCode>(message,j);
            }

            // compute codeword
            shiftpoly(inpoly,nInput,nCode-nInput,inter);
            modpoly(inter,nCode,generator1,nCode-nInput + 1,outpoly);
            addpoly(outpoly,inter,nCode);

            // export codeword
            for(int j = nCodeword; j < (nCodeword + nCode); j++)
            {
                storesymbol<nCode>(outpoly[j - nCodeword].value,codeword,j);
            }
            nCodeword+=nCode;
        }
        delete[] generator1;
        delete[] generator2;
    }
}

#endif