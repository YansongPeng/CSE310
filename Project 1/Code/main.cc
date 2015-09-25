#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

using namespace std;

void start(string _command);
void end(string _command);
void insertionSort(int length);
void select(string selection, int length);
void average(int length);
void parallelAverage(int length);
int importFitnessData(string fileName);
void mergeSort(int left, int right);
void parallelMergeSort(int left, int right);
void merge(int left, int middle, int right);
double sectionAverage(int length, int leftBound);

struct timespec starttime, endtime, ttime; // Statistical values used to track algorithm efficency
double timeinterval; // Result of statistical time calculation

int comparisons, swaps; // Statiscal values to track sorting algorithms

// Name of current record, bound to 20 characters
char name[20];

// Fitness data
int *fitnessData;
int fitnessDataSize; // Size of fitnessData array
int isSorted = false;

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
			end(command);
		} else if (command == commands[2]) {
			/* InsertionSort command called */
			insertionSort(fitnessDataSize);
		} else if (command == commands[3]) {
			/* Read in fitness data file <name>.txt: allocate & assign array */
			importFitnessData(name);

			/* MergeSort command called */
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &starttime); // Get start time for statistics

			mergeSort(0, fitnessDataSize-1);

			// Get end time for statistics and compute elapsed time
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endtime);
			ttime.tv_sec = endtime.tv_sec - starttime.tv_sec;
			ttime.tv_nsec = endtime.tv_nsec - starttime.tv_nsec;
			timeinterval = ttime.tv_sec * 1000000000 + ttime.tv_nsec;

			// Output statistics
			cout << "Number of comparisons made by merge sort: " << comparisons << '\n';
			cout << "Number of swaps made by merge sort: " << swaps << '\n';
			cout << "Time to run merge sort(ms): " << (double) timeinterval/1000000 << '\n';

			swaps = 0;
			comparisons = 0;

			// Set isSorted, so we know <fitnessData> is sorted
			isSorted = true;
		} else if (command == commands[4]) {
			/* ParallelMergeSort command called */
			/* Read in fitness data file <name>.txt: allocate & assign array */
			importFitnessData(name);

			/* MergeSort command called */
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &starttime); // Get start time for statistics

			parallelMergeSort(0, fitnessDataSize-1);

			// Get end time for statistics and compute elapsed time
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endtime);
			ttime.tv_sec = endtime.tv_sec - starttime.tv_sec;
			ttime.tv_nsec = endtime.tv_nsec - starttime.tv_nsec;
			timeinterval = ttime.tv_sec * 1000000000 + ttime.tv_nsec;

			// Output statistics
			cout << "Number of comparisons made by parallel merge sort: " << comparisons << '\n';
			cout << "Number of swaps made by parallel merge sort: " << swaps << '\n';
			cout << "Time to run parallel merge sort(ms): " << (double) timeinterval/1000000 << '\n';

			swaps = 0;
			comparisons = 0;

			// Set isSorted, so we know <fitnessData> is sorted
			isSorted = true;
		} else if (command.find(commands[5]) != std::string::npos) {
			/* Select command called *****/
			select(command, fitnessDataSize);
		} else if (command == commands[6]) {
			/* Average command called */
			average(fitnessDataSize);
		} else if (command == commands[7]) {
			/* ParallelAverage command called */
			parallelAverage(fitnessDataSize);
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

	int result;
	result = importFitnessData(fileName);

	if (result != 0) {
		// Print expected output
		cout << "Processing fitness data of: " << arg << '\n';
	}
}

/* Import Fitness Data
 *
 * Read in fitness data to array
 */
int importFitnessData(string fileName) {
	// Read in <fileName> fitness data
	ifstream inf;
	inf.open(fileName.c_str());

	// If we couldn't open the input file stream for reading
	if (!inf)
	{
		// Print an error and exit
		cerr << "File (" << fileName << ") not found.\n";
		return 0;
	}

	// Read first line <fitnessDataSize>
	inf >> fitnessDataSize;

	// Allocate (C++ style) array to size <fitnessDataSize>
	fitnessData = new int[fitnessDataSize];

	// Foreach line in fitness data, store to <fitnessData>
	for (int i=0; i<fitnessDataSize; i++) {
		inf >> fitnessData[i];
	}

	// Unset isSorted, since our potentially sorted array is no longer in memory
	isSorted = false;

	return 1;
}


