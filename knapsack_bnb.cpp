#include "knapsack_bnb.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct SortedBnbItem {
    int value;
    int weight;
    int originalIndex;
    double ratio;
};

bool readBnbInputFile(const string& filename, int& n, int& capacity, vector<BnbItem>& items) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    fin >> n >> capacity;

    items.resize(n);
    for (int i = 0; i < n; i++) {
        fin >> items[i].value >> items[i].weight;
    }

    fin.close();
    return true;
}

vector<SortedBnbItem> buildSortedBnbItems(const vector<BnbItem>& items) {
    int n = static_cast<int>(items.size());
    vector<SortedBnbItem> sorted(n);

    for (int i = 0; i < n; i++) {
        sorted[i].value = items[i].value;
        sorted[i].weight = items[i].weight;
        sorted[i].originalIndex = i;
        sorted[i].ratio = 0.0;

        if (items[i].weight > 0) {
            sorted[i].ratio = static_cast<double>(items[i].value) / static_cast<double>(items[i].weight);
        }
    }

    sort(sorted.begin(), sorted.end(),
        [](const SortedBnbItem& a, const SortedBnbItem& b) {
            if (a.ratio != b.ratio) {
                return a.ratio > b.ratio;
            }
            if (a.weight != b.weight) {
                return a.weight < b.weight;
            }
            return a.value > b.value;
        }
    );

    return sorted;
}

double calculateUpperBound(
    int level,
    long long currentWeight,
    long long currentValue,
    int capacity,
    const vector<SortedBnbItem>& sorted
) {
    if (currentWeight > capacity) {
        return -1.0;
    }

    double bound = static_cast<double>(currentValue);
    long long freeCapacity = capacity - currentWeight;

    for (int i = level; i < static_cast<int>(sorted.size()); i++) {
        if (sorted[i].weight <= freeCapacity) {
            freeCapacity -= sorted[i].weight;
            bound += sorted[i].value;
        }
        else {
            if (sorted[i].weight > 0) {
                bound += static_cast<double>(freeCapacity) *
                    static_cast<double>(sorted[i].value) /
                    static_cast<double>(sorted[i].weight);
            }
            freeCapacity = 0;
        }
    }

    return bound;
}

void buildGreedyLowerBound(
    int capacity,
    const vector<SortedBnbItem>& sorted,
    long long& bestValue,
    vector<int>& bestStateOriginal
) {
    long long currentWeight = 0;
    long long currentValue = 0;
    vector<int> state(bestStateOriginal.size(), 0);

    for (int i = 0; i < static_cast<int>(sorted.size()); i++) {
        if (currentWeight + sorted[i].weight <= capacity) {
            currentWeight += sorted[i].weight;
            currentValue += sorted[i].value;
            state[sorted[i].originalIndex] = 1;
        }
    }

    if (currentValue > bestValue) {
        bestValue = currentValue;
        bestStateOriginal = state;
    }
}

void branchAndBoundDfs(
    int level,
    int capacity,
    const vector<SortedBnbItem>& sorted,
    vector<int>& currentStateSorted,
    long long currentWeight,
    long long currentValue,
    long long& bestValue,
    vector<int>& bestStateOriginal
) {
    int n = static_cast<int>(sorted.size());

    if (level == n) {
        if (currentWeight <= capacity && currentValue > bestValue) {
            bestValue = currentValue;

            vector<int> stateOriginal(n, 0);
            for (int i = 0; i < n; i++) {
                if (currentStateSorted[i] == 1) {
                    stateOriginal[sorted[i].originalIndex] = 1;
                }
            }

            bestStateOriginal = stateOriginal;
        }
        return;
    }

    double upperBound = calculateUpperBound(level, currentWeight, currentValue, capacity, sorted);

    if (upperBound > static_cast<double>(bestValue)) {
        bool canTake = false;
        if (currentWeight + sorted[level].weight <= capacity) {
            canTake = true;
        }

        if (canTake) {
            currentStateSorted[level] = 1;

            branchAndBoundDfs(
                level + 1,
                capacity,
                sorted,
                currentStateSorted,
                currentWeight + sorted[level].weight,
                currentValue + sorted[level].value,
                bestValue,
                bestStateOriginal
            );

            currentStateSorted[level] = 0;
        }

        currentStateSorted[level] = 0;

        branchAndBoundDfs(
            level + 1,
            capacity,
            sorted,
            currentStateSorted,
            currentWeight,
            currentValue,
            bestValue,
            bestStateOriginal
        );
    }
}

BnbResult solveKnapsackBranchAndBound(int n, int capacity, const vector<BnbItem>& items) {
    BnbResult result;
    result.totalValue = 0;
    result.totalWeight = 0;
    result.state.assign(n, 0);

    vector<SortedBnbItem> sorted = buildSortedBnbItems(items);

    long long bestValue = 0;
    vector<int> bestStateOriginal(n, 0);

    buildGreedyLowerBound(capacity, sorted, bestValue, bestStateOriginal);

    vector<int> currentStateSorted(n, 0);

    branchAndBoundDfs(
        0,
        capacity,
        sorted,
        currentStateSorted,
        0,
        0,
        bestValue,
        bestStateOriginal
    );

    result.state = bestStateOriginal;

    for (int i = 0; i < n; i++) {
        if (result.state[i] == 1) {
            result.totalValue += items[i].value;
            result.totalWeight += items[i].weight;
        }
    }

    return result;
}

void printBnbResult(const BnbResult& result) {
    cout << "Branch and Bound result" << endl;
    cout << "Value: " << result.totalValue << endl;
    cout << "Weight: " << result.totalWeight << endl;

    cout << "Chosen objects: ";
    for (int i = 0; i < static_cast<int>(result.state.size()); i++) {
        if (result.state[i] == 1) {
            cout << i + 1 << " ";
        }
    }
    cout << endl;

    if (result.state.size() <= 100) {
        cout << "Binary vector: ";
        for (int i = 0; i < static_cast<int>(result.state.size()); i++) {
            cout << result.state[i] << " ";
        }
        cout << endl;
    }
}