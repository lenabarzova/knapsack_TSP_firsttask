#include <iostream>
#include <vector>
#include "knapsack_ga.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: knapsack_ga.exe <input_file>" << endl;
        return 1;
    }

    int n = 0;
    int capacity = 0;
    vector<GaItem> items;

    bool ok = readGaInputFile(argv[1], n, capacity, items);
    if (!ok) {
        cout << "File open error" << endl;
        return 1;
    }

    GaResult result = solveKnapsackGenetic(n, capacity, items);
    printGaResult(result);

    return 0;
}