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

void Command::catTCP() {
	cout << "List of TCP connections" << endl;
	//	IPv4
	FILE *fp = fopen("/proc/net/tcp", "r");
	char buf[256];
	while (fgets(buf, 256, fp) != NULL) {
		cout << buf << endl;
	}
	fclose(fp);
	//	IPv6
	fp = fopen("/proc/net/tcp6", "r");
	while (fgets(buf, 256, fp) != NULL) {
		cout << buf << endl;
	}
	fclose(fp);
}

void Command::catUDP() {
	cout << "List of UDP connections" << endl;
	//	IPv4
	FILE *fp = fopen("/proc/net/udp", "r");
	char buf[256];
	while (fgets(buf, 256, fp) != NULL) {
		cout << buf << endl;
	}
	fclose(fp);
	//	IPv6
	fp = fopen("/proc/net/udp6", "r");
	while (fgets(buf, 256, fp) != NULL) {
		cout << buf << endl;
	}
	fclose(fp);
}

void Command::cat() {
	if(this->tcpFlag) {
		this->catTCP();
	}
	if(this->udpFlag) {
		this->catUDP();
	}
	cout << "filter:" << this->filter << endl;
}
