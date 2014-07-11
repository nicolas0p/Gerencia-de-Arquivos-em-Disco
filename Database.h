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
#include <vector>  //TODO remover
#define AVLTree std::vector //TODO remover

#include "Manpage.h"

#define MAX_MANPAGE_NAME_SIZE 80 //estes numeros foram obtidos a partir de testes nas manpages
#define MAX_MANPAGE_CONTENT 392769

struct diskManpage { //struct que vai ser gravada em disco, nos registros
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
	vector<string> contentQuery(string word)

private:
	AVLTree<string> primaryIndex; //AVLTree<string, Manpage*> na verdade o Manpage* é o "endereco" do inicio da manpage no registro
	AVLTree<string> secondaryIndex; //AVLTree<string, list<Manpage*>>
	std::string manpageRecordFileName_;
	std::string invertedListRecordFileName_;
	int invertedListIndex;
	int manpageIndex;
};



#endif /* DATABASE_H_ */
