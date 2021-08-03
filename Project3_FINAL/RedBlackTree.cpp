#include "RedBlackTree.h"

void RedBlackTree::rotateLeft(RBNode* node)
{
	RBNode* rightChild = node->right;
	node->right = rightChild->left;
	if (node->right != nullptr)
		node->right->parent = node;

	rightChild->parent = node->parent;
	if (node->parent == nullptr) {
		root = rightChild;
	}
	else if (node == node->parent->right) {
		node->parent->right = rightChild;
	}
	else {
		node->parent->left = rightChild;
	}
	rightChild->left = node;
	node->parent = rightChild;
}

void RedBlackTree::rotateRight(RBNode* node)
{
	RBNode* leftChild = node->left;
	node->left = leftChild->right;
	if (node->left != nullptr)
		node->left->parent = node;
	leftChild->parent = node->parent;
	if (node->parent == nullptr) {
		root = leftChild;
	}
	else if (node == node->parent->right) {
		node->parent->right = leftChild;
	}
	else {
		node->parent->left = leftChild;
	}
	leftChild->right = node;
	node->parent = leftChild;
}

RedBlackTree::RBNode* RedBlackTree::getUncle(RBNode* node)
{
	RBNode* grandparent = node->parent->parent;
	if (grandparent == nullptr)
		return nullptr;
	if (node->parent == grandparent->left)
		return grandparent->right;
	else
		return grandparent->left;
}

void RedBlackTree::balanceTree(RBNode* node)
{
	if (node->parent == nullptr) {
		node->color = BLACK;
		return;
	}
	if (node->parent->color == BLACK)
		return;
	RBNode* parent = node->parent;
	RBNode* grandparent = node->parent->parent;
	RBNode* uncle = getUncle(node);
	if (uncle != nullptr && uncle->color == RED) {
		parent->color = BLACK;
		uncle->color = BLACK;
		grandparent->color = RED;
		balanceTree(grandparent);
		return;
	}
	if (node == parent->right && parent == grandparent->left) {
		rotateLeft(parent);
		node = parent;
		parent = node->parent;
	}
	else if (node == parent->left && parent == grandparent->right) {
		rotateRight(parent);
		node = parent;
		parent = node->parent;
	}
	parent->color = BLACK;
	grandparent->color = RED;
	if (node == parent->left) {
		rotateRight(grandparent);
	}
	else {
		rotateLeft(grandparent);
	}
}

RedBlackTree::RedBlackTree(int(*_comp)(const Wine*, const Wine*)) : root(nullptr)
{
	nodeCompare = _comp;
}

RedBlackTree::RedBlackTree(Wine::Properties _compBy) : root(nullptr)
{
	switch (_compBy) {
	case Wine::Properties::VARIETY:
		nodeCompare = Wine::varietyComp;
		break;
	case Wine::Properties::PROVINCE:
		nodeCompare = Wine::provinceComp;
		break;
	case Wine::Properties::TITLE:
		nodeCompare = Wine::titleComp;
		break;
	case Wine::Properties::COUNTRY:
		nodeCompare = Wine::countryComp;
		break;
	default:
		nodeCompare = Wine::titleComp;
	}
}

void RedBlackTree::recursiveDestructor(RBNode* node)
{
	if (node != nullptr) {
		recursiveDestructor(node->left);
		recursiveDestructor(node->right);
		duplicateNode* duplicate = node->next;
		delete node;
		while (duplicate != nullptr) {
			duplicateNode* current = duplicate;
			duplicate = duplicate->next;
			delete current;
		}
	}
}

RedBlackTree::~RedBlackTree() {
	recursiveDestructor(root);
}

void RedBlackTree::insert(Wine* w)
{
	RBNode* parent = nullptr;
	RBNode** current = &root; // Uses double pointer to keep track of current pointer location.
	while (*current != nullptr) {
		parent = *current;
		if (nodeCompare((*current)->data, w) < 0) {
			current = &((*current)->right);
		}
		else if (nodeCompare((*current)->data, w) > 0) {
			current = &((*current)->left);
		}
		else {
			duplicateNode* newDuplicate = new duplicateNode(w, (*current)->next);
			(*current)->next = newDuplicate;
			return;
		}
	}
	*current = new RBNode(w, parent);

	balanceTree(*current);
}

void RedBlackTree::search(Wine* searchKey, std::vector<Wine*>& results)
{
	RBNode* current = root;
	while (current != nullptr) {
		if (nodeCompare(current->data, searchKey) < 0) {
			current = current->right;
		}
		else if (nodeCompare(current->data, searchKey) > 0) {
			current = current->left;
		}
		else {
			results.push_back(current->data);
			duplicateNode* duplicates = current->next;
			while (duplicates != nullptr) {
				results.push_back(duplicates->data);
				duplicates = duplicates->next;
			}
			return;
		}
	}
}