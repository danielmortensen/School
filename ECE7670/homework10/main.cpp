//
// Created by dmortensen on 11/21/23.
//
#include<cstdint>
#include<initializer_list>
template<uint32_t nOutput, uint32_t nInput>
class BinConv
{
private:
    uint32_t num[nOutput][nInput] = {0};
    uint32_t den[nOutput][nInput] = {0};
    uint32_t currstate[nOutput][nInput] = {0};
    uint32_t prevstate[nOutput][nInput] = {0};
    uint32_t currVal[nOutput] = {0};
    uint32_t dendeg[nOutput][nInput] = {0};
    uint32_t numdeg[nOutput][nInput] = {0};
    uint32_t denmsk[nOutput][nInput] = {0};
    uint32_t static getdeg(uint32_t val)
    {
        return (31 - __builtin_clz(val));
    }
    constexpr void initnum(std::initializer_list<std::initializer_list<int32_t>> inputnum)
    {
        uint32_t iRow{0}, iCol{0};
        for(auto row : inputnum)
        {
            for(auto val : row)
            {
                num[iRow][iCol] = val;
                if(val > 0)
                {
                    numdeg[iRow][iCol] = getdeg(val);
                }
                iCol++;
            }
            iCol=0;
            iRow++;
        }
    }
    constexpr void initden(std::initializer_list<std::initializer_list<int32_t>> inputden)
    {
        uint32_t iRow{0}, iCol{0};
        for(auto row : inputden)
        {
            for(auto val : row)
            {
                den[iRow][iCol] = val;
                if(val > 0)
                {
                    dendeg[iRow][iCol] = (31 - __builtin_clz(val));
                    denmsk[iRow][iCol] = ((1 << dendeg[iRow][iCol]) - 1);
                }
                iCol++;
            }
            iCol = 0;
            iRow++;
        }
    }
public:
    BinConv(std::initializer_list<std::initializer_list<int32_t>> inputnum, std::initializer_list<std::initializer_list<int32_t>> inputden)
    {
        initnum(inputnum);
        initden(inputden);
    }
    BinConv(std::initializer_list<std::initializer_list<int32_t>> inputnum)
    {
        initnum(inputnum);
    }
    void propagate(uint32_t data[nInput], uint32_t output[nOutput])
    {
        uint32_t overflow;
        for(int iOut = 0; iOut < nOutput; iOut++)
        {
            uint32_t currout{0};
            for(int iData = 0; iData < nInput; iData++)
            {
                uint32_t currdata{0};
                int nShift = numdeg[iOut][iData] + getdeg(data[iData]) + 1; //double check this
                for(int iShift = 0; iShift < nShift; iShift++)
                {
                    overflow = denmsk[iOut][iData]&currstate[iOut][iData];
                    currstate[iOut][iData] = (currstate[iOut][iData] << 1)^(overflow == 0?0:den[iOut][iData]);
                    currdata |= (__builtin_parity((currstate[iOut][iData])&num[iOut][iData]) >> iShift);
                }
                currout^=currdata;
            }
            output[iOut] = currout;
        }
    }

};
#include<iostream>
int main()
{
   // auto temp = BinConv<2,3>{{{0b111, 0b100, 0b011},{0b10,0b01,0b00}}};

    std::cout<<"pop count: " << __builtin_popcount(4) << std::endl;
    std::cout<<"hello world"<<std::endl;
    return 0;
}