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

string Info::handleinode() {
	string linkPid;
	bool isFind = false;
	DIR *dir;
	struct dirent *ptr;
	dir = opendir("/proc");
	//	traverse all pid
	while((ptr = readdir(dir)) != NULL) {
		if(!isdigit(ptr->d_name[0])) continue;
		char innerDirPath[32];
		DIR *innerDir;
		struct dirent *innerPtr;
		sprintf(innerDirPath, "%s%s%s", "/proc/", ptr->d_name, "/fd");
		innerDir = opendir(innerDirPath);
		//	traverse /pid/fd
		while((innerPtr = readdir(innerDir)) != NULL) {
			if(!isdigit(innerPtr->d_name[0])) continue;
			char buf[BUFFSIZE], link[256];
			sprintf(link, "%s%s%s", innerDirPath, "/", innerPtr->d_name);
			readlink(link, buf, sizeof(buf) - 1);
			int pos, leftBound, rightBound;
			string linkInfo(buf);
			if((pos = linkInfo.find("socket")) == std::string::npos) continue;
			leftBound = linkInfo.find_first_of("[", 0);
			rightBound = linkInfo.find_first_of("]", 0);
			linkInfo.assign(linkInfo, leftBound + 1, rightBound - leftBound - 1);
			if(this->inode == linkInfo) {
				string p(ptr->d_name);
				linkPid = p;
				isFind = true;
				break;
			}
		}
		closedir(innerDir);
		if(isFind) {
			char file[32];
			sprintf(file, "%s%s%s", "/proc/", ptr->d_name, "/cmdline");
			FILE *fp = fopen(file, "r");
			char buf[BUFFSIZE];
			if(fgets(buf, BUFFSIZE, fp) != NULL) {
				string arg(buf);
				linkPid.append(arg);
			}
			fclose(fp);
			break;
		}
	}
	closedir(dir);
	return linkPid;
}

void Info::cat(const char *protocol, string filter) {
	cout << left << setw(10) << protocol
		<< left << setw(25) << this->handleInfo(protocol, this->local)
		<< left << setw(25) << this->handleInfo(protocol, this->foreign)
		<< left << setw(25) << this->handleinode()
		<< endl;
}
