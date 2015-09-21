#include	<stdio.h>
#include	<stdlib.h>
#include	<omp.h>
#include	<sys/time.h>
#include	<string>
#include	<fstream>
#include	<iostream>

using namespace std;

int main()
{
	double time; // Statistical values used to track algorithm efficency
	struct timeval begin, end; // Struct for gettimeofday call
	int i, n, *a, local_sum, sum, tid;
	printf( "Example of the reduction construct\n" );
	printf( "Give an upper bound on the array a:\n" );
	scanf( "%d", &n );
	printf( "n = %d\n", n);

	if( ( a = (int * ) malloc( n * sizeof(int) ) ) == NULL )
		perror( "Error allocating a\n" );

	for( i = 0; i < n; i++ )
		a[i] = i;

	sum = 0;

	/* The reduction clause gets the OpenMP compiler to generate code that 	*/
	/* performs the summation in parallel and that the variable sum will 	*/
	/* the result of a reduction.											*/
	gettimeofday(&begin, NULL); // Record starting time of insertionsort

#pragma omp parallel for default(none) shared(n,a) reduction(+:sum) num_threads(2)
	for( i = 0; i < n; i++ )
		sum += a[i];
	/* -- End of parallel reduction -- */

	printf( "Sum should be n(n-1)/2 = %d\n", n*(n-1)/2 );
	printf( "Value of sum after parallel region: %d\n", sum );
	free(a);
	gettimeofday(&end, NULL); // Record ending time of insertionsort
	
	// Sum of elapsed insertionsort time in ms
	time = ((end.tv_sec - begin.tv_sec) +
			((end.tv_usec - begin.tv_usec)/1000000.0))*1000;
	cout << "Time to run the selection (ms): " << time << '\n';
	return(0);
}
