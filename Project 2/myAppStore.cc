#include	<stdio.h>
#include	<iostream>
#include	<string>
#include	<sstream>
#include	<tgmath.h>

using namespace std;

#define	CAT_NAME_LEN	25
#define	APP_NAME_LEN	50
#define	VERSION_LEN	10
#define	UNIT_SIZE	3

struct categories{
	char category[ CAT_NAME_LEN ]; // Name of category
	struct tree* root;  // Pointer to root of search tree for this category
};

struct app_info{
	char category[ CAT_NAME_LEN ]; // Name of category
	char app_name[ APP_NAME_LEN ]; // Name of the application
	char version[ VERSION_LEN ]; // Version number
	float size; // Size of the application
	char units[ UNIT_SIZE ]; // GB or MB
	float price; // Price in $ of the application
};

struct tree{ // A binary search tree
	struct app_info app; // Information about the application
	struct tree* left;  // Pointer to the left subtree
	struct tree* right;  // Pointer to the right subtree
};

struct hash_table_entry{
	char app_name[ APP_NAME_LEN ]; // Name of the application
	struct tree* app_node; // Pointer to node in tree containing the application information
};

// This definition of the tree is to be used for the full project deadline.
/*
 struct tree{ // A 2-3 tree
 // In the case of a 2-node, only app_info and the left and right subtrees fields are used
 //    - app_info2 is empty and mid is NULL
 // In the case of a 3-node, all fields are used
 
	struct app_info app; // Information about the application
 struct app_info app2; // Information about the second application
	struct tree* left;  // Pointer to the left subtree; elements with keys < the key of app_info
	struct tree* mid; // Pointer to the middle subtree; elements with keys > key of app_info and < key app_info2
	struct tree* right;  // Pointer to the right subtree; elements with keys > key of app_info2
 struct tree* parent; // A pointer to the parent may be useful
 };
*/

// Allocate an array of size n of type struct categories
struct categories* categoryList;
int categoryListLength;

// Allocate a hash table of size (first prime number after 2*<numberOfApplications>), keyed by <app_name>
struct hash_table_entry* hashTable;
int hashTableLength;

// Function forward declaration
void insert(tree* node, tree** root);
void printTree(struct tree* root);
int hashASCII(char* appName);
void insertHashEntry(tree* node);
int nextPrimeNumber(int min);

int main()
{
	// Allocate an array of size n of type struct categories
	cin >> categoryListLength;
	categoryList = new categories[categoryListLength]; //TODO memory management
	
	// Clear input stream, fixes line 73 skipping one input
	cin.ignore();
	
	// Allocate an array of size n of type struct categories
	for (int i = 0; i < categoryListLength; i++) {
		string title;
		getline(cin, title);
		strcpy(categoryList[i].category, title.c_str());
		// For safety, set all <root> nodes to NULL
		categoryList[i].root = NULL;
	}
	
	// Populate myAppStore categories BST dwith m applications
	int numberOfApplications;
	cin >> numberOfApplications;
	
	// Intialize <hashTable> to size k that is the first prime number greater than 2 * <numberOfApplications>.
	hashTableLength = nextPrimeNumber(2*numberOfApplications);
	hashTable = new hash_table_entry[hashTableLength]; //TODO memory management
	
	// For safety, set all app_nodes in <hashTable> to NULL
	for (int i = 0; i < hashTableLength; i++) {
		hashTable[i].app_node = NULL;
	}
	
	for (int i = 0; i<numberOfApplications; i++) {
		struct tree* node = new tree();
		
		printf("Enter category for application %d\n", i); //TODO remove this, debug only
		
		// Read in category name from next line of cin
		char categoryName[ CAT_NAME_LEN ];
		cin.ignore();
		cin.getline(categoryName, CAT_NAME_LEN);
		
		printf("Application is being created in category: %s\n", categoryName); //TODO remove this, debug only
		
		int desiredCategoryIndex = -1;
		printf("Searching for category: %s\n", categoryName); //TODO remove this, debug only
															  // Search the array of categories, to find the position matching the category of the application.
		for (int index = 0; index <= categoryListLength; index++) {
			
			// Compare category name at <index> with desired category name
			if (strcmp(categoryName, categoryList[index].category) == 0) {
				desiredCategoryIndex = index;
				printf("Found desired category at index: %d\n", desiredCategoryIndex); //TODO remove this, debug only
				break; // No need to search further
			}
		}
		
		if (desiredCategoryIndex == -1) {
			printf("Category not found.");
		} else {
			// Read the information of the application
			printf("Enter application information:\n"); //TODO remove this, debug only
			strcpy(node->app.category, categoryName);
			cin.getline(node->app.app_name, APP_NAME_LEN);
			cin >> node->app.version;
			cin >> node->app.size;
			cin >> node->app.units;
			cin >> node->app.price;
			
			// Insert the node into the search tree for the desired application category in asecending alphabetic order
			insert(node, &categoryList[desiredCategoryIndex].root);
			
			// Insert the node into a hash table keyed by app_name
			insertHashEntry(node);
			
		}
//		printTree(categoryList[desiredCategoryIndex].root);
	}
}

