/*
 * Database.cpp
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#include "Database.h"

Database::Database(std::string manpageRecordFileName) : primaryIndex(), secondaryIndex(), manpageRecordFileName_(manpageRecordFileName) {
}

Database::~Database() {
}
/**
 * Adiciona uma manpage no banco de dados
 * Isso inclui sua indexacao primaria(pelo seu nome) e secundaria(por todas as palavras que ele contem)
 *
 * @param manpage a ser adicionada no banco de dados
 */
void Database::insert(const Manpage& manpg) {

}
