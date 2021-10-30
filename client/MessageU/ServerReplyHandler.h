#pragma once
#include "Client.h"
#include "Message.h"
#include <string>
#include <vector>
using namespace std;

/// <summary>
/// handles a successfull signup.
/// </summary>
class SuccessfullSignUpReplyHandler {
private:
	string uid;
	static void saveClientIDToInfoFile(const string clientID);
public:
	const unsigned int code = 2000;
	SuccessfullSignUpReplyHandler(string payload) { this->uid = payload;}
	void handle();
};

class ClientListReplyHandler {
private:
	string payload;
public:
	const unsigned int code = 2001;
	ClientListReplyHandler(string payload);
	/// <summary>
	/// returns a list of clients that was given by the server.
	/// </summary>
	/// <returns></returns>
	vector<Client> handle();
};

class ClientPublicKeyReplyHandler {
private:
	string publicKey;
	Client* client;
public:
	const unsigned int code = 2002;
	ClientPublicKeyReplyHandler(string payload, vector<Client>& clientList);
	/// <summary>
	/// updates the client's public key in the client list.
	/// </summary>
	void handle();
};

class PullMessagesReplyHandler {
private:
	string getPrivateKeyFromInfoFile();
	string payload;
	vector<Client>& clientList;
public:
	PullMessagesReplyHandler(string payload, vector<Client>& clientList) : payload(payload), clientList(clientList) {}
	/// <summary>
	/// parses all messages that were pulled from the server.
	/// </summary>
	/// <returns></returns>
	vector<Message> handle();
};