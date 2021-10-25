#pragma once
#include <string>
using namespace std;
class Client
{
public:
	Client(string uid, string name) : uid(uid), name(name){}
	string name;
	string uid;
	string publicAsymKey;
	string symKey;
};

