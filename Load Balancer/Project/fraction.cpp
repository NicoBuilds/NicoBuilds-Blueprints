#include "fraction.h"
#include <stdexcept>
#include <numeric> // std::gcd

Fraction::Fraction(long long num, long long den) {
    if (den == 0) throw std::invalid_argument("Denominator cannot be zero.");
    long long g = std::gcd(num, den);
    numerator = num / g;
    denominator = den / g;
    if (denominator < 0) {
        numerator *= -1;
        denominator *= -1;
    }
}

Fraction Fraction::operator+(const Fraction& other) const {
    long long num = numerator * other.denominator + other.numerator * denominator;
    long long den = denominator * other.denominator;
    return Fraction(num, den);
}

Fraction Fraction::operator*(const Fraction& other) const {
    return Fraction(numerator * other.numerator, denominator * other.denominator);
}

bool Fraction::operator==(const Fraction& other) const {
    return numerator == other.numerator && denominator == other.denominator;
}

std::string Fraction::str() const {
    if (denominator == 1) return std::to_string(numerator);
    return std::to_string(numerator) + "/" + std::to_string(denominator);
}