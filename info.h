#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

class Info {
	private:
			string local;
			string foreign;
			string inode;
			string handleInfo(const char *protocol, string info);
	public:
			Info(char *input);
			void cat(const char *protocol, string filter);
};
