
//
// Created by daniel on 11/26/23.
//
#ifndef ENCODER_TEST_H
#define ENCODER_TEST_H
#include <cstdint>
#include<cassert>
#include"ConvBinEncoder.h"
#include"TestFactory.h"

template<GeneratorType Gen>
int doTest()
{
    // initialize test parameters
    auto Factory = TestFactory<Gen>();
    const auto message = Factory.getMessageStream();
    auto encoder = Factory.getEncoder();


    // loop parameters
    const auto truecodeword = Factory.template getCodewordStream<uint32_t>();
    for (int iMessage = 0; iMessage < message.size(); iMessage++){
        auto m = TestFactory<Rational>::toint(message[iMessage]);
        encoder.propagate(m);
        auto c0 = encoder.getoutput();
        auto c1 = TestFactory<Rational>::toint(truecodeword[iMessage]);
        assert(c0 == c1);
    }

}

int encoder_test() {
    doTest<Polynomial>();
    doTest<Rational>();
    return 0;
}

#endif