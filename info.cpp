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
	this->name = this->handleinode();
}

string Info::transformEndian(string input) {
	for(int i=0;i<32;i+=8) {
		swap(input[i], input[i+6]);
		swap(input[i+1], input[i+7]);
		swap(input[i+2], input[i+4]);
		swap(input[i+3], input[i+5]);
	}
	return input;
}

string Info::transformIPv6(string input) {
	vector<string> ip;
	string ipv6 = "";
	int maxPos = -1, maxLen = 0, curLen = 0;
	for(int i = 0;i < 32;i+=4) {
		string tmp = input.substr(i, 4);
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		int pos = tmp.find_first_not_of('0');
		if(pos == std::string::npos) {
			tmp = '0';
		}
		else {
			tmp.erase(0, pos);
		}
		ip.push_back(tmp);

	}
	for(int i = 0;i < 7;i++) {
		if(ip[i] == "0" && ip[i+1] == "0") {
			curLen++;
			if(curLen > maxLen) {
				maxLen = curLen;
				maxPos = i;
			}
		}
		else {
			curLen = 0;
		}
	}
	int start = maxPos - maxLen + 1,
		end = maxPos + 1;
	for(int i = 0;i < start;i++) {
		if(i == start - 1)
			ipv6 += ip[i];
		else
			ipv6 = ipv6 + ":" + ip[i];
	}
	if(maxLen > 0)
		ipv6 += "::";
	for(int i = end + 1;i < 8;i++) {
		if(i == end + 1)
			ipv6 += ip[i];
		else
			ipv6 = ipv6 + ":" + ip[i];
	}

	return ipv6;
}

string Info::handleInfo(const char *protocol, string info) {
	if(!strcmp(protocol, "tcp") || !strcmp(protocol, "udp")) {
		unsigned int ip, port;
		char buf[256];
		sscanf(info.c_str(), "%x:%x", &ip, &port);
		if(port == 0) {
			sprintf(buf, "%d.%d.%d.%d:%s", ip&0xff, (ip >> 8)&0xff, (ip >> 16)&0xff, ip >> 24, "*");
		}
		else {
			sprintf(buf, "%d.%d.%d.%d:%d", ip&0xff, (ip >> 8)&0xff, (ip >> 16)&0xff, ip >> 24, port);
		}
		return buf;
	}
	else {
		string ip = info;
		unsigned int port;
		char buf[256];
		ip.erase(32, 5);
		ip = this->transformEndian(ip);
		ip = this->transformIPv6(ip);
		sscanf(info.c_str(), "%*x:%x", &port);
		if(port == 0) {
			sprintf(buf, "%s:%s", ip.c_str(), "*");
		}
		else {
			sprintf(buf, "%s:%d", ip.c_str(), port);
		}
		return buf;
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
	string local = this->handleInfo(protocol, this->local),
		foreign = this->handleInfo(protocol, this->foreign);
	int localPos, foreignPos, namePos;
	bool show = false;
	if((localPos = local.find(filter)) != std::string::npos || filter == "") show = true;
	if((foreignPos = foreign.find(filter)) != std::string::npos || filter == "") show = true;
	if((namePos = this->name.find(filter)) != std::string::npos || filter == "") show = true;
	if(!show) return;
	cout << left << setw(10) << protocol
		<< left << setw(40) << local
		<< left << setw(40) << foreign
		<< left << setw(40) << this->name
		<< endl;
}
