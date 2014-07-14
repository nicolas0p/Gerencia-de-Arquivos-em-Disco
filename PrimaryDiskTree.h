/*
 * PrimaryDiskTree.h
 *
 *  Created on: 13/07/2014
 *      Author: nicolas
 */

#ifndef PRIMARYDISKTREE_H_
#define PRIMARYDISKTREE_H_

#include <string>

#include "PrimaryTree.h"

class PrimaryDiskTree {
public:
	PrimaryDiskTree(std::string filename, PrimaryTree tree);
	virtual ~PrimaryDiskTree();

	int search(std::string);

private:
	std::string filename;

	int fullSize(int rootHeight);
	void writeToDisc(PrimaryTree::Node **array, int size);
};

#endif /* PRIMARYDISKTREE_H_ */
