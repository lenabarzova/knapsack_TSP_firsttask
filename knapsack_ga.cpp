#include "knapsack_ga.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct Individual {
    vector<int> genes;
    long long totalValue;
    long long totalWeight;
    long long fitness;
};

bool readGaInputFile(const string& filename, int& n, int& capacity, vector<GaItem>& items) {
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

void evaluateIndividual(Individual& ind, int capacity, const vector<GaItem>& items) {
    ind.totalValue = 0;
    ind.totalWeight = 0;

    for (int i = 0; i < static_cast<int>(items.size()); i++) {
        if (ind.genes[i] == 1) {
            ind.totalValue += items[i].value;
            ind.totalWeight += items[i].weight;
        }
    }

    if (ind.totalWeight <= capacity) {
        ind.fitness = ind.totalValue;
    }
    else {
        long long penalty = (ind.totalWeight - capacity) * 1000000LL;
        ind.fitness = ind.totalValue - penalty;
    }
}

void repairIndividual(Individual& ind, int capacity, const vector<GaItem>& items) {
    evaluateIndividual(ind, capacity, items);

    while (ind.totalWeight > capacity) {
        int worstIndex = -1;
        double worstRatio = 1e100;

        for (int i = 0; i < static_cast<int>(items.size()); i++) {
            if (ind.genes[i] == 1) {
                double ratio = 1e100;
                if (items[i].weight > 0) {
                    ratio = static_cast<double>(items[i].value) / static_cast<double>(items[i].weight);
                }

                if (ratio < worstRatio) {
                    worstRatio = ratio;
                    worstIndex = i;
                }
            }
        }

        if (worstIndex != -1) {
            ind.genes[worstIndex] = 0;
            evaluateIndividual(ind, capacity, items);
        }
        else {
            ind.totalWeight = 0;
        }
    }

    evaluateIndividual(ind, capacity, items);
}

Individual createRandomIndividual(int n, int capacity, const vector<GaItem>& items, mt19937& gen) {
    Individual ind;
    ind.genes.assign(n, 0);

    uniform_int_distribution<int> bitDist(0, 1);

    for (int i = 0; i < n; i++) {
        ind.genes[i] = bitDist(gen);
    }

    repairIndividual(ind, capacity, items);
    return ind;
}

Individual crossoverIndividuals(
    const Individual& a,
    const Individual& b,
    int capacity,
    const vector<GaItem>& items,
    mt19937& gen
) {
    int n = static_cast<int>(a.genes.size());

    Individual child;
    child.genes.assign(n, 0);

    uniform_int_distribution<int> splitDist(0, n - 1);
    int split = splitDist(gen);

    for (int i = 0; i < n; i++) {
        if (i <= split) {
            child.genes[i] = a.genes[i];
        }
        else {
            child.genes[i] = b.genes[i];
        }
    }

    repairIndividual(child, capacity, items);
    return child;
}

Individual mutateIndividual(
    const Individual& source,
    int capacity,
    const vector<GaItem>& items,
    mt19937& gen
) {
    int n = static_cast<int>(source.genes.size());

    Individual mutant = source;

    uniform_int_distribution<int> indexDist(0, n - 1);
    int pos = indexDist(gen);

    if (mutant.genes[pos] == 0) {
        mutant.genes[pos] = 1;
    }
    else {
        mutant.genes[pos] = 0;
    }

    repairIndividual(mutant, capacity, items);
    return mutant;
}

bool betterIndividual(const Individual& a, const Individual& b) {
    if (a.fitness != b.fitness) {
        return a.fitness > b.fitness;
    }

    if (a.totalValue != b.totalValue) {
        return a.totalValue > b.totalValue;
    }

    return a.totalWeight < b.totalWeight;
}

GaResult solveKnapsackGenetic(int n, int capacity, const vector<GaItem>& items) {
    const int populationSize = 30;
    const int generations = 100;

    random_device rd;
    mt19937 gen(rd());

    vector<Individual> population;
    population.reserve(populationSize);

    for (int i = 0; i < populationSize; i++) {
        population.push_back(createRandomIndividual(n, capacity, items, gen));
    }

    for (int iter = 0; iter < generations; iter++) {
        vector<Individual> allCandidates = population;

        for (int i = 0; i < populationSize; i++) {
            for (int j = i + 1; j < populationSize; j++) {
                Individual child = crossoverIndividuals(population[i], population[j], capacity, items, gen);
                allCandidates.push_back(child);
            }
        }

        for (int i = 0; i < populationSize; i++) {
            Individual mutant = mutateIndividual(population[i], capacity, items, gen);
            allCandidates.push_back(mutant);
        }

        sort(allCandidates.begin(), allCandidates.end(), betterIndividual);

        vector<Individual> newPopulation;
        newPopulation.reserve(populationSize);

        for (int i = 0; i < populationSize; i++) {
            newPopulation.push_back(allCandidates[i]);
        }

        population = newPopulation;
    }

    sort(population.begin(), population.end(), betterIndividual);

    GaResult result;
    result.totalValue = population[0].totalValue;
    result.totalWeight = population[0].totalWeight;
    result.state = population[0].genes;

    return result;
}

void printGaResult(const GaResult& result) {
    cout << "Genetic Algorithm result" << endl;
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