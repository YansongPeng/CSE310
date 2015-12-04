#include	<stdio.h>
#include	<iostream>
#include	<string>
#include	<sstream>
#include	<cstdlib>
#include	<string.h>

using namespace std;

/* Function Pre-Declaration */
void graphDegreeDistribution(int degreeCount[], int listLength);

/* Adjacency List Node data structure (edge)
 * Linked List data structure for storing linked vertices
 */
struct adjacencyListNode
{
	int destination;
	struct adjacencyListNode *next;
};

/* Adjacency List Vertex data structure (vertex)
 * <AdjacencyList> consists of pointers to <n> adjacencyListVertex
 */
struct adjacencyListVertex
{
	struct adjacencyListNode *head;
};

/*
 * Class AdjacencyList
 */
class AdjacencyList
{
private:
	int size;
	struct adjacencyListVertex* A;
	bool *visited;
	int componentSize = 0;
	
public:
	/* Constructor:
	 * Allocates null pointer array of size <n>
	 */
	AdjacencyList(int n)
	{
		size = n;
		A = new adjacencyListVertex[n];
		for (int i = 0; i < size; ++i) {
			A[i].head = NULL; // Initialize linked list NULL
		}
	}
	
	/* Creates a new <AdjacencyListNode> linked list node.
	 * Destination: connected vertex [(u, v) edge]
	 * node.next is automatically set to null
	 */
	adjacencyListNode* newNode(int destination)
	{
		adjacencyListNode* node = new adjacencyListNode;
		node->destination = destination;
		node->next = NULL;
		return node;
	}
	
	/* Adds an edge to <AdjacencyList>.
	 * New linked list element (destination) is created and added as connected vertex (edge) to src
	 * Because adjacency graph is non-directed, we add the opposite edge as well
	 */
	void addEdge(int src, int destination)
	{
		adjacencyListNode* node = newNode(destination);
		node->next = A[src].head;
		A[src].head = node;
		node = newNode(src);
		node->next = A[destination].head;
		A[destination].head = node; // Add the opposite path, (vertex, u), since this a non-directed adjacency graph
		
	}
	
	/* Utility method to compute degrees of adjacencyList for graphing.
	 *	Degrees are passed by an integer array <verticesDegree>.
	 */
	void degreeDistribution() {
		int* degreeCount = new int[size]; // Store the count of each degree
		
		int vertex;
		
		// Compute degree of each vertex
		for (vertex = 0; vertex < size; vertex++) {
			int degreeSum = 0;
			
			// For each edge, add to <degreeSum>
			adjacencyListNode* iterator = A[vertex].head;
			while (iterator)
			{
				degreeSum++;
				iterator = iterator->next;
			}
			
			// Increment count of degree[<vertex>], showing that +1 vertex has this degree
			degreeCount[degreeSum] = degreeCount[degreeSum] + 1;
		}
		
		// DEBUG: show count of each degree (REMOVE BEFORE SUBMISSION)
		//	for (int i = 0; i < listLength; i++) {
		//		cout << "Degree of " << i << ": " << degreeCount[i] << "\n";
		//	}
		
		graphDegreeDistribution(degreeCount, size);
		
		// Memory Management
		delete [] degreeCount;
	}
	
	/* Prints the length and path taken of the shortest path in adjacency list between s and t.
	 *  Uses Dijkstra’s algorithm to compute shortest path.
	 *  S: source vertex
	 *  V: destination vertex
	 */
	void shortestPath(int s, int t) {
		cout << "Shortest Path operation will be implemented in complete project submission." << endl;
	}
	
	/* Prints the diameter of the graph defined as the length of the longest shortest path.
	 * Uses Floyd-Warshall all-pairs shortest path algorithm.
	 */
	void diameter() {
		cout << "Diameter operation will be implemented in complete project submission." << endl;
	}
	
	/* Prints the number of connected components in the graph and their size.
	 * Uses depth-first search, marking and counting nodes as the search proceeds.
	 * If any unmarked vertices exist, a new depth-first search is started at unmarked node.
	 * Repeats until all nodes are marked.
	 */
	void components() {
		int components = 0; // Store count of components
		int componentsSize[size]; // Store component size, with a maximum of <listLength> components
		
		visited = new bool[size]; // Track visited vertices
		
		// Initialize all vertices in <visited> to false
		for(int vertex = 0; vertex < size; vertex++) {
			visited[vertex] = false;
		}
		
		// depth-first search for connected components
		for (int vertex = 0; vertex < size; vertex++)
		{
			if (visited[vertex] == false)
			{
				depthFirstSearch(vertex, visited); // Recur for all adjacent vertices
				componentsSize[components] = componentSize; // Store component size sum from recursion processes
				componentSize = 0; // Reset component size counter
				components++; // Increment number of components, since one has finished recurring
			}
		}
		
		// Print the number of connected components in the graph and their size
		cout << "The number of connected components is " << components << endl;
		for (int i = 0; i<components; i++) {
			cout << "Component " << i << " has " << componentsSize[i] << " vertices." << endl;
		}
		
		// Memory Management
		delete [] visited;
	}
	
