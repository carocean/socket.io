/*
 * NetException.cpp
 *
 *  Created on: Dec 26, 2018
 *      Author: caroceanjofers
 */

#include "NetException.h"



NetException::NetException(const char* str) {
	this->msg=str;
}

NetException::~NetException() {

}

