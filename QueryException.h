/*
 * QueryException.h
 *
 *  Created on: 11/07/2014
 *      Author: arthurhortmannerpen
 */

#ifndef QUERYEXCEPTION_H_
#define QUERYEXCEPTION_H_

#include <exception>

class QueryException : public std::exception {
public:
	QueryException();
	virtual ~QueryException() throw ();
};

#endif /* QUERYEXCEPTION_H_ */
