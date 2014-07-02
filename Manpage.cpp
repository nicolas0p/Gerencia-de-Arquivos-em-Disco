/*
 * Manpage.cpp
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#include "Manpage.h"

Manpage::Manpage(string name, list<string> words) : name_(name), words_(words){
}

Manpage::Manpage(const Manpage& manpage) : name_(manpage.name()), words_(manpage.words()){
}

Manpage::~Manpage() {
}

string Manpage::name() const {
	return name_;
}

list<string> Manpage::words() const {
	return words_;
}
