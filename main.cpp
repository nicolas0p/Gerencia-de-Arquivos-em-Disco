/*
 * teste.cpp
 *
 *  Created on: 10/07/2014
 *      Author: nicolas
 */

#include <iostream> //cout
#include <fstream> //fstream
#include <cstring> //strcpy
#include <string>
#include <vector>
#include <sstream>

#include "Manpage.h"

using namespace std;

string FILE_NAME = "manpages.dat";
#define MAX_MANPAGE_NAME_SIZE 80 //estes numeros foram testados
#define MAX_MANPAGE_CONTENT 392769

struct diskManpage { //struct que vai ser gravada em disco, nos registros
	char name[MAX_MANPAGE_NAME_SIZE];
	char content[MAX_MANPAGE_CONTENT];

	diskManpage(const char *name, const char *content) {
		strcpy(this->name, name);
		strcpy(this->content, content);
	}
};
/**
 *	Escreve um registro na posicao index
 *	@param manpage a ser escrita em disco
 *	@param posicao no arquivo onde esse regitros comecara
 */
void writeRecord(diskManpage manpage, int index) {
	ofstream output(FILE_NAME.c_str(), ios::app | ios::binary);

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
diskManpage readRecord(int recordIndex) {
	ifstream input(FILE_NAME.c_str(), ios::in | ios::binary);

	if (!input) {
		cout << "Erro ao abrir arquivo" << endl;
	}

	input.seekg(streampos(recordIndex * sizeof(diskManpage)));

	diskManpage mp("", "");
	input.read((char *) &mp, sizeof(diskManpage));
	input.close();

	return mp;
}

Manpage readFile(string fileName) { //possível otimização para lista com vetor
	list<string> words;
	ifstream file(fileName.c_str());
	string actual;
	while (file >> actual) {
		words.push_back(actual);
		actual.clear();
	}
	file.close();
	return Manpage(fileName, words);
}

int main(int argc, char** argv) {
	vector<diskManpage> array;
	int index = 0;
	for (--argc; argc > 0; --argc) { //argv[0] é o nome do nosso comando
		string filename = argv[argc], concatenated(""), actual;
		ifstream file(filename.c_str());
		while (file >> actual) {
			concatenated += actual + " ";
		}
		writeRecord(diskManpage(filename.c_str(), concatenated.c_str()), index);
		++index;
	}
}
