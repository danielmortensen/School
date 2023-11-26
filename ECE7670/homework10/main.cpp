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
/********************************************************************
 *                     Encoding Portion
 ********************************************************************/
encoder_test();
///********************************************************************
// *                     Send Through Channel
// ********************************************************************/
// uint32_t nReceived = nData/nInput*nOutput;
// double received[nData/nInput*nOutput];
// for(int i = 0; i < nReceived; i++)
// {
//     received[i] = (codeword >> i)%2;
//     std::cout<<received[i]<<" ";
// }
// std::cout<<std::endl;
// auto hamming = [&](const uint32_t& given, const uint32_t idx){
//     double diff{0};
//     for(uint32_t i = idx; i < idx + nOutput; i++)
//     {
//         auto dat = double((given >> (i - idx))%2);
//         diff += (dat > received[i])?(dat - received[i]):(received[i] - dat);
//     }
//     return diff;
// };
//
///********************************************************************
// *                     Decoding Portion
// ********************************************************************/
//    const uint32_t nState{encoder.getNumAllState()};
//    std::queue<std::shared_ptr<node<nInput,nOutput>>> nodequeue;
//    nodequeue.emplace(std::make_shared<node<nInput,nOutput>>());
//    std::vector<std::shared_ptr<node<nInput,nOutput>>> final;
//    std::map<uint32_t,std::shared_ptr<node<nInput,nOutput>>>inQueue;
//    uint32_t nPoint{nData/nInput};
//    uint32_t iPoint{0};
//    while(!nodequeue.empty())
//    {
//        //get next element from f
//        auto curr = nodequeue.front();
//        nodequeue.pop();
//        if(curr->doRemoval) continue;
//        if(curr->iGeneration == nPoint - 1) {final.push_back(curr); continue;}
//        inQueue.erase(curr->id);
//        if(curr->id == 92159)
//        {
//            std::cout<<"10239! " <<std::endl;
//        }
//
//        //create child for every possible input
//        auto sParent = curr->state;
//        auto iParent = curr->iGeneration;
//        for(uint32_t iInput = 0; iInput < (1 << nInput); iInput++)
//        {
//            // propagate encoder with given input
//            encoder.setstate(curr->state);
//            encoder.propagate(iInput);
//            auto currstate = encoder.getstate();
//            auto curroutput = encoder.getoutput();
//            auto key = (iInput | (currstate << nInput)) | (iParent << (nState + nInput));
//
//            //compute difference between current codeword and given codeword
//            auto diff = hamming(curroutput, iParent*nOutput);
//            auto pathlen = diff + curr->pathlength;
//            if((currstate == 552) & (iParent == 4))
//            {
//                std::cout<<std::endl<<"552 " << "iGen: " << iParent <<std::endl<<std::endl;
//            }
//            else if(iParent == 4)
//            {
//                std::cout<<currstate <<" " << "iGen: " << iParent <<std::endl;
//            }
//            //if(pathlen == 0)
//            //{
//            //    std::cout<<"currstate: " << currstate << " curroutput: " << curroutput << " iGen: " << iParent <<std::endl;
//            //    std::cout<<std::endl;
//            //}
//            // create child with: current state and path length
//            if(inQueue.contains(key))
//            {
//                auto temp = inQueue[key];
//                if((inQueue[key]->pathlength > pathlen))
//                {
//
//                    inQueue[key]->doRemoval = true;
//                    std::cout<<inQueue[key]->iGeneration<<std::endl;
//
//                    //change inqueue value to current value
//                    auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, diff,curroutput, iInput,key);
//                    inQueue[key] = child;
//                    nodequeue.push(child);
//                }
//            }
//            else if(!inQueue.contains(key))
//            {
//                //change inqueue value to current value
//                auto child = std::make_shared<node<nInput,nOutput>>(curr,currstate, iParent+1, diff, curroutput, iInput,key);
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
////    for(auto e : final)
////    {
////        auto temp = e;
////        auto len = e->pathlength;
////        uint32_t message{0};
////        while(!temp->isTop)
////        {
////            message <<= nInput;
////            message ^= temp->input;
////            temp = temp->parent;
////        }
////        std::cout<<" decoded message: " << message<<" path length: " << len << std::endl;
////    }
////
////    std::cout<<" original message: " << data << std::endl;
    return 0;
}