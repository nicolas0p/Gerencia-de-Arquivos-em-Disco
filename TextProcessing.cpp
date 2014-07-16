/*
 * TextProcessing.cpp
 *
 *  Created on: 15/07/2014
 *      Author: nicolas
 */

#include <cstring>

#include "TextProcessing.h"

/**
 * Retira tudo que vem depois do ultimo ponto de uma string
 * @param Palavra que se quer tirar a extensao
 * @return A palavra sem extensao
 */
std::string removeExtension(const std::string& name) {
	int i = name.length() - 1;
	for (; i >= 0; --i) {
		if (name[i] == '.') {
			break;
		}
	}
	return name.substr(0, i);
}