/* End
 *
 * Deallocate dynamic memory allocated for Name
 */
void end(string _command) {
	// Deallocate (C++ style) fitnessData
	delete fitnessData;

	// Find position of first space character
	size_t pos = _command.find(" ");
	//Substring after position to isolate command argument
	string arg = _command.substr(pos+1);

	// Unset isSorted, since our potentially sorted array is no longer in memory
	isSorted = false;

	// Print expected output
	cout << "End of processing fitness data for: " << arg << '\n';
}

/* Insertion Sort
 *
 * <fitnessData> is sorted using Insertion Sort algorithm
 * Array is sorted in memory, and IS NOT saved back to <name>.txt fitness data file
 */
void insertionSort(int length) {
	/* Read in fitness data file <name>.txt: allocate & assign array */
	importFitnessData(name);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &starttime); // Get start time for statistics

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

	// Get end time for statistics and compute elapsed time
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endtime);
	ttime.tv_sec = endtime.tv_sec - starttime.tv_sec;
	ttime.tv_nsec = endtime.tv_nsec - starttime.tv_nsec;
	timeinterval = ttime.tv_sec * 1000000000 + ttime.tv_nsec;

	// Set isSorted, so we know <fitnessData> is sorted
	isSorted = true;
done:
	cout << "Number of comparisons made by insertion sort: " << comparisons << '\n';
	cout << "Number of swaps made by insertion sort: " << swaps << '\n';
	cout << "Time to run insertion sort(ms): " << (double) timeinterval/1000000 << '\n';

	swaps = 0;
	comparisons = 0;
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
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &starttime); // Get start time for statistics

		int result; // Store the selected item, print at end of logic tree, to increase codereuse

		/* Pull the command arguments from passed command */
		size_t pos = _selection.find(" ");      // Find position of first space character
		string arg = _selection.substr(pos+1);  //Substring after position to isolate command argument

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
			double median = 0;
			if ((fitnessDataSize%2) == 0) {
				median = (fitnessData[fitnessDataSize/2] + fitnessData[(fitnessDataSize/2)-1])/2.0;
			}
			else {
				median=fitnessData[fitnessDataSize/2];
			}
			result = median;
		} else {
			int selectedIndex = atoi(arg.c_str());
			if (!(selectedIndex >= 1 && selectedIndex <= length)) {
				cerr << "Invalid selection.\n";
				return;
			} else {
				result = fitnessData[selectedIndex-1];
			}
		}

		// Get end time for statistics and compute elapsed time
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endtime);
		ttime.tv_sec = endtime.tv_sec - starttime.tv_sec;
		ttime.tv_nsec = endtime.tv_nsec - starttime.tv_nsec;
		timeinterval = ttime.tv_sec * 1000000000 + ttime.tv_nsec;

		// Output statistics
		cout << "Selecting item: " << result << '\n';
		cout << "Time to run the selection(ms): " << (double) timeinterval/1000000 << '\n';

	} else {
		cerr << "Unable to select from an unsorted array.\n";
	}
}

/* Merge Sort
 * left: index of left most element = 0
 * right: index of right most element: fitnessDataSize
 *
 * Sorts FitnessData array with divide-and-conquer merge sort
 */
void mergeSort(int left, int right){
	// FitnessData reallocation is done in main(), because of recursive structure of this method
	int middle = 0;

	if (left < right) {
		middle = floor((left + right) / 2);
		mergeSort(left, middle);
		mergeSort(middle + 1, right);
		merge(left, middle, right);
	}
}

