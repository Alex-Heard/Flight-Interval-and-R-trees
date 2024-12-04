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
        string flightNumber;
        int max;
        Node *left, *right;
        int height = 1; //ensures height starts correctly

        Node(pair<int, int> interval, const string& flightNumber)
                : interval(interval), flightNumber(flightNumber), max(interval.second),
                  left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;

    //tracks height of different nodes
    int Height(Node *node) {
        if (node != nullptr) {
            return node->height;
        } else {
            return 0;
        }
    }

    int getBalance(Node *node) {
        if (node != nullptr) {
            return Height(node->left) - Height(node->right);
        } else {
            return 0;
        }
    }

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
        //creates root if not already exiisting
        if (root == nullptr) {
            return new Node(interval, flightNumber);
        }
        //get the low of the root node to be able to compare for proper sorting for future node insertions
        int lowVal = root->interval.first;

        if (interval.first < lowVal) {
            root->left = insert(root->left, interval, flightNumber);
        } else if (interval.first > lowVal) {
            root->right = insert(root->right, interval, flightNumber);
        }
        //does this update correctly?? i think so but have to draw it out
        root->max = max(root->max, interval.second);
        //store the intervals so we check overlapping intervals later
        //root->storageOfIntervals.push_back(interval);

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
    void searchFlightNumber(Node* node, string flightNumber, vector<pair<int,int>> intervals) {
        if(node == nullptr) {
            return;
        }
        //goes through left side of tree
        if(node->flightNumber == flightNumber) {
            intervals.push_back(node->interval);
        }

        searchFlightNumber(node->left, flightNumber, intervals);
        searchFlightNumber(node->right, flightNumber, intervals);
    }
//from geeksforgeeks interval tree implementation
    bool doOverlap(pair<int, int> interval1, pair<int, int> interval2) {
        return (interval1.first <= interval2.second && interval2.first <= interval1.second);
    }

//from geeksforgeeks interval tree implementation
    void overlapSearch(Node *root, pair<int, int> comparisonInterval, vector<Node *> &overlaps) {
        if (root == nullptr) {
            return;
        }
        if (doOverlap(root->interval, comparisonInterval)) {
            overlaps.push_back(root);
        }

        if (root->left != nullptr && root->left->max >= comparisonInterval.first) {
            overlapSearch(root->left, comparisonInterval, overlaps);
        }
        if (comparisonInterval.first > comparisonInterval.second) {
            cout << "Error: Invalid interval." << endl;
            return;
        }
        overlapSearch(root->right, comparisonInterval, overlaps);
    }

//inorder traversal if needed
    void inorder(Node *root) {
        if (root == nullptr) {
            return;
        }
        inorder(root->left);
        cout << "[" << root->interval.first << ", " << root->interval.second << "]"
             << " (Flight: " << root->flightNumber << ") max = " << root->max << endl;
        inorder(root->right);
    }

//amount of bytes used calculator
    size_t calculateSpace(Node *root) {
        if (root == nullptr) {
            return 0;
        } else {
            return sizeof(*root) + calculateSpace(root->left) + calculateSpace(root->right);
        }
    }

//destructor to clear potential memory leaks
    void destructor(Node *root) {
        if (root == nullptr) {
            return;
        }
        destructor(root->left);
        destructor(root->right);
        delete root;
    }
public:
    IntervalTree() {
        root = nullptr;
    }

    ~IntervalTree() {
        destructor(root);
    }

    void insert(pair<int, int> interval, const string& flightNumber) {
        root = insert(root, interval, flightNumber);
    }

    vector<pair<int, int>> searchOverlaps(pair<int, int> comparisonInterval) {
        vector<Node *> overlaps;
        overlapSearch(root, comparisonInterval, overlaps);

        vector<pair<int, int>> results;
        for (Node *node: overlaps) {
            results.push_back(node->interval);
        }
        return results;
    }

    vector<pair<int,int>> searchFlight(const string& flightNumber) {
        vector<pair<int,int>> intervals;
        searchFlightNumber(root, flightNumber, intervals);
        if(intervals.empty()) {
            cout << "Flight " << flightNumber << " not found." << endl;
        }
        else {
            cout << "Flight found: " << endl;
            cout << "Interval [" << root->interval.first << ", " << root->interval.second << "] (Flight: " << root->flightNumber << ")" << endl;
        }
        return intervals;
    }
    int getMax() {
        return root->max;
    }
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