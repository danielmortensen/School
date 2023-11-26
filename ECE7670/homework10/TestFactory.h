//
// Created by daniel on 11/26/23.
//

#ifndef HOMEWORK10_TESTFACTORY_H
#define HOMEWORK10_TESTFACTORY_H
#include<vector>
#include<string>
#include"ConvBinEncoder.h"
#include<random>
#include<type_traits>
enum GeneratorType{Polynomial, Rational};
enum ChannelType{BinarySymmetric, AdditiveWhiteGaussian};
template<GeneratorType Gen>
class TestFactory
{
private:
    template<ChannelType Chan>
    static constexpr double defaultErrorRate()
    {
        switch(Chan)
        {
            case BinarySymmetric: { return 0.2;}
            case AdditiveWhiteGaussian: { return 1;}
        }
    }

public:
    static constexpr uint32_t nInput()
    {
        switch(Gen)
        {
            case Polynomial: return 1;
            case Rational: return 2;
        }
    }
    static constexpr uint32_t nOutput()
    {
        switch(Gen)
        {
            case Polynomial: return 2;
            case Rational: return 3;
        }
    }
    static constexpr uint32_t nMessage()
    {
        switch(Gen)
        {
            case Polynomial: return 7;
            case Rational: return 8;
        }
    }
    ConvBinEncoder<nInput(),nOutput()> getEncoder()
    {
        switch(Gen)
        {
            case Polynomial:
            {
                auto numrow1 = {0b101, 0b111};
                auto num = {numrow1};
                auto encoder = ConvBinEncoder<nInput(), nOutput()>{num};
                return encoder;
            }
            case Rational:
            {
                auto numrow1 = {0b0001, 0b0000, 0b0010};
                auto numrow2 = {0b0000, 0b0001, 0b0100};
                auto denrow1 = {0b0000, 0b0000, 0b1001};
                auto denrow2 = {0b0000, 0b0000, 0b1001};
                auto num = {numrow1, numrow2};
                auto den = {denrow1, denrow2};
                auto encoder = ConvBinEncoder < nInput(), nOutput()>{num, den };
                return encoder;
            }
        }
    }

    std::vector<std::vector<uint32_t>> getMessageStream()
    {
        switch(Gen)
        {
            case Rational:
            {
                return {{1,0}, {0,0}, {1,1}, {0,0},
                        {1,0}, {1,1}, {0,1}, {1,1}};
            }
            case Polynomial:
            {
                return{{1}, {1}, {0}, {0}, {1}, {0}, {1}};
            }
        }
    }

    template<class T>
    std::vector<std::vector<T>> getCodewordStream()
    {
        switch(Gen)
        {
            case Rational:
            {
                return {{1, 0, 0}, {0, 0, 1}, {1, 1, 0}, {0, 0, 1},
                        {1, 0, 0}, {1, 1, 1}, {0, 1, 0}, {1, 1, 1}};
            }
            case Polynomial:
            {
                return {{1, 1}, {1,0}, {1,0}, {1, 1}, {1, 1},
                        {0,1}, {0, 0}, {0,1}, {1, 1}};
            }
        }
    }

    template<ChannelType Chan, class T>
    std::vector<T> getReceivedStream(double errorRate=defaultErrorRate<Chan>())
    {
        switch(Chan)
        {
            case BinarySymmetric:
            {
                std::default_random_engine gen;
                std::uniform_real_distribution<double> dist(0.0,1.0);
                auto code = getCodewordStream<uint32_t>();
                for(auto& e : code)
                {
                    for(auto& val : e)
                    {
                        if(dist(gen) < errorRate) val^=1;
                    }
                }
                return code;
            }
            case AdditiveWhiteGaussian:
            {
                std::default_random_engine gen;
                std::normal_distribution<double> distribution(0,errorRate);
                auto code = getCodewordStream<double>();
                for(auto& e : code)
                {
                    for(auto& val : e)
                    {
                        val += distribution(gen);
                    }
                }
                return code;
            }
        }
    }
};
#endif //HOMEWORK10_TESTFACTORY_H
