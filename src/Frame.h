/*
 * Frame.h
 *
 *  Created on: Dec 25, 2018
 *      Author: caroceanjofers
 */

#ifndef FRAME_H_
#define FRAME_H_
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Frame {
private:
	std::map<std::string, std::string> _heads;
	std::map<std::string, std::string> _parameters;
	std::string _content;
	bool endWith(const string &str, const string &tail) {
		return str.compare(str.size() - tail.size(), tail.size(), tail) == 0;
	}

	bool startWith(const string &str, const string &head) {
		return str.compare(0, head.size(), head) == 0;
	}

	void split(const string &strtem, const char a, vector<string> &res) {
		res.clear();
		string::size_type pos1, pos2;
		pos2 = strtem.find(a);
		pos1 = 0;
		while (string::npos != pos2) {
			res.push_back(strtem.substr(pos1, pos2 - pos1));
			pos1 = pos2 + 1;
			pos2 = strtem.find(a, pos1);
		}
		res.push_back(strtem.substr(pos1));
	}

public:
	Frame(std::string frame_line);
	Frame(websocketpp::config::asio_client::message_type::ptr msg);
	std::string toString();
	std::string head(std::string key);
	std::string parameter(std::string key);
	void head(std::string key, std::string value);
	void parameter(std::string key, std::string value);
	std::string url();
	std::string command();
	std::string protocol();
	std::string* content();
	virtual ~Frame();
};

#endif /* FRAME_H_ */
