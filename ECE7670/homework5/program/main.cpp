//
// Created by dmortensen on 10/15/23.
//
#include "ModArnew.h"
#include "polynomialT.h"
#include "polynomialT.cc"
#include<iostream>
#include<functional>

template <class T, class Func>
void xgcd(const T &a, const T &b, T &g, T &s, T &t, Func isDone)
{
    T ri0{0};
    T ri1{b};
    T ri2{a};
    T si0{0};
    T si1{0};
    T si2{1};
    T ti0{0};
    T ti1{1};
    T ti2{0};
    T qi0{0};
    T stop{0};
    do{
       qi0 = ri2/ri1;
       ri0 = ri2 - qi0*ri1;
       si0 = si2 - qi0*si1;
       ti0 = ti2 - qi0*ti1;

       // update values
       ri2 = ri1; ri1 = ri0;
       ti2 = ti1; ti1 = ti0;
       si2 = si1; si1 = si0;
    }while(!isDone(ri0));
    g = ri2; s = si2; t = ti2;
}

template<class T>
void sugiyama(const int nInput, const T input[], polynomialT<T>& g, polynomialT<T>& s, polynomialT<T>& t)
{
    // instantiate our x^n polynomial
    auto coefA = new T[nInput + 1]{};
    coefA[nInput] = T{1};
    polynomialT<T> a(nInput, coefA);

    // instantiate our data polynomial
    polynomialT<T> b(nInput - 1, input);

    // compute the extended Euclidean algorithm
    auto temp = [&](polynomialT<T> in) ->bool{return in.degree == 0;};
    xgcd(a,b,g,s,t,temp);

    g/=t[0];
    s/=t[0];
    t/=t[0];
}

