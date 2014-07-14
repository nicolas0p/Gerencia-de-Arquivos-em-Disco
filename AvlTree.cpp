//============================================================================
// Name        : AvlTree.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include "AvlTree.h"
using namespace std;


AvlTree::AvlTree() : root(0), _size(0) {}

AvlTree::~AvlTree() {
	deleteTree(root);
}

size_t AvlTree::size() const {
	return _size;
}

void AvlTree::insert(int element) {
	root = insert(root, element);
}

AvlTree::Node* AvlTree::insert(Node *node, int element) {
	if (node == 0) {
		node = new Node(0, 0, 0, element);
		++_size;
	} else if (element < node->_data) {
		node->_left = insert(node->_left, element);
		if (height(node->_right) - height(node->_left)  == -2) {
			if (element <= node->_left->_data) {
				node = simpleRight(node);
			} else {
				node = doubleRight(node);
			}
		}
	} else if (element > node->_data) {
		node->_right = insert(node->_right, element);
		if(height(node->_right) - height(node->_left) == 2) {
			if (element >= node->_right->_data) {
				node = simpleLeft(node);
			} else {
				node = doubleLeft(node);
			}
		}
	}
	node->_height = std::max(height(node->_left), height(node->_right)) + 1;
	return node;
}

void AvlTree::erase(int element) {
	root = erase(root, element);
}

AvlTree::Node* AvlTree::erase(Node* node, int element) { //TODO
	Node *returned = node;
	if (node == 0) {
		return 0;
	} else if (element < node->_data) {
		node->_left = erase(node->_left, element);
		if (height(node->_left) - height(node->_right) == -2) {
			if (height(node->_right->_left) - height (node->_right->_right) <= 0) {
				returned = simpleLeft(node);
			} else {
				returned = doubleLeft(node);
			}
		}
	} else if (element > node->_data) {
		node->_right = erase(node->_right, element);
		if (height(node->_left) - height(node->_right) == 2) {
			if (height(node->_right->_left) - height (node->_right->_right) >= 0) {
				returned = simpleRight(node);
			} else {
				returned = doubleRight(node);
			}
		}
	} else if (node->_left != 0 && node->_right != 0) {
		Node* min = node->_right;
		while (min->_left != 0) {
			min = min->_left;
		}
		node->_data = min->_data;
		node->_right = erase(node->_right, min->_data);
		if (height(node->_left) - height(node->_right) == 2) {
			if (height(node->_right->_left) - height (node->_right->_right) >= 0) {
				returned = simpleRight(node);
			} else {
				returned = doubleRight(node);
			}
		}
	} else {
		if (node->_left != 0) {
			returned = node->_left;
		} else if (node->_right != 0) {
			returned = node->_right;
		} else {
			returned = 0;
		}
		delete node;
		--_size;
	}
	return returned;
}

AvlTree::Node* AvlTree::simpleRight(Node* upper) {
	Node *left = upper->_left;
	upper->_left = left->_right;
	left->_right = upper;
	upper->_height = std::max(height(upper->_left), height(upper->_right)) + 1;
	left->_height = std::max(height(left->_left), height(left->_right)) + 1;
	return left;
}

AvlTree::Node* AvlTree::simpleLeft(Node *upper) {
	Node *right = upper->_right;
	upper->_right = right->_left;
	right->_left = upper;
	upper->_height = std::max(height(upper->_left), height(upper->_right)) + 1;
	right->_height = std::max(height(right->_left), height(right->_right)) + 1;
	return right;
}

AvlTree::Node* AvlTree::doubleRight(Node *upper) {
	upper->_left = simpleLeft(upper->_left);
	return simpleRight(upper);
}

AvlTree::Node* AvlTree::doubleLeft(Node *upper) {
	upper->_right = simpleRight(upper->_right);
	return simpleLeft(upper);
}

void AvlTree::deleteTree(Node *node) {
	if (node != 0) {
		Node *left = node->_left;
		Node *right = node->_right;
		delete node;
		deleteTree(left);
		deleteTree(right);
	}
}

int AvlTree::height(Node *node) const {
	if (node == 0) {
		return -1;
	}
	return node->_height;
}