/* Parallel Merge Sort
 * left: index of left most element = 0
 * right: index of right most element: fitnessDataSize
 *
 * Sorts FitnessData array with classic divide-and-conquer merge sort, but in parallel threads
 */
void parallelMergeSort(int left, int right)
{
	// Determine the location of the middle element in the fitnessDatator
	int middle = 0;


	if (left < right) {
		middle = floor((left + right) / 2);


		omp_set_num_threads(8);
#pragma omp parallel sections
		{	
#pragma omp section
			{
				mergeSort(left, middle);
			}
#pragma omp section
			{
				mergeSort(middle + 1, right);
			}		
		}	
		merge(left, middle, right);
	}
}

// Utility method: merge two halves of array
void merge(int left, int middle, int right){
	int i = 0, j = 0, k = 0, temporary[fitnessDataSize];

	i = left;
	k = left;
	j = middle + 1;

	while (i <= middle && j <= right) {
		comparisons++;
		if (fitnessData[i] <= fitnessData[j]) {
			temporary[k] = fitnessData[i];
			i++;
		} else {
			temporary[k] = fitnessData[j];
			j++;
		}
		k++;
	}

	while (i <= middle) {
		temporary[k] = fitnessData[i];
		k++;
		i++;
	}

	while (j <= right) {
		temporary[k] = fitnessData[j];
		k++;
		j++;
	}

	for (i = left; i < k; i++) {
		fitnessData[i] = temporary[i];
	}
	swaps++;
}

/* Average
 * length: The length of <fitnessData>
 *
 * Computes the average of <fitnessData>
 */
void average(int length) {
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &starttime); // Get start time for statistics

	int stepsSum = 0; // Sum of all steps taken (stepsSum / length is Average Steps Per Day)

	for (int i = 0; i < length; ++i) {
		stepsSum +=fitnessData[i];
	}

	int avgDailySteps = stepsSum / length;

	// Get end time for statistics and compute elapsed time
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endtime);
	ttime.tv_sec = endtime.tv_sec - starttime.tv_sec;
	ttime.tv_nsec = endtime.tv_nsec - starttime.tv_nsec;
	timeinterval = ttime.tv_sec * 1000000000 + ttime.tv_nsec;

	// Output statistics
	cout << "Average number of steps: " << avgDailySteps << '\n';
	cout << "Time to run the average(ms): " << (double) timeinterval/1000000 << '\n';
}

/* Average
 * length: The length of <fitnessData>
 *
 * Computes the average of <fitnessData> in parallel
 */
void parallelAverage(int length) {
	/* Read in fitness data file <name>.txt: allocate & assign array */
	importFitnessData(name);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &starttime); // Get start time for statistics

	double averageSum = 0;
	double averageL = 0.0;
	double averageR = 0.0;
	int middleElement = 0;

	middleElement = length-1 / 2;	// Finds half of the length

	omp_set_num_threads(4);
#pragma omp parallel sections		// Split fitnessData in half
	{
#pragma omp section
		{
			averageL = sectionAverage(middleElement, 0); // Recursively split and find average of half #1
		}
#pragma omp section
		{
			averageR = sectionAverage(length-1, middleElement + 1); // Recursively split and find average of half #2
		}
	}


	averageSum = averageL + averageR;

	// Get end time for statistics and compute elapsed time
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endtime);
	ttime.tv_sec = endtime.tv_sec - starttime.tv_sec;
	ttime.tv_nsec = endtime.tv_nsec - starttime.tv_nsec;
	timeinterval = ttime.tv_sec * 1000000000 + ttime.tv_nsec;

	// Set isSorted, so we know <fitnessData> is sorted
	isSorted = true;

	// Output statistics
	cout << "Average number of steps (parallel sum): " << averageSum << '\n';
	cout << "Time to run the average (parallel sum) (ms): " << (double) timeinterval/1000000 << '\n';
}

// Utility method for parallel average
double sectionAverage(int length, int leftBound) {
	double sum = 0;

	for (int i = leftBound; i < length; i++)
		sum = sum + fitnessData[i];

	return sum / length;
}
