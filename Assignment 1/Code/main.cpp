//  main.cpp
//  A serial program that executes a sequence of commands that operate on individuals’ fitness data.
//
//  Valid milestone commands include:
//  Start, End, InsertionSort, Select, Average, and Exit
//
//  Created by Trent Rand

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    string command; // Command from current standard input line
    char commands[9][20] = {"Start", "End", "InsertionSort", "MergeSort", "ParallelMergeSort", "Select", "Average", "ParallelAverage", "Exit"};
    
    while (command != commands[8]) {
        // Set <command> to current line of standard input
        getline(cin, command);
        
        // Determine method to call in respect to <command>
        if(command.find(commands[0]) != std::string::npos) {
            // Start command called *****
            cout << commands[0] << "\n";
        } else if (command.find(commands[1]) != std::string::npos) {
            // End command called *****
            cout << commands[1] << "\n";
        } else if (command == commands[2]) {
            // InsertionSort command called *****
            cout << commands[2] << "\n";
        } else if (command == commands[3]) {
            // MergeSort command called
            cout << commands[3] << "\n";
        } else if (command == commands[4]) {
            // ParallelMergeSort command called
            cout << commands[4] << "\n";
        } else if (command.find(commands[5]) != std::string::npos) {
            // Select command called *****
            cout << commands[5] << "\n";
        } else if (command == commands[6]) {
            // Average command called *****
            cout << commands[6] << "\n";
        } else if (command == commands[7]) {
            // ParallelAverage command called
            cout << commands[7] << "\n";
        } else if (command == commands[8]) {
            // Exit command called *****
            cout << commands[8] << "\n";
            break;
        }
    }
    
    // Exit with status 0 - OK
    return 0;
}