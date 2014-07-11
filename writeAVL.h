/*
 * writeAVL.h
 *
 *  Created on: 10/07/2014
 *      Author: nicolas
 */

#ifndef WRITEAVL_H_
#define WRITEAVL_H_

#include <fstream>
#include <deque>

struct node {
	char key[50];
	node *left, *right;
};

struct backNode {
	char key[50];
	int left, right;

	backNode(const node& node, int left, int right) : key(node.key), left(left), right(right){
	}
};

#define sizeAVL 500 //tamanho da arvore

node root;

using namespace std;

template <typename T>
void writeAVLToDisk(string fileName) {
	ofstream file(fileName, ios::out);
	std::deque<node> queue;
	queue.push_back(root);
	writeNodeChildren(queue, file);
}

void writeNodeChildren(std::deque<node> queue, ofstream file) {
	node e = queue.pop_front();
	queue.push_back(*e.left);
	queue.push_back(*e.right);
	file << e;
}


#endif /* WRITEAVL_H_ */
