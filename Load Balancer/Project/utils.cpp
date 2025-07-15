#include "utils.h"
#include <regex>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <iostream>

Fraction parseFraction(const std::string& input) {
    std::regex mixed(R"(^\s*(\d+)[ +](\d+)/(\d+)\s*$)");
    std::regex proper(R"(^\s*(\d+)/(\d+)\s*$)");
    std::regex decimal(R"(^\s*(\d+)\.(\d+)\s*$)");
    std::regex whole(R"(^\s*(\d+)\s*$)");

    std::smatch match;

    if (std::regex_match(input, match, mixed)) {
        long long whole = std::stoll(match[1]);
        long long num = std::stoll(match[2]);
        long long den = std::stoll(match[3]);
        return Fraction(whole * den + num, den);
    }
    else if (std::regex_match(input, match, proper)) {
        return Fraction(std::stoll(match[1]), std::stoll(match[2]));
    }
    else if (std::regex_match(input, match, decimal)) {
        std::string whole = match[1];
        std::string frac = match[2];
        long long den = 1;
        for (size_t i = 0; i < frac.size(); ++i) den *= 10;
        long long num = std::stoll(whole) * den + std::stoll(frac);
        return Fraction(num, den);
    }
    else if (std::regex_match(input, match, whole)) {
        return Fraction(std::stoll(match[1]), 1);
    }

    throw std::invalid_argument("Unrecognized fraction format.");
}

void calculateRatios(const Fraction& a, const Fraction& b, Fraction& a_out, Fraction& b_out) {
    Fraction total = a + b;

    // Cross-multiply a/total
    long long a_num = a.numerator * total.denominator;
    long long a_den = a.denominator * total.numerator;

    // Cross-multiply b/total
    long long b_num = b.numerator * total.denominator;
    long long b_den = b.denominator * total.numerator;

    Fraction rawA(a_num, a_den);
    Fraction rawB(b_num, b_den);

    // If same denominator after simplification, perfect
    if (rawA.denominator == rawB.denominator) {
        a_out = rawA;
        b_out = rawB;
        return;
    }

    // Otherwise rescale both to common denominator (LCM)
    long long lcm = std::lcm(rawA.denominator, rawB.denominator);

    a_out = Fraction(rawA.numerator * (lcm / rawA.denominator), lcm);
    b_out = Fraction(rawB.numerator * (lcm / rawB.denominator), lcm);
}

int findProductOf2sAnd3sOrAbove(int target, std::vector<int>& factorsOut) {
    int current = target;

    while (true) {
        int n = current;
        std::vector<int> factors;

        // Factor out 3s
        while (n % 3 == 0) {
            factors.push_back(3);
            n /= 3;
        }

        // Factor out 2s
        while (n % 2 == 0) {
            factors.push_back(2);
            n /= 2;
        }

        if (n == 1) {
            factorsOut = factors;
            return current;
        }

        ++current;  // try the next number
    }
}


bool assignDigitsFromWeights(const std::vector<long long>& targets, const std::vector<int>& factors, std::vector<std::vector<long long>>& result) {
    size_t n = factors.size();
    std::vector<long long> weights(n, 1);
    std::vector<long long> maxUsages(n);

    // Step 1: Calculate weights
    for (long long i = n - 2; i >= 0; --i) {
        weights[i] = weights[i + 1] * factors[i + 1];
    }

    // Step 2: Determine usage availability per position
    for (size_t i = 0; i < n; ++i) {
        maxUsages[i] = (i == n - 1)
            ? static_cast<long long>(factors[i])
            : static_cast<long long>(factors[i]) - 1;
    }

    result.clear();
    std::vector<long long> available = maxUsages;

    for (size_t t = 0; t < targets.size(); ++t) {
        long long remaining = targets[t];
        std::vector<long long> digits(n, 0);

        for (size_t i = 0; i < n; ++i) {
            long long w = weights[i];
            int usable = static_cast<int>(std::min(static_cast<long long>(available[i]), remaining / w));
            digits[i] = usable;
            available[i] -= usable;
            remaining -= usable * w;
        }

        if (remaining != 0) {
            return false;
        }

        result.push_back(digits);
    }

    return true;
}

bool validateDigitAssignments(
    const std::vector<int>& factors,
    const std::vector<std::vector<long long>>& digitSets
) {
    const size_t depth = factors.size();
    std::vector<long long> usage(depth, 0);
    std::vector<long long> maxAvailable(depth, 0);

    for (size_t i = 0; i < depth; ++i) {
        if (i == depth - 1) {
            // Last level: all outputs usable
            maxAvailable[i] = factors[i];
        }
        else {
            // All other levels: factor - 1 outputs usable
            maxAvailable[i] = factors[i] - 1;
        }
    }

    // Sum up total usage from each digit vector
    for (const auto& digits : digitSets) {
        for (size_t i = 0; i < depth; ++i) {
            usage[i] += digits[i];
        }
    }

    // Check for under-usage or over-usage
    for (size_t i = 0; i < depth; ++i) {
        if (usage[i] < 1 || usage[i] > maxAvailable[i]) {
            return false;
        }
    }

    return true;
}

void renderGraphicalLayout(const std::vector<std::vector<long long>>& digitRepresentations) {
    const size_t numSplitters = digitRepresentations[0].size();
    const size_t numLines = digitRepresentations.size(); // 2 or 3 usually

    // Step 1: count how many outputs per splitter
    std::vector<std::vector<char>> connections(numSplitters);
    for (size_t row = 0; row < numLines; ++row) {
        for (size_t col = 0; col < numSplitters; ++col) {
            for (int i = 0; i < digitRepresentations[row][col]; ++i) {
                connections[col].push_back('1' + row);  // '1', '2', or '3'
            }
        }
    }

    // Step 2: Render lines
    std::string top, mid, bot;

    for (size_t i = 0; i < numSplitters; ++i) {
        const auto& conns = connections[i];
        std::string topC = "   ", midC = " X ", botC = "   ";

        if (conns.size() == 1) {
            topC[1] = conns[0];
        }
        else if (conns.size() == 2) {
            topC[1] = conns[0];
            botC[1] = conns[1];
        }
        else if (conns.size() == 3) {
            topC[1] = conns[0];
            botC[0] = conns[1];
            botC[2] = conns[2];
        }

        // Append with spacing
        top += topC;
        mid += midC;
        bot += botC;

        // Add a connector line between splitters
        if (i != numSplitters - 1) {
            top += " ";
            mid += "=";
            bot += " ";
        }
    }

    std::cout << "\nGraphical Layout:\n";
    std::cout << top << "\n" << mid << "\n" << bot << "\n";
}

void printTestInstructions(const Fraction& a_ratio, const Fraction& b_ratio) {
    std::cout << "\nAfter building the load balancer, it's always good practice to test it!\n";
    std::cout << "You are trying to achieve the following division ratios:\n";
    std::cout << " - A: " << a_ratio.str() << "\n";
    std::cout << " - B: " << b_ratio.str() << "\n";

    std::cout << "\nSo, connect an industrial storage container into the input,\n";
    std::cout << "and two industrial storage containers into each output.\n";

    std::cout << "\nDump " << a_ratio.denominator << " items into the input.\n";
    std::cout << "You should end up seeing:\n";
    std::cout << " - " << a_ratio.numerator << " items in the first output (A)\n";
    std::cout << " - " << b_ratio.numerator << " items in the second output (B)\n\n";
}