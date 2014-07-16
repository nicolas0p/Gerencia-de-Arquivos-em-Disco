/*
 * Database.cpp
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#include <cstring> //strcpy
#include <iostream> //cout
#include <fstream> //file stream
#include <deque>
#include <sstream>

#include "Database.h"
#include "DiskOperations.h"
#include "AvlTree.h"
#include "TextProcessing.h"

using namespace std;

/**
 * Construtor de database
 * @param Nome do arquivo onde sera armazenado as listas invertidas para cada palavra
 * @param Nome do arquivo onde sera armazenado os registros de manpage
 */
Database::Database(string manpageFileName, string primaryIndexFileName,
		string secondayIndexFileName, string invertedListFileName) :
						primaryIndexTree(), secondaryIndexTree(), manpageFileName_(manpageFileName), primaryIndexFileName_(
								primaryIndexFileName), secondaryIndexFileName_(
										secondayIndexFileName), invertedListFileName_(
												invertedListFileName), manpageIndex_(0) {
}

Database::~Database() {
}

/**
 * Adiciona uma manpage no banco de dados
 * Isso inclui sua indexacao primaria(pelo seu nome) e secundaria(por todas as palavras que ela contem)
 * @param Nome do arquivo da manpage a ser adicionada no banco de dados
 */
void Database::insert(string filename) {
	deque<string> words;
	string concatenated(""), actual;
	ifstream file(filename.c_str());
	if(!file) {
		cout << "Problema ao abrir o arquivo " << filename << endl;
	}

	while (getline(file, actual)) {
		concatenated += actual + "\n";
		stringstream line(actual);

		while (line >> actual) {
			words.push_back(actual);
		}

	}
	filename = removeExtension(filename);
	diskManpage disk(filename.c_str(), concatenated.c_str());

	writeManPage(disk, manpageFileName_, manpageIndex_);
	primaryIndexTree.insert(filename, manpageIndex_);

	for (size_t i = 0; i < words.size(); ++i) { //adicionar todas as palavras do conteudo na indexacao secundaria
		secondaryIndexTree.insert(words[i], manpageIndex_);
	}
	++manpageIndex_; //atualizar index
}

/**
 * Pesquisa no banco de dados a manpage com o nome recebido
 * @param nome da manpage a ser procurada
 * @return manpage com o nome recebido
 */
diskManpage Database::nameQuery(string name) const {
	int position = searchTreeOnDisk(primaryIndexFileName_, name);
	return readManPage(manpageFileName_, position);
}
/**
 * Pesquisa no banco de dados pelas manpages que contem a palavra recebida
 * @param palavra que se deseja encontrar nas manpages
 * @return lista dos nomes das manpages que contem esta palavra
 */
deque<string> Database::contentQuery(string word) const {
	int listPosition = searchTreeOnDisk(secondaryIndexFileName_, word);
	deque<int> manpagesPositions = readInvertedList(invertedListFileName_, listPosition);
	deque<string> ret;
	while(!manpagesPositions.empty()) {
		int manPos = manpagesPositions.front();
		manpagesPositions.pop_front();
		string name = readName(manpageFileName_,manPos);
		ret.push_back(name);
	}
	return ret;
}

/**
 * Pesquisa no banco de dados por manpages que contenham as duas palavras recebidas
 * @param Palavra 1 que se quer encontrar manpages que contenham
 * @param Palavra 1 que também se quer encontrar manpages que contenham
 * @return vetor contendo o nome de todas as manpages que contem ambas as palavras
 */
deque<string> Database::multipleContentQuery(string first, string second) const {
	int firstPosition = searchTreeOnDisk(secondaryIndexFileName_, first);
	int secondPosition = searchTreeOnDisk(secondaryIndexFileName_, second);
	deque<int> one = readInvertedList(invertedListFileName_, firstPosition);
	deque<int> another = readInvertedList(invertedListFileName_, secondPosition);

	deque<int> *greater = &one;
	deque<int> *lesser = &another;
	if(lesser->size() > greater->size()) {
		swap(lesser, greater);
	}

	deque<int> both;
	while(!lesser->empty()) {
		int element = lesser->front();
		lesser->pop_front();

		bool found = false;
		int first = 0;
		int last = greater->size() - 1; //realiza uma busca binaria, verificando se o elemento do deque lesser existe no deque greater.
		while (first <= last) {
			int middle = (first + last) / 2;
			int actual = greater->at(middle);
			if (element == actual) {
				found = true;
				break;
			} else if (element > actual) {
				first = middle + 1;
			} else {
				last = middle - 1;
			}
		}

		if(found) {
			both.push_back(element);
		}
	}

	deque<string> ret;
	while (!both.empty()) {
		int actual = both.front();
		both.pop_front();
		ret.push_back(readName(manpageFileName_, actual));
	}

	return ret;
}
/**
 * Deleta os arquivos de indice primário, secundário, lista invertida e manpages do disco
 */
void Database::clear() {
	remove(manpageFileName_.c_str());
	remove(primaryIndexFileName_.c_str());
	remove(secondaryIndexFileName_.c_str());
	remove(invertedListFileName_.c_str());
}

/**
 * Remove da indexacao secundaria os conectivos
 */
void Database::removeConnectives() {
	deque<string> toRemove;
	toRemove.push_back("a"); toRemove.push_back("an"); toRemove.push_back("the"); toRemove.push_back("or");
	toRemove.push_back("for"); toRemove.push_back("and"); toRemove.push_back("nor"); toRemove.push_back("but");
	toRemove.push_back("or"); toRemove.push_back("yet"); toRemove.push_back("so"); toRemove.push_back("you");
	toRemove.push_back("what"); toRemove.push_back("which"); toRemove.push_back("who"); toRemove.push_back("whom");
	toRemove.push_back("whose"); toRemove.push_back("whichever");  toRemove.push_back("whoever"); toRemove.push_back("whomever");
	toRemove.push_back("anybody"); toRemove.push_back("anyone"); toRemove.push_back("anything"); toRemove.push_back("each");
	toRemove.push_back("either"); toRemove.push_back("everybody"); toRemove.push_back("everyone"); toRemove.push_back("everything");
	toRemove.push_back("nothing"); toRemove.push_back("one"); toRemove.push_back("both"); toRemove.push_back("few");
	toRemove.push_back("many"); toRemove.push_back("several"); toRemove.push_back("all"); toRemove.push_back("any");
	toRemove.push_back("most"); toRemove.push_back("none"); toRemove.push_back("some");

	while(!toRemove.empty()) {
		string rem = toRemove.front();
		toRemove.pop_front();
		secondaryIndexTree.erase(rem); //segfault aqui!
	}
}

/**
 * Escreve as arvores de indexacao primaria e secundaria no disco em forma de lista ordenada
 */
void Database::writeIndexToDisk() {
	writePrimaryTreeToDisk(primaryIndexFileName_, primaryIndexTree);
	writeSecondaryTreeToDisk(secondaryIndexFileName_, invertedListFileName_, secondaryIndexTree);
}

/**
 * Verifica se os arquivos de indexacao ja existem
 * @return verdadeiro se os arquivos ja existirem
 */
bool Database::filesIndexed() {
	std::ifstream file(manpageFileName_.c_str());

	if(!file.is_open())
		return false;
	file.close();

	file.open(primaryIndexFileName_.c_str());
	if(!file.is_open())
		return false;
	file.close();

	file.open(secondaryIndexFileName_.c_str());
	if(!file.is_open())
		return false;
	file.close();

	file.open(invertedListFileName_.c_str());
	if(!file.is_open())
		return false;
	return true;
}
