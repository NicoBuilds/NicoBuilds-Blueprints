#pragma once
#pragma once
#include <string>

class Fraction {
public:
    long long numerator;
    long long denominator;

    Fraction(long long num = 0, long long den = 1);

    Fraction operator+(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    bool operator==(const Fraction& other) const;
    std::string str() const;
};