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

const int MAX_MANPAGE_NAME_SIZE = 53; //estes numeros foram obtidos a partir de testes nas manpages
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
	Database(std::string manpageFileName, std::string primaryIndexFileName, std::string secondaryIndexFileName, std::string invertedListFileName);
	~Database();

	void insert(string filename);
	diskManpage nameQuery(string name) const;
	deque<string> contentQuery(string word) const;
	deque<string> multipleContentQuery(string word1, string word2) const;
	void clear();

private:
	//variaveis
	PrimaryTree primaryIndexTree;
	SecundaryTree secondaryIndexTree;
	std::string manpageFileName_;
	std::string primaryIndexFileName_;
	std::string secondaryIndexFileName_;
	std::string invertedListFileName_;
	int manpageIndex_;

	//metodos
	void writeManPage(diskManpage& manpage, string fileName, int index);
	diskManpage readManPage(string fileName,int recordIndex) const;
	string readName(string fileName,int recordIndex) const;
	string removeExtension(string& name) const;
};

#endif /* DATABASE_H_ */
