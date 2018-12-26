/*
 * NetException.h
 *
 *  Created on: Dec 26, 2018
 *      Author: caroceanjofers
 */

#ifndef NETEXCEPTION_H_
#define NETEXCEPTION_H_
#include <exception>
#include <iostream>
using namespace std;

class NetException: public exception {
private:
	const char* msg;
public:
	NetException(const char* str);
	virtual ~NetException();
};

#endif /* NETEXCEPTION_H_ */
