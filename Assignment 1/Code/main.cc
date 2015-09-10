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
#include <vector>

using namespace std;

void start(string _command);
void end();
void insertionSort();

// Name of current record, bound to 20 characters
char name[20];

// Fitness data
vector <int> fitnessData;

int main(int argc, char** argv) {
	string command; // Command from current standard input line
	char commands[9][20] = {"Start", "End", "InsertionSort", "MergeSort", "ParallelMergeSort", "Select", "Average", "ParallelAverage", "Exit"};
	
	// Control flow: iterate commands until "Exit" is executed
	while (command != commands[8]) {
		// Set <command> to current line of standard input
		getline(cin, command);
		
		// Determine method to call in respect to <command>
		if(command.find(commands[0]) != std::string::npos) {
			/* Start command called */
			start(command);
		} else if (command.find(commands[1]) != std::string::npos) {
			/* End command called */
			end();
		} else if (command == commands[2]) {
			/* InsertionSort command called */
			insertionSort();
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
void start(string _command) {
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
	fitnessData.reserve(designatedDataSize);
	
	int temp;
	// Foreach line in fitness data, store to <fitnessData>
	for (int i=0; i<designatedDataSize; i++) {
		inf >> temp;
		fitnessData[i] = temp;
		cout << fitnessData[i] << '\n';
	}
	
	// Print expected output
	cout << "Processing fitness data of: " << name << '\n';
}

/* End
 *
 * Deallocate dynamic memory allocated for Name
 */
void end() {
	// Deallocate any used memory for name
	
	// Print expected output
	cout << "End of processing fitness data for: " << name << '\n';
}

/* Insertion Sort
 *
 * <fitnessData> is sorted using Insertion Sort algorithm
 */
void insertionSort() {
	int comparisons, swaps, time;
	
	cout << "Number of comparisons made by insertion sort: " << comparisons << '\n';
	cout << "Number of swaps made by insertion sort: " << swaps << '\n';
	cout << "Time to run insertion sort (ms): " << time << '\n';
}