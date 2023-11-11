//
// Created by dmortensen on 10/22/23.
//
#include<iostream>
#include"GF2.h"
#include"polynomialT.h"
#include"polynomialT.cc"

int main()
{
    GF2::set(generator,0b10011);
    GF2 xval[] = {0, 1};
    polynomialT<GF2> x(1,xval);
    GF2 errorCoef[] = {1, 0b1010, 0b11};
    polynomialT<GF2> error(2,errorCoef);
    for(int i = 1; i < 16; i++)
    {   auto temp2 = GF2{i};
        auto temp = error%(x - temp2);
        auto temp3 = error/(x - temp2);
        if(temp == 0)
        {
            std::cout<<"i: " << i << " remainder: " << temp << "zero: " << temp2 << "other: " << temp3 << std::endl;
        }
    }


   // /*-----------------------------------------------------------------------------
   //  *                    tests for g(x) = x^3 + x + 1
   //  -----------------------------------------------------------------------------*/
   // {
   //     GF2::set(generator, 0b1011); // g(x) = x^3 + x + 1

   //     // test viewing functions
   //     for (int i = 1; i < 8; i++) {
   //         auto a = GF2(i);
   //         a.set(viewmode, binary);
   //         std::cout << i << " | " << a << " | ";
   //         a.set(viewmode, power);
   //         std::cout << a << " | ";
   //         a.set(viewmode, vector);
   //         std::cout << a << std::endl;
   //     }

   //     // test multiplication, division, exponentiation, and addition for GF(8)
   //     auto a = GF2(5);
   //     auto b = GF2(4);
   //     std::cout << "a * a: " << a * b << std::endl;
   //     std::cout << "a^2: " << (a ^ 2) << std::endl;
   //     std::cout << "a/a: " << (a / a) << std::endl;
   //     std::cout << "a + c" << a + b << std::endl;
   // }

   // /*-----------------------------------------------------------------------------
   //  *                    tests for g(x) = x^4 + x + 1
   //  -----------------------------------------------------------------------------*/
   // {
   //     GF2::set(generator, 0b10011); // g(x) = x^4 + x + 1

   //     // observe table values
   //     for (int i = 1; i < 16; i++) {
   //         auto a = GF2(i);
   //         a.set(viewmode, binary);
   //         std::cout << i << " | " << a << " | ";
   //         a.set(viewmode, power);
   //         std::cout << a << " | ";
   //         a.set(viewmode, vector);
   //         std::cout << a << std::endl;
   //     }

   //     // test multiplication, division, exponentiation, and addition for GF(8)
   //     auto a = GF2(5);
   //     auto b = GF2(4);
   //     std::cout << "a * a: " << a * b << std::endl;
   //     std::cout << "a^2: " << (a ^ 2) << std::endl;
   //     std::cout << "a/a: " << (a / a) << std::endl;
   //     std::cout << "a + c: " << a + b << std::endl;
   // }
    return 0;
}