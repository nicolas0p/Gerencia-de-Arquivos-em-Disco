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

#include "Database.h"

using namespace std;

/**
 * Construtor de database
 * @param Nome do arquivo onde sera armazenado as listas invertidas para cada palavra
 * @param Nome do arquivo onde sera armazenado os registros de manpage
 */
Database::Database(string manpageRecordFileName,
		string invertedListRecordFileName) :
		primaryIndex(), secondaryIndex(), manpageRecordFileName_(
				manpageRecordFileName), invertedListRecordFileName_(invertedListRecordFileName), manpageIndex_(0) {
}

Database::~Database() {
}

/**
 * Adiciona uma manpage no banco de dados
 * Isso inclui sua indexacao primaria(pelo seu nome) e secundaria(por todas as palavras que ela contem)
 *
 * @param manpage a ser adicionada no banco de dados
 * @param diskManpage a ser adicionada nos registros
 */
void Database::insert(const Manpage& manpage, const diskManpage& disk) {
	writeRecord(disk, manpageRecordFileName_, manpageIndex_);
	primaryIndex.insert(manpage.name(), manpageIndex_);

	vector<string> words = manpage.words();
	for (size_t i = 0; i < words.size(); ++i) { //adicionar todas as palavras do conteudo na indexacao secundaria
		secondaryIndex.insert(words[i], manpageIndex_);
	}
	++manpageIndex_; //atualizar index
}

/**
 * Pesquisa no banco de dados a manpage com o nome recebido
 * @param nome da manpage a ser procurada
 * @return manpage com o nome recebido
 */
diskManpage Database::nameQuery(string name) {
	int position = primaryIndex.search(name);
	return readRecord(manpageRecordFileName_, position);
}
/**
 * Pesquisa no banco de dados pelas manpages que contem a palavra recebida
 * @param palavra que se deseja encontrar nas manpages
 * @return lista dos nomes das manpages que contem esta palavra
 */
vector<string> Database::contentQuery(string word) {
	deque<int> invertedList = secondaryIndex.search(word);
	vector<string> ret;
	for (size_t i = 0; i < invertedList.size(); ++i) {
		ret.push_back(readName(manpageRecordFileName_, invertedList[i]));
	}
	return ret;
}

/**
 * Pesquisa no banco de dados por manpages que contenham as duas palavras recebidas
 * @param Palavra 1 que se quer encontrar manpages que contenham
 * @param Palavra 1 que também se quer encontrar manpages que contenham
 * @return vetor contendo o nome de todas as manpages que contem ambas as palavras
 */
vector<string> Database::multipleContentQuery(string word1, string word2) {
	vector<string> ret;
	deque<int> lesser = secondaryIndex.search(word1);
	deque<int> greater = secondaryIndex.search(word2);

	if (greater.size() < lesser.size()) {
		swap(greater, lesser);
	}

	for (size_t i = 0; i < lesser.size(); ++i) {
		for (size_t j = 0; j < greater.size(); ++j)
			if (lesser[i] == greater[j]) {
				string toAdd = readName(manpageRecordFileName_, lesser[i]);
				ret.push_back(toAdd);
			}
	}

	return ret;
}
/**
 * Deleta os arquivos de indice primário e secundário do disco
 */
void Database::clear() {
	ofstream manpage(manpageRecordFileName_.c_str(), ios::trunc);
	ofstream inverted(invertedListRecordFileName_.c_str(), ios::trunc);
}

/**
 *	Escreve um registro na posicao index
 *	@param manpage a ser escrita em disco
 *	@param nome do arquivo onde a manpage será escrita
 *	@param posicao relativa do arquivo no arquivo de manpages
 */
void Database::writeRecord(diskManpage manpage, string fileName, int index) {
	ofstream output(fileName.c_str(), ios::app | ios::binary);

	if (!output) {
		cout << "Erro ao abrir arquivo." << endl;
		return;
	}

	if (index > -1) {
		output.seekp(streampos(index * sizeof(diskManpage)));
	}

	diskManpage disk(manpage.name, manpage.content);

	output.write((char *) &disk, sizeof(diskManpage));
}

/**
 * Le um registro de manpage do arquivo fileName na posicao recordIndex
 * @param O nome do arquivo que se deseja ler
 * @param A posicao do registro que sera lido
 * @return diskManpage lido da posicao recordIndex
 */
diskManpage Database::readRecord(string fileName, int recordIndex) {
	ifstream input(fileName.c_str(), ios::in | ios::binary);

	if (!input) {
		cout << "Erro ao abrir arquivo" << endl;
	}

	input.seekg(streampos(recordIndex * sizeof(diskManpage)));

	diskManpage mp("", "");
	input.read((char *) &mp, sizeof(diskManpage));

	return mp;
}

/**
 * Le o nome do comando na posicao recordIndex
 * @param O nome do arquivo que se deseja ler
 * @param a posicao do registro que sera lido
 * @return nome do comando na posicao recordIndex
 */
string Database::readName(string fileName, int recordIndex) {
	ifstream input(fileName.c_str(), ios::in | ios::binary);

	if (!input) {
		cout << "Erro ao abrir arquivo" << endl;
	}

	input.seekg(streampos(recordIndex * sizeof(diskManpage)));

	char name[MAX_MANPAGE_NAME_SIZE];

	input.read(name, MAX_MANPAGE_NAME_SIZE);


	return name;
}
