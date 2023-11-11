//
// Created by dmortensen on 9/14/23.
//
#include<iostream>
#include"BinLFSR.h"
#include "BinPolyDiv.h"
void runBinLFSR(unsigned int startState, unsigned int connect, int nPoly)
{
    BinLFSR X(connect, nPoly, startState);
    char out;
    unsigned int state;
    int nStep = 16;

    std::cout << " out " << "State" << std::endl;
    for(int i = 0; i < nStep; i++)
    {
        out = X.step(state);
        std::cout<< "iStep: " << i << " " << int(out) << " ";
        for(int ii = 0; ii < 4; ii++)
        {
            std::cout << (state >> ii)%2;
        }
        std::cout << std::endl;
    }
}

void runPolyDiv(unsigned int divisor, unsigned char dividend[], int nDegreeDivisor, int nDegreeDividend)
{
    unsigned int quotient;
    unsigned int remainder;
    unsigned int nDegreeQuotient, nDegreeRemainder;
    BinPolyDiv X(divisor,nDegreeDivisor);
    remainder = X.div(dividend, nDegreeDividend, quotient, nDegreeQuotient, nDegreeRemainder);
    std::cout<< "remainder: ";
    for(int i = 0; i <= nDegreeRemainder; i++)
    {
        std::cout << (remainder >> i)%2 << " ";
    }
    std::cout << "quotient: ";
    for(int i = 0; i <= nDegreeQuotient; i++)
    {
        std::cout << (quotient >> i)%2 << " ";
    }
    std::cout << std::endl;
}

int main()
{
    // Programming Part BinLFSR -- Example 4.38
    runBinLFSR(0b0001,0b10111,4); // start state of (1,0,0,0), divisor(x) = x^4 + x^2 + x + 1
    runBinLFSR(0b0011,0b10011,4); // start state of (1,1,0,0), divisor(x) = x^4 + x^2 + x + 1
    runBinLFSR(0b1101,0b10111,4); // start state of (1,0,1,1), divisor(x) = x^4 + x^2 + x + 1

    // Programming Part BinLFSR -- Table 4.12
    runBinLFSR(0b0001,0b10011,4); // start state of (1,0,0,0), divisor(x) = x^4 + x + 1

    // Programming Part BinPolyDiv -- Preliminary Exercise
    unsigned int divisor = 0b00011011; // x^4 + x^3 + x + 1
    unsigned char dividend[] ={1, 1, 0, 1, 1, 1, 0, 1, 1}; //1 + x + x^3 + x^4 + x^5 + x^7 + x^8
    int nDegreeDivisor = 4;
    int nDegreeDividend = 8;
    runPolyDiv(divisor,dividend,nDegreeDivisor,nDegreeDividend);

    // Programming Part BinPolyDiv -- Example 4.24
    unsigned int divisor2 = 0b00100011;
    unsigned char dividend2[] = {1, 1, 0, 0, 0, 1, 0, 1, 1};
    int nDegreeDivisor2 = 5;
    int nDegreeDividend2 = 8;
    runPolyDiv(divisor2,dividend2,nDegreeDivisor2,nDegreeDividend2);

    // Programming Part Follow-On Ideas and Problems
    runBinLFSR(0b0001, 0b10011, 4); // start state of (0, 0, 0, 0), divisor(x) = x^4 + x + 1
    runBinLFSR(0b0001,0b10111,4); // start state (0, 0, 0, 1), divisor(x) = x^4 + x^2 + x + 1

    return 0;
}
