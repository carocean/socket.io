/*
 * Frame.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: caroceanjofers
 */
#include <stdexcept>
#include "Frame.h"
#include "NetException.h"

Frame::Frame(std::string frame_line) {
	string fline = frame_line;
	while (startWith(fline, " ")) {
		fline = fline.substr(1, fline.length());
	}
	while (endWith(fline, " ")) {
		fline = fline.substr(0, fline.length() - 1);
	}
	string cmd = "";
	string url = "";
	string pt = "";
	int pos = fline.find(' ');
	if (pos < 0) {
		throw 1; //侦格式错误：没有包含段
	}
	cmd = fline.substr(0, pos);
	string remain = fline.substr(pos, fline.length() - cmd.length());
	while (startWith(remain, " ")) {
		remain = remain.substr(1, remain.length());
	}

	if (!startWith(remain, "/")) {
		throw NetException("侦格式错误：没有url或，url不是以/开头"); //侦格式错误：没有url或，url不是以/开头
	}
	pos = remain.find(' ');
	if (pos < 0) {
		throw NetException("侦格式错误：缺少协议或url"); //侦格式错误：缺少协议或url
	}
	if (remain.length() <= 0) {
		throw NetException("侦格式错误：缺少协议");//侦格式错误：缺少协议
	}
	url = remain.substr(0, pos);
	remain = remain.substr(pos, remain.length() - url.length());
	while (startWith(remain, " ")) {
		remain = remain.substr(1, remain.length());
	}
	pt = remain;

	(_heads)["command"] = cmd;
	(_heads)["url"] = url;
	(_heads)["protocol"] = pt;
}

std::string Frame::toString() {
	string refstr;
	string *str = &refstr;
	for (map<std::string, std::string>::iterator it = _heads.begin();
			it != _heads.end(); it++) {
		str->append(it->first + "=" + it->second + "\r\n");
	}
	str->append("\r\n");
	for (map<std::string, std::string>::iterator it = _parameters.begin();
			it != _parameters.end(); it++) {
		str->append(it->first + "=" + it->second + "\r\n");
	}
	str->append("\r\n");
	if (_content.length() > 0) {
		str->append(_content);
	}

	return *str;
}

std::string Frame::head(std::string key) {
	return (_heads)[key];
}

std::string Frame::parameter(std::string key) {
	return (_parameters)[key];
}

void Frame::head(std::string key, std::string value) {
	(_heads)[key] = value;
}

void Frame::parameter(std::string key, std::string value) {
	(_parameters)[key] = value;
}

std::string Frame::url() {
	return (_heads)["url"];
}

std::string Frame::command() {
	return (_heads)["command"];
}

std::string Frame::protocol() {
	return (_heads)["protocol"];
}

std::string* Frame::content() {
	return &_content;
}

Frame::Frame(websocketpp::config::asio_client::message_type::ptr msg) {
	string frameRaw = msg->get_payload();
	int up = 0;
	int down = 0;
	char field = 0; // 0=heads;1=params;2=content
	while (down < frameRaw.length()) {
		if (field < 2) { // 修改了当内容的头几行是连续空行的情况的bug因此使用了field<2
			if (frameRaw[up] == '\r'
					&& (up + 1 < frameRaw.length() && frameRaw[up + 1] == '\n')) { // 跳域
				field++;
				up += 2;
				down += 2;
				continue;
			}
		} else {
			down = frameRaw.length();
			int len=down - up;
			char* b = new char[len];
			frameRaw.copy(b, len, up);
			_content.append(b,len);
			break;
		}

		if (frameRaw[down] == '\r'
				&& (down + 1 < frameRaw.length() && frameRaw[down + 1] == '\n')) { // 跳行
			int len=down - up;
			char* b = new char[len];
			frameRaw.copy(b, len, up);
			try {
				switch (field) {
				case 0: {
					string kv(b,len);//由于c++字符串\0的问题，所以必须指定char*的长度，否则尾部乱码
					int at = kv.find("=");
					string k = kv.substr(0, at);
					string v = kv.substr(at + 1, kv.length() - at+1);
					_heads[k] = v;
					break;
				}
				case 1: {
					string kv(b,len);
					int at = kv.find("=");
					string k = kv.substr(0, at);
					string v = kv.substr(at + 1, kv.length() - at+1);
					_parameters[k] = v;
					break;
				}
				}
			} catch (...) {
				cout << "解析侦时出错" << endl;
				throw NetException("解析侦时出错");
			}
			down += 2;
			up = down;
			continue;
		}
		down++;
	}
}

Frame::~Frame() {
	this->_heads.clear();
	this->_parameters.clear();
}

