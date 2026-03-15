#include <iostream>
#include <vector>
#include "tsp.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: tsp.exe <input_file>" << endl;
        return 1;
    }

    cout << "Program started" << endl;
    cout << "Input file: " << argv[1] << endl;

    vector<Point> points;

    bool ok = readTspInputFile(argv[1], points);
    if (!ok) {
        cout << "File open error" << endl;
        return 1;
    }

    cout << "File was opened successfully" << endl;
    cout << "Number of points: " << points.size() << endl;
    cout << "Start solving..." << endl;

    TspResult result = solveTspLocalSearch(points);

    cout << "Solving finished" << endl;
    printTspResult(result);

    return 0;
}