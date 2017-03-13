#include "info.h"

Info::Info(char *input) {
	input[strlen(input)] = '\0';
	string info(input), buf;
	stringstream ss(info);
	vector<string> tokens;

	while(ss >> buf) {
		tokens.push_back(buf);
	}
	this->local = tokens[1];
	this->foreign = tokens[2];
	this->inode = tokens[9];
}

string Info::handleInfo(const char *protocol, string info) {
	if(!strcmp(protocol, "tcp") || !strcmp(protocol, "udp")) {
		unsigned int ip, port;
		char buf[256];
		sscanf(info.c_str(), "%x:%x", &ip, &port);
		sprintf(buf, "%d.%d.%d.%d:%d", ip&0xff, (ip >> 8)&0xff, (ip >> 16)&0xff, ip >> 24, port);
		return buf;
	}
	else {
		return info;
	}
}

void Info::cat(const char *protocol, string filter) {
	cout << left << setw(10) << protocol
		<< left << setw(25) << this->handleInfo(protocol, this->local)
		<< left << setw(25) << this->handleInfo(protocol, this->foreign)
		<< left << setw(25) << this->inode
		<< endl;
}
