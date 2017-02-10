#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "KeyGenerator.h"

using namespace std;

const bool DEBUG = false;

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

struct CharCnt
{
	CharCnt(char _ch, int _cnt): ch(_ch), cnt(_cnt) {}
	char ch;
	int cnt;
	void incr() { ++cnt; }
};

// The output must be sorted according to the frequencies of the 
// ciphertext characters in descending order. If there is a tie, 
// you must output letters earlier in the alphabet first. 
bool CharCntCmp(const CharCnt &self, const CharCnt &other) {
	if (self.cnt != other.cnt) {
		return self.cnt > other.cnt;
	} else {
		return self.ch < other.ch;
	}
}

void histo(int period, int which, std::istream &in) {
	if (period < 1 || which < 1 || which > period) {
		cerr << "Wrong period or which" << endl;
		exit(1);
	}
	vector<CharCnt> cntr;
	for (int i = 0; i < 26; ++i) {
		cntr.push_back(CharCnt('a'+i, 0));
	}
	size_t total = 0;
	char *buf = new char[period];
	while (in.read(buf, period)) {
		char tmp = buf[which-1];
		if (tmp < 'a' || tmp > 'z') {
			continue;
		}
		cntr[tmp-'a'].incr();
		++total;
	}
	sort(cntr.begin(), cntr.end(), CharCntCmp);
	printf("L=%lu\n", total);
	for (int i = 0; i < 26; ++i) {
		const CharCnt &tmp = cntr[i];
		printf("%c: %d (%.02f%%)\n", tmp.ch, tmp.cnt, 100.0*tmp.cnt/total);
	}
	delete []buf;
}

inline bool check_alpha(const string &str) {
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] < 'a' || str[i] > 'z') {
			return false;
		}
	}
	return true;
}

// No hash map avaliable in c++98. Do a brutal-force search.
void kasiski(const string &file) {
	cout << "Kasiski Method" << endl; 
	cout << "==============" << endl;
	int len;
	for (len = 4; ; ++len) {
		bool matches = false;
		for (int i = 0; i <= (int)file.size()-len; ++i) {
			string cur = file.substr(i, len);
			if (!check_alpha(cur)) {
				continue;
			}
			int j = i;
			while (true) {
				j = file.find(cur, j+1);
				if ((size_t)j == string::npos) {
					break;
				}
				matches = true;
				printf("len=%d, i=%d, j=%d, j-i=%d, %s\n", len, i, j, j-i, cur.c_str());
			}
		}
		if (!matches) {
			break;
		}
	}
	printf("len=%d, no more matches\n", len);
}

void average_index_of_coincidence(const string &file, int max_t) {
	cout << "Average Index of Coincidence" << endl;
	cout << "============================" << endl;
	const double kp=0.0658, kr=1.0/26;
	vector<char> cntr(26, 0);
	int L = 0;
	for (int i = 0; i < (int)file.size(); ++i) {
		char ch = file[i];
		if ('a' <= ch && ch <= 'z') {
			++cntr[ch-'a'];
			++L;
		}
	}
	double IC = 0.0;
	for (int i = 0; i < (int)cntr.size(); ++i) {
		IC += cntr[i] * (cntr[i]-1);
	}
	IC /= L*(L-1);
	printf("L=%d\n", L);
	for (char ch = 'a'; ch <= 'z'; ++ch) {
		printf("f('%c')=%d\n", ch, cntr[ch-'a']);
	}
	printf("IC=%f\n", IC);
	for (int t = 1; t <= max_t; ++t) {
		printf("t=%d, E(IC)=%lf\n", t, kp*(L-t)/(L-1)/t + kr*(t-1)*L/(L-1)/t);
	}
}

void auto_correlation(const string &file, int max_t) {
	cout << "Auto-correlation Method" << endl;
	cout << "=======================" << endl;
	for (int t = 1; t <= max_t; ++t) {
		int cnt = 0;
		for (int i = 0; i < (int)file.size()-t; ++i) {
			if (file[i] >= 'a' and file[i] <= 'z') {
				cnt += file[i] == file[i+t];
			}
		}
		printf("t=%d, count=%d\n", t, cnt);
	}
}

void solve(int max_t, string file) {
	ifstream in(file.c_str());
	if (!in.is_open()) {
		cerr << "file not exists" << endl;
		exit(1);
	}
	stringstream buf;
	buf << in.rdbuf();
	kasiski(buf.str());
	cout << endl;
	average_index_of_coincidence(buf.str(), max_t);
	cout << endl;
	auto_correlation(buf.str(), max_t);
}
