//
// Created by dmortensen on 10/22/23.
//

#ifndef PROGRAM_GF2_H
#define PROGRAM_GF2_H
namespace numsys{
    enum viewmodeenum {
        vector, power, binary
    };
    enum staticoption {
        viewmode
    };
    enum inputmode {
        integer, exponent
    };

    template<uint32_t G>
    class GF2 {
    private:
        const static inline uint32_t gen{G};
        static inline uint32_t genDegree{0};
        static inline uint32_t nElement{0};
        static inline int64_t *toPower{nullptr};
        static inline int64_t *toVector{nullptr};
        static inline bool isInitialized{false};
        static inline enum viewmodeenum viewAs{vector};

        static inline uint32_t modplus(int64_t a) {
            return ((a % (nElement - 1) + (nElement - 1)) % (nElement - 1));
        } //TODO: Make this faster, take advantage of the fact that we are modulo 2^n - 1 perhaps...

        static void init() {
            if (!isInitialized) {
                genDegree = toDegree(gen);
                nElement = 1 << genDegree;
                toPower = new int64_t[nElement];
                toVector = new int64_t[nElement];

                uint32_t val = 1;
                toVector[0] = val;
                toPower[val] = 0;
                for (int i = 1; i < nElement - 1; i++) {
                    val = (val >> (genDegree - 1)) > 0 ? (val << 1) ^ gen : (val << 1);
                    toPower[val] = i;
                    toVector[i] = val;
                }
                isInitialized = true;
            }
        }

        static uint32_t toDegree(const uint32_t &p) {
            uint32_t degree = 0;
            for (int i = 0; i < 32; i++) {
                if ((p >> i) % 2) {
                    degree = i;
                }
            }
            return degree;
        }

    public:
        int64_t value;

        GF2(int64_t v, inputmode inputAs = integer) {
            init();
            if (!isInitialized) {
                throw std::invalid_argument("Cannot run until initialized");
            }
            value = (inputAs == exponent) ? toVector[modplus(v)] : v;
        }

        GF2() : value{0} {}

        GF2(const GF2 &input) = default;

        GF2 &operator=(const GF2 &input) = default;

        ~GF2() = default;

        template<class T>
        static void set(staticoption opt, T input) {
            switch (opt) {
                case viewmode: {
                    GF2::viewAs = viewmodeenum(input);
                    break;
                }
                default:
                    std::cerr << "setter input not found. Options: generator, defaultinputtype, viewmode";
                    exit(-1);
            }
        }

        // GF2 -- int operators

        GF2 operator+(const int a) const {
            return {value ^ a, integer};
        }

        GF2 operator-(const int a) const { return {value ^ a, integer}; }

        GF2 operator*(const int a) const {
            if (value == 0 | a == 0) {
                return {0, integer};
            } else {
                return {modplus(toPower[value] + toPower[a]), exponent};
            }
        }

        GF2 operator/(const int a) const {
            if (a == 0) {
                throw std::invalid_argument("Cannot divide by zero");
            }
            return {modplus(toPower[value] - toPower[a]), exponent};
        }

        bool operator==(const int a) const { return a == value; }

        bool operator!=(const int a) const { return a != value; }

        // GF2 -- int assignment operators
        GF2 &operator++() {
            value = (value + 1) % nElement;
            return *this;
        }

        GF2 operator++(int) {
            GF2 temp = *this;
            value = (value + 1) % nElement;
            return temp;
        }

        GF2 &operator-() { return *this; }

        GF2 &operator+=(const int a) {
            value ^= a;
            return *this;
        }

        GF2 &operator-=(const int a) {
            value ^= a;
            return *this;
        }

        GF2 &operator*=(const int a) {
            value = (value == 0 | a == 0) ? 0 : toVector[modplus(toPower[value] + toPower[a])];
            return *this;
        }

        GF2 &operator/=(const int a) {
            if (a == 0) {
                throw std::invalid_argument("Cannot divide by zero");
            }
            value = toVector[modplus(toPower[value] - toPower[a])];
            return *this;
        }

        // GF2 -- GF2 assignment operators
        GF2 &operator+=(const GF2 &a) {
            value ^= a.value;
            return *this;
        }

        GF2 &operator-=(const GF2 &a) {
            value ^= a.value;
            return *this;
        }

        GF2 &operator*=(const GF2 &a) {
            value = (value == 0 | a.value == 0) ? 0 : toVector[modplus(toPower[a.value] + toPower[value])];
            return *this;
        }

        GF2 &operator/=(const GF2 &a) {
            if (a.value == 0) {
                throw std::invalid_argument("Cannot divide by zero");
            }
            value = toVector[modplus(toPower[value] - toPower[a.value])];
            return *this;
        }

        GF2 &operator^=(const int exp) {
            value = toVector[modplus(toPower[value] * exp)];
            return *this;
        }

        // GF2 -- GF2 operators
        GF2 operator+(const GF2 &a) const {
            return {a.value ^ value, integer};
        }

        GF2 operator-(const GF2 &a) const { return {a.value ^ value, integer}; }

        GF2 operator*(const GF2 &a) const {
            if (a.value == 0 | value == 0)
                return {0, integer};
            else {
                return {modplus(toPower[a.value] + toPower[value]), exponent};
            }
        }

        GF2 operator/(const GF2 &a) const {
            if (a.value == 0) {
                throw std::invalid_argument("Cannot divide by zero");
            }
            return {modplus(toPower[value] - toPower[a.value]), exponent};
        }

        GF2 operator^(const int e) const {
            return {modplus(toPower[value] * e), exponent};
        }

        bool operator==(const GF2 &a) const { return (a.value == value); }

        bool operator!=(const GF2 &a) const { return (a.value != value); }

        friend std::ostream &operator<<(std::ostream &os, const GF2 &a) {
            bool isfirst = true;
            switch (a.viewAs) {
                case vector:
                    if (genDegree == 1) {
                        os << ((a.value % 2 == 1) ? "1" : "0");
                        break;
                    }
                    if (a.value % 2 == 1) {
                        os << "1";
                        isfirst = false;
                    }
                    for (int i = 1; i < genDegree - 1; i++) {
                        if ((a.value >> i) % 2 == 1) {
                            if (!isfirst) {
                                os << " + ";
                            }
                            os << "A^" << i;
                            isfirst = false;
                        }
                    }
                    if ((a.value >> (genDegree - 1)) % 2 == 1) {
                        if (!isfirst) {
                            os << " + ";
                        }
                        os << "A^" << (genDegree - 1);
                    }
                    break;
                case power:
                    if (a.value == 0) { os << "A^(-inf)"; }
                    else { os << "A^" << int(toPower[a.value]); }
                    break;
                case binary:
                    for (int i = 0; i < genDegree; i++) {
                        os << (a.value >> i) % 2 << " ";
                    }
            }
            return os;
        }
    };
}
#endif //PROGRAM_GF2_H
