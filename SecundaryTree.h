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

class SecundaryTree {

	struct Node {
		Node(Node *left, Node *right, std::string key, int manpage_index) :
			left(left), right(right), height(0), key(key), manpage_indexes(new std::deque<int>) {
			manpage_indexes->push_back(manpage_index);
		}

		~Node() {
			delete manpage_indexes;
		}

		Node *left, *right;
		int height;
		std::string key;
		std::deque<int> *manpage_indexes;
	};

public:
	SecundaryTree();
	virtual ~SecundaryTree();
	void insert(std::string, int);
	void erase(std::string);
	size_t size() const;
	std::deque<int> search(std::string);

private:
	Node *root_;
	size_t size_;

	Node* insert(Node*, std::string, int);
	Node* erase(Node*, std::string);
	std::deque<int>* search(Node*, std::string);

	Node* simpleRight(Node*);
	Node* simpleLeft(Node*);
	Node* doubleRight(Node*);
	Node* doubleLeft(Node*);

	int height(Node*);
	void deleteTree(Node *node);


};

#endif /* SecundaryTree_H_ */
