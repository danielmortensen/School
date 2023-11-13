//
// Created by dmortensen on 11/9/23.
//
#include<iostream>
#include"GF2.h"
#include"ModAr.h"
#include"algorithm.hpp"
#include"polynomialT.h"
#include"polynomialT.cc"

#include"test_berlekampmassey.cpp"
template<uint32_t T>
polynomialT<numsys::GF2<T>> chiensearch(polynomialT<numsys::GF2<T>> error)
{

    // compute number of options per coefficient
    auto val = T;
    int idx = 0;
    while(val != 0) { val >>= 1; idx ++; }
    int nVal = (1 << (idx - 1));

    //
    return error;
}
int main()
{
//    test_berlekampmassey();
    polynomialT<numsys::GF2<0b10011>> test{1};
    auto temp = chiensearch(test);
    return 0;

}