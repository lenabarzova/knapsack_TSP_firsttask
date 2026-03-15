#include "knapsack.h"
#include <iostream>
#include <fstream>

using namespace std;

bool readInputFile(const string& filename, int& N, int& W, vector<Item>& items) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    fin >> N >> W;

    items.resize(N);
    for (int i = 0; i < N; i++) {
        fin >> items[i].value >> items[i].weight;
    }

    fin.close();
    return true;
}

Result solveKnapsackHamming(int N, int W, const vector<Item>& items) {
    Result result;
    result.totalValue = 0;
    result.totalWeight = 0;
    result.state.assign(N, 0);

    for (int i = 0; i < N; i++) {
        if (result.totalWeight + items[i].weight <= W) {
            result.state[i] = 1;
            result.totalWeight += items[i].weight;
            result.totalValue += items[i].value;
        }
    }

    bool improved = true;

    while (improved) {
        improved = false;

        int bestType = 0;
        int bestI = -1;
        int bestJ = -1;
        long long bestValue = result.totalValue;
        long long bestWeight = result.totalWeight;

        for (int i = 0; i < N; i++) {
            if (result.state[i] == 0) {
                long long newWeight = result.totalWeight + items[i].weight;
                long long newValue = result.totalValue + items[i].value;

                if (newWeight <= W && newValue > bestValue) {
                    bestType = 1;
                    bestI = i;
                    bestJ = -1;
                    bestValue = newValue;
                    bestWeight = newWeight;
                }
            }
        }

        for (int i = 0; i < N; i++) {
            if (result.state[i] == 1) {
                for (int j = 0; j < N; j++) {
                    if (result.state[j] == 0) {
                        long long newWeight = result.totalWeight - items[i].weight + items[j].weight;
                        long long newValue = result.totalValue - items[i].value + items[j].value;

                        if (newWeight <= W && newValue > bestValue) {
                            bestType = 2;
                            bestI = i;
                            bestJ = j;
                            bestValue = newValue;
                            bestWeight = newWeight;
                        }
                    }
                }
            }
        }

        if (bestType == 1) {
            result.state[bestI] = 1;
            result.totalValue = bestValue;
            result.totalWeight = bestWeight;
            improved = true;
        }

        if (bestType == 2) {
            result.state[bestI] = 0;
            result.state[bestJ] = 1;
            result.totalValue = bestValue;
            result.totalWeight = bestWeight;
            improved = true;
        }
    }

    return result;
}

void printResult(const Result& result) {
    cout << "maximum value was found as: " << result.totalValue << endl;
    cout << "weight sum: " << result.totalWeight << endl;

    cout << "choosed objects: ";
    for (int i = 0; i < static_cast<int>(result.state.size()); i++) {
        if (result.state[i] == 1) {
            cout << i + 1 << " ";
        }
    }
    cout << endl;

    cout << "binary vector of solution: ";
    for (int i = 0; i < static_cast<int>(result.state.size()); i++) {
        cout << result.state[i] << " ";
    }
    cout << endl;
}