#include "command.h"

int Command::checkIndexOfFilter(int argc, char **argv) {
	int index;
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
	if(argc == 1) {
		this->tcpFlag = true;
		this->udpFlag = true;
		this->filter = "";
		return;
	}
	int ch;
	int indexOfFilter = this->checkIndexOfFilter(argc,argv);
	if(indexOfFilter > 0) {
		this->filter = argv[indexOfFilter];
	}
	while((ch = getopt_long(argc, argv, shortOptions, longOptions, NULL)) != -1) {
		switch(ch) {
			case 't':
				this->tcpFlag = true;
				break;
			case 'u':
				this->udpFlag = true;
				break;
			default:
				break;
		}
	}
}

void Command::cat() {
	cout << "tcp:" << this->tcpFlag << endl;
	cout << "udp:" << this->udpFlag << endl;
	cout << "filter:" << this->filter << endl;
}
