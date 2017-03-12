#include "command.h"

int Command::checkIndexOfFilter(int argc, char **argv) {
	int index = -1;
	for(int i = 1;i < argc;i++) {
		string tmp(argv[i]);
		if(tmp.find_first_of("-", 0) != 0) {
			index = i;
			break;
		}
	}
	return index > 0 ? index : -1;
}

Command::Command(int argc, char **argv): tcpFlag(false), udpFlag(false), filter("") {
	int optionCount = 0,
		ch,
		indexOfFilter = this->checkIndexOfFilter(argc,argv);
	if(indexOfFilter > 0) {
		this->filter = argv[indexOfFilter];
	}
	while((ch = getopt_long(argc, argv, shortOptions, longOptions, NULL)) != -1) {
		switch(ch) {
			case 't':
				this->tcpFlag = true;
				optionCount++;
				break;
			case 'u':
				this->udpFlag = true;
				optionCount++;
				break;
			default:
				break;
		}
	}
	if(optionCount == 0) {
		this->tcpFlag = true;
		this->udpFlag = true;
	}
}

void Command::cat() {
	cout << "tcp:" << this->tcpFlag << endl;
	cout << "udp:" << this->udpFlag << endl;
	cout << "filter:" << this->filter << endl;
}
