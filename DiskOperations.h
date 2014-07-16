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


const int MAX_MANPAGE_NAME_SIZE = 53; //estes numeros foram obtidos a partir de testes nas manpages
const int MAX_MANPAGE_CONTENT = 140000;
const int GREATEST_WORD_SIZE = 84; //numero pesquisado nas manpages


/**
 * Objeto que será gravado no disco.
 * Contém o nome do comando e o seu conteudo em texto.
 */

struct diskManpage {
	char name[MAX_MANPAGE_NAME_SIZE];
	char content[MAX_MANPAGE_CONTENT];

	diskManpage(const char *name, const char *content) {
		strcpy(this->name, name);
		strcpy(this->content, content);
	}
};

struct diskNode {
	char word[GREATEST_WORD_SIZE];
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
int binarySearch(const std::deque<int> &deque, int key);
void writeManPage(diskManpage& manpage, std::string fileName, int index);
diskManpage readManPage(std::string fileName, int manpageIndex);
std::string readName(std::string fileName, int recordIndex);

#endif /* WRITETREETODISK_H_ */
