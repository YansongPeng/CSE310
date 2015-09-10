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
#include <fstream>

using namespace std;

int start(string _command);
void end();
void insertionSort(int length);

// Name of current record, bound to 20 characters
char name[20];

// Fitness data
int *fitnessData;
int sizeOfFitnessData;

int main(int argc, char** argv) {
    string command; // Command from current standard input line
	int fitnessDataSize; // Size of fitnessData array
    char commands[9][20] = {"Start", "End", "InsertionSort", "MergeSort", "ParallelMergeSort", "Select", "Average", "ParallelAverage", "Exit"};
    
    // Control flow: iterate commands until "Exit" is executed
    while (command != commands[8]) {
        // Set <command> to current line of standard input
        getline(cin, command);
        
        // Determine method to call in respect to <command>
        if(command.find(commands[0]) != std::string::npos) {
            /* Start command called */
            fitnessDataSize = start(command);
        } else if (command.find(commands[1]) != std::string::npos) {
            /* End command called */
            end();
        } else if (command == commands[2]) {
            /* InsertionSort command called */
            insertionSort(fitnessDataSize);
        } else if (command == commands[3]) {
            /* MergeSort command called */
        } else if (command == commands[4]) {
            /* ParallelMergeSort command called */
        } else if (command.find(commands[5]) != std::string::npos) {
            /* Select command called *****/
        } else if (command == commands[6]) {
            /* Average command called *****/
        } else if (command == commands[7]) {
            /* ParallelAverage command called */
        } else if (command == commands[8]) {
            /* Exit command called */
            cout << "Program terminating.\n";
            break;
        }
    }

    // Exit with status 0 - OK
    return 0;
}

/* Start
 * _command: Necessary to extract argument from command
 * 
 * Extracts and assigns <name> from line command
 * Allocates array of specified size and stores fitness data
 */
int start(string _command) {
    // Find position of first space character
    size_t pos = _command.find(" ");
    //Substring after position to isolate command argument
    string arg = _command.substr(pos+1);
	
    // Assign argument _name to global <name>
    strcpy(name, arg.c_str());
	
	/* Read in fitness data file <name>.txt: allocate & assign array */
	char *fileName = name;
	strcat(fileName, ".txt");
	
	// Read in <fileName> fitness data
	ifstream inf(fileName);
	
	// If we couldn't open the input file stream for reading
	if (!inf)
	{
		// Print an error and exit
		cerr << "File (" << fileName << ") not found.\n";
		exit(1);
	}
	
	// Read first line <designatedDataSize>
	int designatedDataSize;
	inf >> designatedDataSize;
	
	// Allocate (C++ style) array to size <designatedDataSize>
	fitnessData = new int[designatedDataSize];
	
	// Foreach line in fitness data, store to <fitnessData>
	for (int i=0; i<designatedDataSize; i++) {
		inf >> fitnessData[i];
	}
	
	// Print expected output
	cout << "Processing fitness data of: " << name << '\n';
	
	return designatedDataSize;
}

/* End
 *
 * Deallocate dynamic memory allocated for Name
 */
void end() {
	// Deallocate (C++ style) fitnessData
	delete fitnessData;
	
    // Print expected output
    cout << "End of processing fitness data for: " << name << '\n';
}

/* Insertion Sort
 *
 * <fitnessData> is sorted using Insertion Sort algorithm
 
 */
void insertionSort(int length) {
	int comparisons, swaps, time;
	
	// Base case
	if (length <= 1) {
		goto done; //break
	}
	
	// Start at index 1
	for (int i = 1; i<length; i++) {
		int temp = fitnessData[i];
		int j=i;
		while (j > 0 && temp < fitnessData[j-1]) {
			// Shift element
			fitnessData[j] = fitnessData[j - 1];
			--j;
			// Element compared, increase counter
		}
		//Swap element with temp
		fitnessData[j] = temp;
		// Element swapped, increase counter
		++swaps;
	}
	cout << "Sorted array: \n";
	for (int p = 0; p < length; p++) {
		cout << fitnessData[p] << '\n';
	}
	
	
done:
	cout << "Number of comparisons made by insertion sort: " << comparisons << '\n';
	cout << "Number of swaps made by insertion sort: " << swaps << '\n';
	cout << "Time to run insertion sort (ms): " << time << '\n';
}
