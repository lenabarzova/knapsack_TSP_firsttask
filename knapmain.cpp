#include <iostream>
#include <vector>
#include "knapsack.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "no arguments" << endl;
        return 1;
    }

    int N = 0;
    int W = 0;
    vector<Item> items;

    bool ok = readInputFile(argv[1], N, W, items);
    if (!ok) {
        cout << "File open error" << endl;
        return 1;
    }

    Result result = solveKnapsackHamming(N, W, items);
    printResult(result);

    return 0;
}