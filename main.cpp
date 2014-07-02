/*
 * main.cpp
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#include <fstream>
#include <stdlib.h>
#include <list>
#include <iostream>

#include "Manpage.h"
#include "Database.h"

using namespace std;

Manpage readFile(string fileName) { //possível otimização para lista com vetor
	list<string> words;
	ifstream file(fileName.c_str());
	string actual;
	while(file >> actual) {
		words.push_back(actual);
		actual.clear();
	}
	file.close();
	return Manpage(fileName, words);
}

int main(int argc, char** argv) {
	Database database;
	for(--argc ; argc > 0 ; --argc) {
		database.insert(readFile(argv[argc]));
	}
}
