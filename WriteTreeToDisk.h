/*
 * WriteTreeToDisk.h
 *
 *  Created on: 13/07/2014
 *      Author: nicolas
 */

#ifndef WRITETREETODISK_H_
#define WRITETREETODISK_H_

#include <cstring>
#include <deque>
#include <string>

#include "PrimaryTree.h"
#include "SecundaryTree.h"

const int greatestWordSize = 84; //numero pesquisado nas manpages

struct diskNode {
	char word[greatestWordSize];
	int listPosition;

	diskNode(const char *name, int listPosition) :
		listPosition(listPosition) {
		strcpy(this->word, name);
	}
};

void writeSecondaryTreeToDisk(std::string treeFilename,	std::string invertedListFilename, SecundaryTree& tree);
void writePrimaryTreeToDisk(std::string treeFilename, PrimaryTree& tree);
std::deque<int> readInvertedList(std::string invertedListFileName, int indexOfList);
int searchTreeOnDisk(std::string filename, std::string toSearch);

#endif /* WRITETREETODISK_H_ */
