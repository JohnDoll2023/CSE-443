// Copyright (C) 2021 John Doll

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;

using StrVec = vector<vector<string>>;

vector<string> split(istringstream& ifs) {
    vector<string> row;
    // splits the line into different values and adds them to a vector
    for (string value; ifs >> quoted(value);) {
        row.push_back(value);
    }
    return row;
}

StrVec parseFile(ifstream& is) {
    // create variables 
    StrVec data;
    string line;
    // read through each line of the file
    while (getline(is, line)) {
        istringstream iss(line);
        vector<string> row = split(iss);
        // adds row vector to vector of rows
        data.push_back(row);
    }
    return data;
}

int convertColNames(string argument, StrVec data) {
    // convert column argument names to numbers by finding it in the vectors
    for (size_t i = 0; i < data.size(); i++) {
        if (data[0][i] == argument) {
            return i;
        }
    }
    return -1;
}

void printData(StrVec data, vector<int> colNums, string filter) {
    // loop through every column
    for (size_t i = 0; i < data.size(); i++) {
        // loop through every selected row
        for (size_t j = 0; j < colNums.size(); j++) {
            if (data[i][colNums[0]].find(filter) != string::npos || 
                filter == "" || i == 0) {
                // print entry, add tab if not end of row
                cout << data[i][colNums[j]];
                if (j + 1 != colNums.size()) {
                    cout << "\t";
                }
            }
        }
        // checks to see if row was printed, prints endl if true
        if (data[i][colNums[0]].find(filter) != string::npos || filter == "" ||
            i == 0) {
            cout << endl; 
        }
    }
}

vector<int> columnsChosen(StrVec data) {
    // adds every column to the column selector list
    vector<int> cols;
    for (size_t i = 0; i < data[0].size(); i++) {
        cols.push_back(i);
    }
    return cols;
}

int main(int argc, char *argv[]) {
    // create stream and check if it is good
    ifstream is(argv[1]);
    if (is.good()) {
        // create vector of vectors of strings to hold our data
        StrVec data;
        // create cols to select which columns we want to return
        vector<int> cols;
        // read in data
        data = parseFile(is);
        // get command to search the data with
        string command = argv[2];
        // check if non filter command was inputted
        bool multiCol = (argc >= 5 || command != "--filter") ? true : false;
        // check if two commands were used
        string command2 = command == "--filter" ? argv[4] : "";
        // start for loop at 5 if a --filter was entered and three otherwise
        int argumentIterator = command == "--filter" ? 5 : 3;
        // if entered command is --cols, then get convert col names to ints
        // in order to filter by them when printing
        if (command == "--cols" || command2 == "--cols") {
            for (int i = argumentIterator; i < argc; i++) {
                cols.push_back(convertColNames(argv[i], data));
            }
        } else if (command == "--colnums" || command2 == "--colnums") {
            // add col numbers to col vector to filter by them when printing
            for (int i = argumentIterator; i < argc; i++) {
                cols.push_back(stoi(argv[i]));
            }
        }
        // set cols to include all cols if not cols were specified
        cols = multiCol ? cols : columnsChosen(data);
        // set filter string
        string filter = command == "--filter" ? argv[3] : "";
        // print
        printData(data, cols, filter);
    }
    return 0;
}

// End of source code
