/*
 * PrimaryDiskTree.cpp
 *
 *  Created on: 13/07/2014
 *      Author: nicolas
 */

#include <deque>
#include <math.h>
#include <fstream>

#include "PrimaryDiskTree.h"

/**
 * Construtor de PrimaryDiskTree
 * @param Nome do arquivo onde a arvore deve ser gravada
 * @param Árvore primária que será gravado em disco
 */
PrimaryDiskTree::PrimaryDiskTree(std::string filename, PrimaryTree tree) :
		filename(filename) {
	int size = fullSize(tree.root_->height);
	PrimaryTree::Node* array[size];
	std::deque<PrimaryTree::Node*> queue;
	queue.push_back(tree.root_);
	for (size_t i = 0; i < tree.size(); ++i) {
		PrimaryTree::Node* e;
		e = queue.back();
		queue.pop_back();
		queue.push_back(e->left);
		queue.push_back(e->right);
		array[i] = e;
	}
	writeToDisc(array, size);
}

PrimaryDiskTree::~PrimaryDiskTree() {
}

/**
 * Retorna o tamanho de árvore se todos os níveis da arvore estivessem completos
 * @param Altura da raiz da arovore
 * @return 2 ^ (altura da raiz+1) - 1
 */
int PrimaryDiskTree::fullSize(int rootHeight) {
	return exp2(rootHeight+1)-1;
}

void PrimaryDiskTree::writeToDisc(PrimaryTree::Node **array, int size) {
	std::ofstream output(filename.c_str(), std::ios::binary);


	//output.write((char *) &disk, );
}


