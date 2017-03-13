#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define BUFFSIZE 4096

using namespace std;

class Info {
	private:
			string local;
			string foreign;
			string inode;
			string handleInfo(const char *protocol, string info);
			string handleinode();
	public:
			Info(char *input);
			void cat(const char *protocol, string filter);
};
