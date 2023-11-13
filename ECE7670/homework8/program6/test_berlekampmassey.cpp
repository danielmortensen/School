//
// Created by dmortensen on 11/13/23.
//
#ifndef TEST_BERLEKAMPMASSEY_CPP
#define TEST_BERLEKAMPMASSEY_CPP

#include<iostream>
#include"ModAr.h"
#include"GF2.h"
#include"algorithm.hpp"

int test_berlekampmassey() {

    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (2), berlekampmassey                      " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;
// Preliminary exercise 1 for berlekampmassey method
    {
        numsys::ModAr<5> s[] = {2, 3, 4, 2, 2, 3};
        int nElement = 6;
        auto output = alg::berlekampmassy(s, nElement);
        std::cout << "Preliminary Exercise 1, berlekampmassey " << std::endl;
        std::cout << "Expected: 1 + 3x + 4x^2 + 2x^3, computed: " << output << std::endl;
    }

// Preliminary Exercise 2 for berlekampmassey method
    {
        numsys::GF2<0b11> s[] = {1, 1, 1, 0, 1, 0, 0};
        int nElement = 7;
        auto output = alg::berlekampmassy(s, nElement);
        std::cout << "Preliminary Exercise 2, berlekampmassey: " << std::endl;
        std::cout << "Expected: 1 + x + x^3, computed: " << output << std::endl;
    }

// Preliminary Exercise 3 for berlekampmassey method
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {0, {3, numsys::exponent}, {4, numsys::exponent}, {7, numsys::exponent}};
        int nElement = 4;
        auto output = alg::berlekampmassy(s, nElement);
        std::cout << "Preliminary Exercise 3, berlekampmassey: " << std::endl;
        std::cout << "Expected: 1 + ax + a^10x^2, computed: " << output << std::endl;
    }

    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (3), berlekampmassey                      " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;
// verifies the sequence {a^10, a, a^13, a^2, a^5, a^11} produces 1 + a^8x + a^2x^2 in GF(2^4)
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {{8,  numsys::exponent},
                                    {1,  numsys::exponent},
                                    {13, numsys::exponent},
                                    {2,  numsys::exponent},
                                    {5,  numsys::exponent},
                                    {11, numsys::exponent}};
        int nElement = 6;
        auto output = alg::berlekampmassy(s, nElement);
        std::cout << "Expected: 1 + a^8x + a^2x^2, computed: " << output << std::endl;
    }

    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (4), berlekampmassey                      " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;

// verifies the sequence {0, 0, a^5, 0, 1, a^10}
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {0, 0, {5, numsys::exponent}, 0, 1, {10, numsys::exponent}};
        int nElement = 6;
        auto output = alg::berlekampmassy(s, nElement);
        std::cout << "Expected: 1 + a^10x^2 + a^5x^3, computed: " << output << std::endl;
    }


    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (5), berlekampmassey                      " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;
// verifies the sequence {a^14, a^13, 1, a^11, a^5, 1} produces 1 + a^14x + a^11x^2 + a^14x^3
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {{0,  numsys::exponent},
                                    {0,  numsys::exponent},
                                    {10, numsys::exponent},
                                    {0,  numsys::exponent},
                                    {10, numsys::exponent},
                                    {5,  numsys::exponent}};
        int nElement = 6;
        auto output = alg::berlekampmassy(s, nElement, alg::BCH);
        std::cout << "Expected: 1 + x + a^5x^3, Computed: " << output << std::endl;
    }
// verifies the sequence {a^14, a^13, 1, a^11, a^5, 1} produces 1 + a^14x + a^11x^2 + a^14x^3
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {{14, numsys::exponent},
                                    {13, numsys::exponent},
                                    {0,  numsys::exponent},
                                    {11, numsys::exponent},
                                    {5,  numsys::exponent},
                                    {0,  numsys::exponent}};
        int nElement = 6;
        auto output = alg::berlekampmassy(s, nElement, alg::BCH);
        std::cout << "Expected: 1 + a^14x + a^11x^2 + a^14x^3, Computed: " << output << std::endl;
    }


    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (1), berlekampmassey2                     " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;