	/* Utility method for Depth First Search of adjacency list.
	 * Marks node <vertex> as visited and prints
	 */
	void depthFirstSearch(int vertex, bool visited[]) {
		visited[vertex] = true; // Set vertices as visited
		componentSize++; // Increase size count of component
		
		//	cout << "Visiting: " << vertex << endl; // Debug, see where the depth first search visits
		
		adjacencyListNode* iterator = A[vertex].head;
		while (iterator)
		{
			if (visited[iterator->destination] == false) {
				depthFirstSearch(iterator->destination, visited); // Recurse the tree at all non-visited vertices
			}
			iterator = iterator->next;
		}
		
	}
	
	// Debug: Print graph structure
	void printGraph()
	{
		int vertex;
		
		cout << "Adjacency List:" << endl;
		for (vertex = 0; vertex < size; ++vertex)
		{
			adjacencyListNode* iterator = A[vertex].head;
			cout << vertex << ": ";
			while (iterator)
			{
				cout << iterator->destination << ". ";
				iterator = iterator->next;
			}
			cout<<endl;
		}
		cout<<endl;
	}
};

/* Prints the degree distribution of the graph as a histogram.
 *	Histogram counts the number of nodes of degree i, 0 ≤ i ≤ n.
 *	The node degree is given on the x-axis, and the count is the y-axis;
 *	if n = |V| is large, the node degree is aggregated.
 */
void graphDegreeDistribution(int degreeCount[], int listLength) {
	int xMax = listLength; // x-axis trim modifier
	int yMax = 0; // y-axis trim modifier
	
	// Trim graph x-axis from <listLength> to 0
	for (int  i = listLength; i > 0; i--) {
		if (degreeCount[i] == 0) {
			xMax--;
		} else if (degreeCount[i] > yMax) {
			yMax = degreeCount[i]; // Keep track of maximum frequency of all degrees, for y-axis trimming
		}
	}
	
	// Compensate for index starting at 0
	xMax++;
	
	//	cout << "xMax: " << xMax << endl; // DEBUG, show maximum degree
	//	cout << "yMax: " << yMax << endl; // DEBUG, show maximum frequency of all degrees
	
	// Graph degree distribution as histogram
	for (int y = yMax; y > 0; y--) {
		// Display count on y-axis
		cout << y << '\t';
		for (int x = 0; x < xMax; x++) {
			if (degreeCount[x] >= y) {
				// Draw a bar
				cout << "\u2588";
				//				cout << 'X'; // REPLACE WITH THIS IF GRAPH IS UNREADABLE
			} else {
				cout << ' ';
			}
		}
		cout << endl;
	}
	
	// Display node degree on x-axis
	cout << '\t';
	for (int x = 0; x < xMax; x++) {
		cout << x;
	}
	cout << endl;
	cout << "Count/Node Degree" << endl;
}


// P3: Main
int main()
{
	//	Graph G = (V,E) consists of an AdjacencyList <adjList> of <n> = |V| pointers to linked lists, one for each vertex in V. For each u ∈ V , the AdjacencyList adjList[u] contains all the vertices v such that there is an edge (u, v) ∈ E. That is, adjList[u] consists of linked list of all the vertices adjacent to u in G. The vertices in each adjacency list are typically stored in an arbitrary order.
	
	int n;	// The first line gives n = | V |, an array A of pointers to linked lists, one for each vertex in V. (# of vertices/nodes)
	cin >> n; cin.ignore(); // Read n, clear buffer
	
	AdjacencyList adjList(n); // Initialize Graph <adjList> with <n> vertices
	
	int m; // m = |E|, the number of edges in the graph. (# of paths/edges)
	cin >> m; cin.ignore(); // Read m, clear buffer
	
	// Each of the next m lines gives an edge e = (u,v) ∈ E specified by its two endpoints.
	for (int i = 0; i < m; i++) {
		int u, v; // Endpoints of an edge
		cin >> u >> v; cin.ignore(); // Read in adjacency list in form "u \t v"
		
		// cout << u << '\t' << v << endl; // DEBUG: reprints what was read in (REMOVE BEFORE SUBMISSION)
		
		// If there are no self-loops, i.e., u != v, insert them into the graph.
		if (!(u == v)) {
			adjList.addEdge(u, v); // Add (u, v), as given through input
			// addEdge() automatically adds the opposite path, (v, u), since this a non-directed adjacency graph
		}
	}
 
	// DEBUG: print graph (REMOVE BEFORE SUBMISSION)
	//		adjList.printGraph();
	
	int numOperations = 0;
	cin >> numOperations; cin.ignore(); // Read in the number of operations to perform
	
	// Perform <numOperations> given functions
	for (int op = 0; op < numOperations; op++) {
		string operation;
		cin >> operation; cin.ignore();
		
		// Perform appropriate operation
		if (operation == "degree-distribution") {
			adjList.degreeDistribution();
		} else if (operation == "shortest-path") {
			int s, t;
			cin >> s >> t; cin.ignore();
			adjList.shortestPath(s, t);
		} else if (operation == "diameter") {
			//			diameter();
		} else if (operation == "components") {
			adjList.components();
		} else {
			cout << "Invalid operation." << endl;
		}
	}
	
	// Memory Management for main()
	// TODO
	
	return 0;
}
