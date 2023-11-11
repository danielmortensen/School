//
// Created by dmortensen on 11/9/23.
//
#include<iostream>
#include"GF2.h"
#include"ModAr.h"
#include"polynomialT.h"
#include"polynomialT.cc"

template <class T>
polynomialT<T> berlekampmassy(const T* s, int n)
{
    int L = 0;
    polynomialT<T> c{1};
    polynomialT<T> p{1};
    polynomialT<T> x{1};

    int l{1};
    T dm{1};
    for(int k = 0; k < n; k++)
    {
        T d = s[k];
        for(int j = 1; j <= c.getdegree(); j++)
        {
            d += c[j]*s[k - j];
        }

        if(d == 0)  l++;
        else
        {
            if (2*L >= k + 1)
            {
                c = c - (x << l)*p*(d/dm);
                l++;
            }
            else
            {
                auto t{c};
                c = c - (x << l)*p*(d/dm);
                L = k - L + 1;
                p = t;
                dm = d;
                l = 1;
            }
        }
    }
    return c;
}

template <class T>
void berlekampmassy2(const T* s, int n, T* c, int& L)
{

    //preallocate placeholders and temporary variables
    auto out = c;
    T* p = new T[n]();
    T* t = new T[n]();
    T* temp;

    // initialize memory
    c[0] = 1;
    p[0] = 1;
    L = 0;
    for(int i = 1; i < n; i ++)
    {
        c[i] = 0;
    }

    //initialize other variables
    int l{1};
    T dm{1};

    // loop over each input sequence character and update
    for(int k = 0; k < n; k++)
    {

        // compute discriminant
        T d = s[k];
        for(int j = 1; j <= L; j++)
        {
            d += c[j]*s[k - j];
        }

        // update LFSR polynomial
        if(d == 0)  l++;
        else
        {
            auto ddm = (d/dm);

            if (2*L >= k + 1)
            {
                // update when no size change is needed
                for(int j = l; j <= L; j++)
                {
                    c[j] -= p[j - l]*ddm;
                }
                l++;
            }
            else
            {
                // update when the length of the LFSR must increase
                for(int j = 0; j <= L + l ; j++)
                {
                    t[j] = (j < l)?c[j]:(c[j] - p[j - l]*ddm);
                }
                temp = p;
                p = c;
                c = t;
                t = temp;
                L = k - L + 1;
                dm = d;
                l = 1;
            }
        }
    }

    // package data for export
    L++;
    for(int i = 0; i < L; i ++)
    {
        out[i] = c[i];
    }
}
int main()
{
    // Preliminary exercise 1 for berlekampmassey method
    {
       ModAr<5> s[] = {2, 3, 4, 2, 2, 3};
       int nElement = 6;
       auto output = berlekampmassy(s,nElement);
       std::cout<< "Preliminary Exercise 1, berlekampmassey: " << output << std::endl;
    }

    // Preliminary Exercise 2 for berlekampmassey method
    {
        GF2<0b11> s[] = {1, 1, 1, 0, 1, 0, 0};
        int nElement = 7;
        auto output = berlekampmassy(s, nElement);
        std::cout<<"Preliminary Exercise 2, berlekampmassey: " << output << std::endl;
    }

    // Preliminary Exercise 3 for berlekampmassey method
    {
        GF2<0b10011>::set(viewmode,power);
        GF2<0b10011> s[] = {0, {3,exponent}, {4,exponent}, {7,exponent}};
        int nElement = 4;
        auto output = berlekampmassy(s,nElement);
        std::cout<<"Preliminary Exercise 3, berlekampmassey: " << output << std::endl;
    }

    // Preliminary exercise 1 for berlekampmassey2 method
    {
        ModAr<5> s[] = {2, 3, 4, 2, 2, 3};
        int nElement = 6;
        auto c = new ModAr<5>[nElement]();
        int L{0};
        berlekampmassy2(s, nElement, c, L);
        std::cout<< "Preliminary Exercise 1, berlekampmassey2: ";
        for(int i = 0; i < L; i++)
        {
            std::cout << c[i] << " ";
        }
        std::cout<<std::endl;
    }

    // Preliminary Exercise 2 for berlekampmassey method
    {
        GF2<0b11> s[] = {1, 1, 1, 0, 1, 0, 0};
        int nElement = 7;
        auto c = new GF2<0b11>[nElement]();
        int L{0};
        berlekampmassy2(s, nElement, c, L);
        std::cout<<"Preliminary Exercise 2, berlekampmassey2: ";
        for(int i = 0; i < L; i++)
        {
            std::cout << c[i] << " ";
        }
        std::cout<<std::endl;
    }

    // Preliminary Exercise 3 for berlekampmassey method
    {
        GF2<0b10011>::set(viewmode,power);
        GF2<0b10011> s[] = {0, {3,exponent}, {4,exponent}, {7,exponent}};
        int nElement = 4;
        auto c = new GF2<0b10011>[nElement]();
        int L{0};
        berlekampmassy2(s,nElement, c, L);
        std::cout<<"Preliminary Exercise 3, berlekampmassey2: ";
        for(int i = 0; i < L; i++)
        {
            std::cout << c[i] << " ";
        }
        std::cout<<std::endl;
    }

}