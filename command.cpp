#include "command.h"

int Command::findIndexOfFilter(int argc, char **argv) {
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
		indexOfFilter = this->findIndexOfFilter(argc,argv);
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

void Command::catNav() {
	cout << left << setw(10) << "Proto"
		<< left << setw(40) << "Local Address"
		<< left << setw(40) << "Foreign Address"
		<< left << setw(40) << "PID/Program name and arguments"
		<< endl;
}

void Command::catInfo(const char *protocol) {
	char file[16] = "/proc/net/", p[16];
	strcpy(p, protocol);
	strcat(file, p);
	FILE *fp = fopen(file, "r");
	char buf[BUFFSIZE];
	bool isNavRead = false;
	while (fgets(buf, BUFFSIZE, fp) != NULL) {
		if(!isNavRead) {
			isNavRead = true;
		}
		else {
			Info info(buf);
			info.cat(protocol, this->filter);
		}
	}
	fclose(fp);
}

void Command::catTCP() {
	cout << "List of TCP connections:" << endl;
	this->catNav();
	this->catInfo("tcp");
	this->catInfo("tcp6");
}

void Command::catUDP() {
	cout << "List of UDP connections:" << endl;
	this->catNav();
	this->catInfo("udp");
	this->catInfo("udp6");
}

void Command::cat() {
	if(this->tcpFlag) {
		this->catTCP();
	}
	if(this->udpFlag) {
		this->catUDP();
	}
}
