/*
 * SecundaryTree.h
 *
 *  Created on: 30/06/2014
 *      Author: arthurhortmannerpen
 */

#ifndef SecundaryTree_H_
#define SecundaryTree_H_

#include <unistd.h>
#include <deque>
#include <string>

#include "AvlTree.h"

struct StringTreeUnion {

	StringTreeUnion(std::string string, AvlTree *tree) : string(string), tree(tree) {
	}

	std::string string;
	AvlTree *tree;
};

class SecundaryTree {

	struct Node {
		Node(Node *left, Node *right, std::string key, int manpage_index) :
			left(left), right(right), height(0), key(key), manpage_indexes(new AvlTree) {
			manpage_indexes->insert(manpage_index);
		}

		~Node() {
			delete manpage_indexes;
		}

		Node *left, *right;
		int height;
		std::string key;
		AvlTree *manpage_indexes;
	};



public:
	SecundaryTree();
	virtual ~SecundaryTree();
	void insert(const std::string&, int);
	void erase(const std::string&);
	AvlTree* search(const std::string&) const;
	size_t size() const;
	size_t greatestListSize() const;
	std::deque<StringTreeUnion> toDeque();

private:
	Node *root_;
	size_t size_;

	Node* insert(Node*, const std::string&, int);
	Node* erase(Node*, const std::string&);
	AvlTree* search(Node*, const std::string&) const;
	void toDeque(Node *node, std::deque<StringTreeUnion> &deque);

	Node* simpleRight(Node*);
	Node* simpleLeft(Node*);
	Node* doubleRight(Node*);
	Node* doubleLeft(Node*);

	int height(Node*) const;
	size_t greatestListSize(Node *) const;
	void deleteTree(Node *);
};



#endif /* SecundaryTree_H_ */
