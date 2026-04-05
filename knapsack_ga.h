#pragma once

#include <vector>
#include <string>

struct GaItem {
    int value;
    int weight;
};

struct GaResult {
    long long totalValue;
    long long totalWeight;
    std::vector<int> state;
};

bool readGaInputFile(const std::string& filename, int& n, int& capacity, std::vector<GaItem>& items);
GaResult solveKnapsackGenetic(int n, int capacity, const std::vector<GaItem>& items);
void printGaResult(const GaResult& result);