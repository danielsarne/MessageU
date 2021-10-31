#pragma once
#include "AESWrapper.h"
#include <string>
using namespace std;
/// <summary>
/// a class that represents a user.
/// </summary>
class User
{
public:
	User(string uid, string name) : uid(uid), name(name), symKey(nullptr) {}
	~User() { delete[] symKey; }
	string name;
	string uid;
	string publicKey;
	unsigned char* symKey;
};

