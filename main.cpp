//
//Created by Alex Heard on 11/30
//
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <limits>
#include <fstream>
#include <sstream>
#include <chrono>
#include "RtreeImpliment.h"
#include "IntervalTree.h";

using namespace std;

//readFile function in order to read from the csv files
void readIntervalFile(const string &_file, ifstream &file, IntervalTree itree){
    file.open(_file);
    string line;
    IntervalTree::Node *root = nullptr;
    if(!file.is_open()) {
        cout << "File is not open." << endl;
        exit(1);
    }
    while (getline(file, line)) {
        stringstream ss(line);
        string unixTime, flightNumber, elapsed;

        getline(ss, unixTime, ',');
        getline(ss, flightNumber, ',');
        getline(ss, elapsed, ',');

        int UT = stoi(unixTime);
        int elapsedTime = stoi(elapsed);
        pair<int, int> interval = {UT, UT + elapsedTime};

        itree.insert(interval, flightNumber);
    }
}

int main() {
    bool start = true;
    string input;
    int option = 0;
    FlightRTree rtree;
    IntervalTree itree;
    using time = chrono::time_point<chrono::system_clock, chrono::duration<double>>;
    using duration = std::chrono::duration<int64_t, std::ratio<1, 1000>>;
    time iTimeStart = time::min();
    time rTimeStart = time::min();
    time iTimeEnd = time::min();
    time rTimeEnd = time::min();
    duration iDuration = duration::min();
    duration rDuration = duration::min();
    cout << "Project 3: Flight Interval Trees\nCreated by Alexander Heard, Skyler Steiert, Carter Swarm\n----------------------------------------\n";
    while(start){
        cout << "Menu: \n1. Load File\n2. Display Tree Complexities\n3. Insert Interval\n4. Search Interval\n5. Quit";
        try {
            cin >> input;
            option = stoi(input);
            if (option == 1) {
                bool invalid = false;
                while (!invalid) {
                    try{
                        cout << "What file would you like to load?\n1. American Airlines departures from JFK 2021-2024\n2. American Airlines departures from JFK 2017-2024\n";
                        cin >> input;
                        option = stoi(input);
                    }
                    catch (const exception& f) {
                        cerr << "Invalid Input" << endl;
                        continue;
                    }
                    if(option == 1){
                        try{
                            rTimeStart = chrono::high_resolution_clock::now();
                            rtree.loadFromCSV("50000.csv");
                            rTimeEnd = chrono::high_resolution_clock::now();
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeStart - rTimeEnd);
                            ifstream file;
                            iTimeStart = chrono::high_resolution_clock::now();
                            readIntervalFile("50000.csv", file, itree);
                            iTimeEnd = chrono::high_resolution_clock::now();
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeStart - iTimeEnd);
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                    if(option == 2){
                        try{
                            auto start = high_resolution_clock::now();
                            rtree.loadFromCSV("100000.csv");
                            ifstream file;
                            readIntervalFile("100000.csv",file);
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                }
            }
            else if (option == 2) {
                cout << fixed;
                cout.precision(2);
                cout << "R-tree: \nTime Complexity: " << rDuration.count() << "\nSpace Complexity: " << sizeof(rtree) << endl;
                cout << "Interval-tree: \nTime Complexity: " << iDuration.count() << "\nSpace Complexity: " << sizeof(itree) << endl;
            }
            else if (option == 3) {
                string flightNumber;
                string start;
                double startD;
                int startI;
                string duration;
                double durationD;
                int durationI;
                pair<int, int> interval;
                bool flightBool = false;
                bool startBool = false;
                bool durationBool = false;
                //root = itree.insert(root, interval, flightNumber);
                while(!flightBool){
                    try{
                        cout << "Flight Number: ";
                        cin >> flightNumber;
                        flightBool = true;
                    }
                    catch(const exception& g){
                        cerr << "Invalid Flight Number" << endl;
                    }
                }
                while(!startBool){
                    try{
                        cout << "Start(Unix Time): ";
                        cin >> start;
                        startD = stod(start);
                        startI = stoi(start);
                        startBool = true;
                    }
                    catch(const exception& g){
                        cerr << "Invalid Unix Time" << endl;
                    }
                }
                while(!durationBool){
                    try{
                        cout << "Duration(Seconds): ";
                        cin >> duration;
                        durationD = stod(duration);
                        durationI = stoi(duration);
                        durationBool = true;
                    }
                    catch(const exception& g){
                        cerr << "Invalid Unix Time" << endl;
                    }
                }
                interval = make_pair(startI, startI + durationI);
                rtree.insert(flightNumber, startD, durationD);
                itree.insert(root, interval, flightNumber);
            }
            else if (option == 4) {

            }
            else if (option == 5) {
                start = false;
            }
            else{
                cout << "Invalid Input" << endl;
            }
        }
        catch (const exception& e) {
            cerr << "Invalid Input" << endl;
        }
    }
    destroyTree(root);
    return 0;
}