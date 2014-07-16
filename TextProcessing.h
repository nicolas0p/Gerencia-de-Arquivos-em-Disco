/*
 * TextProcessing.h
 *
 *  Created on: 15/07/2014
 *      Author: nicolas
 */

#ifndef TEXTPROCESSING_H_
#define TEXTPROCESSING_H_

#include <deque>
#include <string>


std::deque<std::string> tokenizer(char text[], char limiters[]);
std::string removeExtension(std::string& name);

#endif /* TEXTPROCESSING_H_ */
