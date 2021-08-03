#include "Wine.h"

class HashTable {
private:
	// Struct for node in table (data and pointer for dupes):
	struct HTNode {
		Wine* data;
		HTNode* next;
		HTNode(Wine* _data, HTNode* _next = nullptr) : data(_data), next(_next) { }
	};

	// The amount of data entries from the data set.
	// Default hash table size = val * 2.
	int tableSize;

	// Data structure that stores the hashes.
	vector<HTNode*> hashTable;

	// Wine property being processed (key type).
	Wine::Properties hashBy;

	// Calculates the hashcode for a key.
	// Returns the index it is located at in the hash table.
	int hashFunction(string key);

	// Eq. of switch to get data's key value based on hashBy.
	string(Wine::* getHashedValue)() const;
public:
	// Constructor for size based on hashBy type.
	HashTable(Wine::Properties _hashBy);
	// Default constructor.
	HashTable(int _numData, Wine::Properties _hashBy);
	// Destructor.
	~HashTable();

	// Adds a new wine object based on pointer. 
	// Obtained through the temp vector of wine pointers.
	void insert(Wine* data);

	// Takes in inputted search value and returns all wine objects that match with the key.
	// Prints data of all objects within function.
	// Used for search values.
	void search(string strKey, vector<Wine*>& results);
};