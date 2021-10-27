#pragma once
#include "Client.h"
#include "Message.h"
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

class ClientPublicKeyReplyHandler {
private:
	string publicKey;
public:
	const unsigned int code = 2002;
	ClientPublicKeyReplyHandler(string payload);
	string handle();
};

class PullMessagesReplyHandler {	
private:
	string payload;
public:
	PullMessagesReplyHandler(string payload) : payload(payload) {}
	vector<Message> handle();
};