//  // Preliminary exercise 1 for berlekampmassey2 method
    {
        numsys::ModAr<5> s[] = {2, 3, 4, 2, 2, 3};
        int nElement = 6;
        auto c = new numsys::ModAr<5>[nElement]();
        int L{0};
        alg::berlekampmassy(s, nElement, c, L);
        std::cout << "Preliminary Exercise 1, berlekampmassey2" << std::endl;
        std::cout << "Expected: 1 3 4 2. Computed: ";
        for (int i = 0; i < L; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << std::endl;
    }

// Preliminary Exercise 2 for berlekampmassey2 method
    {
        numsys::GF2<0b11> s[] = {1, 1, 1, 0, 1, 0, 0};
        int nElement = 7;
        auto c = new numsys::GF2<0b11>[nElement]();
        int L{0};
        alg::berlekampmassy(s, nElement, c, L);
        std::cout << "Preliminary Exercise 2, berlekampmassey2" << std::endl;
        std::cout << "Expected: 1 1 0 1. Computed: ";
        for (int i = 0; i < L; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << std::endl;
    }

// Preliminary Exercise 3 for berlekampmassey2 method
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {0, {3, numsys::exponent}, {4, numsys::exponent}, {7, numsys::exponent}};
        int nElement = 4;
        auto c = new numsys::GF2<0b10011>[nElement]();
        int L{0};
        alg::berlekampmassy(s, nElement, c, L);
        std::cout << "Preliminary Exercise 3, berlekampmassey2." << std::endl;
        std::cout << "Expected: 1 a a^10. Computed: ";
        for (int i = 0; i < L; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (3), berlekampmassey2                     " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {{8,  numsys::exponent},
                                    {1,  numsys::exponent},
                                    {13, numsys::exponent},
                                    {2,  numsys::exponent},
                                    {5,  numsys::exponent},
                                    {11, numsys::exponent}};
        int nElement = 6;
        auto c = new numsys::GF2<0b10011>[nElement]();
        int L{0};
        alg::berlekampmassy(s, nElement, c, L);
        std::cout << "berlekampmassey2: Expected: 1 a^8 a^2. Computed: ";
        for (int i = 0; i < L; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (4), berlekampmassey2                     " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {0, 0, {5, numsys::exponent},
                                    0, {0, numsys::exponent}, {10, numsys::exponent}};
        int nElement = 6;
        auto c = new numsys::GF2<0b10011>[nElement]();
        int L{0};
        alg::berlekampmassy(s, nElement, c, L);
        std::cout << "berlekampmassey2: Expected: 1 0 a^10 a^5. Computed: ";
        for (int i = 0; i < L; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << " -------------------------------------------------------------------" << std::endl;
    std::cout << "                     Test (5), berlekampmassey2                     " << std::endl;
    std::cout << " -------------------------------------------------------------------" << std::endl;
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {{0,  numsys::exponent},
                                    {0,  numsys::exponent},
                                    {10, numsys::exponent},
                                    {0,  numsys::exponent},
                                    {10, numsys::exponent},
                                    {5,  numsys::exponent}};
        int nElement = 6;
        auto c = new numsys::GF2<0b10011>[nElement]();
        int L{0};
        alg::berlekampmassy(s, nElement, c, L, alg::BCH);
        std::cout << "berlekampmassey2: Expected: 1, 1, 0, a^5. Computed: ";
        for (int i = 0; i < L; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << std::endl;
    }
    {
        numsys::GF2<0b10011>::set(numsys::viewmode, numsys::power);
        numsys::GF2<0b10011> s[] = {{14, numsys::exponent},
                                    {13, numsys::exponent},
                                    {0,  numsys::exponent},
                                    {11, numsys::exponent},
                                    {5,  numsys::exponent},
                                    {0,  numsys::exponent}};
        int nElement = 6;
        auto c = new numsys::GF2<0b10011>[nElement]();
        int L{0};
        alg::berlekampmassy(s, nElement, c, L, alg::BCH);
        std::cout << "berlekampmassey2: Expected: 1 a^14 a^11 a^14. Computed: ";
        for (int i = 0; i < L; i++) {
            std::cout << c[i] << " ";
        }
        std::cout << std::endl;
    }
}
#endif