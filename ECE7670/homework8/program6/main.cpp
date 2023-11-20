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
    /* Implement a Reed-Solomon encoder for primitive, narrow-sense codes.
       Verify that the function works by encoding the data shown in Example 6.19.
    */
    {
        const uint32_t N_CODE{15};
        const uint32_t N_INPUT{9};
        const uint32_t N_MESSAGE = 1;

        // Store data from Example 6.19 in two unsigned 32-bit integers
        uint32_t message[2];
        message[0] = 0b11111010001110000110000100100101;
        message[1] = 0b00000000000000000000000000000100;

        // prepare location for the resulting codewords
        const uint32_t N_CODEWORD_PACKET = 2;
        uint32_t codeword[N_CODEWORD_PACKET];
        uint32_t nCodeword;

        // encode the data with a Reed-Solomon code
        alg::encodeRs<N_CODE,N_INPUT>(message,N_MESSAGE,codeword,nCodeword);

        // the results reflect the values from Example 6.19.

    }

    {
//        uint32_t m[2] = {};
//        m[0] = 0b1010001110000110000100100101;
//        m[1] = 0b0000000000000000000000000100;
//        uint32_t r[2];
//        r[0] = 0b00100101001001101000110101000101;
//        r[1] = 0b00000110111110100011100001100010;
//
//        const int N_CODE = 15;
//        const int N_BIT_PER_SYMBOL{numsys::rouToDeg<N_CODE>()};
//        alg::decodeRs<N_CODE,N_BIT_PER_SYMBOL>(r,15,3);
//        std::cout<<"temp"<<std::endl;
    }
    /* Implement a Reed-Solomon decoder for primitive, narrow-sense codes.
       Use the Berlekamp-Massey algorithm to find lambda(x), followed by
       the Chien search to find the error locators and the Forney algorithm
       to find the error values. You should be able to use much of the code
       that you have written previously.

       Test your decoder by decoding 10,000 patterns of up to 3 errors
       for the (255,249) code over the field GF(2^8) using
       p(x) = x^8 + x^4 + x^3 + x^2 + 1.
    */
    {
        const int N_CODE = 255;
        const int N_BIT_PER_SYMBOL{numsys::rouToDeg<N_CODE>()};
        const int ERR_CAPACITY{3};
        const int BIT_PER_PACKET{alg::N_BIT_PER_PACKET};
        uint32_t codeword[N_CODE*N_BIT_PER_SYMBOL/BIT_PER_PACKET + 1] = {0};
        const int N_TEST = 10000;
        for(int i = 0; i < N_TEST; i++)
         {
             // assign  up to three random errors
             int loc0 = rand()%255;
             int loc1 = rand()%255;
             int loc2 = rand()%255;
             alg::storesymbol<255>(1,codeword,(loc0));
             alg::storesymbol<255>(1,codeword,(loc1));
             alg::storesymbol<255>(1,codeword,(loc2));

             alg::decodeRs<255,numsys::rouToDeg<255>()>(codeword,N_CODE,ERR_CAPACITY);
             for(int j = 0; j < 8; j++)
             {
                 assert(codeword[j] == 0);
             }
         }
    }


    //GF2Rou<N_CODE>::set(numsys::viewmode,numsys::power);
//    uint32_t data[N_PACKET]{0};
//    for(int i = 0; i < 15; i++)
//    {
//        for(int j = 0; j < 15; j++)
//        {
//            for(int k = 0; k < 15; k++)
//            {
//                if(i == k | i == j | j == k) continue;
//                memset(data,0,sizeof(uint32_t)*N_PACKET);
//                uint32_t iPacket = (i*N_BIT_PER_SYMBOL)/N_DATA_PER_PACKET;
//                uint32_t iShift = i - ((i*N_BIT_PER_SYMBOL)/N_DATA_PER_PACKET)*N_DATA_PER_PACKET;
//                data[iPacket]^=(1 << iShift);
//
//                uint32_t jPacket = (j*N_BIT_PER_SYMBOL)/N_DATA_PER_PACKET;
//                uint32_t jShift = j - ((j*N_BIT_PER_SYMBOL)/N_DATA_PER_PACKET)*N_DATA_PER_PACKET;
//                data[iPacket]^=(1 << jShift);
//
//                uint32_t kPacket = (k*N_BIT_PER_SYMBOL)/N_DATA_PER_PACKET;
//                uint32_t kShift = k - ((k*N_BIT_PER_SYMBOL)/N_DATA_PER_PACKET)*N_DATA_PER_PACKET;
//                data[kPacket]^=(1 << kShift);
//
//                alg::decode<N_CODE, N_BIT_PER_SYMBOL>(data, N_DATA_SYMBOL, N_ERROR);
//                for(unsigned int l : data){
//                 assert(l == 0);
//                 }
//            }
//        }
//    }

    return 0;

}