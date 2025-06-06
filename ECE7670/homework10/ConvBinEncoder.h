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
                numdeg[iRow][iCol] = getdeg(val);
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
                dendeg[iRow][iCol] = getdeg(val);
                denmsk[iRow][iCol] = (1 << (dendeg[iRow][iCol] + 1));
                den[iRow][iCol] = val;
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
                nTotalState += (alldeg[iInput][iOutput]);
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
    static uint32_t toint(std::vector<uint32_t> input)
    {
        uint32_t output{0};
        for(uint32_t iInput = 0; iInput < input.size(); iInput++)
        {
            output |= input[iInput] << iInput;
        }
        return output;
    }

    template<class T>
    static std::vector<T> tovec(uint32_t data, uint32_t nData)
    {
        auto output = std::vector<T>(nData);
        for(int i = 0; i < nData; i++)
        {
            output[i] = (data >> i)&1;
        }
        return output;
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
                uint32_t threshold = (1 << (alldeg[iIn][iOut] - 1));
                doOverflow = (currstate[iIn][iOut] >= threshold);
                currstate[iIn][iOut] <<=1;
                if(doOverflow) currstate[iIn][iOut] ^= den[iIn][iOut];
                currstate[iIn][iOut] ^= currdata;
                currstate[iIn][iOut] &= ((1 << (alldeg[iIn][iOut]))- 1);
            }
        }
    }
    uint32_t getoutput(uint32_t input)
    {
        uint32_t output{0};
        uint32_t curroutput;
        for(int iOut = 0; iOut < nOutput; iOut++)
        {
            curroutput = 0;
            for(int iIn = 0; iIn < nInput; iIn++)
            {
                // local variables
                uint32_t currIn = mod2(input >> iIn);
                uint32_t threshold = (1 << (alldeg[iIn][iOut] - 1));
                uint32_t s = (currstate[iIn][iOut] << 1)^ currIn;

                // execute feedback
                if((currstate[iIn][iOut]  >= threshold) & (den[iIn][iOut] != 0))
                {
                    s^=(1 << (alldeg[iIn][iOut]));
                    s^=den[iIn][iOut];
                }

                // append results to output
                curroutput |= (binsum((s& num[iIn][iOut])) << iIn);
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
                auto nShift = alldeg[iInput][iOutput];
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
                iShift += nDeg;
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
    constexpr uint32_t getNumInput()
    {
        return nInput;
    }
    constexpr uint32_t getNumOutput()
    {
        return nOutput;
    }
    uint32_t getNumAllState() { return nTotalState; }

};
#endif //HOMEWORK10_CONVBINENCODER_H
