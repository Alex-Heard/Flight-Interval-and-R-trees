#ifndef INTERVALTREEP3_INTERVALTREE_H
#define INTERVALTREEP3_INTERVALTREE_H
#include <iostream>
#include <utility>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;
class IntervalTree {
private:
    struct Node {
        pair<int, int> interval;
        string flightNumber = "";
        int max;
        Node *left, *right;
        int height = 1; //ensures height starts correctly

        Node(pair<int, int> interval, const string& flightNumber)
                : interval(interval), flightNumber(flightNumber), max(interval.second),
                  left(nullptr), right(nullptr), height(1) {}
    };
    //creates interval tree root
    Node* root;
    //tracks height of different nodes
    int Height(Node *node) {
        if (node != nullptr) {
            return node->height;
        } else {
            return 0;
        }
    }
    //get the balance of a node for rebalancing purposes later
    int getBalance(Node *node) {
        if (node != nullptr) {
            return Height(node->left) - Height(node->right);
        } else {
            return 0;
        }
    }
//taking from lectcure 4 balance trees
    Node *leftRotation(Node *node) {
        Node *newParent = node->right;
        Node *temp = newParent->left;

        newParent->left = node;
        node->right = temp;

        node->height = max(Height(node->left), Height(node->right)) + 1;
        newParent->height = max(Height(newParent->left), Height(newParent->right)) + 1;

        newParent->max = max(newParent->interval.second, max(newParent->left->max, newParent->right->max));

        return newParent;
    }
//taking from lectcure 4 balance trees
    Node *rightRotation(Node *node) {
        Node *newParent = node->left;
        Node *temp = newParent->right;

        newParent->right = node;
        node->left = temp;

        node->height = max(Height(node->left), Height(node->right)) + 1;
        newParent->height = max(Height(newParent->left), Height(newParent->right)) + 1;

        return newParent;
    }
//insert nodes into the interval tree
    Node *insert(Node *root, pair<int, int> interval, const string &flightNumber) {
        //creates root if not already existing
        if (root == nullptr) {
            return new Node(interval, flightNumber);
        }
        //get the low of the root node to be able to compare for proper sorting for future node insertions
        int lowVal = root->interval.first;
        //sorting of node insertions
        if (interval.first < lowVal) {
            root->left = insert(root->left, interval, flightNumber);
        } else if (interval.first > lowVal) {
            root->right = insert(root->right, interval, flightNumber);
        }
        //updates the max value
        root->max = max(root->max, interval.second);

        int treesBalance = getBalance(root);

        //left left rotation
        if (treesBalance > 1 && interval.first < root->left->interval.first) {
            return rightRotation(root);
        }
        //right right rotation
        if (treesBalance < -1 && interval.first > root->right->interval.first) {
            return leftRotation(root);
        }
        //right left rotation
        if (treesBalance < -1 && interval.first < root->right->interval.first) {
            root->right = rightRotation(root->right);
            return leftRotation(root);
        }
        //left right rotation
        if (treesBalance > 1 && interval.first > root->left->interval.first) {
            root->left = leftRotation(root->left);
            return rightRotation(root);
        }
        //update of the height
        root->height = 1 + max(Height(root->left), Height(root->right));

        return root;
    }
    //taking search from mod 4 lecture BST Search and Insert
    void searchFlightNumber(Node* node, const string &flightNumber, vector<pair<int,int>> &intervals) {
        //if tree is empty there cannot be any findable flight number
        if(node == nullptr) {
            return;
        }
        else {
            //store the interval of the found flight
            string flight = node->flightNumber;
            if (flight == flightNumber) {
                intervals.push_back(node->interval);
            }
            //goes through left side of tree
            searchFlightNumber(node->left, flightNumber, intervals);
            //goes through right side of tree
            searchFlightNumber(node->right, flightNumber, intervals);
        }
    }
//from geeksforgeeks interval tree implementation
    bool doOverlap(pair<int, int> interval1, pair<int, int> interval2) {
        return (interval1.first <= interval2.second && interval2.first <= interval1.second);
    }
//from geeksforgeeks interval tree implementation
    void overlapSearch(Node *node, pair<int, int> comparisonInterval, vector<Node *> &overlaps) {
        //if tree is empty there cannot be any findable interval
        if (node == nullptr) {
            return;
        }
        //store the node of the found interval
        if (doOverlap(node->interval, comparisonInterval)) {
            overlaps.push_back(node);
        }
        //only searches left if the left node's max is greater than the comparisons first, saving time
        if (node->left != nullptr && node->left->max >= comparisonInterval.first) {
            overlapSearch(node->left, comparisonInterval, overlaps);
        }
        //first of interval has to be less to be valid
        if (comparisonInterval.first > comparisonInterval.second) {
            cout << "Error: Invalid interval." << endl;
            return;
        }
        //searches in the right
        overlapSearch(node->right, comparisonInterval, overlaps);
    }
//inorder traversal if needed
    void inorder(Node *node) {
        if (node == nullptr) {
            return;
        }
        inorder(node->left);
        cout << "[" << node->interval.first << ", " << node->interval.second << "]"
             << " (Flight: " << node->flightNumber << ") max = " << node->max << endl;
        inorder(node ->right);
    }
//amount of bytes used calculator
    size_t calculateSpace(Node *node) {
        //size is nothing if tree is empty
        if (node == nullptr) {
            return 0;
        } else {
            //sizeof calculates the bytes of what it takes in
            return sizeof(*node) + calculateSpace(node->left) + calculateSpace(node->right);
        }
    }
//destructor to clear potential memory leaks
    void destructor(Node *node) {
        //cant destroy if tree does not exist
        if (node == nullptr) {
            return;
        }
        destructor(node->left);
        destructor(node->right);
        delete node;
    }
public:
    //contructor
    IntervalTree() {
        root = nullptr;
    }
    //destructor
    ~IntervalTree() {
        destructor(root);
    }
    //insert method
    void insert(pair<int, int> interval, const string& flightNumber) {
        root = insert(root, interval, flightNumber);
    }
    //output a vector of all overlapping intervals
    vector<pair<int, int>> searchOverlaps(pair<int, int> comparisonInterval) {
        vector<Node *> overlaps;
        overlapSearch(root, comparisonInterval, overlaps);

        vector<pair<int, int>> results;
        for (Node *node: overlaps) {
            results.push_back(node->interval);
        }
        return results;
    }
    //outputs a flight info if found
    vector<pair<int,int>> searchFlight(const string& flightNumber) {
        vector<pair<int,int>> intervals;
        searchFlightNumber(root, flightNumber, intervals);
        if (!intervals.empty()) {
            cout << "Flight found: " << endl;
            cout << "Interval [" << root->interval.first << ", " << root->interval.second << "] (Flight: " << root->flightNumber << ")" << endl;
        }
        else {
            cout << "Flight " << flightNumber << " not found." << endl;
        }
        return intervals;
    }
    //gets the max value
    int getMax() {
        return root->max;
    }
    //gets the flight number
    string getFlightNumber(const pair<int, int>& interval) {
        Node* node = root;
        while (node != nullptr) {
            if (node->interval == interval) {
                return node->flightNumber;
            } else if (interval.first < node->interval.first) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
    }
//    void display() {
//        inorder(root);
//    }

    size_t calculateSpace() {
        return calculateSpace(root);
    }
};
#endif //INTERVALTREEP3_INTERVALTREE_H