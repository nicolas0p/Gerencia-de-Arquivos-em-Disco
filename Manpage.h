/*
 * Manpage.h
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#ifndef MANPAGE_H_
#define MANPAGE_H_

#include <string>
#include <list>

using namespace std;

class Manpage {
public:
	Manpage(string name, list<string> words);
	Manpage(const Manpage& manpage);
	~Manpage();

	string name() const;
	list<string> words() const;
	string content() const;

private:
	string name_;
	list<string> words_;
};

#endif /* MANPAGE_H_ */
