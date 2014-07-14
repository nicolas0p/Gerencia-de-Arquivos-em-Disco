//============================================================================
// Name        : SecundaryTree.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>

#include "SecundaryTree.h"
#include "QueryException.h"
#include "AvlTree.h"

SecundaryTree::SecundaryTree() : root_(0), size_(0) {}

SecundaryTree::~SecundaryTree() {
	deleteTree(root_);
}

void SecundaryTree::insert(const std::string &string, int index) {
	root_ = insert(root_, string, index);
}

SecundaryTree::Node* SecundaryTree::insert(Node *node, const std::string &string, int index) {
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
	} else { //manpage == node->key
		node->manpage_indexes->insert(index);
	}
	node->height = std::max(height(node->left), height(node->right)) + 1;
	return node;
}

void SecundaryTree::erase(const std::string &string) {
	root_ = erase(root_, string);
}

SecundaryTree::Node* SecundaryTree::erase(Node* node, const std::string &string) {
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
		delete node->manpage_indexes;
		node->manpage_indexes = new AvlTree(*(min->manpage_indexes)); //faz copia da lista, pois a lista sera deletada
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

AvlTree SecundaryTree::search(const std::string &string) const{
	return *(search(root_, string));
}

AvlTree* SecundaryTree::search(Node *node, const std::string &string) const{
	if (node == 0) {
		throw QueryException();
	} else if (string > node->key) {
		return search(node->right, string);
	} else if (string < node->key) {
		return search(node->left, string);
	} else {
		return node->manpage_indexes;
	}
}



SecundaryTree::Node* SecundaryTree::simpleRight(Node* upper) {
	Node *left = upper->left;
	upper->left = left->right;
	left->right = upper;
	upper->height = std::max(height(upper->left), height(upper->right)) + 1;
	left->height = std::max(height(left->left), height(left->right)) + 1;
	return left;
}

SecundaryTree::Node* SecundaryTree::simpleLeft(Node *upper) {
	Node *right = upper->right;
	upper->right = right->left;
	right->left = upper;
	upper->height = std::max(height(upper->left), height(upper->right)) + 1;
	right->height = std::max(height(right->left), height(right->right)) + 1;
	return right;
}

SecundaryTree::Node* SecundaryTree::doubleRight(Node *upper) {
	upper->left = simpleLeft(upper->left);
	return simpleRight(upper);
}

SecundaryTree::Node* SecundaryTree::doubleLeft(Node *upper) {
	upper->right = simpleRight(upper->right);
	return simpleLeft(upper);
}

void SecundaryTree::deleteTree(Node *node) {
	if (node != 0) {
		Node *left = node->left;
		Node *right = node->right;
		delete node;
		deleteTree(left);
		deleteTree(right);
	}
}

int SecundaryTree::height(Node *node) const{
	if (node == 0) {
		return -1;
	}
	return node->height;
}

size_t SecundaryTree::size() const {
	return size_;
}

size_t SecundaryTree::greatestListSize() const {
	return greatestListSize(root_);
}

size_t SecundaryTree::greatestListSize(Node *node) const {
	if (node == 0) {
		return 0;
	}
	return std::max(std::max(node->manpage_indexes->size(), greatestListSize(node->left)), greatestListSize(node->right));
}
