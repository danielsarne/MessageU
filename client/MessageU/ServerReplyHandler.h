#pragma once
#include "User.h"
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
	static void saveUserIDToInfoFile(const string userID);
public:
	const unsigned int code = 2000;
	SuccessfullSignUpReplyHandler(string payload) { this->uid = payload;}
	void handle();
};

class UserListReplyHandler {
private:
	string payload;
public:
	const unsigned int code = 2001;
	UserListReplyHandler(string payload);
	/// <summary>
	/// returns a list of users that was given by the server.
	/// </summary>
	/// <returns></returns>
	vector<User> handle();
};

class UserPublicKeyReplyHandler {
private:
	string publicKey;
	User* user;
public:
	const unsigned int code = 2002;
	UserPublicKeyReplyHandler(string payload, vector<User>& userList);
	/// <summary>
	/// updates the user's public key in the user list.
	/// </summary>
	void handle();
};

class PullMessagesReplyHandler {
private:
	string getPrivateKeyFromInfoFile();
	string payload;
	vector<User>& userList;
public:
	PullMessagesReplyHandler(string payload, vector<User>& userList) : payload(payload), userList(userList) {}
	/// <summary>
	/// parses all messages that were pulled from the server.
	/// </summary>
	/// <returns></returns>
	vector<Message> handle();
};