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
#include "RtreeImpliment.h"

using namespace std;

//readFile function in order to read from the csv files
void readIntervalFile(const string &_file, fstream &file){
    //Variables
    string line;
    string lineNum;
    file.open(_file, ios::in);
    if(file.is_open()){
        getline(file, lineNum);
        //read pixels
        for(int i = 0; i < (width*height); i++) {
            file.read(reinterpret_cast<char *>(&pixel_char_data), 1);
            temp_pixel.setBlue(pixel_char_data);
            file.read(reinterpret_cast<char *>(&pixel_char_data), 1);
            temp_pixel.setGreen(pixel_char_data);
            file.read(reinterpret_cast<char *>(&pixel_char_data), 1);
            temp_pixel.setRed(pixel_char_data);
            _image.addImageData(temp_pixel);
        }
        file.close();
    }
}

int main() {
    bool start = true;
    string input;
    int option = 0;
    FlightRTree rtree;
    IntervalTree itree;
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
                            rtree.loadFromCSV("50000.csv");
                            itree.loadFromCSV("50000.csv");
                        }
                        catch (const exception& g) {
                            cerr << "Error loading CSV File: " << g.what() << endl;
                            return 1;
                        }
                    }
                    if(option == 2){
                        try{
                            rtree.loadFromCSV("100000.csv");
                            itree.loadFromCSV("100000.csv");
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
                cout << "R-tree: \nTIme Complexity: " << rtree.time() << "Space Complexity: " << sizeof(rtree) << endl;
                cout << "Interval-tree: \nTIme Complexity: " << itree.time() << "Space Complexity: " << sizeof(itree) << endl;
            }
            else if (option == 3) {

            }
            else if (option == 4) {

            }
            else if (option == 5) {

            }
            else if (option == 6) {
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
//try {
//        tree.loadFromCSV("flights3.csv");
//        //tree.loadFromCSV("flights.csv");
//        cout << fixed;
//        cout.precision(0);
//        tree.traverse();
//        vector<string> searchTimes = {"1609443000", "1609448000", "1609453000", "1483249022"};
//        for (auto it = searchTimes.begin(); it != searchTimes.end(); ++it) {
//            const auto& timeStr = *it;
//            double queryTime = stod(timeStr);
//            cout << "\nFlights at time " << queryTime << ":" << endl;
//            auto results = tree.search(queryTime);
//            for (const auto& flight : results) {
//                cout << "- Flight " << flight.flightNumb << " [" << flight.start << ", " << flight.getTotalTime() << "]" << endl;
//            }
//        }
//    } catch (const exception& e) {
//        cerr << "Error loading CSV File: " << e.what() << endl;
//        return 1;
//    }
