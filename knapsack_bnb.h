#pragma once

#include <vector>
#include <string>

struct BnbItem {
    int value;
    int weight;
};

struct BnbResult {
    long long totalValue;
    long long totalWeight;
    std::vector<int> state;
};

bool readBnbInputFile(const std::string& filename, int& n, int& capacity, std::vector<BnbItem>& items);
BnbResult solveKnapsackBranchAndBound(int n, int capacity, const std::vector<BnbItem>& items);
void printBnbResult(const BnbResult& result);