// Insert passed node into passed binary search tree
void insert(tree* node, tree** root)
{
	if (*root == NULL) {
		*root = node;
		(*root)->left = NULL;
		(*root)->right = NULL;
	}

	else if(strcmp(node->app.app_name, (*root)->app.app_name) > 0) {
		insert(node, &((*root)->right));
	}
	else if(strcmp(node->app.app_name, (*root)->app.app_name) < 0) {
		insert(node, &((*root)->left));
	}
}

// A utility function to do inorder traversal of BST
void printTree(struct tree* root)
{
	if (root != NULL)
	{
		printTree(root->left);
		printf("%s\n", root->app.app_name);
		printTree(root->right);
	}
}

// Hash application name as sum of ASCII chars % <hashTableLength>
int hashASCII(char* appName) {
	int i = 0, asciiSum = 0;
	
	// Sum all characters in <appName> until '\0' is found
	while (true) {
		if (appName[i] == NULL) break;
		int ascii = appName[i];
		asciiSum += ascii;
		i++;
	}
	
	return (asciiSum % hashTableLength);
}

void insertHashEntry(tree* node) {
	int insertionIndex = hashASCII(node->app.app_name);
	
	// Check if linear probing is necessary
	if (hashTable[insertionIndex].app_node == NULL) {
		cout << "Hash location empty! Inserting at " << insertionIndex << endl;
		strcpy(hashTable[insertionIndex].app_name, node->app.app_name);
		hashTable[insertionIndex].app_node = node;
	} else { // Linear probe
		int i = insertionIndex+1; // Start linear probing at insertionIndex+1, since we already know that hashTable[insertionIndex] is full
		while (hashTable[i].app_node != NULL) { // Traverse through hashTable until empty slot is found
			if (i > hashTableLength) i = 0;
			cout << "Linear probing at " << i << endl;
			i++;
			if (i == insertionIndex) {
				cout << "Hash table full, myAppstore exiting." << endl;
				// Memory Management
				free(categoryList);
				free(hashTable);
				exit(1);
			}
		}
		// hashTable[i] is NULL, insert here
		cout << "Hash location empty! Inserting at " << i << endl;
		strcpy(hashTable[i].app_name, node->app.app_name);
		hashTable[i].app_node = node;
	}
}

int nextPrimeNumber(int min) {
	if (min == 2) return 2;
	if (min == 3) return 3;
	
	int i = 4; // Start at 4, since we already covered 2 & 3
	while (true) { // Keep looking for a prime number until return
		for (int j=2; j*j <= i; j++)
		{
			if (i % j == 0)
				break; // Composite number - ignore
			else if (j+1 > sqrt(i)) { // Prime number
				if (i > min) {
					return i; // Next prime number found greater than <min>
				}
			}
		}
		i++;
	}
}