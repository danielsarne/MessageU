#pragma once
#include "AESWrapper.h"
#include <string>
using namespace std;
class Client
{
public:
	Client(string uid, string name) : uid(uid), name(name), symKey(nullptr) {}
	string name;
	string uid;
	string publicKey;
	unsigned char* symKey;
};

