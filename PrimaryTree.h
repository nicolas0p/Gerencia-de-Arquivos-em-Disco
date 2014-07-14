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
#include <stack>


struct StringIntUnion {

	StringIntUnion(std::string string, int integer) : string(string), integer(integer) {
	}

	std::string string;
	int integer;
};


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

	class iterator_base {

	public:

		iterator_base(Node *root_) {
			while (root_ != 0) {
				node_stack.push(root_);
				root_ = root_->left;
			}
			if (node_stack.empty()) {
				current = 0;
			} else {
				current = node_stack.top();
				node_stack.pop();
			}
		}

		StringIntUnion operator*() {
			return StringIntUnion(current->key, current->manpage_index);
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
	PrimaryTree();
	virtual ~PrimaryTree();
	void insert(std::string, int);
	void erase(std::string);
	size_t size() const;
	int search(std::string);

	typedef iterator_base iterator;

	iterator begin() {
		return iterator(root_);
	}

	iterator end() {
		return iterator(0);
	}

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
