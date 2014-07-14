/*
* PrimaryDiskTree.cpp
 *
 *  Created on: 13/07/2014
 *      Author: nicolas
 */

#include <deque>
#include <math.h>
#include <fstream>
#include <string>

#include "SecundaryTree.h"

const int greatestWordSize = 84;

struct diskSecondaryNode {
	char word[greatestWordSize];
	int listPosition;

	diskSecondaryNode(const char *name, int listPosition) : listPosition(listPosition) {
			strcpy(this->word, name);
		}
};


/**
 * Escreve uma arvore de indexacao secundaria no disco como um vetor ordenado
 * @param Nome do arquivo onde essa arvore sera gravada
 * @param Nome do arquivo onde as listas invertidas serao gravadas
 * @param Arvore que sera gravada no arquivo
 */
void writeSecondaryTreeToDisk(std::string treeFilename, std::string inverseListFilename, SecundaryTree tree) {
	std::ofstream treeFile(treeFilename.c_str(), std::ios::binary);

	treeFile.write((char *)tree.size(),sizeof(int)); //grava o tamanho da lista
	treeFile.seekp(sizeof(int));

	int greatestListSize = tree.greatestListSize()+1; //+1 espaço para o -1 no fim

	int invertedListPosition = 0;
	for(SecundaryTree::iterator it = tree.begin(); it != tree.end(); ++it) {
		StringTreeUnion actual = *it;
		diskSecondaryNode add(actual.string, invertedListPosition);
		writeInverseListToDisk(inverseListFilename, greatestListSize, actual.tree, invertedListPosition);
		++invertedListPosition;
	}
}

/**
 * Escreve uma lista invertida em formato arvore avl na memoria como um vetor ordenado
 * @param Nome do arquivo onde sera gravado o vetor
 * @param Tamanho da maior lista para ser o tamanho do registro
 * @param Arvore que sera gravada
 * @param Posicao no arquivo onde esta lista sera gravada
 */
void writeInverseListToDisk(std::string filename, int listRegisterSize, AvlTree* tree, int index) {
	std::ofstream file(filename.c_str(), std::ios::app | std::ios::binary);
	file.seekp(index * listRegisterSize);

	int i = 0;
	for(AvlTree::iterator it = tree->begin();it != tree->end();++it) {
		file.write((char *) *it, sizeof(int));
		file.seekp(index * listRegisterSize + i * sizeof(int));
		++i;
	}
	file.write((char *) -1, sizeof(int)); //escreve -1 depois do ultimo elemento para marcar o fim da lista
}
