#include	<stdio.h>
#include	<iostream>
#include	<string>
#include	<sstream>
#include	<string.h>

using namespace std;

int main()
{
	/* Initialization of graph G 
	 
		Graph G = (V,E) consists of an array A of n = |V| pointers to linked lists, one for each vertex in V. For each u ∈ V , the adjacency list A[u] contains all the vertices v such that there is an edge (u, v) ∈ E. That is, A[u] consists of linked list of all the vertices adjacent to u in G. The vertices in each adjacency list are typically stored in an arbitrary order.
	*/
	
	int n;	// The first line gives n = | V |, an array A of pointers to linked lists, one for each vertex in V. (# of nodes)
	cin >> n; cin.ignore(); // read n, clear buffer
	
	int m; // m = |E|, the number of edges in the graph. (# of paths)
	cin >> m; cin.ignore(); // read m, clear buffer
	
	// Each of the next m lines gives an edge e = (u,v) ∈ E specified by its two endpoints.
	for (int i = 0; i < m; i++) {
		string line; // temporarily hold line from cin
		getline(cin, line);
		
		int u, v; // Endpoints of an edge
		size_t tabPos = line.find('\t');
		u = atoi(line.substr(0, tabPos).c_str());
		cout << u << endl;
		v = atoi(line.substr(tabPos, line.length()).c_str());
		cout << v << endl;
		
		// If there are no self-loops, i.e., u != v, insert them into the graph.
		if (!(u == v)) {
			
		}
	}
	
	
}