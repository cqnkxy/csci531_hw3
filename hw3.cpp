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
	const string USAGE = 
		"\n\thw3 keygen -p=pphrase -t=period\n" 
	    "\thw3 crypt -k=keyfile [file]\n" 
	    "\thw3 invkey keyfile\n" 
	    "\thw3 histo -t=period -i=which [file]\n" 
	    "\thw3 solve -l=max_t file\n";
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

void parse_crypt(int argc, char *argv[]) {
	if (argc != 3 && argc != 4) {
		malformed_command();
	}
	if (strncmp(argv[2], "-k=", 3) != 0) {
		malformed_command();
	}
	string keyfile(argv[2]+3);
	if (argc == 3) {
		crypt(keyfile, cin);
	} else {
		ifstream infile(argv[3]);
		if (!infile.is_open()) {
			cerr << "File not exists" << endl;
			exit(1);
		}
		crypt(keyfile, infile);
		infile.close();
	}
}

void parse_invkey(int argc, char *argv[]) {
	if (argc != 3) {
		malformed_command();
	}
	invkey(argv[2]);
}

void parse_histo(int argc, char *argv[]) {
	if (argc != 4 && argc != 5) {
		malformed_command();
	}
	int period, which;
	try {
		for (int i = 2; i <= 3; ++i) {
			string opt(argv[i]);
			if (opt.substr(0, 3) == "-t=") {
				period = atoi(opt.substr(3, -1).c_str());
			} else if (opt.substr(0, 3) == "-i=") {
				which = atoi(opt.substr(3, -1).c_str());
			} else {
				malformed_command();
			}
		}
	} catch(...) {
		malformed_command();
	}
	if (argc == 5) {
		ifstream infile(argv[4]);
		if (!infile.is_open()) {
			cerr << "file not exits" << endl;
			exit(1);
		}
		histo(period, which, infile);
		infile.close();
	} else {
		histo(period, which, cin);
	}
}

void parse_solve(int argc, char *argv[]) {
	if (argc != 4) {
		malformed_command();
	}
	int max_t;
	try {
		if (strncmp(argv[2], "-l=", 3) != 0) {
			malformed_command();
		}
		max_t = atoi(argv[2]+3);
		if (max_t < 1) {
			malformed_command();
		}
		solve(max_t, argv[3]);
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
	} else if (strcmp(argv[1], "crypt") == 0) {
		parse_crypt(argc, argv);
	} else if (strcmp(argv[1], "invkey") == 0) {
		parse_invkey(argc, argv);
	} else if (strcmp(argv[1], "histo") == 0) {
		parse_histo(argc, argv);
	} else if (strcmp(argv[1], "solve") == 0) {
		parse_solve(argc, argv);
	} else {	
		malformed_command();
	}
}

int main(int argc, char *argv[]) 
{
	parse_cmd_run(argc, argv);
	return 0;
}
