#pragma once

#include <vector>
#include <string>

struct Item {
    int value;
    int weight;
};

struct Result {
    long long totalValue;
    long long totalWeight;
    std::vector<int> state;
};

bool readInputFile(const std::string& filename, int& N, int& W, std::vector<Item>& items);
Result solveKnapsackHamming(int N, int W, const std::vector<Item>& items);
void printResult(const Result& result);