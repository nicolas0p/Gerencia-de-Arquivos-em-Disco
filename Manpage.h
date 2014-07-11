/*
 * Manpage.h
 *
 *  Created on: 02/07/2014
 *      Author: nicolas
 */

#ifndef MANPAGE_H_
#define MANPAGE_H_

#include <string>
#include <vector>

using namespace std;

class Manpage {
public:
	Manpage(string name, vector<string> words);
	Manpage(const Manpage& manpage);
	~Manpage();

	string name() const;
	vector<string> words() const;

private:
	string name_;
	vector<string> words_;
};

#endif /* MANPAGE_H_ */
