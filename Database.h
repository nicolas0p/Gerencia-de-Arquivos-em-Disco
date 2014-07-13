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

#include "Manpage.h"
#include "PrimaryTree.h"
#include "SecundaryTree.h"

const int MAX_MANPAGE_NAME_SIZE = 52; //estes numeros foram obtidos a partir de testes nas manpages
const int MAX_MANPAGE_CONTENT = 140000;


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


class Database {
public:
	Database(std::string manpageRecordFileName, std::string invertedListRecordFileName);
	~Database();

	void insert(const Manpage& manpage, const diskManpage& disk);
	diskManpage nameQuery(string name);
	vector<string> contentQuery(string word);

private:
	PrimaryTree primaryIndex;
	SecundaryTree secondaryIndex;
	std::string manpageRecordFileName_;
	std::string invertedListRecordFileName_;
	int invertedListIndex;
	int manpageIndex;


	void writeRecord(diskManpage manpage, string fileName, int index);
	diskManpage readRecord(string fileName,int recordIndex);
	string readName(string fileName,int recordIndex);
};



#endif /* DATABASE_H_ */
