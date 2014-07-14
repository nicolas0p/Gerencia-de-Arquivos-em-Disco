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
 * Retira tudo que vem depois do ultimo ponto de uma string
 * @param Palavra que se quer tirar a extensao
 * @return A palavra sem extensao
 */
string clearExtension(string name) {
	int i = name.length() - 1;
	for (; i >= 0; --i) {
		if (name[i] == '.') {
			break;
		}
	}
	return name.substr(0, i);
}

void pesquisaNome(Database database) {
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

void pesquisaConteudo(Database database) {
	string input;
	while (true) {
		cout << "Deseja pesquisar por qual palavra?" << endl;
		getline(cin, input);
		if (input.length() > 0)
			break;
	}
	vector<string> result;
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
}

void pesquisaMultiplosConteudos(Database database) {
	string input;
	int quant;
	deque<string> words;
	while (true) {
		cout << "Deseja pequisar por quantas palavras?" << endl;
		getline(cin, input);
		stringstream nro(input);
		nro >> quant;
		if (quant > 0)
			break;
		cout << "Opcao invalida. Digite um numero positivo." << endl;
	}
	for(; quant > 0; --quant) {
		while(true) {
			cout << "Digite a palavra que deseja:" << endl;
			getline(cin, input);
			if(input.length() > 0)
				break;
		}
		words.push_back(input);
	}
	vector<string> result;
	try {
		//result = database.multipleContentQuery();
	} catch(QueryException& e) {
		cout << "Nenhuma manpage contem todas estas palavras!" << endl;
	}
}

/**
 * Adiciona todas as manpages ao banco de dados
 */
int main(int argc, char** argv) {
	Database database("manpages.dat", "invertedLists.dat");

	vector<string> words;
	for (--argc; argc > 0; --argc) { //argv[0] é o nome do nosso comando
		string filename = clearExtension(argv[argc]);
		string concatenated(""), actual;
		ifstream file(filename.c_str());
		while (getline(file, actual)) {
			concatenated += actual + "\n";
			stringstream line(actual);
			while (line >> actual) {
				words.push_back(actual);
			}
		}
		database.insert(Manpage(filename, words),
				diskManpage(filename.c_str(), concatenated.c_str()));
	}

	string word("and");

}

void menu() {

}
/**
 * retorna a posicao do elemento searched no array
 */
int search(int* array, int searched, int left, int right) {
	int middle = (right + left) / 2;
	if (searched == array[middle])
		return middle;
	if (searched > array[right - left]) {
		return search(array, searched, right - left, right);
	} else {
		return search(array, searched, left, right - left);
	}
	if (left == right)
		return -1;
}

void console() {
	Database database("manpages.dat", "invertedLists.dat");

	string input;

	char deletar = 'k';
	while (deletar != 's' && deletar != 'n' && deletar != 'y') {
		cout << "Deseja utilizar a indexação de manpages existente? Se nao, os arquivos anteriores serão deletados:"<< endl;
		getline(cin, input);
		deletar = tolower(input.at(0));
	}
	if (deletar == 'n') {
		database.clear();
	}

	int opcao;
	while (true) {
		cout << "Opcoes:" << endl;
		cout << "1) Fazer uma pesquisa por nome da manpage." << endl;
		cout
		<< "2) Fazer uma pesquisa por manpage contendo uma palavra expecifica"
		<< endl;
		cout << "3) Fazer uma pesquisa por manpage contendo diversas palavras"
				<< endl;
		getline(cin, input);
		stringstream nro(input);
		nro >> opcao;
		if (opcao > 0 && opcao < 4)
			break;
		cout << "Opcao invalida. Digite um numero entre 1 e 3" << endl;
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
	}

}
