/*
 * Database.h
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <vector>  //TODO remover
#define AVLTree std::vector //TODO remover

#include "Manpage.h"

class Database {
public:
	Database();
	~Database();

	void insert(const Manpage& manpage);

private:
	AVLTree<Manpage> primaryIndex; //AVLTree<string, Manpage*> na verdade o Manpage* é o "endereco" do inicio da manpage no registro
	AVLTree<Manpage> secondaryIndex; //AVLTree<string, list<Manpage*>>

};

#endif /* DATABASE_H_ */
