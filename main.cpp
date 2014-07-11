/*
 * teste.cpp
 *
 *  Created on: 10/07/2014
 *      Author: nicolas
 */

#include <iostream> //cout
#include <fstream> //fstream
//#include <cstring> //strcpy
#include <string>
#include <vector>
#include <sstream>

#include "Manpage.h"
#include "Database.h"

using namespace std;

Manpage readFile(string fileName) { //possível otimização para lista com vetor
	vector<string> words;
	ifstream file(fileName.c_str());
	string actual;
	while (file >> actual) {
		words.push_back(actual);
		actual.clear();
	}
	file.close();
	return Manpage(fileName, words);
}

/**
 * Adiciona todas as manpages ao banco de dados
 */
int main(int argc, char** argv) {
	Database database("manpages.dat", "invertedLists.dat");
	vector<string> words;
	for (--argc; argc > 0; --argc) { //argv[0] é o nome do nosso comando
		string filename = argv[argc], concatenated(""), actual;
		ifstream file(filename.c_str());
		while (file >> actual) {
			concatenated += actual + " ";
			words.push_back(actual);
		}
		database.insert(Manpage(filename, words), diskManpage(filename.c_str(), concatenated.c_str()));
	}
}
