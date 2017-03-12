#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <getopt.h>
#include <iostream>

using namespace std;

static const char *shortOptions = "tu";
static struct option longOptions[] = {
	{"tcp", no_argument, NULL, 't'},
	{"udp", no_argument, NULL, 'u'},
	{0, 0, 0, 0}
};

class Command {
	private:
			bool tcpFlag;
			bool udpFlag;
			string filter;
			//	return index of filter, only return the first filter
			int checkIndexOfFilter(int argc, char **argv);
			void catTCP();
			void catUDP();
	public:
			Command(int argc, char **argv);
			void cat();
};
