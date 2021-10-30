#pragma once
#include "AESWrapper.h"
#include <string>
using namespace std;
/// <summary>
/// a class that represents a client.
/// </summary>
class Client
{
public:
	Client(string uid, string name) : uid(uid), name(name), symKey(nullptr) {}
	~Client() { delete[] symKey; }
	string name;
	string uid;
	string publicKey;
	unsigned char* symKey;
};

