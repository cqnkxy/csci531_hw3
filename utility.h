#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <iostream>
#include <string>

void keygen(char const*, int);
void crypt(std::string keyfile, std::istream &in);
void invkey(std::string keyfile);

#endif