int main()
{
    // Definitions for Integer Euclidean Algorithm
    int g,s,t;
    auto stopCriteriaInt = [](int in) -> bool{return in == 0;};

    // Compute (24,18)
    xgcd(24,18,g,s,t,stopCriteriaInt);
    std::cout<<"a: "<<24<<std::endl;
    std::cout<<" b: "<<18<<std::endl;
    std::cout<<" g: "<<g<<std::endl;
    std::cout<<" s: "<<s<<std::endl;
    std::cout<<" t: "<<t<<std::endl<<std::endl;

    // Compute (851,966)
    xgcd(851,966,g,s,t,stopCriteriaInt);
    std::cout<<"a: "<<24<<std::endl;
    std::cout<<" b: "<<18<<std::endl;
    std::cout<<" g: "<<g<<std::endl;
    std::cout<<" s: "<<s<<std::endl;
    std::cout<<" t: "<<t<<std::endl<<std::endl;

    // Definitions for Z5[X]
    polynomialT<ModAr<5>> g1, s1, t1;
    auto stopCriteriaPZ5 = [](polynomialT<ModAr<5>> in) -> bool{return in.degree == 0;};

    // compute (3x^7 + 4x^6 + 3x^4 + x^3 + 1, 4x^4 + x^3 + x)
    {
        ModAr<5> coefA[] = {1, 0, 0, 1, 3, 0, 4, 3};
        polynomialT<ModAr<5>> a(7, coefA);
        ModAr<5> coefB[] = {0, 1, 0, 1, 4};
        polynomialT<ModAr<5>> b(4, coefB);
        xgcd(a, b, g1, s1, t1, stopCriteriaPZ5);
        std::cout<<"first"<<std::endl;
        std::cout<<"a: "<<a<<std::endl;
        std::cout<<" b: "<<b<<std::endl;
        std::cout<<" g: "<<g1<<std::endl;
        std::cout<<" s: "<<s1<<std::endl;
        std::cout<<" t: "<<t1<<std::endl<<std::endl;
    }

     // Compute (2x^5 + 3x^4 + 4x^3 + 3x^2 + 2x + 1, x^4 + 2x^3 + 3x^2 + 4x + 3)
    {
        ModAr<5> coefA[] = {1, 2, 3, 4, 3, 2};
        polynomialT<ModAr<5>> a(5, coefA);
        ModAr<5> coefB[] = {3, 4, 3, 2, 1};
        polynomialT<ModAr<5>> b(4, coefB);
        xgcd(a, b, g1, s1, t1, stopCriteriaPZ5);
        std::cout<<"second"<<std::endl;
        std::cout<<"a: "<<a<<std::endl;
        std::cout<<" b: "<<b<<std::endl;
        std::cout<<" g: "<<g1<<std::endl;
        std::cout<<" s: "<<s1<<std::endl;
        std::cout<<" t: "<<t1<<std::endl<<std::endl;
    }

    // Compute (2 + 8x + 10x^2 + 4x^3, 1 + 7x + 14x^2 + 8x^3)
    {
        polynomialT<double> g2, s2, t2;
        auto stopCriteriaPR = [](polynomialT<double> in) -> bool
                {
                    bool done = true;
                    for(int i = 1; i < in.degree; i++)
                    {
                        done&= in[i] < 1e-50;
                    }
                    return done;
                };
        double coefA[] = {2, 8, 10, 4};
        polynomialT<double> a(3, coefA);
        double coefB[] = {1, 7, 14, 8};
        polynomialT<double> b(3, coefB);
        xgcd(a, b, g2, s2, t2, stopCriteriaPR);
        std::cout<<"third"<<std::endl;
        std::cout<<"a: "<<a<<std::endl;
        std::cout<<" b: "<<b<<std::endl;
        std::cout<<" g: "<<g1<<std::endl;
        std::cout<<" s: "<<s1<<std::endl;
        std::cout<<" t: "<<t1<<std::endl<<std::endl;
    }

    // Compute t(x) using the Sugiyama algorithm
    {
        ModAr<5> data[] = {3, 2, 3, 1, 4, 0, 4, 3};
        sugiyama(8, data, g1, s1, t1);
        std::cout<<"fourth"<<std::endl;
        std::cout<<" g: "<<g1<<std::endl;
        std::cout<<" s: "<<s1<<std::endl;
        std::cout<<" t: "<<t1<<std::endl<<std::endl;
    }

    // Compute t(x) using the Sugiyama algorithm
    {
        ModAr<5> data[] = {3, 2, 1, 0, 4, 3, 2, 1};
        sugiyama(8, data, g1, s1, t1);
        std::cout<<"fifth"<<std::endl;
        std::cout<<" g: "<<g1<<std::endl;
        std::cout<<" s: "<<s1<<std::endl;
        std::cout<<" t: "<<t1<<std::endl<<std::endl;
    }

    // Compute values for Preliminary Exercises
    {
        ModAr<5> coefA[] = {1,2,3,4,3,2};
        polynomialT<ModAr<5>> a(5, coefA);
        ModAr<5> coefB[] = {3,4,3,2,1};
        polynomialT<ModAr<5>> b(4, coefB);
        xgcd(a, b, g1, s1, t1, stopCriteriaPZ5);
        std::cout<<"Preliminary 1"<<std::endl;
        std::cout<<"a: "<<a<<std::endl;
        std::cout<<" b: "<<b<<std::endl;
        std::cout<<" g: "<<g1<<std::endl;
        std::cout<<" s: "<<s1<<std::endl;
        std::cout<<" t: "<<t1<<std::endl<<std::endl;
    }
    {
        polynomialT<double> g2, s2, t2;
        auto stopCriteriaPR = [](polynomialT<double> in) -> bool
        {
            bool done = true;
            for(int i = 1; i < in.degree; i++)
            {
                done&= in[i] < 1e-50;
            }
            return done;
        };
        double coefA[] = {4,1,2,1};
        polynomialT<double> a(3, coefA);
        double coefB[] = {4,3,1};
        polynomialT<double> b(2, coefB);
        xgcd(a, b, g2, s2, t2, stopCriteriaPR);
        std::cout<<"Preliminary 2: "<<std::endl;
        std::cout<<"a: "<<a<<std::endl;
        std::cout<<" b: "<<b<<std::endl;
        std::cout<<" g: "<<g1<<std::endl;
        std::cout<<" s: "<<s1<<std::endl;
        std::cout<<" t: "<<t1<<std::endl<<std::endl;
    }

    return 0;
}


