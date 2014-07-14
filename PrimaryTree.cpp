//============================================================================
// Name        : PrimaryTree.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <iostream>
#include <string>

#include "PrimaryTree.h"
#include "QueryException.h"


PrimaryTree::PrimaryTree() : root_(0), size_(0) {}

PrimaryTree::~PrimaryTree() {
	deleteTree(root_);
}

size_t PrimaryTree::size() const {
	return size_;
}

void PrimaryTree::insert(std::string string, int index) {
	root_ = insert(root_, string, index);
}

PrimaryTree::Node* PrimaryTree::insert(Node *node, std::string string, int index) {
	if (node == 0) {
		node = new Node(0, 0, string, index);
		++size_;
	} else if (string < node->key) {
		node->left = insert(node->left, string, index);
		if (height(node->right) - height(node->left)  == -2) {
			if (string <= node->left->key) {
				node = simpleRight(node);
			} else {
				node = doubleRight(node);
			}
		}
	} else if (string > node->key) {
		node->right = insert(node->right, string, index);
		if(height(node->right) - height(node->left) == 2) {
			if (string >= node->right->key) {
				node = simpleLeft(node);
			} else {
				node = doubleLeft(node);
			}
		}
	}
	node->height = std::max(height(node->left), height(node->right)) + 1;
	return node;
}

void PrimaryTree::erase(std::string string) {
	root_ = erase(root_, string);
}

PrimaryTree::Node* PrimaryTree::erase(Node* node, std::string string) {
	Node *returned = node;
	if (node == 0) {
		return 0;
	} else if (string < node->key) {
		node->left = erase(node->left, string);
		if (height(node->left) - height(node->right) == -2) {
			if (height(node->right->left) - height (node->right->right) <= 0) {
				returned = simpleLeft(node);
			} else {
				returned = doubleLeft(node);
			}
		}
	} else if (string > node->key) {
		node->right = erase(node->right, string);
		if (height(node->left) - height(node->right) == 2) {
			if (height(node->right->left) - height (node->right->right) >= 0) {
				returned = simpleRight(node);
			} else {
				returned = doubleRight(node);
			}
		}
	} else if (node->left != 0 && node->right != 0) {
		Node* min = node->right;
		while (min->left != 0) {
			min = min->left;
		}
		node->key = min->key;
		node->manpage_index = min->manpage_index;
		node->right = erase(node->right, min->key);
		if (height(node->left) - height(node->right) == 2) {
			if (height(node->right->left) - height (node->right->right) >= 0) {
				returned = simpleRight(node);
			} else {
				returned = doubleRight(node);
			}
		}
	} else {
		if (node->left != 0) {
			returned = node->left;
		} else if (node->right != 0) {
			returned = node->right;
		} else {
			returned = 0;
		}
		delete node;
		--size_;
	}
	return returned;
}

int PrimaryTree::search(std::string string) const {
	return search(root_, string);
}

int PrimaryTree::search(Node *node, std::string string) const{

	if (node == 0) {
		throw QueryException();
	} else if (string > node->key) {
		return search(node->right, string);
	} else if (string < node->key) {
		return search(node->left, string);
	} else {
		return node->manpage_index;
	}
}

PrimaryTree::Node* PrimaryTree::simpleRight(Node* upper) {
	Node *left = upper->left;
	upper->left = left->right;
	left->right = upper;
	upper->height = std::max(height(upper->left), height(upper->right)) + 1;
	left->height = std::max(height(left->left), height(left->right)) + 1;
	return left;
}

PrimaryTree::Node* PrimaryTree::simpleLeft(Node *upper) {
	Node *right = upper->right;
	upper->right = right->left;
	right->left = upper;
	upper->height = std::max(height(upper->left), height(upper->right)) + 1;
	right->height = std::max(height(right->left), height(right->right)) + 1;
	return right;
}

PrimaryTree::Node* PrimaryTree::doubleRight(Node *upper) {
	upper->left = simpleLeft(upper->left);
	return simpleRight(upper);
}

PrimaryTree::Node* PrimaryTree::doubleLeft(Node *upper) {
	upper->right = simpleRight(upper->right);
	return simpleLeft(upper);
}

void PrimaryTree::deleteTree(Node *node) {
	if (node != 0) {
		Node *left = node->left;
		Node *right = node->right;
		delete node;
		deleteTree(left);
		deleteTree(right);
	}
}

int PrimaryTree::height(Node *node) const {
	if (node == 0) {
		return -1;
	}
	return node->height;
}
