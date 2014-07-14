/*
 * Manpage.h
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#ifndef MANPAGE_H_
#define MANPAGE_H_

#include <string>
#include <deque>

using namespace std;

class Manpage {
public:
	Manpage(string name, deque<string> words);
	Manpage(const Manpage& manpage);
	~Manpage();

	string name() const;
	deque<string> words() const;

private:
	string name_;
	deque<string> words_;
};

#endif /* MANPAGE_H_ */
