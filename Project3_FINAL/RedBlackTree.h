#pragma once
#include "Wine.h"

class RedBlackTree
{
private:
	enum Color { RED, BLACK };
	// Struct for chaining duplicate keys:
	struct duplicateNode
	{
		Wine* data;
		duplicateNode* next;
		duplicateNode(Wine* _data, duplicateNode* _next) : data(_data), next(_next) {}
	};

	// Struct for key nodes:
	struct RBNode
	{
		Wine* data;
		bool color;
		RBNode* left, * right, * parent;
		duplicateNode* next;
		RBNode(Wine* _data, RBNode* _parent) : data(_data), left(nullptr), right(nullptr), next(nullptr), parent(_parent), color(RED) { }
	};

	RBNode* root;
	// Function that dictates how search and insertion will be preformed, i.e. based on which wine property.
	int (*nodeCompare)(const Wine*, const Wine*);

	// Functions for self balancing nature of RBTree; 
	void rotateLeft(RBNode* node);
	void rotateRight(RBNode* node);
	void balanceTree(RBNode* node);

	// Recursive helper function for post-order traversal to deallocate all tree and duplicate nodes.
	void recursiveDestructor(RBNode* node);

	static RBNode* getUncle(RBNode* node);
public:
	RedBlackTree(int(*_comp)(const Wine*, const Wine*));
	RedBlackTree(Wine::Properties _searchBy);
	~RedBlackTree(); // Uses a post order travresal to deallocate all tree nodes.

	void insert(Wine* w);
	void search(Wine* key, std::vector<Wine*>& results); // Search returns a vector of all matching results.
};