
#pragma once

#include <vector>
#include <string>

struct Point {
    double x;
    double y;
};

struct TspResult {
    double length;
    std::vector<int> route;
};

bool readTspInputFile(const std::string& filename, std::vector<Point>& points);
TspResult solveTspLocalSearch(const std::vector<Point>& points);
void printTspResult(const TspResult& result);
