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
#include "WriteTreeToDisk.h"
#include "AvlTree.h"

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
	Manpage manpage(filename, words);
	diskManpage disk(filename.c_str(), concatenated.c_str());

	writeManPage(disk, manpageFileName_, manpageIndex_);
	primaryIndexTree.insert(manpage.name(), manpageIndex_);

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
	deque<string> ret;
	AvlTree *lesser = secondaryIndexTree.search(first);
	AvlTree *greater = secondaryIndexTree.search(second);

	if (greater->size() < lesser->size()) {
		swap(greater, lesser);
	}
	//cout << " hueeee" << endl;cout << " hueeee" << endl;cout << " hueeee" << endl;cout << " hueeee" << endl;cout << " hueeee" << endl;cout << " hueeee" << endl;cout << " hueeee" << endl;cout << " hueeee" << endl;cout << " hueeee" << endl;
	for (AvlTree::iterator it = lesser->begin(); it != lesser->end(); ++it) {
		if (greater->search(*it)) {
			ret.push_back(readName(manpageFileName_, *it));
		}
	}

	return ret;
}
/**
 * Deleta os arquivos de indice primário e secundário do disco
 */
void Database::clear() {
	ofstream manpage(manpageFileName_.c_str(), ios::trunc);
	ofstream primary(primaryIndexFileName_.c_str(), ios::trunc);
	ofstream secondary(secondaryIndexFileName_.c_str(), ios::trunc);
	ofstream inverted(invertedListFileName_.c_str(), ios::trunc);
}

/**
 *	Escreve um registro na posicao index
 *	@param manpage a ser escrita em disco
 *	@param nome do arquivo onde a manpage será escrita
 *	@param posicao relativa do arquivo no arquivo de manpages
 */
void Database::writeManPage(diskManpage& manpage, string fileName, int index) {
	ofstream output(fileName.c_str(), ios::app | ios::binary);

	if (!output) {
		cout << "Erro ao abrir arquivo " << fileName << endl;
		return;
	}

	if (index > -1) {
		output.seekp(index * sizeof(diskManpage));
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
diskManpage Database::readManPage(string fileName, int manpageIndex) const {
	ifstream input(fileName.c_str(), ios::in | ios::binary);

	if (!input) {
		cout << "Erro ao abrir arquivo " << fileName << endl;
	}

	input.seekg(manpageIndex * sizeof(diskManpage));

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
string Database::readName(string fileName, int recordIndex) const {
	ifstream input(fileName.c_str(), ios::in | ios::binary);

	if (!input) {
		cout << "Erro ao abrir arquivo " << fileName << endl;
	}

	input.seekg(recordIndex * sizeof(diskManpage));

	char name[MAX_MANPAGE_NAME_SIZE];

	input.read(name, MAX_MANPAGE_NAME_SIZE);

	return name;
}

/**
 * Retira tudo que vem depois do ultimo ponto de uma string
 * @param Palavra que se quer tirar a extensao
 * @return A palavra sem extensao
 */
string Database::removeExtension(string& name) const {
	int i = name.length() - 1;
	for (; i >= 0; --i) {
		if (name[i] == '.') {
			break;
		}
	}
	return name.substr(0, i);
}
