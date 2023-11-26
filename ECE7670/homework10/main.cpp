//
// Created by dmortensen on 11/21/23.
//
#include<iostream>
#include<cstdint>
#include<initializer_list>
#include<memory>
#include<vector>
#include<queue>
#include<map>
#include<cassert>

template<uint32_t nInput, uint32_t nOutput>
class BinConv
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
    BinConv(std::initializer_list<std::initializer_list<int32_t>> inputnum, std::initializer_list<std::initializer_list<int32_t>> inputden)
    {
        initnum(inputnum);
        initden(inputden);
        computeNumTotalState();
    }
    BinConv(std::initializer_list<std::initializer_list<int32_t>> inputnum)
    {
        std::cerr << "numerator only constructor untested" << std::endl;
        exit(-1);
        std::cout<<"line 73"<<std::endl;
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
                //    std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                    doOverflow = (((denmsk[iIn][iOut] >> 1)&currstate[iIn][iOut]) != 0);
                //    std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                    if(doOverflow) currstate[iIn][iOut] ^= den[iIn][iOut];
                //     std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                    currstate[iIn][iOut] <<=1;
                //     std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                    currstate[iIn][iOut] |= currdata;
                //     std::cout<<"currstate: " << currstate[iIn][iOut] <<std::endl;
                    currstate[iIn][iOut] &= (denmsk[iIn][iOut] == 0)?0:(denmsk[iIn][iOut] - 1);
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
template<uint32_t nInput, uint32_t nOutput>
class node
{
public:
    double pathlength{0};
    std::shared_ptr<node<nInput,nOutput>> parent;
    uint32_t state{0};
    uint32_t iGeneration{0};
    uint32_t output{0};
    uint32_t input{0};
    uint32_t id{0};
    bool isTop;
    bool doRemoval{false};

    node() : isTop(true){};
    node(std::shared_ptr<node<nInput,nOutput>> parent, uint32_t s, uint32_t iGen, double len, uint32_t output, uint32_t input, uint32_t key) :
    parent{parent}, state{s}, iGeneration{iGen}, pathlength{len}, output{output}, input{input}, id{key}, isTop{false}{};
};
int main()
{
/********************************************************************
 *                     Encoding Portion
 ********************************************************************/
    // define general parameters
    const uint32_t nOutput{3};
    const uint32_t nInput{2};
    const uint32_t nData{16};//insert number of bits here.
    uint32_t data{0b1110110100110001};

    // initialize encoder
    auto numrow1= {0b0001, 0b0000, 0b0010};
    auto numrow2= {0b0000, 0b0001, 0b0100};
    auto denrow1= {0b0000, 0b0000, 0b1001};
    auto denrow2= {0b0000, 0b0000, 0b1001};
    auto num = {numrow1, numrow2};
    auto den = {denrow1, denrow2};
    auto encoder = BinConv<nInput,nOutput>{num,den};

    // loop parameters
    uint32_t codeword{0};
    uint32_t currdat{0};
    uint32_t mask{((1 << nInput) - 1)};
    for(int i = 0; i < nData; i+=nInput)
    {
        currdat = (data >> i)&mask;
        encoder.propagate(currdat);
        std::cout<<"i: " << i << " Input: " << currdat << " Output: " << encoder.getoutput() << " state: " << encoder.getstate() << std::endl;
        codeword |= (encoder.getoutput() << ((i / nInput) * nOutput));
    }
    uint32_t expected{0b00000000111010111001100011100001};
    assert(expected == codeword);

/********************************************************************
 *                     Send Through Channel
 ********************************************************************/
 uint32_t nReceived = nData/nInput*nOutput;
 double received[nData/nInput*nOutput];
 for(int i = 0; i < nReceived; i++)
 {
     received[i] = (codeword >> i)%2;
     std::cout<<received[i]<<" ";
 }
 std::cout<<std::endl;
 auto hamming = [&](const uint32_t& given, const uint32_t idx){
     double diff{0};
     for(uint32_t i = idx; i < idx + nOutput; i++)
     {
         auto dat = double((given >> (i - idx))%2);
         diff += (dat > received[i])?(dat - received[i]):(received[i] - dat);
     }
     return diff;
 };

/********************************************************************
 *                     Decoding Portion
 ********************************************************************/
    const uint32_t nState{encoder.getNumAllState()};
    std::queue<std::shared_ptr<node<nInput,nOutput>>> nodequeue;
    nodequeue.emplace(std::make_shared<node<nInput,nOutput>>());
    std::vector<std::shared_ptr<node<nInput,nOutput>>> final;
    std::map<uint32_t,std::shared_ptr<node<nInput,nOutput>>>inQueue;
    uint32_t nPoint{nData/nInput};
    uint32_t iPoint{0};
    while(!nodequeue.empty())
    {
        //get next element from f
        auto curr = nodequeue.front();
        nodequeue.pop();
        if(curr->doRemoval) continue;
        if(curr->iGeneration == nPoint - 1) {final.push_back(curr); continue;}
        inQueue.erase(curr->id);
        if(curr->id == 92159)
        {
            std::cout<<"10239! " <<std::endl;
        }

        //create child for every possible input
        auto sParent = curr->state;
        auto iParent = curr->iGeneration;
        for(uint32_t iInput = 0; iInput < (1 << nInput); iInput++)
        {
            // propagate encoder with given input
            encoder.setstate(curr->state);
            encoder.propagate(iInput);
            auto currstate = encoder.getstate();
            auto curroutput = encoder.getoutput();
            auto key = (iInput | (currstate << nInput)) | (iParent << (nState + nInput));

            //compute difference between current codeword and given codeword
            auto diff = hamming(curroutput, iParent*nOutput);
            auto pathlen = diff + curr->pathlength;
            if((currstate == 552) & (iParent == 4))
            {
                std::cout<<std::endl<<"552 " << "iGen: " << iParent <<std::endl<<std::endl;
            }
            else if(iParent == 4)
            {
                std::cout<<currstate <<" " << "iGen: " << iParent <<std::endl;
            }
            //if(pathlen == 0)
            //{
            //    std::cout<<"currstate: " << currstate << " curroutput: " << curroutput << " iGen: " << iParent <<std::endl;
            //    std::cout<<std::endl;
            //}
            // create child with: current state and path length
            if(inQueue.contains(key))
            {
                auto temp = inQueue[key];
                if((inQueue[key]->pathlength > pathlen))
                {

                    inQueue[key]->doRemoval = true;
                    std::cout<<inQueue[key]->iGeneration<<std::endl;

                    //change inqueue value to current value
                    auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, diff,curroutput, iInput,key);
                    inQueue[key] = child;
                    nodequeue.push(child);
                }
            }
            else if(!inQueue.contains(key))
            {
                //change inqueue value to current value
                auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, diff, curroutput, iInput,key);
                inQueue[key] = child;
                nodequeue.push(child);
            }
        }

    }

    // get best terminating path
    std::shared_ptr<node<nInput,nOutput>> best = final[0];
    double dist = final[0]->pathlength;
    for(auto e : final)
    {
        if(e->pathlength < dist)
        {
            best = e;
            dist = e->pathlength;
        }
    }

    // traverse the best path to reconstruct the message
//    for(auto e : final)
//    {
//        auto temp = e;
//        auto len = e->pathlength;
//        uint32_t message{0};
//        while(!temp->isTop)
//        {
//            message <<= nInput;
//            message ^= temp->input;
//            temp = temp->parent;
//        }
//        std::cout<<" decoded message: " << message<<" path length: " << len << std::endl;
//    }
//
//    std::cout<<" original message: " << data << std::endl;
    return 0;
}