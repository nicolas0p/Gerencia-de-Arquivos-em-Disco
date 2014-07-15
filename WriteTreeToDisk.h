/*
 * PrimaryDiskTree.cpp
 *
 *  Created on: 13/07/2014
 *      Author: nicolas
 */

#include <deque>
//#include <math.h>
#include <fstream>
#include <string>

#include "QueryException.h"
#include "SecundaryTree.h"

const int greatestWordSize = 84;

struct diskNode {
	char word[greatestWordSize];
	int listPosition;

	diskNode(const char *name, int listPosition) :
		listPosition(listPosition) {
		strcpy(this->word, name);
	}
};

void writeInvertedListToDisk(std::string filename, int listRegisterSize,
		AvlTree* tree, int index);

/**
 * Escreve uma arvore de indexacao secundaria no disco como um vetor ordenado
 * @param Nome do arquivo onde essa arvore sera gravada
 * @param Nome do arquivo onde as listas invertidas serao gravadas
 * @param Arvore que sera gravada no arquivo
 */
void writeSecondaryTreeToDisk(std::string treeFilename,	std::string invertedListFilename, SecundaryTree& tree) {
	std::ofstream treeFile(treeFilename.c_str(), std::ios::binary);

	int treeSize = tree.size();
	treeFile.write((char *) &treeSize, sizeof(int)); //grava o tamanho da lista

	int greatestListSize = tree.greatestListSize() + 1; //+1 espaço para o -1 no fim

	//escreve o tamanho da maior lista
	std::ofstream invertedListFile(invertedListFilename.c_str(), std::ios::binary);
	invertedListFile.write((char *) &greatestListSize, sizeof(int));

	int invertedListPosition = 0;
	int treeNodeIndex = 0;
	for (SecundaryTree::iterator it = tree.begin(); it != tree.end(); ++it) {
		StringTreeUnion actual = *it;
		diskNode add(actual.string.c_str(), invertedListPosition); //nodo contendo a palavra e a localizacao de sua lista no arquivo
		writeInvertedListToDisk(invertedListFilename, greatestListSize,	actual.tree, invertedListPosition);

		treeFile.seekp(treeNodeIndex * sizeof(diskNode) + sizeof(int));
		treeFile.write((char *) &add, sizeof(diskNode));

		++treeNodeIndex;
		++invertedListPosition; //atualizar posicao onde a proxima lista vai ser gravada no arquivo
	}
}

/**
 * Escreve uma lista invertida em formato arvore avl na memoria como um vetor ordenado
 * @param Nome do arquivo onde sera gravado o vetor
 * @param Tamanho da maior lista para ser o tamanho do registro
 * @param Arvore que sera gravada
 * @param Posicao no arquivo onde esta lista sera gravada
 */
void writeInvertedListToDisk(std::string filename, int greaterListSize, AvlTree* tree, int index) {
	std::ofstream file(filename.c_str(), std::ios::binary);
	int listPosition = index * greaterListSize; //posicao dessa lista no arquivo = i * tamanho padrao lista

	int i = 0;
	for (AvlTree::iterator it = tree->begin(); it != tree->end(); ++it) { //itera sobre a arvore
		int toWrite = *it;
		int elementPosition = i * sizeof(int); //posicao do elemento na lista

		file.seekp(listPosition + elementPosition + sizeof(int));
		file.write((char *) &toWrite, sizeof(int));
		++i;
	}
	int endOfList = -1;
	file.seekp(listPosition + (i+1) * sizeof(int));
	file.write((char *) &endOfList, sizeof(int)); //escreve -1 depois do ultimo elemento para marcar o fim da lista
}

/**
 * Le uma lista invertida do disco
 * @param nome do arquivo onde as listas estao salvas
 * @param posicao da lista que se deseja ler no arquivo de listas
 */
std::deque<int> readInvertedList(std::string invertedListFileName, int indexOfList) {
	std::ifstream file(invertedListFileName.c_str(), std::ios::binary);
	std::deque<int> numbers;
	int size;
	file.read((char *) &size, sizeof(int)); //lê o tamanho padrao das listas
	int listPosition = indexOfList * size; //posicao da lista no arquivo

	int read;
	for (int i = 0; i < size; ++i) {
		int elementPosition = i * sizeof(int); //posicao desse elemento na lista

		file.seekg(listPosition + elementPosition + sizeof(int)); //posicao lista + posicao elemento + int (tamanho no inicio)
		file.read((char *) &read, sizeof(int));
		if(read == -1) //fim da lista
			break;
		numbers.push_back(read);
	}
	return numbers;
}

/**
 * Faz uma pesquisa numa arvore em disco, retornando o inteiro guardado junto com a string
 * @param Nome do arquivo onde a arvore esta gravada
 * @param Palavra que será pesquisada na arvore
 *
 * A arvore e gravada no disco como uma lista ordenada, tendo seu tamanho no inicio
 */
int searchTreeOnDisk(std::string& filename, std::string toSearch) {
        std::ifstream list(filename.c_str(), std::ios::binary);
        int size = 0;
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
