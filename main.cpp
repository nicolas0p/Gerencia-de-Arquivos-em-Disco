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

#include "Database.h"
#include "QueryException.h"
#include "SecundaryTree.h"
#include "DiskOperations.h"

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
		cout << "Nenhuma manpage contem ambas as palavras!" << endl;
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
	database.removeConnectives();
	database.writeIndexToDisk();
}

/**
 * Mostra as opcoes ao usuario
 * @param database que sera usada
 */
void console(int argc, char** argv, Database& database) {
	bool state = true;
	string input;
	if (!database.filesIndexed()) {
		cout << "Os arquivos ainda não foram indexados. Para realizar alguma busca, é necessário indexa-los (isso pode levar algum tempo)." << endl;
		cout << "Você deseja indexar os arquivos? Se não, o programa será fechado." << endl;
		char opcao = 'k';
		while (opcao != 'n' && opcao != 's') {
			cout << "Digite sim ou não." << endl;
			getline(cin, input);
			opcao = tolower(input.at(0));
			if (opcao == 's') {
				database.clear();
				indexFiles(argc, argv, database);
			} else if (opcao == 'n') {
				state = false;
			}
		}
	}
	while (state) {
		int opcao;
		while (true) {
			cout << "Digite o numero da opcao desejada:" << endl;
			cout << "1) Fazer uma pesquisa por nome da manpage." << endl;
			cout << "2) Fazer uma pesquisa por palavra no conteúdo de uma manpage" << endl;
			cout << "3) Fazer uma pesquisa por manpage contendo duas palavras expecíficas" << endl;
			cout << "4) Indexar arquivos novamente (pode levar algum tempo)." << endl;
			cout << "5) Sair do programa" << endl;
			getline(cin, input);
			stringstream nro(input);
			nro >> opcao;
			if (opcao > 0 && opcao < 6)
				break;
			cout << "Opcao invalida. Digite um numero entre 1 e 5" << endl;
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
			database.clear();
			indexFiles(argc, argv, database);
			database.removeConnectives();
			database.writeIndexToDisk();
			break;
		case 5:
			state = false;
			break;
		}
	}
}

int main(int argc, char** argv) {
	Database database("manpages.dat", "primaryIndex.dat", "secondaryIndex.dat","invertedLists.dat");
	//database.clear();
	console(argc, argv, database);

}
/*
int main() {
	Database database("manpages.dat", "primaryIndex.dat", "secondaryIndex.dat","invertedLists.dat");
	database.clear();
	cout << database.filesIndexed();
}*/
