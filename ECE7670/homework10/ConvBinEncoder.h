//
// Created by daniel on 11/26/23.
//

#ifndef HOMEWORK10_CONVBINENCODER_H
#define HOMEWORK10_CONVBINENCODER_H
#include<cstdint>
#include<initializer_list>
#include<iostream>
template<uint32_t nInput, uint32_t nOutput>
class ConvBinEncoder
{
private:
    uint32_t num[nInput][nOutput] = {0};
    uint32_t den[nInput][nOutput] = {0};
    uint32_t currstate[nInput][nOutput] = {0};
    uint32_t dendeg[nInput][nOutput] = {0};
    uint32_t numdeg[nInput][nOutput] = {0};
    uint32_t denmsk[nInput][nOutput] = {0};
    uint32_t alldeg[nInput][nOutput] = {0};
    uint32_t nTotalState{0};
    constexpr uint32_t mod2(uint32_t in){return (__builtin_ffs(in) == 1);}
    constexpr uint32_t binsum(uint32_t in){return __builtin_parity(in);}
    uint32_t static getdeg(uint32_t val)
    {
        return (val == 0)?0:(31 - __builtin_clz(val));
    }
    constexpr void initnum(std::initializer_list<std::initializer_list<int32_t>> inputnum)
    {
        uint32_t iRow{0}, iCol{0};
        for(auto row : inputnum)
        {
            for(auto val : row)
            {
                num[iRow][iCol] = val;
                numdeg[iRow][iCol] = (val == 0)?0:getdeg(val);
                iCol++;
            }
            iCol=0;
            iRow++;
        }
    }
    constexpr void initden(std::initializer_list<std::initializer_list<int32_t>> inputden)
    {
        uint32_t iRow{0}, iCol{0};
        for(auto row: inputden)
        {
            for(auto val : row)
            {
                dendeg[iRow][iCol] = (val == 0)?0:getdeg(val);
                denmsk[iRow][iCol] = (1 << (dendeg[iRow][iCol] + 1));
                den[iRow][iCol] = (val & (denmsk[iRow][iCol] - 1));
                iCol++;
            }
            iCol = 0;
            iRow++;
        }
    }
    constexpr void computeNumTotalState()
    {
        for(int iInput = 0; iInput < nInput; iInput++)
        {
            for(int iOutput = 0; iOutput < nOutput; iOutput++)
            {
                alldeg[iInput][iOutput] = std::max(dendeg[iInput][iOutput],numdeg[iInput][iOutput]);
                nTotalState += (alldeg[iInput][iOutput] + 1);
            }
        }

    }
public:
    ConvBinEncoder(std::initializer_list<std::initializer_list<int32_t>> inputnum, std::initializer_list<std::initializer_list<int32_t>> inputden)
    {
        initnum(inputnum);
        initden(inputden);
        computeNumTotalState();
    }
    ConvBinEncoder(std::initializer_list<std::initializer_list<int32_t>> inputnum)
    {
        initnum(inputnum);
        computeNumTotalState();
    }
    void propagate(uint32_t data)
    {
        bool doOverflow;

        //propagate each row with the respective input bit
        for(int iIn= 0; iIn< nInput; iIn++)
        {
            uint32_t currdata = mod2(data >> iIn);
            for(int iOut = 0; iOut < nOutput; iOut++)
            {
                auto msk = (1 << (alldeg[iIn][iOut] + 1)) - 1;
                //    std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                doOverflow = (((denmsk[iIn][iOut] >> 1)&currstate[iIn][iOut]) != 0);
                //    std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                if(doOverflow) currstate[iIn][iOut] ^= den[iIn][iOut];
                //     std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                currstate[iIn][iOut] <<=1;
                //     std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                currstate[iIn][iOut] |= currdata;
                //     std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                currstate[iIn][iOut] &= msk; //(denmsk[iIn][iOut] == 0)?0:(denmsk[iIn][iOut] - 1);
                //    std::cout<<"currstate: " << currstate[iIn][iOut] << std::endl;
            }
        }
    }
    uint32_t getoutput()
    {
        uint32_t output{0};
        uint32_t curroutput{0};
        uint32_t feedback{0};
        bool doFeedback{0};
        uint32_t observation{0};
        for(int iOut = 0; iOut < nOutput; iOut++)
        {
            curroutput = 0;
            for(int iIn = 0; iIn < nInput; iIn++)
            {
                uint32_t currmsk{0xffffffff};
                uint32_t msk0 = denmsk[iIn][iOut] - 1;
                currmsk ^= msk0;
                doFeedback = (currstate[iIn][iOut] & currmsk) != 0;
                feedback = doFeedback?den[iIn][iOut]:0;
                observation = currstate[iIn][iOut]^feedback;

                curroutput |= (binsum(observation & num[iIn][iOut]) << iIn);
            }
            output |= (binsum(curroutput) << iOut);
        }
        return output;
    }
    void setstate(uint32_t instate)
    {
        for(int iInput = 0; iInput < nInput; iInput++)
        {
            for(int iOutput = 0; iOutput < nOutput; iOutput++)
            {
                currstate[iInput][iOutput] = 0;
                auto nShift = alldeg[iInput][iOutput] + 1;
                currstate[iInput][iOutput] |= (instate & ((1 << nShift) - 1));
                instate >>= nShift;
            }
        }
    }

    void setstate(uint32_t gidx0, uint32_t gidx1, uint32_t s)
    {
        currstate[gidx0][gidx1] = s;
    }
    void setinput(uint32_t input)
    {
        for(int iInput = 0; iInput < nInput; iInput++)
        {
            auto curr = mod2(input >> iInput);
            for(int iOutput = 0; iOutput < nOutput; iOutput++)
            {
                if(mod2(currstate[iInput][iOutput]) != curr) currstate[iInput][iOutput]^=1;
            }
        }
    }
    uint32_t getstate(uint32_t gidx0, uint32_t gidx1)
    {
        return currstate[gidx0][gidx1];
    }
    uint32_t getstate()
    {
        uint32_t output{0};
        uint32_t iShift{0};
        uint32_t nDeg;
        for(int iInput = 0; iInput < nInput; iInput++)
        {
            for(int iOutput = 0; iOutput < nOutput; iOutput++)
            {
                nDeg = alldeg[iInput][iOutput];
                output |= (currstate[iInput][iOutput])  << iShift;
                iShift += (nDeg + 1);
            }
        }
        return output;
    }
    uint32_t getinput()
    {
        uint32_t output{0};
        uint32_t iShift{0};
        for(int iInput = 0; iInput < nInput; iInput++)
        {
            output |= mod2(currstate[iInput][0])  << iShift;
            iShift += 1;
        }
        return output;
    }
    uint32_t getNumAllState() { return nTotalState; }

};
#endif //HOMEWORK10_CONVBINENCODER_H
