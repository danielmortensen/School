//
// Created by dmortensen on 11/9/23.
//
#include<iostream>
#include"GF2.h"
#include"algorithm.hpp"
#include"polynomialT.h"


#include"test_berlekampmassey.cpp"

int main()
{
//    test_berlekampmassey();
//    numsys::GF2<4>::set(numsys::viewmode,numsys::power);
//    numsys::GF2<4> temp[] = {1, 1, 0,{5,numsys::exponent}};
//    auto output = alg::chiensearch(polynomialT<numsys::GF2<4>>{3, temp});
//    for(auto c : output)
//    {
//        std::cout << c << " ";
//    }

    const int N_CODE{15};
    const int N_BIT_PER_SYMBOL{4};
    const int N_ERROR{3};
    const int N_DATA_PER_PACKET{32};
    const uint32_t N_DATA_SYMBOL{15};
    numsys::GF2<N_BIT_PER_SYMBOL> encoded[] = {{8, numsys::exponent},{2, numsys::exponent},{14, numsys::exponent},
                              {3, numsys::exponent},{5, numsys::exponent},{1, numsys::exponent},
                              {8, numsys::exponent}, {1, numsys::exponent},{0, numsys::exponent},
                              {5, numsys::exponent},{3, numsys::exponent},{4, numsys::exponent},
                              {9, numsys::exponent},{12, numsys::exponent},{2, numsys::exponent}};

    // deep copy the data and add errors
    numsys::GF2<N_BIT_PER_SYMBOL> corrupted[N_DATA_SYMBOL] = {};
    for(int i = 0; i < N_DATA_SYMBOL; i++) corrupted[i] = encoded[i];
    corrupted[2] = {13,numsys::exponent};
    corrupted[8] = {1, numsys::exponent};
    corrupted[14] = {5, numsys::exponent};

    for(int i =0; i < N_DATA_SYMBOL; i++)
    {
        std::cout<<"i: " << i << " ideal: " << encoded[i].value << " corrupted: " << corrupted[i].value << " diff: " << (corrupted[i].value^encoded[i].value) << std::endl;
    }
    // translate the data to integers
    auto nBitPerSymbol = 4;
    auto serialized = new uint32_t[(N_DATA_SYMBOL*nBitPerSymbol)/32 + (N_DATA_SYMBOL*nBitPerSymbol)%32 == 0]();
    int intIdx = 0;
    int symIdx = 0;
    int nSymbolPerInteger{32/nBitPerSymbol};
    for(int i = 0; i < N_DATA_SYMBOL; i ++)
    {
        intIdx = i/nSymbolPerInteger;
        symIdx = i - intIdx*nSymbolPerInteger;
        serialized[intIdx] |= (corrupted[i].value << (symIdx*nBitPerSymbol));
        std::cout<<"iPacket: " << intIdx << " iShift: " << symIdx*nBitPerSymbol << std::endl;
    }

    // find and correct errors
    alg::decode<N_CODE, N_BIT_PER_SYMBOL>(serialized, N_DATA_SYMBOL, N_ERROR);

    // retrieve symbols from data
    numsys::GF2<4> corrected[N_DATA_SYMBOL] = {};
    const uint32_t nSymbolPerPacket{N_DATA_PER_PACKET/N_BIT_PER_SYMBOL};
    const uint32_t mask{(1 << N_BIT_PER_SYMBOL) - 1};
    uint32_t nUncorrected = 0;
    for(int i = 0; i <N_DATA_SYMBOL; i++)
    {
        uint32_t packetIdx = i/nSymbolPerPacket;
        uint32_t symbolIdx = i - packetIdx*nSymbolPerPacket;
        uint32_t shiftIdx = symbolIdx*N_BIT_PER_SYMBOL;
        corrected[i] = numsys::GF2<N_BIT_PER_SYMBOL>{(serialized[packetIdx] >> shiftIdx)&mask};
        nUncorrected += (corrected[i] != encoded[i]);
    }
    std::cout<<nUncorrected<<std::endl;

    return 0;

}