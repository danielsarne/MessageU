#pragma once
#include "Client.h"
#include <string>
#include <vector>
using namespace std;

class SuccessfullSignUpReplyHandler{
private:
	string uid;
	static void saveClientIDToInfoFile(const string clientID);
public:
	const unsigned int code = 2000;
	SuccessfullSignUpReplyHandler(string payload) { this->uid = payload; }
	void handle();
};

class ClientListReplyHandler{
private:
	string payload;
public:
	const unsigned int code = 2001;
	ClientListReplyHandler(string payload);
	vector<Client> handle();
};
