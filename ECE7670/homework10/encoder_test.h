
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
        auto m = encoder.toint(message[iMessage]);
        auto c0 = encoder.getoutput(m);
        encoder.propagate(m);
        auto s = encoder.getstate();
        encoder.setstate(s);
        auto c1 = encoder.toint(truecodeword[iMessage]);
        assert(c0 == c1);
    }

}

int encoder_test() {
    doTest<Polynomial>();
    doTest<Rational>();
    return 0;
}

#endif