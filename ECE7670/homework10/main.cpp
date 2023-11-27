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
#include"encoder_test.h"
#include<functional>

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
template<uint32_t nInput, uint32_t nOutput>
void printpath(std::shared_ptr<node<nInput,nOutput>> p)
{
    auto curr = p;
    while(!curr->isTop)
    {
        std::cout<<"id: " << curr->id << " input: " << curr->input<< " idx: " << curr->iGeneration << " length: " << curr->pathlength << std::endl;
        curr = curr->parent;
    }
}
template<uint32_t nInput, uint32_t nOutput>
std::vector<std::vector<uint32_t>> viturbi(ConvBinEncoder<nInput,nOutput> Encoder, const std::vector<std::vector<double>>& received, std::function<double(const std::vector<uint32_t>&, const std::vector<std::vector<double>>&, const uint32_t& idx)> metric)
{
    const uint32_t nState{Encoder.getNumAllState()};
    const uint32_t nInputs{(1 << nInput)};
    std::queue<std::shared_ptr<node<nInput,nOutput>>> nodequeue;
    nodequeue.emplace(std::make_shared<node<nInput,nOutput>>());
    std::vector<std::shared_ptr<node<nInput,nOutput>>> final;
    std::map<uint32_t,std::shared_ptr<node<nInput,nOutput>>>inQueue;
    const uint32_t nPoint{uint32_t(received.size())};
    while(!nodequeue.empty())
    {
        //get next element from f
        auto curr = nodequeue.front();
        nodequeue.pop();
        if(curr->doRemoval) continue;
        inQueue.erase(curr->id);
        if(curr->iGeneration == nPoint) { final.push_back(curr); continue;}

        //create child for every possible input
        auto iParent = curr->iGeneration;
        for(uint32_t iInput = 0; iInput < nInputs; iInput++)
        {
            // propagate encoder with given input
            Encoder.setstate(curr->state);
            auto curroutput = Encoder.template tovec<uint32_t>(Encoder.getoutput(iInput),nOutput);
            Encoder.propagate(iInput);
            auto currstate = Encoder.getstate();
            auto key = (currstate  | ((iParent + 1) << nState));

            //compute difference between current received and given received
            auto diff = metric(curroutput, received, iParent);
            auto pathlen = diff + curr->pathlength;

            // create child with: current state and path length
            if(inQueue.contains(key))
            {
                auto temp = inQueue[key];
                if((inQueue[key]->pathlength > pathlen))
                {

                    inQueue[key]->doRemoval = true;

                    //change inqueue value to current value
                    auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, pathlen,Encoder.toint(curroutput), iInput,key);
                    inQueue[key] = child;
                    nodequeue.push(child);
                }
            }
            else if(!inQueue.contains(key))
            {
                //change inqueue value to current value
                auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, pathlen, Encoder.toint(curroutput), iInput,key);
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
    std::vector<std::vector<uint32_t>> decoded;
    auto temp = best;
    while(!temp->isTop)
    {
        std::vector<uint32_t> m;
        for(int iInput = 0; iInput < nInput; iInput++)
        {
            auto val = (temp->input >> iInput)&1;
            m.push_back(val);
        }
        decoded.push_back(m);
        temp = temp->parent;
    }
    std::reverse(decoded.begin(),decoded.end());
    return decoded;
}
double hamming(const std::vector<uint32_t>& given, const std::vector<std::vector<double>>& codeword, const uint32_t& idx){
    double diff{0};
    for(int i = 0; i < given.size(); i++)
    {
        double dist1 = std::abs(codeword[idx][i] - 1);
        double dist2 = std::abs(codeword[idx][i]);
        uint32_t val = (dist1 < dist2)?1:0;
        diff += given[i]^val;
    }
    return diff;
};

template<GeneratorType Gen>
void testWithoutError()
{
    auto Factory = TestFactory<Gen>();
    auto Encoder = Factory.getEncoder();
    auto message = Factory.getMessageStream();
    auto codeword = Factory.template getCodewordStream<double>();

    // define hamming metric
    auto decoded = viturbi(Encoder,codeword,&hamming);
    for(int i = 0; i < message.size(); i++)
    {
        for(int j = 0; j < message[0].size(); j++)
        {
            assert(message[i][j] == decoded[i][j]);
        }
    }
}
template<GeneratorType Gen>
void testWith1Error()
{
    auto Factory = TestFactory<Gen>();
    auto Encoder = Factory.getEncoder();
    auto message = Factory.getMessageStream();
    auto codeword = Factory.template getCodewordStream<double>();

    for(int ii = 0; ii < codeword.size(); ii++)
    {
        for(int jj = 0; jj < codeword[ii].size(); jj++)
        {
           auto received = codeword;

            // insert error
            received[ii][jj] = received[ii][jj] == 0?1:0;

            //decode and validate
            auto decoded = viturbi(Encoder,codeword,&hamming);
            for(int i = 0; i < message.size(); i++)
            {
                for(int j = 0; j < message[0].size(); j++)
                {
                    assert(message[i][j] == decoded[i][j]);
                }
            }
        }
    }


}
int main()
{
    //unit test the encoder
    encoder_test();

    //unit test the decoder
    testWithoutError<Polynomial>();
    testWithoutError<Rational>();
    testWith1Error<Polynomial>();
    testWith1Error<Rational>();

    return 0;
}