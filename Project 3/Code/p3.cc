#include	<stdio.h>
#include	<iostream>
#include	<string>
#include	<sstream>
#include	<string.h>
#include	<list>

using namespace std;

/* Function Pre Declaration */
void printList(list<int>* adjlist, int listLength);

/* Global Variables */
list<int> *adjacencyList; // Stores our vertex-edge adjacency list

int main()
{
	//	Graph G = (V,E) consists of an array A of n = |V| pointers to linked lists, one for each vertex in V. For each u ∈ V , the adjacency list A[u] contains all the vertices v such that there is an edge (u, v) ∈ E. That is, A[u] consists of linked list of all the vertices adjacent to u in G. The vertices in each adjacency list are typically stored in an arbitrary order.
	
	int n;	// The first line gives n = | V |, an array A of pointers to linked lists, one for each vertex in V. (# of vertices/nodes)
	cin >> n; cin.ignore(); // read n, clear buffer
	
	adjacencyList = new list<int>[n]; // Allocate <adjacencyList> with <n> vertices
	
	int m; // m = |E|, the number of edges in the graph. (# of paths/edges)
	cin >> m; cin.ignore(); // read m, clear buffer
	
	
	// Each of the next m lines gives an edge e = (u,v) ∈ E specified by its two endpoints.
	for (int i = 0; i < m; i++) {
		int u, v; // Endpoints of an edge
		cin >> u >> v; // Read in adjacency list in form "u \t v"
		
//		cout << u << '\t' << v << endl; // Debug, reprints what was read in

		// If there are no self-loops, i.e., u != v, insert them into the graph.
		if (!(u == v)) {
			adjacencyList[u].push_back(v); // Add U:V, as given through input
			adjacencyList[v].push_back(u); // Add the opposite path, V:U, since this a non-directed adjacency graph
		}
	}
	
//	printList(adjacencyList, n); // Debug, prints the adjacency list
	
	int numOperations = 0;
	cin >> numOperations; // Read in the number of operations to perform
	
	// Perform <numOperations> given functions
	for (int o = 0; o < numOperations; o++) {
	}
}

/* Prints the adjacency list, for debugging */
void printList(list<int>* adjlist, int listLength) {
//print the adjacency list (just for sanity check, not required in P3)
	for(int index = 0; index < listLength; index++){
		cout << index << " ";
		for(list<int>::iterator it = adjlist[index].begin(); it != adjlist[index].end(); ++it){
			cout << *it << " ";
		}
		cout << "\n";
	}
}