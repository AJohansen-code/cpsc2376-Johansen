#include "mixedFraction.h"

MixedFraction::MixedFraction(int whole, int n, int d) : Fraction(whole * d + n, d) {}

MixedFraction::MixedFraction(const Fraction& fraction) : Fraction(fraction.getNumerator(), fraction.getDenominator()) {}

std::ostream& operator<<(std::ostream& os, const MixedFraction& mixedFraction) {
    int whole = mixedFraction.getNumerator() / mixedFraction.getDenominator();
    int remainder = mixedFraction.getNumerator() % mixedFraction.getDenominator();
    int denominator = mixedFraction.getDenominator();

    if (whole != 0) {
        os << (Fraction)mixedFraction;
    } else if (remainder == 0) {
        os << whole;
    } else {
        os << whole << " " << remainder << "/" << denominator;
    }
    return os;
}