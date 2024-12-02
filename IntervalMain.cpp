#include <iostream>
#include <utility>
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono>
#include <climits>
#include "IntervalTree.h"
using namespace std;

int main() {
    using namespace chrono;
    ifstream file("data - Sheet1.csv");
    string line;
    IntervalTree IT;

    if(!file.is_open()) {
        cout << "File is not open." << endl;
        exit(1);
    }

    auto start = high_resolution_clock::now();

    while (getline(file, line)) {
        stringstream ss(line);
        string unixTime, flightNumber, elapsed;
        //take in first line that has number of lines

        getline(ss, unixTime, ',');
        getline(ss, flightNumber, ',');
        getline(ss, elapsed, ',');

        int UT = stoi(unixTime);
        int elapsedTime = stoi(elapsed);
        pair<int, int> interval = {UT, UT + elapsedTime};

        IT.insert(interval, flightNumber);
    }
    auto end = high_resolution_clock::now();
    //cout << "Inorder traversal Interval Tree:\n";
    //inorder(root);
    start = high_resolution_clock::now();
    int max = IT.getMax();
    cout << "All Overlaps: " << endl;

    for (const auto& interval : IT.searchOverlaps({0, max})) {
        vector<pair<int, int>> overlaps = IT.searchOverlaps(interval);

        cout << "Interval [" << interval.first << ", " << interval.second << "] overlaps with:" << endl;

        for (const auto& overlap : overlaps) {
            if (overlap != interval) {
                cout << "  [" << overlap.first << ", " << overlap.second << "] (Flight: "
                     << IT.getFlightNumber(overlap) << ")" << endl;
            }
        }
    }
    end = high_resolution_clock::now();
    auto duration = duration_cast<seconds >(end - start);

    cout << "Bytes: " << IT.calculateSpace() << endl;
    cout << "Time: " << duration.count() << " seconds" << endl;

    return 0;
}



