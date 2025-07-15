
#include <sstream>
#include <string>
#include <regex>
#include <stdexcept>
#include <numeric> // for std::gcd
#include <iostream>
#include <iomanip> 
#include "fraction.h"
#include "utils.h"


int main()
{
    while (true)
    {
        std::string inputA, inputB;

        std::cout << "Enter first number: (Formats allowed: A, A B/C, A+B/C)\n";
        std::getline(std::cin, inputA);

        std::cout << "Enter second number: (Formats allowed: A, A B/C, A+B/C)\n";
        std::getline(std::cin, inputB);

        try {
            Fraction a = parseFraction(inputA);
            Fraction b = parseFraction(inputB);


            Fraction a_ratio, b_ratio;
            calculateRatios(a, b, a_ratio, b_ratio);

            long long denominator = a_ratio.denominator;
            std::vector<int> factors;
            int closest = findProductOf2sAnd3sOrAbove(denominator, factors);

            std::vector<long long> targets = { a_ratio.numerator, b_ratio.numerator };
            std::vector<std::vector<long long>> all_digits;

            if (closest > denominator) {
                long long feedback = closest - denominator;
                targets.push_back(feedback);
            }

            if (!assignDigitsFromWeights(targets, factors, all_digits)) {
                // Check if the last factor is 3, and give it a second chance
                if (!factors.empty() && factors.back() == 3) {

                    std::vector<int> adjustedFactors = factors;
                    adjustedFactors.pop_back();
                    adjustedFactors.push_back(2);
                    adjustedFactors.push_back(2);

                    if (!assignDigitsFromWeights(targets, adjustedFactors, all_digits)) {
                        std::cout << "The load balancer can't be created using the generic blueprint.\n";
                        continue;
                    }
                    else {
                        factors = adjustedFactors; // Update factors to match what worked
                    }
                }
                else {
                    std::cout << "The load balancer can't be created using the generic blueprint.\n";
                    continue;
                }
            }

            if (!validateDigitAssignments(factors, all_digits)) {
                std::cout << "Internal error: This is never supposed to happen! Please, I would love if you went to my YoutubeChannel (NicoBuilds) and tell me it failed, also letting me know the numbers you tried!.\n";
                continue;
            }


            if (all_digits.at(0).size() <= 9)
                std::cout << "Start by plopping the blueprint \"Programmable Load Balancer N = " + std::to_string(all_digits.at(0).size()) + "\"\n\n";
            else
                std::cout << "Wow, you are cooking with some weird numbers. Generic blueprint is not big enough, plop a \"Programmable Load Balancer N = 9\" blueprint, and add " + std::to_string(all_digits.at(0).size() - 9) + " splitters and mergers on the end.\n\n";
            if (closest == denominator)
                std::cout << "This load balancer doesnt need a feedback. You can dismantle the merger at the input, and all of the mergers that are on top!\n";
            std::cout << "Connect lifts according to this diagram. The X represent the bottom level splitters (left one is the first one) and the numbers on top or on bottom to what level you need to connect the sides of the spliiter (1 to 3)\n";
            renderGraphicalLayout(all_digits);
            std::cout << "\n";

            printTestInstructions(a_ratio, b_ratio);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    }
}