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

	class iterator_base {

	public:

		iterator_base(Node *root) {
			while (root != 0) {
				node_stack.push(root);
				root = root->left;
			}
			if (node_stack.empty()) {
				current = 0;
			} else {
				current = node_stack.top();
				node_stack.pop();
			}
		}

		int operator*() {
			return current->manpage_indexes;
		}

		iterator_base& operator++() {
			if (current->right) {
				node_stack.push(current->right);
				Node *min = current->right->left;
				while (min) {
					node_stack.push(current->right->left);
					min = min->left;
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
			if (current->right) {
				node_stack.push(current->right);
				Node *min = current->right->left;
				while (min) {
					node_stack.push(current->right->left);
					min = min->left;
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
	SecundaryTree();
	virtual ~SecundaryTree();
	void insert(const std::string&, int);
	void erase(const std::string&);
	AvlTree search(const std::string&);
	size_t size() const;
	size_t greatestListSize() const;

private:
	Node *root_;
	size_t size_;

	Node* insert(Node*, const std::string&, int);
	Node* erase(Node*, const std::string&);
	AvlTree* search(Node*, const std::string&);

	Node* simpleRight(Node*);
	Node* simpleLeft(Node*);
	Node* doubleRight(Node*);
	Node* doubleLeft(Node*);

	int height(Node*);
	size_t greatestListSize(Node *) const;
	void deleteTree(Node *);


};

#endif /* SecundaryTree_H_ */
