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
#include <sys/time.h>

using namespace std;

int start(string _command);
void end();
void insertionSort(int length);
void select(string selection, int length);
void average(int length);

// Name of current record, bound to 20 characters
char name[20];

// Fitness data
int *fitnessData;
int isSorted = false;

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
			select(command, fitnessDataSize);
		} else if (command == commands[6]) {
			/* Average command called */
			average(fitnessDataSize);
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
	
	// Unset isSorted, since our potentially-sorted array is no longer in memory
	isSorted = false;
	
	// Print expected output
	cout << "End of processing fitness data for: " << name << '\n';
}

/* Insertion Sort
 *
 * <fitnessData> is sorted using Insertion Sort algorithm
 * Array is sorted in memory, and IS NOT saved back to <name>.txt fitness data file
 */
void insertionSort(int length) {
	int comparisons, swaps;
	double time; // Statistical values used to track algorithm efficency
	
	struct timeval begin, end; // Struct for gettimeofday call
	gettimeofday(&begin, NULL); // Record starting time of insertionsort
	
	// Base case
	if (length <= 1) {
		goto done; //break
	}
	
	int j, workingValue;
	// For every element after index 1
	for(int i = 1; i < length; i++)
	{
		// <workingValue> is element we are currently sorting
		workingValue = fitnessData[i];
		// For each occurence of the former element being larger then our <workingValue>
		for (j = i - 1; j >= 0 && fitnessData[j] > workingValue; j--)
		{
			// Increment our comparison counter
			++comparisons;
			// Shift element right
			fitnessData[j + 1] = fitnessData[j];
		}
		// Insert our <workingValue> to appropriate spot
		fitnessData[j + 1] = workingValue;
		// Increment our swaps counter
		++swaps;
	}
	
	// Array sorted successfully
	
	/* DEBUG
	 * View printout of our sorted array
	 cout << "Sorted array: \n";
	 for (int p = 0; p < length; p++) {
		cout << fitnessData[p] << '\n';
	 }
	 */
	gettimeofday(&end, NULL); // Record ending time of insertionsort
	
	// Sum of elapsed insertionsort time in ms
	time = ((end.tv_sec - begin.tv_sec) +
			((end.tv_usec - begin.tv_usec)/1000000.0))*1000;
	
	// Set isSorted, so we know <fitnessData> is sorted
	isSorted = true;
done:
	cout << "Number of comparisons made by insertion sort: " << comparisons << '\n';
	cout << "Number of swaps made by insertion sort: " << swaps << '\n';
	cout << "Time to run insertion sort (ms): " << time << '\n';
}

/* Select
 * selection: The index to select
 * lenght: The length of <fitnessData>
 *
 * Selects an item from a sorted array
 */
void select(string _selection, int length) {
	// If <fitnessData> is not sorted, throw error
	if (isSorted) {
		double time; // Statistical values used to track algorithm efficency
		struct timeval begin, end; // Struct for gettimeofday call
		gettimeofday(&begin, NULL); // Record starting time of insertionsort

		int result; // Store the selected item, print at end of logic tree, to increase codereuse
		
		/* Pull the command arguments from passed command */
		size_t pos = _selection.find(" "); 	// Find position of first space character
		string arg = _selection.substr(pos+1); 	//Substring after position to isolate command argument
		
		if (arg == "max") {
			// <max> is the last element in array
			int max = fitnessData[length - 1];
			// For all elements in <fitnessData>
			for (int i=length-2; i > 0; i--) {
				// If max is smaller then the compared element
				if (max < fitnessData[i]) {
					// Make the compared element the new max
					max = fitnessData[i];
				}
			}
			// Set <result> to computed max value
			result = max;
			
		} else if (arg == "min") {
			// <min> is the first element in array
			int min = fitnessData[0];
			// For all elements in <fitnessData>
			for (int i=1; i < length; i++) {
				// If min is greater then the compared element
				if (min > fitnessData[i]) {
					// Make the compared element the new min
					min = fitnessData[i];
				}
			}
			// Set <result> to computed min value
			result = min;
			
		} else if (arg == "median") {
			// <median> is the index of median element
			int median = (length - 1) / 2;
			// Set <result> to computed median element
			result = fitnessData[median];
		} else {
			int selectedIndex = stoi(arg);
			if (!(selectedIndex >= 1 && selectedIndex <= length)) {
				cerr << "Invalid selection.\n";
			} else {
				result = fitnessData[selectedIndex];
			}
		}
		
		gettimeofday(&end, NULL); // Record ending time of insertionsort
		
		// Sum of elapsed insertionsort time in ms
		time = ((end.tv_sec - begin.tv_sec) +
				((end.tv_usec - begin.tv_usec)/1000000.0))*1000;
		
		// Output statistics
		cout << "Selecting item: " << result << '\n';
		cout << "Time to run the selection (ms): " << time << '\n';
		
	} else {
		cerr << "Unable to select from an unsorted array.\n";
	}
}

/* Average
 * length: The length of <fitnessData>
 *
 * Computes the average of <fitnessData>
 */
void average(int length) {
	double time; // Statistical values used to track algorithm efficency
	struct timeval begin, end; // Struct for gettimeofday call
	gettimeofday(&begin, NULL); // Record starting time of insertionsort
	
	int stepsSum = 0; // Sum of all steps taken (stepsSum / length is Average Steps Per Day)
	
	for (int i = 0; i < length; ++i) {
		stepsSum +=fitnessData[i];
	}
	
	int avgDailySteps = stepsSum / length;
	
	gettimeofday(&end, NULL); // Record ending time of insertionsort

	// Sum of elapsed insertionsort time in ms
	time = ((end.tv_sec - begin.tv_sec) +
			((end.tv_usec - begin.tv_usec)/1000000.0))*1000;
	
	// Output statistics
	cout << "Average number of steps (parallel sum): " << avgDailySteps << '\n';
	cout << "Time to run the average (parallel sum) (ms): " << time << '\n';
}






