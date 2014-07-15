/*
 * WriteTreeToDisk.cpp
 *
 *  Created on: 15/07/2014
 *      Author: nicolas
 */

#include <fstream>
#include <iostream>

#include "QueryException.h"
#include "WriteTreeToDisk.h"

using namespace std;

/**
 * Escreve uma arvore de indexacao secundaria no disco como um vetor ordenado
 * ao mesmo tempo em que escreve a lista invertida contendo os elementos de cada subarvore da arvore
 * de indexacao secundaria
 * @param Nome do arquivo onde essa arvore sera gravada
 * @param Nome do arquivo onde as listas invertidas serao gravadas
 * @param Arvore que sera gravada no arquivo
 */
void writeSecondaryTreeToDisk(std::string treeFilename,
		std::string invertedListFilename, SecundaryTree& tree) {
	std::ofstream treeFile(treeFilename.c_str(), std::ios::binary);

	if (!treeFile) {
		std::cout << "Erro ao abrir o arquivo " << treeFilename << std::endl;
	}

	int treeSize = tree.size();
	treeFile.write((char *) &treeSize, sizeof(int)); //grava o tamanho da lista
	int greatestListSize = tree.greatestListSize() + 1; //+1 espaço para o -1 no fim
	//escreve o tamanho da maior lista
	std::ofstream invertedListFile(invertedListFilename.c_str(),
			std::ios::binary);
	if (!invertedListFile) {
		std::cout << "Erro ao abrir o arquivo " << invertedListFilename
				<< std::endl;
	}

	invertedListFile.write((char *) &greatestListSize, sizeof(int));

	int count = 0;
	std::deque<StringTreeUnion> deque = tree.toDeque();
	while (deque.size() > 0) {
		StringTreeUnion actual = deque.front();
		deque.pop_front();
		int i = 0;
		std::deque<int> indexes = actual.tree->toDeque();
		while (indexes.size() > 0) {
			int toWrite = indexes.front();
			indexes.pop_front();
			invertedListFile.seekp(
					count * greatestListSize * sizeof(int) + i * sizeof(int)
							+ sizeof(int));
			invertedListFile.write((char *) &toWrite, sizeof(int));
			++i;
		}
		int endOfList = -1;
		invertedListFile.seekp(
				count * greatestListSize * sizeof(int) + i * sizeof(int)
						+ sizeof(int));
		invertedListFile.write((char *) &endOfList, sizeof(int)); //escreve -1 depois do ultimo elemento para marcar o fim da lista
		diskNode add(actual.string.c_str(), count); //nodo contendo a palavra e a localizacao de sua lista no arquivo
		treeFile.seekp(count * sizeof(diskNode) + sizeof(int));
		treeFile.write((char *) &add, sizeof(diskNode));
		++count; //atualizar posicao onde a proxima lista vai ser gravada no arquivo
	}
}

/**
 * Escreve uma arvore de indexacao primaria no disco como um vetor ordenado
 * @param Nome do arquivo onde essa arvore sera gravada
 * @param Arvore que sera gravada no arquivo
 */
void writePrimaryTreeToDisk(std::string treeFilename, PrimaryTree& tree) {
	std::ofstream treeFile(treeFilename.c_str(), std::ios::binary);

	if (!treeFile) {
		std::cout << "Erro ao abrir o arquivo " << treeFilename << std::endl;
	}
	int treeSize = tree.size();
	treeFile.write((char *) &treeSize, sizeof(int)); //grava o tamanho da lista
	int count = 0;
	std::deque<StringIntUnion> nodes = tree.toDeque();
	while (nodes.size() > 0) {
		StringIntUnion actual = nodes.front();
		nodes.pop_front();
		cout << actual.string << " " << actual.integer << endl;
		diskNode add(actual.string.c_str(), actual.integer); //nodo contendo a palavra e a localizacao da sua manpage no arquivo
		treeFile.seekp(count * sizeof(diskNode) + sizeof(int));
		treeFile.write((char *) &add, sizeof(diskNode));
		++count; //atualizar posicao onde a proxima lista vai ser gravada no arquivo
	}
}

/**
 * Le uma lista invertida do disco
 * @param nome do arquivo onde as listas estao salvas
 * @param posicao da lista que se deseja ler no arquivo de listas
 * @return lista de posicoes das manpages que esta lista contem
 */
std::deque<int> readInvertedList(std::string invertedListFileName,
		int indexOfList) {
	std::ifstream file(invertedListFileName.c_str(), std::ios::binary);

	if (!file) {
		std::cout << "Erro ao abrir o arquivo " << invertedListFileName
				<< std::endl;
	}

	std::deque<int> numbers;
	int size;
	file.read((char *) &size, sizeof(int)); //lê o tamanho padrao das listas
	int listPosition = indexOfList * size; //posicao da lista no arquivo

	int read;
	for (int i = 0; i < size; ++i) {
		file.seekg(listPosition * sizeof(int) + i * sizeof(int) + sizeof(int)); //posicao lista + posicao elemento + int (tamanho no inicio)
		file.read((char *) &read, sizeof(int));
		if (read == -1) //fim da lista
			break;
		numbers.push_back(read);
	}
	return numbers;
}

/**
 * Faz uma pesquisa numa arvore em disco, retornando o inteiro guardado junto com a string
 * @param Nome do arquivo onde a arvore  esta gravada
 * @param Palavra que será pesquisada na arvore
 *
 * A arvore e gravada no disco como uma lista ordenada, tendo seu tamanho no inicio
 */
int searchTreeOnDisk(std::string filename, std::string toSearch) {
	std::ifstream list(filename.c_str(), std::ios::binary);

	if (!list) {
		std::cout << "Erro ao abrir o arquivo " << filename << std::endl;
	}

	int size;
	list.read((char *) &size, sizeof(int));
	list.seekg(sizeof(int));

	int left = 0, right = size - 1;
	diskNode found("", 0);
	while (left <= right) {
		int middle = (right + left) / 2;
		list.seekg(middle * sizeof(diskNode) + sizeof(int));
		list.read((char *) &found, sizeof(diskNode));
		if (toSearch == found.word) {
			return found.listPosition;
		} else if (toSearch > found.word) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}
	throw QueryException();
}

int binarySearch(const std::deque<int> &array, int first, int last, int search_key) {
	int index;

	if (first > last) {
		index = -1;
	}
	else {
		int middle = (first + last) / 2;

		if (search_key == array[middle])
			index = middle;
		else if (search_key < array[middle])
			index = binarySearch(array, first, middle -1, search_key);
		else
			index = binarySearch(array, middle - 1, last, search_key);

	}
	return index;
}
