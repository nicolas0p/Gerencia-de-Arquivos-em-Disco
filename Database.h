/*
 * Database.h
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <cstring> //strcpy
#include <deque>

#include "PrimaryTree.h"
#include "SecundaryTree.h"
#include "DiskOperations.h"

using namespace std;

class Database {
public:
	Database(std::string manpageFileName, std::string primaryIndexFileName, std::string secondaryIndexFileName, std::string invertedListFileName);
	~Database();

	void insert(string filename);
	diskManpage nameQuery(string name) const;
	deque<string> contentQuery(string word) const;
	deque<string> multipleContentQuery(string word1, string word2) const;
	void clear();
	void writeIndexToDisk();
	void removeConnectives();
	bool filesIndexed();

private:
	PrimaryTree primaryIndexTree;
	SecundaryTree secondaryIndexTree;
	std::string manpageFileName_;
	std::string primaryIndexFileName_;
	std::string secondaryIndexFileName_;
	std::string invertedListFileName_;
	int manpageIndex_;
};

#endif /* DATABASE_H_ */
