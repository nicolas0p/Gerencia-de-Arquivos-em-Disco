/*
 * teste.cpp
 *
 *  Created on: 10/07/2014
 *      Author: nicolas
 */

#include <iostream> //cout
#include <fstream> //fstream
#include <string>
#include <vector>
#include <sstream>

#include "Manpage.h"
#include "Database.h"
#include "QueryException.h"
#include "SecundaryTree.h"

using namespace std;

/**
 * Console de pesquisa por nome da manpage
 * @param Database onde será feita a pesquisa por nome
 */
void pesquisaNome(Database& database) {
	string input;
	while (true) {
		cout << "Deseja pesquisar por qual manpage?" << endl;
		getline(cin, input);
		if (input.length() > 0)
			break;
	}
	diskManpage result("", "");
	try {
		result = database.nameQuery(input);
	} catch (QueryException& e) {
		cout << "Nenhuma manpage encontrada com este nome" << endl;
	}
	cout << result.content << endl;
}

/**
 * Console de pesquisa por conteudo em manpages
 * @param Database onde será feita a pesquisa
 */
void pesquisaConteudo(const Database& database) {
	string input;
	while (true) {
		cout << "Deseja pesquisar por qual palavra?" << endl;
		getline(cin, input);

		if (input.length() > 0)
			break;
	}
	deque<string> result;
	try {
		result = database.contentQuery(input);
	} catch (QueryException& e) {
		cout << "Nenhuma manpage contém esta palavra" << endl;
		return;
	}
	cout << "Manpages que contem esta palavra:" << endl;
	for (size_t i = 0; i < result.size(); ++i) {
		cout << result[i] << "	";
		if (i % 4 == 3) { //mostra 4 manpages por linha
			cout << endl;
		}
	}

	cout << endl;
}

/**
 * Console para pesquisa por duas palavras contendo nas manpages
 * @param database onde a pesquisa será feita
 */
void pesquisaMultiplosConteudos(const Database& database) {
	string input;
	int quant = 2;
	deque<string> words;
	for (; quant > 0; --quant) {
		while (true) {
			cout << "Digite a palavra que deseja:" << endl;
			getline(cin, input);
			if (input.length() > 0)
				break;
		}
		words.push_back(input);
	}
	deque<string> result;
	try {
		result = database.multipleContentQuery(words[0], words[1]);
	} catch (QueryException& e) {
		cout << "Nenhuma manpage contem todas estas palavras!" << endl;
		return;
	}
	cout << "Manpages que contem as duas palavra:" << endl;
	for (size_t i = 0; i < result.size(); ++i) {
		cout << result[i] << "	";
		if (i % 4 == 3) { //mostra 4 manpages por linha
			cout << endl;
		}
	}
	cout << endl;
}

/**
 * retorna a posicao do elemento searched no array por busca binaria
 */
int search(int* array, int searched, int left, int right) {
	int middle = (right + left) / 2;
	if (searched == array[middle])
		return middle;
	if (searched > array[middle]) {
		return search(array, searched, middle, right);
	} else {
		return search(array, searched, left, middle);
	}
	if (left == right)
		return -1;
}

/**
 * Escreve os arquivos recebidos de parametro na execucao do programa
 * @param numero de argumentos
 * @param array de argumentos
 * @param Database onde sera adicionados as manpages
 */
void indexFiles(int argc, char** argv, Database& database) {
	int i = 0;
	for (--argc; argc > 0; --argc) { //argv[0] é o nome do nosso comando
		string filename = argv[argc];
		database.insert(filename);
		cout << "Arquivo-" << i << " "<< filename << " lido" << endl;
		++i;
	}
}

/**
 * Mostra as opcoes ao usuario
 * @param database que sera usada
 */
void console(int argc, char** argv, Database& database) {
	string input;

	char deletar = 'n'; //trocar
	while (deletar != 's' && deletar != 'n' && deletar != 'y') {
		cout << "Deseja utilizar a indexação de manpages existente? Se nao, os arquivos anteriores serao deletados:" << endl;
		getline(cin, input);
		deletar = tolower(input.at(0));
	}
	if (deletar == 'n') {
		database.clear();
	}

	indexFiles(argc, argv, database);

	bool state = true;
	while (state) {
		int opcao;
		while (true) {
			cout << "Opcoes:" << endl;
			cout << "1) Fazer uma pesquisa por nome da manpage." << endl;
			cout << "2) Fazer uma pesquisa por manpage contendo uma palavra expecifica" << endl;
			cout << "3) Fazer uma pesquisa por manpage contendo diversas palavras" << endl;
			cout << "4) Sair do programa" << endl;
			getline(cin, input);
			stringstream nro(input);
			nro >> opcao;
			if (opcao > 0 && opcao < 5)
				break;
			cout << "Opcao invalida. Digite um numero entre 1 e 4" << endl;
		}
		switch (opcao) {
		case 1:
			pesquisaNome(database);
			break;
		case 2:
			pesquisaConteudo(database);
			break;
		case 3:
			pesquisaMultiplosConteudos(database);
			break;
		case 4:
			state = false;
			break;
		}
	}
}

int main(int argc, char** argv) {
	Database database("manpages.dat", "invertedLists.dat");

	console(argc, argv, database);
}
