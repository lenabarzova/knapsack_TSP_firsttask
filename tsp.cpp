#include "tsp.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

double distanceBetweenPoints(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

bool readTspInputFile(const string& filename, vector<Point>& points) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    int n = 0;
    fin >> n;

    points.resize(n);
    for (int i = 0; i < n; i++) {
        fin >> points[i].x >> points[i].y;
    }

    fin.close();
    return true;
}

double routeLength(const vector<Point>& points, const vector<int>& route) {
    int n = static_cast<int>(route.size());
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        int a = route[i];
        int b = route[(i + 1) % n];
        sum += distanceBetweenPoints(points[a], points[b]);
    }

    return sum;
}

vector<int> buildInitialRoute(const vector<Point>& points) {
    int n = static_cast<int>(points.size());

    vector<int> route;
    route.reserve(n);

    vector<int> used(n, 0);

    route.push_back(0);
    used[0] = 1;

    for (int step = 1; step < n; step++) {
        int lastVertex = route[static_cast<int>(route.size()) - 1];

        double bestDistance = 1e100;
        int bestVertex = -1;

        for (int i = 0; i < n; i++) {
            if (used[i] == 0) {
                double currentDistance = distanceBetweenPoints(points[lastVertex], points[i]);

                if (currentDistance < bestDistance) {
                    bestDistance = currentDistance;
                    bestVertex = i;
                }
            }
        }

        if (bestVertex != -1) {
            route.push_back(bestVertex);
            used[bestVertex] = 1;
        }
    }

    return route;
}

void reverseSegment(vector<int>& route, int left, int right) {
    while (left < right) {
        int temp = route[left];
        route[left] = route[right];
        route[right] = temp;
        left++;
        right--;
    }
}

bool improveBy2Opt(const vector<Point>& points, vector<int>& route) {
    int n = static_cast<int>(route.size());

    double bestDelta = 0.0;
    int bestI = -1;
    int bestK = -1;

    for (int i = 0; i < n - 1; i++) {
        for (int k = i + 2; k < n; k++) {
            bool allowed = true;

            if (i == 0 && k == n - 1) {
                allowed = false;
            }

            if (allowed) {
                int a = route[i];
                int b = route[i + 1];
                int c = route[k];
                int d = route[(k + 1) % n];

                double oldLength = distanceBetweenPoints(points[a], points[b]) +
                    distanceBetweenPoints(points[c], points[d]);

                double newLength = distanceBetweenPoints(points[a], points[c]) +
                    distanceBetweenPoints(points[b], points[d]);

                double delta = newLength - oldLength;

                if (delta < bestDelta) {
                    bestDelta = delta;
                    bestI = i;
                    bestK = k;
                }
            }
        }
    }

    if (bestI != -1 && bestK != -1) {
        reverseSegment(route, bestI + 1, bestK);
        return true;
    }

    return false;
}

vector<int> reverseCopy(const vector<int>& part) {
    vector<int> result = part;
    int left = 0;
    int right = static_cast<int>(result.size()) - 1;

    while (left < right) {
        int temp = result[left];
        result[left] = result[right];
        result[right] = temp;
        left++;
        right--;
    }

    return result;
}

