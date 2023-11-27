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
    //unit test the encoder
    encoder_test();


//    // initialize parameters for Viturbi Algorithm
//    auto Factory = TestFactory<Polynomial>();
//    auto Encoder = Factory.getEncoder();
//    auto message = Factory.getMessageStream();
//    auto codeword = Factory.getCodewordStream<uint32_t>();
//
//    // define hamming metric
//     auto hamming = [&](const std::vector<uint32_t> given, const uint32_t idx){
//         double diff{0};
//         for(int i = 0; i < given.size(); i++)
//         {
//             diff += given[i]^codeword[idx][i];
//         }
//         return diff;
//     };
//
///********************************************************************
// *                     Decoding Portion
// ********************************************************************/
//    const uint32_t nState{Encoder.getNumAllState()};
//    const uint32_t nInput{TestFactory<Polynomial>::nInput()};
//    const uint32_t nInputs{(1 << nInput)};
//    const uint32_t nOutput{TestFactory<Polynomial>::nOutput()};
//    std::queue<std::shared_ptr<node<nInput,nOutput>>> nodequeue;
//    nodequeue.emplace(std::make_shared<node<nInput,nOutput>>());
//    std::vector<std::shared_ptr<node<nInput,nOutput>>> final;
//    std::map<uint32_t,std::shared_ptr<node<nInput,nOutput>>>inQueue;
//    const uint32_t nPoint{uint32_t(codeword.size()/nInput)};
//    while(!nodequeue.empty())
//    {
//        //get next element from f
//        auto curr = nodequeue.front();
//        std::cout<<"popped node: " << std::endl;
//        std::cout<<"Index: " << curr->iGeneration << std::endl;
//        std::cout<<"State: " << curr->state << std::endl;
//        std::cout<<"doRemoval: " << curr->doRemoval << std::endl;
//        std::cout<<"Input: " << curr->input << std::endl;
//        std::cout<<"Output: " << curr->output << std::endl;
//        std::cout<<"Path Length: " << curr->pathlength << std::endl;
//        std::cout<<"Id: " << curr->id<<std::endl<<std::endl;
//        nodequeue.pop();
//        if(curr->doRemoval) continue;
//        if(curr->iGeneration == nPoint) {final.push_back(curr); continue;}
//        inQueue.erase(curr->id);
//
//        //create child for every possible input
//        auto sParent = curr->state;
//        auto iParent = curr->iGeneration;
//        for(uint32_t iInput = 0; iInput < nInputs; iInput++)
//        {
//            // propagate encoder with given input
//            Encoder.setstate(curr->state);
//            Encoder.propagate(iInput);
//            auto currstate = Encoder.getstate();
//            auto curroutput = Encoder.tovec<uint32_t>(Encoder.getoutput(),nOutput);
//            auto key = (currstate  | ((iParent + 1) << nState));
//
//            //compute difference between current codeword and given codeword
//            auto diff = hamming(curroutput, iParent);
//            auto pathlen = diff + curr->pathlength;
//
//            // create child with: current state and path length
//            if(inQueue.contains(key))
//            {
//                auto temp = inQueue[key];
//                if((inQueue[key]->pathlength > pathlen))
//                {
//
//                    inQueue[key]->doRemoval = true;
//
//                    //change inqueue value to current value
//                    auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, diff,Encoder.toint(curroutput), iInput,key);
//                    inQueue[key] = child;
//                    nodequeue.push(child);
//                }
//            }
//            else if(!inQueue.contains(key))
//            {
//                //change inqueue value to current value
//                auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, diff, Encoder.toint(curroutput), iInput,key);
//                inQueue[key] = child;
//                nodequeue.push(child);
//            }
//        }
//
//    }
//
//    // get best terminating path
//    std::shared_ptr<node<nInput,nOutput>> best = final[0];
//    double dist = final[0]->pathlength;
//    for(auto e : final)
//    {
//        if(e->pathlength < dist)
//        {
//            best = e;
//            dist = e->pathlength;
//        }
//    }
//
//    // traverse the best path to reconstruct the message
//        std::vector<std::vector<uint32_t>> decoded;
//        auto len = best->pathlength;
//        auto temp = best;
//        while(!temp->isTop)
//        {
//            std::vector<uint32_t> m;
//            for(int iInput = 0; iInput < nInput; iInput++)
//            {
//                auto val = (temp->input >> iInput)&1;
//                 m.push_back(val);
//                 std::cout<<val;
//            }
//            std::cout<<"  ";
//            decoded.push_back(m);
//            temp = temp->parent;
//        }
//        std::cout<<"path length: " << len << std::endl;

    return 0;
}