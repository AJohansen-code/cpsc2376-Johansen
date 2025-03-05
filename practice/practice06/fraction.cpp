#include "fraction.h"

Fraction::Fraction() : numerator(0), denominator(1) {}

Fraction::Fraction(int n, int d) : numerator(n), denominator(d) {
    if (denominator == 0) {
        throw std::invalid_argument("Denominator cannot be zero.");
    }
    simplify();
}

int Fraction::getNumerator() const {
    return numerator;
}

int Fraction::getDenominator() const {
    return denominator;
}

void Fraction::setNumerator(int n) {
    numerator = n;
}

void Fraction::setDenominator(int d) {
    if (d == 0) {
        throw std::invalid_argument("Denominator cannot be zero.");
    }
    denominator = d;
    simplify();
}

void Fraction::simplify() {
    if (denominator == 0) {
        throw std::invalid_argument("Denominator cannot be zero.");
    }

    int a = numerator, b = denominator;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    int gcd = a;

    numerator /= gcd;
    denominator /= gcd;

    if (denominator < 0) {
        numerator *= -1;
        denominator *= -1;
    }
}

Fraction operator+(const Fraction& lhs, const Fraction& rhs) {
   return Fraction(lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator, lhs.denominator * rhs.denominator);
}

Fraction operator-(const Fraction& lhs, const Fraction& rhs) {
    return Fraction(lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator, lhs.denominator * rhs.denominator);
}

Fraction operator*(const Fraction& lhs, const Fraction& rhs) {
    return Fraction(lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator);
}

Fraction operator/(const Fraction& lhs, const Fraction& rhs) {
    if (rhs.numerator == 0) {
        throw std::invalid_argument("Cannot divide by zero.");
    }
    return Fraction(lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator);
}

std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {
    os << fraction.numerator << "/" << fraction.denominator;
    return os;
}