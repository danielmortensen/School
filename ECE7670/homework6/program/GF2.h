//
// Created by dmortensen on 10/22/23.
//

#ifndef PROGRAM_GF2_H
#define PROGRAM_GF2_H
#define MODPLUS(a) ((a%(nElement-1) + (nElement-1))%(nElement-1))

enum viewmode{vector, power, binary};
enum setoption{viewmode};
enum staticoption{generator};

class GF2
{
private:
    static uint32_t gen;
    static uint32_t genDegree;
    static uint32_t nElement;
    static uint32_t* toPower;
    static uint32_t* toVector;
    static bool isInitialized;

    static uint32_t toDegree(const uint32_t& p)
    {
        uint32_t degree = 0;
        for(int i = 0; i < 32; i++)
        {
            if((p >> i)%2)
            {
                degree = i;
            }
        }
        return degree;
    }
protected:
    enum viewmode viewAs;

public:
    uint32_t value;
    explicit GF2(uint32_t v): viewAs(vector), value(v)
    {
        if(!isInitialized)
        {
            throw std::invalid_argument("Cannot run until initialized");
        }
    }
    GF2(int v): viewAs(vector), value(v)
    {
        if(!isInitialized)
        {
            throw std::invalid_argument("Cannot run until initialized");
        }
    }
    GF2():value{0},viewAs{vector}{}
    GF2(const GF2& input) = default;
    GF2& operator=(const GF2& input) = default;
    GF2& operator=(const int& input)
    {
       value = input;
       viewAs = vector;
       return *this;
    }
    ~GF2() = default;

    template<class T>
    void set(setoption opt, T input)
    {
        switch(opt)
        {
            case viewmode:
                viewAs = input;
            break;
        }
    }

    template<class T>
    static void set(staticoption opt, T input)
    {
        switch(opt)
        {
            case generator:
                gen = input;
                genDegree = toDegree(gen);
                nElement = 1<<genDegree;
                toPower = new uint32_t[nElement];
                toVector = new uint32_t[nElement];

                uint32_t val = 1;
                toVector[0] = val;
                toPower[val] = 0;
                for(int i = 1; i < nElement - 1; i++)
                {
                    val = (val >> (genDegree - 1)) > 0?(val << 1)^gen:(val<<1);
                    toPower[val] = i;
                    toVector[i] = val;
                }
                GF2::isInitialized = true;
        }
    }

    // GF2 -- int operators

    GF2 operator+(const int a) const {return GF2(value^a);}
    GF2 operator-(const int a) const {return GF2(value^a);}
    GF2 operator*(const int a) const {
        auto doZero = (value == 0 | a == 0);
        auto nextVal = doZero ? 0 : toVector[MODPLUS(toPower[value] + toPower[a])];
        return GF2(nextVal);
    }
    GF2 operator/(const int a) const
    {
        if(a == 0)
        {
            throw std::invalid_argument("Cannot divide by zero");
        }
        return GF2(toVector[MODPLUS(toPower[value] - toPower[a])]);
    }
    bool operator==(const int a)const {return a == value;}
    bool operator!=(const int a)const {return a != value;}

    // GF2 -- int assignment operators
    GF2& operator++()
    {
        value = (value + 1)%nElement;
        return *this;
    }
    GF2 operator++(int)
    {
        GF2 temp = *this;
        value = (value + 1)%nElement;
        return temp;
    }
    GF2& operator-(){return *this;}
    GF2& operator+=(const int a) {value^=a; return *this;}
    GF2& operator-=(const int a) {value^=a; return *this;}
    GF2& operator*=(const int a)
    {
        value = (value == 0 | a == 0)?0:toVector[MODPLUS(toPower[value] + toPower[a])];
        return *this;
    }
    GF2& operator/=(const int a)
    {
        if(a == 0)
        {
            throw std::invalid_argument("Cannot divide by zero");
        }
        value = toVector[MODPLUS(toPower[value] - toPower[a])];
        return *this;
    }

    // GF2 -- GF2 assignment operators
    GF2& operator+=(const GF2 &a) { value ^= a.value; return *this;}
    GF2& operator-=(const GF2 &a) { value ^= a.value; return *this;}
    GF2& operator*=(const GF2 &a)
    {
        value = (value == 0|a.value == 0)?0:toVector[MODPLUS(toPower[a.value]+toPower[value])];
        return *this;
    }
    GF2& operator/=(const GF2 &a)
    {
        if(a.value==0)
        {
            throw std::invalid_argument("Cannot divide by zero");
        }
        value = toVector[MODPLUS(toPower[value] - toPower[a.value])];
        return *this;
    }
    GF2& operator^=(const int exp){value = toVector[MODPLUS(toPower[value]*exp)]; return *this;}

    // GF2 -- GF2 operators
    GF2 operator+(const GF2 &a) const {return GF2(a.value^value);}
    GF2 operator-(const GF2 &a) const {return GF2(a.value^value);}
    GF2 operator*(const GF2 &a) const {
            return a.value == 0?GF2{0}:GF2(toVector[MODPLUS(toPower[a.value] + toPower[value])]);
    }
    GF2 operator/(const GF2 &a) const
    {
        if(a.value == 0)
        {
            throw std::invalid_argument("Cannot divide by zero");
        }
        return GF2(toVector[MODPLUS(toPower[value] - toPower[a.value])]);
    }
    GF2 operator^(const int e) const
    {
        return GF2(toVector[MODPLUS(toPower[value]*e)]);
    }
    bool operator==(const GF2 &a) const {return (a.value == value);}
    bool operator!=(const GF2 &a) const {return (a.value != value);}
    friend std::ostream& operator<<(std::ostream &os, const GF2 &a)
    {
        bool isfirst = true;
        switch(a.viewAs)
        {
            case vector:
                if (a.value%2 == 1)
                {
                    os << "1";
                    isfirst = false;
                }
                for(int i = 1; i < genDegree - 1; i++)
                {
                    if((a.value >> i)%2 == 1)
                    {
                        if(!isfirst)
                        {
                            os <<" + ";
                        }
                        os << "A^"<<i;
                        isfirst = false;
                    }
                }
                if((a.value >> (genDegree - 1))%2 == 1)
                {
                    if(!isfirst)
                    {
                        os <<" + ";
                    }
                    os << "A^"<<(genDegree - 1);
                }
                break;
            case power:
                os << "A^" << int(toPower[a.value]);
                break;
            case binary:
               for(int i = 0; i < genDegree; i++)
               {
                   os << (a.value >> i)%2 << " ";
               }
        }
        return os;
    }
};

uint32_t GF2::gen = 0;
uint32_t GF2::genDegree = 0;
uint32_t* GF2::toVector = nullptr;
uint32_t* GF2::toPower = nullptr;
uint32_t GF2::nElement = 0;
bool GF2::isInitialized = false;
#endif //PROGRAM_GF2_H
