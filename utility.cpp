#include <iostream>
#include <fstream>
#include <vector>
#include "KeyGenerator.h"

using namespace std;

const bool DEBUG = true;

struct RandomInteger {
	RandomInteger(char const *pphrase): kg(pphrase){}
	unsigned int next() {
		unsigned int ret = 0;
		for (int i = 3; i >= 0; --i) {
			ret |= kg.next() << (i * 8);
		}
		return ret;
	}
	KeyGenerator kg;
};

void keygen(char const *pphrase, int period) {
	RandomInteger rd(pphrase);
	for (int p = 0; p < period; ++p) {
		string state = "abcdefghijklmnopqrstuvwxyz";
		// Permutation algo given in http://merlot.usc.edu/cs531-s17/homeworks/hw3/#shuffle
		for (int i = 0; i < 2; ++i) {
			unsigned int L = 26;
			while (L > 1) {
				unsigned int X = rd.next();
				unsigned int R = X % L;
				if (R != L-1) {
					swap(state[R], state[L-1]);
				}
				if (DEBUG) {
					printf("X = 0x%08x, L = %d, R = %d, state -> %s\n", X, L, R, state.c_str());
				}
				--L;
			}
		}
		cout << state << endl;
	}
}

void read_keyfile(string keyfile, vector<string> &keys) {
	ifstream kfile(keyfile.c_str());
	if (!kfile.is_open()) {
		cerr << "keyfile not exists" << endl;
		exit(1);
	}
	string line;
	while (kfile >> line) {
		keys.push_back(line);
	}
	kfile.close();
}

void check_keyfile(vector<string> &keys) {
	if (keys.empty()) {
		cerr << "keyfile is empty" << endl;
		exit(1);
	}
	for (int i = 0; i < (int)keys.size(); ++i) {
		if (keys[i].size() != 26) {
			cerr << "Keyfile COL: " << i+1 << " invalid length!" << endl;
			exit(1);
		}
		for (int j = 0; j < 26; ++j) {
			if (keys[i][j] < 'a' || keys[i][j] > 'z') {
				cerr << "Keyfile COL: " << i+1 << " invalid character!" << endl;
				exit(1); 
			}
		}
	}
}

void crypt(string keyfile, std::istream &in) {
	vector<string> keys;
	read_keyfile(keyfile, keys);
	check_keyfile(keys);
	size_t idx = 0, T = keys.size();
	char ch;
	// assume that the original plaintext is a single line of ASCII text. 
	// There is only a single '\n' at the end of the line. You may stop
	// reading the input as soon as you see a '\n' character. If you
	// encounter a character less than 0x20 or greater than 0x7e 
	// (except for '\n'), please abort your program and print an error 
	// message.
	while (in >> noskipws >> ch) {
		if (ch == '\n') {
			break;
		} else if (ch < 0x20 or ch > 0x7e) {
			cerr << "Invalid input character!" << endl;
			exit(1);
		} else if (ch >= 'a' && ch <= 'z') {
			size_t col = idx % T;
			cout << keys[col][ch-'a'];
		} else {
			cout << ch;
		}
		idx = (idx+1) % T;
	}
}

void invkey(string keyfile) {
	vector<string> keys;
	read_keyfile(keyfile, keys);
	check_keyfile(keys);
	for (int i = 0; i < (int)keys.size(); ++i) {
		string inv(26, ' ');
		for (char ch = 'a', j = 0; ch <= 'z'; ++ch, ++j) {
			inv[keys[i][j]-'a'] = ch;
		}
		cout << inv << endl;
	}
}
