/*
 * Database.h
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <vector>  //TODO remover
#define AVLTree std::vector //TODO remover

#include "Manpage.h"

class Database {
public:
	Database(std::string manpageRecordFileName);
	~Database();

	void insert(const Manpage& manpage);

private:
	AVLTree<string> primaryIndex; //AVLTree<string, Manpage*> na verdade o Manpage* é o "endereco" do inicio da manpage no registro
	AVLTree<string> secondaryIndex; //AVLTree<string, list<Manpage*>>
	std::string manpageRecordFileName_;
};

#endif /* DATABASE_H_ */
