#include <iostream>
#include <vector>
#include "knapsack_bnb.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: knapsack_bnb.exe <input_file>" << endl;
        return 1;
    }

    int n = 0;
    int capacity = 0;
    vector<BnbItem> items;

    bool ok = readBnbInputFile(argv[1], n, capacity, items);
    if (!ok) {
        cout << "File open error" << endl;
        return 1;
    }

    BnbResult result = solveKnapsackBranchAndBound(n, capacity, items);
    printBnbResult(result);

    return 0;
}