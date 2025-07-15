#pragma once
#include <string>
#include <vector>
#include "fraction.h"

Fraction parseFraction(const std::string& input);
void calculateRatios(const Fraction& a, const Fraction& b, Fraction& a_out, Fraction& b_out);
int findProductOf2sAnd3sOrAbove(int target, std::vector<int>& factorsOut);
bool assignDigitsFromWeights(const std::vector<long long>& targets, const std::vector<int>& factors, std::vector<std::vector<long long>>& result);
bool validateDigitAssignments(const std::vector<int>& factors,const std::vector<std::vector<long long>>& digitSets);
void renderGraphicalLayout(const std::vector<std::vector<long long>>& digitRepresentations);
void printTestInstructions(const Fraction& a_ratio, const Fraction& b_ratio);