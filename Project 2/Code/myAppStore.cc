#include	<stdio.h>
#include	<iostream>
#include	<string>
#include	<sstream>
#include	<string.h>
#include	<stdlib.h>
#include	<math.h>

using namespace std;

#define	CAT_NAME_LEN	25
#define	APP_NAME_LEN	50
#define	VERSION_LEN	10
#define	UNIT_SIZE	3
#define OPERATION_LEN 7


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

// Operations and queries
char *operationSubjects[] = { "app", "category", "price" };
void findHashEntry(char* appName);
void findCategory(char* catName);
int traversePriceFree(tree *node, int found);
void findPriceFree();
void rangePrice(char* category);
int traverseRangePrice(tree *node, int found, int low, int high);
void rangeApp(char* category);
int traverseRangeApp(tree *node, int found, char* low, char* high);

int main()
{
	// Allocate an array of size n of type struct categories
	cin >> categoryListLength;
	categoryList = new categories[categoryListLength]; // Deallocate on exit
	
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
	int numberOfApplications; cin >> numberOfApplications;
	
	// Intialize <hashTable> to size k that is the first prime number greater than 2 * <numberOfApplications>.
	hashTableLength = nextPrimeNumber(2*numberOfApplications);
	hashTable = new hash_table_entry[hashTableLength]; // Deallocate on exit
	
	// For safety, set all app_nodes in <hashTable> to NULL
	for (int i = 0; i < hashTableLength; i++) {
		hashTable[i].app_node = NULL;
	}
	
	// Populate myAppStore with m applications
	for (int i = 0; i<numberOfApplications; i++) {
		struct tree* node = new tree();

//		printf("Enter category for application %d\n", i); //TODO remove this, debug only
		
		// Read in category name from next line of cin
		char categoryName[ CAT_NAME_LEN ];
		cin.ignore();
		cin.getline(categoryName, CAT_NAME_LEN);
		
//		printf("Application is being created in category: %s\n", categoryName); //TODO remove this, debug only
		
		int desiredCategoryIndex = -1;
//		printf("Searching for category: %s\n", categoryName); //TODO remove this, debug only
		// Search the array of categories, to find the position matching the category of the application.
		for (int index = 0; index <= categoryListLength; index++) {
			
			// Compare category name at <index> with desired category name
			if (strcmp(categoryName, categoryList[index].category) == 0) {
				desiredCategoryIndex = index;
//				printf("Found desired category at index: %d\n", desiredCategoryIndex); //TODO remove this, debug only
				break; // No need to search further
			}
		}
		
		if (desiredCategoryIndex == -1) {
			printf("Category not found.");
		} else {
			// Read the information of the application
//			printf("Enter application information:\n"); //TODO remove this, debug only
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
		// printTree(categoryList[desiredCategoryIndex].root); // TODO remove this, debug only
	}
	
	// Process q queries and updates
	int q; cin >> q;
	char operation[OPERATION_LEN];
	
	for (int i = 0; i < q; i++) {
		// Read in first keyword, the operation to perform
		cin >> operation;
		if (strcmp(operation, "find") == 0) { // find <subject> <key>
			char subject[9]; cin >> subject;
			if (strcmp(subject, operationSubjects[0]) == 0) { // find app <appName>
				char appName[APP_NAME_LEN]; cin >> appName;
				findHashEntry(appName);
			} else if (strcmp(subject, operationSubjects[1]) == 0) { // find category <catName>
				char catName[CAT_NAME_LEN]; cin >> catName;
				findCategory(catName);
			} else if (strcmp(subject, operationSubjects[2]) == 0) { // find price free
				char price[5]; cin >> price; // Clear buffer, protentionally find specific price
				findPriceFree();
			}
		} else if (strcmp(operation, "range") == 0) {
			char category[ CAT_NAME_LEN ]; cin >> category; // category of <categoryList>
			char subject [ 9 ]; cin >> subject; // price or app
			if (strcmp(subject, operationSubjects[2]) == 0) { // range <category> price <low> <high>
				rangePrice(category);
			} else if (strcmp(subject, operationSubjects[0]) == 0) { // range <category> app <low> <high>
				rangeApp(category);
			}
		} else if (strcmp(operation, "delete") == 0) {
			
		}
	}
	
	// Terminate myAppStore gracefully, with proper memory management
	for (int i = 0; i < categoryListLength; i++) { // Free search tree associated with each application category
		free(categoryList[i].root);
	}
	free(categoryList); // Free array of categories
	free(hashTable); // Free hash table
	
	return 0; // Exit gracefully
}

// Insert passed node into passed binary search tree
void insert(tree* node, tree** root) {
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
void printTree(struct tree* root) {
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

// Insert node into hash table
void insertHashEntry(tree* node) {
	int insertionIndex = hashASCII(node->app.app_name);
	
	// Check if linear probing is necessary
	if (hashTable[insertionIndex].app_node == NULL) {
//		cout << "Hash location empty! Inserting at " << insertionIndex << endl;
		strcpy(hashTable[insertionIndex].app_name, node->app.app_name);
		hashTable[insertionIndex].app_node = node;
	} else { // Linear probe
		int i = insertionIndex+1; // Start linear probing at insertionIndex+1, since we already know that hashTable[insertionIndex] is full
		while (hashTable[i].app_node != NULL) { // Traverse through hashTable until empty slot is found
			if (i > hashTableLength) i = 0;
//			cout << "Linear probing at " << i << endl;
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
//		cout << "Hash location empty! Inserting at " << i << endl;
		strcpy(hashTable[i].app_name, node->app.app_name);
		hashTable[i].app_node = node;
	}
}

// Find and print node in hash table by app_name
void findHashEntry(char* appName) {
	int hashIndex = hashASCII(appName); // Hash appName to <hashIndex>
	int foundIndex = -1; // Store index of application when found, -1 if not found
	
	// Linear probe from hashIndex+1 to hashIndex
	int i = hashIndex+1;
	
	if (strcmp(hashTable[hashIndex].app_name, appName) == 0) {
		foundIndex = hashIndex;
		goto end;
	}
	
	while (strcmp(hashTable[i].app_name, appName) != 0) {
		if (i > hashTableLength) i = 0; // Traverse in circular fashion
		if (i == hashIndex) goto end;
		i++;
	}
	
	foundIndex = i; // Hash entry found
	
end:
	if (foundIndex == -1) {
		cout << "Application not found." << endl;
	} else {
		tree* node = hashTable[foundIndex].app_node;
		cout << "Category: " << node->app.category << endl;
		cout << "Application Name: " << node->app.app_name << endl;
		cout << "Version: " << node->app.version << endl;
		cout << "Size: " << node->app.size << endl;
		cout << "Units: " << node->app.units << endl;
		cout << "Price: " << node->app.price << endl;
	}
}

// Finds category and prints nodes in-order by app_name
void findCategory(char* catName) {
	int foundIndex = -1;
	
	for (int i = 0; i < categoryListLength; i++) {
		if (strcmp(categoryList[i].category, catName) == 0) {
			foundIndex = i;
			break;
		}
	}
	
	if (foundIndex == -1) {
		cout << "Category not found." << endl;
	} else { // Category Found, print all apps in-order
		printTree(categoryList[foundIndex].root);
	}
	
}

// Find and print all free apps
void findPriceFree() {
	for (int i = 0; i < categoryListLength; i++) { // For each category
		// Organize output by category
		cout << "Category: " << categoryList[i].category << endl;
		tree* root = categoryList[i].root;
		int found = traversePriceFree(root, 0);
		if (found == 0) {
			cout << "No free applications found." << endl;
		}
	}
}

// Traverse tree, if price == 0, printout and found++
int traversePriceFree(tree* node, int found) {
	if (node == NULL) return 0;
	traversePriceFree(node->left, found);
	if (node->app.price == 0) {
		cout << node->app.app_name << endl;
		++found;
	}
	traversePriceFree(node->right, found);
	
	return found;
}

// Find and print all apps in category whos price is between low and high criteria
void rangePrice(char* category) {
	int desiredIndex;
	// Look through category list for appropriate index
	for (int i = 0; i < categoryListLength; i++) {
		if (strcmp(categoryList[i].category, category) == 0) {
			desiredIndex = i; // Index found
			break;
		}
	}
	
	// At categoryList[desiredIndex], look for app between <low> and <high>
	int low, high;
	cin >> low; cin >> high;
	
	tree* root = categoryList[desiredIndex].root;
	int found = traverseRangePrice(root, 0, low, high);
	if (found == 0) {
		cout << "No applications found for given range." << endl;
	}
}

// Traverse tree, if low <= price <= high, print app_name
int traverseRangePrice(tree *node, int found, int low, int high) {
	if (node == NULL) return 0;
	traverseRangePrice(node->left, found, low, high);
	if (node->app.price >= low && node->app.price <= high) {
		cout << node->app.app_name << endl;
		++found;
	}
	traverseRangePrice(node->right, found, low, high);
	
	return found;
}

// Find and print all apps in category whos price is between low and high criteria
void rangeApp(char* category) {
	int desiredIndex;
	// Look through category list for appropriate index
	for (int i = 0; i < categoryListLength; i++) {
		if (strcmp(categoryList[i].category, category) == 0) {
			desiredIndex = i; // Index found
			break;
		}
	}
	
	// At categoryList[desiredIndex], look for app_name between <low> and <high>
	char low[25], high[25];
	cin >> low; cin >> high;
	
	tree* root = categoryList[desiredIndex].root;
	int found = traverseRangeApp(root, 0, low, high);
	if (found == 0) {
		cout << "No applications found for given range." << endl;
	}
}

// Traverse tree, if low <= price <= high, print app_name
int traverseRangeApp(tree *node, int found, char* low, char* high) {
	if (node == NULL) return 0;
	traverseRangeApp(node->left, found, low, high);
	if (strcmp(node->app.app_name, low) >= 0 && strcmp(node->app.app_name, high) <= 0) {
		cout << node->app.app_name << endl;
		++found;
	}
	traverseRangeApp(node->right, found, low, high);
	
	return found;
}


// Find and return next prime number after min
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
