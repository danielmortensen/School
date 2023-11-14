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
//    numsys::GF2<0b10011>::set(numsys::viewmode,numsys::power);
//    numsys::GF2<0b10011> temp[] = {1, 1, 0,{5,numsys::exponent}};
//    auto output = alg::chiensearch(polynomialT<numsys::GF2<0b10011>>{3, temp});
//    for(auto c : output)
//    {
//        std::cout << c << " ";
//    }
//    std::cout<<std::endl;
    uint32_t data{0b010100001000000};
    int nData = 15;
    int nErrorCapacity = 3;
    alg::decode<15>(data,nErrorCapacity);
    return 0;

}