vector<int> build3OptCandidate(const vector<int>& route, int i, int j, int k, int type) {
    vector<int> a;
    vector<int> b;
    vector<int> c;
    vector<int> d;

    for (int p = 0; p <= i; p++) {
        a.push_back(route[p]);
    }

    for (int p = i + 1; p <= j; p++) {
        b.push_back(route[p]);
    }

    for (int p = j + 1; p <= k; p++) {
        c.push_back(route[p]);
    }

    for (int p = k + 1; p < static_cast<int>(route.size()); p++) {
        d.push_back(route[p]);
    }

    vector<int> rb = reverseCopy(b);
    vector<int> rc = reverseCopy(c);

    vector<int> candidate;

    if (type == 1) {
        candidate.insert(candidate.end(), a.begin(), a.end());
        candidate.insert(candidate.end(), rb.begin(), rb.end());
        candidate.insert(candidate.end(), c.begin(), c.end());
        candidate.insert(candidate.end(), d.begin(), d.end());
    }

    if (type == 2) {
        candidate.insert(candidate.end(), a.begin(), a.end());
        candidate.insert(candidate.end(), b.begin(), b.end());
        candidate.insert(candidate.end(), rc.begin(), rc.end());
        candidate.insert(candidate.end(), d.begin(), d.end());
    }

    if (type == 3) {
        candidate.insert(candidate.end(), a.begin(), a.end());
        candidate.insert(candidate.end(), rb.begin(), rb.end());
        candidate.insert(candidate.end(), rc.begin(), rc.end());
        candidate.insert(candidate.end(), d.begin(), d.end());
    }

    if (type == 4) {
        candidate.insert(candidate.end(), a.begin(), a.end());
        candidate.insert(candidate.end(), c.begin(), c.end());
        candidate.insert(candidate.end(), b.begin(), b.end());
        candidate.insert(candidate.end(), d.begin(), d.end());
    }

    if (type == 5) {
        candidate.insert(candidate.end(), a.begin(), a.end());
        candidate.insert(candidate.end(), rc.begin(), rc.end());
        candidate.insert(candidate.end(), b.begin(), b.end());
        candidate.insert(candidate.end(), d.begin(), d.end());
    }

    if (type == 6) {
        candidate.insert(candidate.end(), a.begin(), a.end());
        candidate.insert(candidate.end(), c.begin(), c.end());
        candidate.insert(candidate.end(), rb.begin(), rb.end());
        candidate.insert(candidate.end(), d.begin(), d.end());
    }

    if (type == 7) {
        candidate.insert(candidate.end(), a.begin(), a.end());
        candidate.insert(candidate.end(), rc.begin(), rc.end());
        candidate.insert(candidate.end(), rb.begin(), rb.end());
        candidate.insert(candidate.end(), d.begin(), d.end());
    }

    return candidate;
}

bool improveBy3Opt(const vector<Point>& points, vector<int>& route) {
    int n = static_cast<int>(route.size());

    double bestLength = routeLength(points, route);
    vector<int> bestRoute = route;
    bool improved = false;

    for (int i = 0; i < n - 5; i++) {
        for (int j = i + 2; j < n - 3; j++) {
            for (int k = j + 2; k < n - 1; k++) {
                for (int type = 1; type <= 7; type++) {
                    vector<int> candidate = build3OptCandidate(route, i, j, k, type);
                    double candidateLength = routeLength(points, candidate);

                    if (candidateLength < bestLength) {
                        bestLength = candidateLength;
                        bestRoute = candidate;
                        improved = true;
                    }
                }
            }
        }
    }

    if (improved) {
        route = bestRoute;
    }

    return improved;
}


TspResult solveTspLocalSearch(const vector<Point>& points) {
    TspResult result;

    int n = static_cast<int>(points.size());

    if (n == 0) {
        result.length = 0.0;
        return result;
    }

    if (n == 1) {
        result.route.push_back(0);
        result.length = 0.0;
        return result;
    }

    vector<int> route = buildInitialRoute(points);

    bool improved = true;

    while (improved) {
        improved = false;

        bool improved2Opt = improveBy2Opt(points, route);
        if (improved2Opt) {
            improved = true;
        }
    }

    if (n <= 30) {
        bool improved3Opt = true;

        while (improved3Opt) {
            improved3Opt = improveBy3Opt(points, route);
        }
    }

    result.route = route;
    result.length = routeLength(points, route);

    return result;
}

void printTspResult(const TspResult& result) {
    cout << "Tour length found: " << result.length << endl;

    cout << "Route: ";
    for (int i = 0; i < static_cast<int>(result.route.size()); i++) {
        cout << result.route[i] + 1 << " ";
    }

    if (!result.route.empty()) {
        cout << result.route[0] + 1;
    }

    cout << endl;
}