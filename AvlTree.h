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

	class iterator_base {

	public:

		iterator_base(Node *root) {
			while (root != 0) {
				node_stack.push(root);
				root = root->_left;
			}
			if (node_stack.empty()) {
				current = 0;
			} else {
				current = node_stack.top();
				node_stack.pop();
			}
		}

		int operator*() {
			return current->_data;
		}

		iterator_base& operator++() {
			if (current->_right) {
				node_stack.push(current->_right);
				Node *min = current->_right->_left;
				while (min) {
					node_stack.push(current->_right->_left);
					min = min->_left;
				}
			}
			if (node_stack.size() == 0) {
				current = 0;
				return *this;
			}
			current = node_stack.top();
			node_stack.pop();
			return *this;
		}

		iterator_base operator++(int) {
			iterator_base copy = *this;
			if (current->_right) {
				node_stack.push(current->_right);
				Node *min = current->_right->_left;
				while (min) {
					node_stack.push(current->_right->_left);
					min = min->_left;
				}
			}
			if (node_stack.size() == 0) {
				current = 0;
				return copy;
			}
			current = node_stack.top();
			node_stack.pop();
			return copy;
		}

		bool operator==(const iterator_base& other) const {
			return current == other.current;
		}

		bool operator!=(const iterator_base& other) const {
			return !(current == other.current);
		}

	private:
		std::stack<Node *> node_stack;
		Node *current;
	};

public:
	AvlTree();
	virtual ~AvlTree();
	void insert(int);
	void erase(int);
	size_t size() const;

	typedef iterator_base iterator;

	iterator begin() {
		return iterator(root);
	}

	iterator end() {
		return iterator(0);
	}

private:
	Node *root;
	size_t _size;

	Node* insert(Node*, int);
	Node* erase(Node*, int);

	Node* simpleRight(Node*);
	Node* simpleLeft(Node*);
	Node* doubleRight(Node*);
	Node* doubleLeft(Node*);

	int height(Node*) const;
	void deleteTree(Node *node);
};

#endif /* AVLTREE_H_ */
