/*
 * TextProcessing.cpp
 *
 *  Created on: 15/07/2014
 *      Author: nicolas
 */

#include <cstring>

#include "TextProcessing.h"

/**
 * Separa as palavras pelos caracteres em limiters
 */
std::deque<std::string> tokenizer(char text[], char limiters[]){
    std::deque<std::string> words;
    char *word = strtok(text, limiters);
    for(int i = 0; word; i++) {
        words.push_back(std::string(word));
        word = strtok(NULL, limiters);
    }
    return words;
}

/**
 * Retira tudo que vem depois do ultimo ponto de uma string
 * @param Palavra que se quer tirar a extensao
 * @return A palavra sem extensao
 */
std::string removeExtension(std::string& name) {
	int i = name.length() - 1;
	for (; i >= 0; --i) {
		if (name[i] == '.') {
			break;
		}
	}
	return name.substr(0, i);
}
