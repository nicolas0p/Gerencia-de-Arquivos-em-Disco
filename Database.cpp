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

void writeRecord(diskManpage manpage, string fileName, int index);
diskManpage readRecord(string fileName,int recordIndex);

/**
 * Construtor de database
 * @param Nome do arquivo onde sera armazenado as listas invertidas para cada palavra
 * @param Nome do arquivo onde sera armazenado os registros de manpage
 */
Database::Database(string manpageRecordFileName,
		string invertedListRecordFileName) :
		primaryIndex(), secondaryIndex(), manpageRecordFileName_(
				manpageRecordFileName), invertedListRecordFileName_(
				invertedListRecordFileName), invertedListIndex(0), manpageIndex(0) {
}

Database::~Database() {
}

/**
 * Adiciona uma manpage no banco de dados
 * Isso inclui sua indexacao primaria(pelo seu nome) e secundaria(por todas as palavras que ele contem)
 *
 * @param manpage a ser adicionada no banco de dados
 * @param diskManpage a ser adicionada nos registros
 */
void Database::insert(const Manpage& manpage, const diskManpage& disk) {
	writeRecord(disk, manpageRecordFileName_, manpageIndex);
	//primaryIndex.insert(manpage.name(), manpageIndex);
	vector<string> words = manpage.words();
	for(size_t i = 0 ; i < manpage.words().size() ; ++i) { //adicionar todas as palavras do conteudo na indexacao secundaria
		//secondaryIndex.insert(words[i], manpageIndex);
	}
	++manpageIndex; //atualizar index
}

/**
 * Pesquisa no banco de dados a manpage com o nome recebido
 * @param nome da manpage a ser procurada
 * @return manpage com o nome recebido
 */
diskManpage Database::nameQuery(string name) {
	int position = 0;//primaryIndex.search(name);
	return readRecord(manpageRecordFileName_, position);
}
/**
 * Pesquisa no banco de dados pelas manpages que contem a palavra recebida
 * @param palavra que se deseja encontrar nas manpages
 * @return lista dos nomes das manpages que contem esta palavra
 */
vector<string> Database::contentQuery(string word) {
	deque<int> invertedList;//= secondaryIndex.search(word);
	vector<string> ret;
	for(size_t i = 0 ; i < invertedList.size() ; ++i) {
		diskManpage a = readRecord(manpageRecordFileName_, invertedList[i]);
		ret.push_back(a.name);
	}
	return ret;
}


/**
 *	Escreve um registro na posicao index
 *	@param manpage a ser escrita em disco
 *	@param posicao no arquivo onde esse regitros comecara
 */
void writeRecord(diskManpage manpage, string fileName, int index) {
	ofstream output(fileName.c_str(), ios::app | ios::binary);

	if (!output) {
		cout << "Erro ao abrir arquivo." << endl;
		return;
	}

	if (index != -1) {
		output.seekp(streampos(index * sizeof(diskManpage)));
	}

	diskManpage disk(manpage.name, manpage.content);

	output.write((char *) &disk, sizeof(diskManpage));
	output.close();
}
/**
 * Le um registro de manpage do arquivo FILE_NAME na posicao recordIndex
 * @param a posicao do registro que sera lido
 * @return diskManpage lido da posicao recordIndex
 */
diskManpage readRecord(string fileName,int recordIndex) {
	ifstream input(fileName.c_str(), ios::in | ios::binary);

	if (!input) {
		cout << "Erro ao abrir arquivo" << endl;
	}

	input.seekg(streampos(recordIndex * sizeof(diskManpage)));

	diskManpage mp("", "");
	input.read((char *) &mp, sizeof(diskManpage));
	input.close();

	return mp;
}
