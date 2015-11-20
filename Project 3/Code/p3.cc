#include	<stdio.h>
#include	<iostream>
#include	<string>
#include	<sstream>
#include	<string.h>
#include	<list>

using namespace std;

/* Function Pre-Declaration */
void printList(list<int>* adjlist, int listLength);
void degreeDistribution();
void shortestPath(int s, int t);
void diameter();
void components();

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
		cin >> u >> v; cin.ignore(); // Read in adjacency list in form "u \t v"
		
//		cout << u << '\t' << v << endl; // Debug, reprints what was read in

		// If there are no self-loops, i.e., u != v, insert them into the graph.
		if (!(u == v)) {
			adjacencyList[u].push_back(v); // Add U:V, as given through input
			adjacencyList[v].push_back(u); // Add the opposite path, V:U, since this a non-directed adjacency graph
		}
	}
	
//	printList(adjacencyList, n); // Debug, prints the adjacency list
	
	int numOperations = 0;
	cin >> numOperations; cin.ignore(); // Read in the number of operations to perform
	
	// Perform <numOperations> given functions
	for (int o = 0; o < numOperations; o++) {
		string operation;
		cin >> operation; cin.ignore();
		
		// Perform appropriate operation
		if (operation == "degree-distribution") {
			degreeDistribution();
		} else if (operation == "shortest-path") {
			int s, t;
			cin >> s >> t; cin.ignore();
			shortestPath(s, t);
		} else if (operation == "diameter") {
			diameter();
		} else if (operation == "components") {
			components();
		} else {
			cout << "Invalid operation." << endl;
		}
	}
}

/* Prints the adjacency list, for debugging */
void printList(list<int>* adjacenyList, int listLength) {
	for(int i = 0; i < listLength; i++) {
		cout << i << " ";
		for(list<int>::iterator it = adjacenyList[i].begin(); it != adjacenyList[i].end(); ++it) {
			cout << *it << " ";
		}
		cout << endl;
	}
}

/* Prints the degree distribution of the graph as a histogram.
 *	Histogram counts the number of nodes of degree i, 0 ≤ i ≤ n.
 *	The node degree is given on the x axis, and the count is the y axis;
 *	if n = |V| is large, the node degree is  aggregated.
 */
void degreeDistribution() {

}

/* Prints the length and path taken of the shortest path in adjacency list between s and t.
 *  Uses Dijkstra’s algorithm to compute shortest path.
 *  S: source vertex
 *  V: destination vertex
 */
void shortestPath(int s, int t) {

}

/* Prints the diameter of the graph defined as the length of the longest shortest path.
 * Uses Floyd-Warshall all-pairs shortest path algorithm.
 */
void diameter() {

}

/* Prints the number of connected components in the graph and their size.
 * Size = number of vertices in each component.
 * Uses breadth-first search, marking and counting nodes as the search proceeds.
 * If any unmarked vertices exist, a new breadth-first search is started at unmarked node.
 * Repeats until all nodes are marked.
 */
void components() {

}