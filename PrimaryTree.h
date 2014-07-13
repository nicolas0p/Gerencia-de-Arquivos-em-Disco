/*
 * PrimaryTree.h
 *
 *  Created on: 30/06/2014
 *      Author: arthurhortmannerpen
 */

#ifndef PrimaryTree_H_
#define PrimaryTree_H_

#include <unistd.h>
#include <deque>
#include <string>

class PrimaryTree {

	struct Node {
		Node(Node *left, Node *right, std::string key, int manpage_index) :
			left(left), right(right), height(0), key(key), manpage_index(manpage_index) {
		}

		Node *left, *right;
		int height;
		std::string key;
		int manpage_index;
	};

public:
	PrimaryTree();
	virtual ~PrimaryTree();
	void insert(std::string, int);
	void erase(std::string);
	size_t size() const;
	int search(std::string);

	void writeToFile(std::string filename);

private:
	Node *root_;
	size_t size_;

	Node* insert(Node*, std::string, int);
	Node* erase(Node*, std::string);
	int search(Node*, std::string);

	Node* simpleRight(Node*);
	Node* simpleLeft(Node*);
	Node* doubleRight(Node*);
	Node* doubleLeft(Node*);

	int height(Node*);
	void deleteTree(Node *node);
};

#endif /* PrimaryTree_H_ */
