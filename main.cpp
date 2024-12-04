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
void readIntervalFile(const string &_file, IntervalTree iTree){
    ifstream file(_file);
    string line;
    //checks if file is not open
    if(!file.is_open()) {
        cout << "File is not open." << endl;
        exit(1);
    }
    //gets each line, parses it, and converts the values into a format that can be inserted intp the tree
    while (getline(file, line)) {
        stringstream ss(line);
        string unixTime, flightNumber, elapsed;

        getline(ss, unixTime, ',');
        getline(ss, flightNumber, ',');
        getline(ss, elapsed, ',');

        int UT = stoi(unixTime);
        int elapsedTime = stoi(elapsed);
        pair<int, int> interval = {UT, UT + elapsedTime};

        iTree.insert(interval, flightNumber);
    }
}

int main() {
    //variables
    bool start = true;
    string input;
    int option = 0;
    //initializes both trees
    FlightRTree rTree;
    IntervalTree iTree;
    //shortcuts for the time and duration objects
    using time = chrono::time_point<chrono::system_clock, chrono::duration<double>>;
    using duration = std::chrono::duration<int64_t, std::ratio<1, 1000>>;
    //setting these times to min in order to initialize them and update them later. This way they are local in the scope of the main so I can access the to determine the time complexity
    time iTimeStart = time::min();
    time rTimeStart = time::min();
    time iTimeEnd = time::min();
    time rTimeEnd = time::min();
    duration iDuration = duration::min();
    duration rDuration = duration::min();
    //display the menu options
    cout << "Project 3: Flight Interval Trees\nCreated by Alexander Heard, Skyler Steiert, Carter Swarm\n----------------------------------------\n";
    while(start){
        iTimeStart = time::min();
        rTimeStart = time::min();
        iTimeEnd = time::min();
        rTimeEnd = time::min();
        iDuration = duration::min();
        rDuration = duration::min();
        cout << "Menu: \n1. Load File\n2. Display Tree Complexities\n3. Insert Interval\n4. Search Interval\n5. Quit" << endl;
        //try/catch is meant to catch any errors with the stoi
        try {
            cin >> input;
            option = stoi(input);
            //if block meant to check the option value and catch any invalid inputs
            if (option == 1) {
                option = 0;
                bool valid = false;
                //while loops through the menu prompt until it recieves a valid input
                while (!valid) {
                    //try block used in case of an error with stoi
                    try{
                        cout << "What file would you like to load?\n1. American Airlines departures from JFK 2021-2024\n2. American Airlines departures from JFK 2017-2024\n";
                        cin >> input;
                        //reuses option to get the option for the load prompt
                        option = stoi(input);
                    }
                    catch (const exception& f) {
                        cerr << "Invalid Input" << endl;
                        continue;
                    }
                    if(option == 1){
                        //in case of error loading csv
                        try{
                            //stores start time
                            rTimeStart = chrono::high_resolution_clock::now();
                            //loads rtree
                            rTree.loadFromCSV("50000.csv");
                            //stores end time
                            rTimeEnd = chrono::high_resolution_clock::now();
                            //computes duration
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeStart - rTimeEnd);
                            //stores start time
                            iTimeStart = chrono::high_resolution_clock::now();
                            //loads itree
                            readIntervalFile("50000.csv", iTree);
                            //stores end time
                            iTimeEnd = chrono::high_resolution_clock::now();
                            //computes duration
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeStart - iTimeEnd);
                            //ends loop
                            valid = true;
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                    else if(option == 2){
                        try{
                            //same procedure as above but loading from different csv file
                            rTimeStart = chrono::high_resolution_clock::now();
                            rTree.loadFromCSV("100000.csv");
                            rTimeEnd = chrono::high_resolution_clock::now();
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeStart - rTimeEnd);
                            ifstream file;
                            iTimeStart = chrono::high_resolution_clock::now();
                            readIntervalFile("100000.csv", iTree);
                            iTimeEnd = chrono::high_resolution_clock::now();
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeStart - iTimeEnd);
                            valid = true;
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                }
            }
            else if (option == 2) {
                //setting precision so cout only has 2 decimal places
                cout << fixed;
                cout.precision(2);
                //computes time and space complexity using the space functions defined in the tree classes and the durations computed when building the trees
                cout << "R-tree: \nTime Complexity: " << rDuration.count() << "\nSpace Complexity: " << sizeof(rTree) << endl;
                cout << "Interval-tree: \nTime Complexity: " << iDuration.count() << "\nSpace Complexity: " << iTree.calculateSpace() << endl;
            }
            else if (option == 3) {
                //double and int required to insert the different inputs into the separate trees which take their own separate types
                string flightNumber;
                string startS;
                double startD;
                int startI;
                string duration;
                double durationD;
                int durationI;
                pair<int, int> interval;
                //bools to tell when to exit the loops prompting the user for input
                bool flightBool = false;
                bool startBool = false;
                bool durationBool = false;
                //root = iTree.insert(root, interval, flightNumber);
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
                        cin >> startS;
                        startD = stod(startS);
                        startI = stoi(startS);
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
                rTree.insert(flightNumber, startD, durationD);
                iTree.insert(interval, flightNumber);
            }
            else if (option == 4) {
                option = 0;
                //flightNumber is never used but used to make sure that input is a integer
                int flightNumber = 0;
                bool search = false;
                bool flightNum = false;
                while(!search) {
                    try {
                        cout << "Which tree would you like to search?\n1. Interval Tree\n2. R-tree" << endl;
                        cin >> input;
                        //reuses option to get the option for the load prompt
                        option = stoi(input);
                        search = true;
                    }
                    catch (const exception &f) {
                        cerr << "Invalid Input" << endl;
                        continue;
                    }
                }
                while(!flightNum){
                    try{
                        cout << "What flight number would you like to search: ";
                        cin >> input;
                        flightNumber = stoi(input);
                        flightNum = true;
                    }
                    catch (const exception& f) {
                        cerr << "Invalid Input" << endl;
                        continue;
                    }
                }
                //assumes search function will return a vector of flights in (Unix Start, Unix End)
                vector<pair<int,int>> flights;
                if(option == 1){
                    flights = iTree.searchFlight(input);
                }
                else if(option == 2){
                    //flights = rTree.searchFlight(input);
                }
                //cout statements for displaying the search vector
                cout << "Flight #" << input << " (Unix Start, Unix End):" << endl;
                for(pair<int,int> i: flights){
                    cout << "(" << i.first << ", " << i.second << "), ";
                }
                cout << endl;
            }
            //exit the start loop if option = 6
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
    return 0;
}