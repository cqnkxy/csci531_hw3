#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/md5.h>
#include "utility.h"

using namespace std;

const bool DEBUG = false;

void malformed_command() 
{
	const string USAGE = "\n\thw3 stream -p=pphrase -l=len\n\thw3 encrypt -p=pphrase -out=name [pbmfile]\n\thw3 merge pbmfile1 pbmfile2\n\thw3 decrypt [pbmfile]\n";
	cerr << "Malformed command. Use it as:" << endl << USAGE << endl;
	exit(1);
}

void parse_keygen(int argc, char *argv[]) {
	if (argc != 4) {
		malformed_command();
	}
	string pphrase;
	int period;
	try {
		for (int i = 2; i < argc; i++) {
			string opt(argv[i]);
			if (opt.substr(0, 3) == "-p=") {
				pphrase = opt.substr(3, -1);
			} else if (opt.substr(0, 3) == "-t=") {
				period = atoi(opt.substr(3, -1).c_str());
			}
		}
		if (pphrase == "") {
			malformed_command();
		}
		keygen(pphrase.c_str(), period);
	} catch(...) {
		malformed_command();
	}
}

void parse_cmd_run(int argc, char *argv[]) 
{
	if (argc < 2) {
		malformed_command();
	} else if (strcmp(argv[1], "keygen") == 0) {
		parse_keygen(argc, argv);
	} else {
		malformed_command();
	}
}

int main(int argc, char *argv[]) 
{
	parse_cmd_run(argc, argv);
	return 0;
}
