#pragma once
#include "Request.h"
#include "User.h"
#include "Message.h"
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

#define SIGNUP_CODE 1000
#define CLIENT_LIST_CODE 1001
#define GET_CLIENT_KEY_CODE 1002
#define PULL_MESSAGES_CODE 1004
#define SEND_MESSAGE_CODE 1003

#define SIGNUP_USER_INPUT_ID 10
#define CLIENT_LIST_USER_INPUT_ID 20
#define GET_CLIENT_KEY_USER_INPUT_ID 30
#define PULL_MESSAGES_USER_INPUT_ID 40
#define SEND_MESSAGE_USER_INPUT_ID 50
#define GET_CLIENT_SYM_KEY_USER_INPUT_ID 51
#define SEND_CLIENT_SYM_KEY_USER_INPUT_ID 52


string getLineFromIndex(string filename, int index);



/// <summary>
/// A class that is used as a base class for all builders, which generate a user request.
/// </summary>
class RequestBuilder
{
protected:
	/// <summary>
	/// 
	/// </summary>
	string getPrivateKeyFromInfoFile();
	string getUsernameFromInfoFile();
	string getUserIDFromInfoFile();
public:
	RequestBuilder(unsigned short code);
	unsigned short code;
	unsigned int version = 2;
	string userID;
	/// <summary>
	/// returns the input id for the given request.
	/// </summary>
	/// <returns></returns>
	virtual unsigned int getUserInpuId() const = 0;
	/// <summary>
	/// returns the string to print for the application menu.
	/// </summary>
	virtual string getMenuString() const = 0;
	/// <summary>
	/// builds the the request according to builder params.
	/// </summary>
	/// <returns></returns>
	virtual Request build();
};

class SignUpRequestBuilder : public RequestBuilder {
private:
	string name;

public:
	SignUpRequestBuilder() : RequestBuilder(SIGNUP_CODE) {}
	virtual Request build();
	virtual unsigned int getUserInpuId() const { return SIGNUP_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Signup to the application"; }
	string generatePublicKey();
	/// <summary>
	/// Generates uuid for first request.
	/// </summary>
	string generateUUID();
	/// <summary>
	/// The function takes the user info and writes it to the me.info file.
	/// </summary>
	void saveGeneratedInfoToFile(string username, string publicKey);
};

class GetUserListRequestBuilder : public RequestBuilder {

public:
	GetUserListRequestBuilder() : RequestBuilder(CLIENT_LIST_CODE) {}
	virtual unsigned int getUserInpuId() const { return CLIENT_LIST_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get The list of users from the server."; }
};

class GetUserKeyRequestBuilder : public RequestBuilder {
private:
	vector<User>& userList;
public:
	GetUserKeyRequestBuilder(vector<User>& userList) : RequestBuilder(GET_CLIENT_KEY_CODE), userList(userList) {}
	virtual unsigned int getUserInpuId() const { return GET_CLIENT_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get a user public key"; }
	virtual Request build();
};

class PullMessagesRequestBuilder : public RequestBuilder {

public:
	PullMessagesRequestBuilder() : RequestBuilder(PULL_MESSAGES_CODE) {}
	virtual unsigned int getUserInpuId() const { return PULL_MESSAGES_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Pull messages from the server."; }
};

class AbstractMessageRequestBuilder : public RequestBuilder {
protected:
	Message message;
	vector<User>& userList;
public:
	AbstractMessageRequestBuilder(vector<User>& userList) : RequestBuilder(SEND_MESSAGE_CODE), userList(userList) {}
	unsigned int code = SEND_MESSAGE_CODE;
	virtual Request build();
};

class SendMessageRequestBuilder : public AbstractMessageRequestBuilder {
protected:
	unsigned char messageType = SEND_TEXT_MESSAGE_TYPE_CODE;
public:
	SendMessageRequestBuilder(vector<User>& userList) : AbstractMessageRequestBuilder(userList) {}
	virtual unsigned int getUserInpuId() const { return SEND_MESSAGE_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Send a message to another user"; }
	virtual Request build();
};

class GetUserSymKeyRequestBuilder : public AbstractMessageRequestBuilder {
protected:
	unsigned char messageType = GET_SYM_KEY_MESSAGE_TYPE_CODE;
public:
	GetUserSymKeyRequestBuilder(vector<User>& userList) : AbstractMessageRequestBuilder(userList) {}
	virtual unsigned int getUserInpuId() const { return GET_CLIENT_SYM_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get a user sym key."; }
	virtual Request build();
};

class SendUserSymKeyRequestBuilder : public AbstractMessageRequestBuilder {
protected:
	unsigned char messageType = SEND_SYM_KEY_MESSAGE_TYPE_CODE;
public:
	SendUserSymKeyRequestBuilder(vector<User>& userList) : AbstractMessageRequestBuilder(userList) {}
	virtual unsigned int getUserInpuId() const { return SEND_CLIENT_SYM_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Send a sym key to another user."; }
	virtual Request build();
};