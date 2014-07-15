/*
 * AvlTree.h
 *
 *  Created on: 30/06/2014
 *      Author: arthurhortmannerpen
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <unistd.h>
#include <stack>
#include <iostream>

class AvlTree {

	struct Node {
		Node(Node *left, Node *right, int height, int data) :
			_left(left), _right(right), _height(height), _data(data) {
		}

		Node(const Node& other) :
			_left(other._left), _right(other._right), _height(other._height), _data(other._data) {
		}

		~Node() {
		}

		Node *_left, *_right;
		int _height;
		int _data;
	};


public:
	AvlTree();
	virtual ~AvlTree();
	void insert(int);
	void erase(int);
	bool search (int) const;
	size_t size() const;
	std::deque<int> toDeque();

private:
	Node *root;
	size_t _size;

	Node* insert(Node*, int);
	Node* erase(Node*, int);
	bool search(Node*, int) const;
	void toDeque(Node *, std::deque<int> &);

	Node* simpleRight(Node*);
	Node* simpleLeft(Node*);
	Node* doubleRight(Node*);
	Node* doubleLeft(Node*);

	int height(Node*) const;
	void deleteTree(Node *node);
};

#endif /* AVLTREE_H_ */
