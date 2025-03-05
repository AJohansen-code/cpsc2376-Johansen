#ifndef MIXEDFRACTION_H
#define MIXEDFRACTION_H

#include "fraction.h"

class MixedFraction : public Fraction {
public:
    MixedFraction(int whole, int n, int d);
    MixedFraction(const Fraction& fraction);
    friend std::ostream& operator<<(std::ostream& os, const MixedFraction& mixedFraction);
};

#endif