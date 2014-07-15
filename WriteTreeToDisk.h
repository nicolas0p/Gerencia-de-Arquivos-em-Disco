/*
 * PrimaryDiskTree.cpp
 *
 *  Created on: 13/07/2014
 *      Author: nicolas
 */

#ifndef WRITETREETODISK_H_
#define WRITETREETODISK_H_

#include <cstring>
#include <deque>
#include <string>

#include "SecundaryTree.h"

const int greatestWordSize = 84;

struct diskNode {
	char word[greatestWordSize];
	int listPosition;

	diskNode(const char *name, int listPosition) :
		listPosition(listPosition) {
		strcpy(this->word, name);
	}
};

void writeSecondaryTreeToDisk(std::string treeFilename,	std::string invertedListFilename, SecundaryTree& tree);
std::deque<int> readInvertedList(std::string invertedListFileName, int indexOfList);
int searchTreeOnDisk(std::string filename, std::string toSearch);

#endif /* WRITETREETODISK_H_ */
