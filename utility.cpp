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
