/*
 * Database.cpp
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#include "Database.h"

Database::Database() : primaryIndex(), secondaryIndex() {
}

Database::~Database() {
}

void Database::insert(const Manpage& manpg) {
	Manpage* manpage = new Manpage(manpg);

}
