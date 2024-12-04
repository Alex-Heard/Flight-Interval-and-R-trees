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
#include <thread>
#include "RtreeImpliment.h"
#include "IntervalTree.h";

using namespace std;

//readFile function in order to read from the csv files
void readIntervalFile(const string &_file, IntervalTree& iTree){
    ifstream file(_file);
    string line;
    //checks if file is not open
    if(!file.is_open()) {
        cout << "File is not open. in readIntervalTree" << endl;
        exit(1);
    }
    //Node* root;
    //gets each line, parses it, and converts the values into a format that can be inserted intp the tree
    getline(file, line);
    //cout << line << endl;
    while (getline(file, line)) {
        stringstream ss(line);
        string unixTime, flightNumber, elapsed;

        getline(ss, unixTime, ',');
        getline(ss, flightNumber, ',');
        getline(ss, elapsed, ',');

        int UT = stoi(unixTime);
        int elapsedTime = stoi(elapsed);
        pair<int, int> interval = {UT, UT + elapsedTime};
        //cout << "Start Insert: " << flightNumber << endl;
        iTree.insert(interval, flightNumber);
        //cout << "inserted: " << "UT start: " << interval.first << ", UT end: " << interval.second << ", Flight Number: " << flightNumber << endl;
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
    using duration = std::chrono::duration<int64_t, std::milli>;
    //setting these times to min in order to initialize them and update them later. This way they are local in the scope of the main so I can access the to determine the time complexity
    time iTimeStart = time{duration{0}};
    time rTimeStart = time{duration{0}};
    time iTimeEnd = time{duration{0}};
    time rTimeEnd = time{duration{0}};
    duration iDuration = duration{0};
    duration rDuration = duration{0};
    //display the menu options
    cout << "Project 3: Flight Interval Trees\nCreated by Alexander Heard, Skyler Steiert, Carter Swarm\n----------------------------------------------------------------------------\n";
    while(start){
        //cout << "R-tree Start Time" << rTimeStart.time_since_epoch().count() << ", I-tree Start Time" << iTimeStart.time_since_epoch().count() << endl;
        cout << "\nMenu: \n1. Load File\n2. Display Tree Complexities\n3. Insert Interval\n4. Search Interval\n5. Quit" << endl;
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
                        cout << "\nWhat file would you like to load?\n1. American Airlines departures from JFK 2021-2024\n2. American Airlines departures from JFK 2017-2024\n";
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
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeEnd - rTimeStart);
                            //stores start time
                            iTimeStart = chrono::high_resolution_clock::now();
                            //loads itree
                            readIntervalFile("50000.csv", iTree);
                            //stores end time
                            iTimeEnd = chrono::high_resolution_clock::now();
                            cout << "File Loaded" << endl;
                            //computes duration
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeEnd - iTimeStart);
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
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeEnd - rTimeStart);
                            ifstream file;
                            iTimeStart = chrono::high_resolution_clock::now();
                            readIntervalFile("100000.csv", iTree);
                            iTimeEnd = chrono::high_resolution_clock::now();
                            cout << "File Loaded" << endl;
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeEnd - iTimeStart);
                            valid = true;
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                    if(option == 3){
                        //in case of error loading csv
                        try{
                            //stores start time
                            rTimeStart = chrono::system_clock::now();
                            //cout << "R start time" << rTimeStart.time_since_epoch().count() << endl;
                            //loads rtree
                            rTree.loadFromCSV("25.csv");
                            //this_thread::sleep_for(std::chrono::seconds(2));
                            //stores end time
                            rTimeEnd = chrono::system_clock::now();
                            //cout << "R end time" << rTimeEnd.time_since_epoch().count() << endl;
                            //computes duration
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeEnd - rTimeStart);
                            //stores start time
                            iTimeStart = chrono::system_clock::now();
                            //cout << "I start time" << iTimeEnd.time_since_epoch().count() << endl;
                            //loads itree
                            readIntervalFile("25.csv", iTree);
                            //this_thread::sleep_for(std::chrono::seconds(2));
                            //stores end time
                            iTimeEnd = chrono::system_clock::now();
                            cout << "File Loaded" << endl;
                            //cout << "I end time" << iTimeEnd.time_since_epoch().count() << endl;
                            //computes duration
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeEnd - iTimeStart);
                            //ends loop
                            valid = true;
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                    if(option == 4){
                        //in case of error loading csv
                        try{
                            //stores start time
                            rTimeStart = chrono::high_resolution_clock::now();
                            //loads rtree
                            rTree.loadFromCSV("25000.csv");
                            //stores end time
                            rTimeEnd = chrono::high_resolution_clock::now();
                            //computes duration
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeEnd - rTimeStart);
                            //stores start time
                            iTimeStart = chrono::high_resolution_clock::now();
                            //loads itree
                            readIntervalFile("25000.csv", iTree);
                            //stores end time
                            iTimeEnd = chrono::high_resolution_clock::now();
                            cout << "File Loaded" << endl;
                            //computes duration
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeEnd - iTimeStart);
                            //ends loop
                            valid = true;
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                    if(option == 5){
                        //in case of error loading csv
                        try{
                            //stores start time
                            rTimeStart = chrono::high_resolution_clock::now();
                            //loads rtree
                            rTree.loadFromCSV("5000.csv");
                            //stores end time
                            rTimeEnd = chrono::high_resolution_clock::now();
                            //computes duration
                            rDuration = chrono::duration_cast<chrono::milliseconds>(rTimeEnd - rTimeStart);
                            //stores start time
                            iTimeStart = chrono::high_resolution_clock::now();
                            //loads itree
                            readIntervalFile("5000.csv", iTree);
                            //stores end time
                            iTimeEnd = chrono::high_resolution_clock::now();
                            cout << "File Loaded" << endl;
                            //computes duration
                            iDuration = chrono::duration_cast<chrono::milliseconds>(iTimeEnd - iTimeStart);
                            //ends loop
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
                cout << "\nR-tree: \nTime Complexity: " << rDuration.count() << " Milliseconds\nSpace Complexity: ";
                rTree.calculateSpace();
                cout << "\nInterval-tree: \nTime Complexity: " << iDuration.count() << " Milliseconds\nSpace Complexity: " << iTree.calculateSpace() << " Bytes" << endl;
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
                        cout << "\nFlight Number: ";
                        cin >> flightNumber;
                        flightBool = true;
                    }
                    catch(const exception& g){
                        cerr << "Invalid Flight Number" << endl;
                    }
                }
                while(!startBool){
                    try{
                        cout << "\nStart(Unix Time): ";
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
                        cout << "\nDuration(Seconds): ";
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
                        cout << "\nWhich tree would you like to search?\n1. Interval Tree\n2. R-tree" << endl;
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
                        cout << "\nWhat flight number would you like to search: ";
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
                vector<pair<int,int>> iFlights;
                if(option == 1){
                    iFlights = iTree.searchFlight(input);
                    //cout statements for displaying the search vector
                    cout << "\nFlight #" << input << " (Unix Start, Unix End):" << endl;
                    for(pair<int,int> i: iFlights){
                        cout << "(" << i.first << ", " << i.second << "), ";
                    }
                }
                else if(option == 2){
                    vector<FlightInterval> rFlights = rTree.searchByFlightNumber(input);
                    //cout statements for displaying the search vector
                    cout << "\nFlight #" << input << " (Unix Start, Unix End):" << endl;
                    for(FlightInterval i: rFlights){
                        cout << "(" << i.start << ", " << i.start + i.timeInAir << "), ";
                    }
                }
                cout << endl;
            }
            //exit the start loop if option = 6
            else if (option == 5) {
                start = false;
            }
            else if(option = 6){
                iTree.display();

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