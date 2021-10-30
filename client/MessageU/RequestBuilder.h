#pragma once
#include "Request.h"
#include "Client.h"
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

void hexify(const unsigned char* buffer, unsigned int length);


class RequestBuilder
{
protected:
	string getPrivateKeyFromInfoFile();
	string getUsernameFromInfoFile();
	string getClientIDFromInfoFile();
public:
	RequestBuilder(unsigned short code);
	unsigned short code;
	unsigned int version = 2;
	string clientID;
	virtual unsigned int getUserInpuId() const { return 0; }
	virtual string getMenuString() const { return ""; };
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
	string generateUUID();
	/*
	 * The function takes the user info and writes it to the me.info file.
	 */
	void saveGeneratedInfoToFile(string username, string publicKey);
};

class GetClientListRequestBuilder : public RequestBuilder {

public:
	GetClientListRequestBuilder() : RequestBuilder(CLIENT_LIST_CODE) {}
	virtual unsigned int getUserInpuId() const { return CLIENT_LIST_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get The list of clients from the server."; }
};

class GetClientKeyRequestBuilder : public RequestBuilder {
private:
	vector<Client>& clientList;
public:
	GetClientKeyRequestBuilder(vector<Client>& clientList) : RequestBuilder(GET_CLIENT_KEY_CODE), clientList(clientList) {}
	virtual unsigned int getUserInpuId() const { return GET_CLIENT_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get a client public key"; }
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
	vector<Client>& clientList;
public:
	AbstractMessageRequestBuilder(vector<Client>& clientList) : RequestBuilder(SEND_MESSAGE_CODE), clientList(clientList) {}
	unsigned int code = SEND_MESSAGE_CODE;
	virtual Request build();
};

class SendMessageRequestBuilder : public AbstractMessageRequestBuilder {
protected:
	unsigned char messageType = SEND_TEXT_MESSAGE_TYPE_CODE;
public:
	SendMessageRequestBuilder(vector<Client>& clientList) : AbstractMessageRequestBuilder(clientList) {}
	virtual unsigned int getUserInpuId() const { return SEND_MESSAGE_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Send a message to another client"; }
	virtual Request build();
};

class GetClientSymKeyRequestBuilder : public AbstractMessageRequestBuilder {
protected:
	unsigned char messageType = GET_SYM_KEY_MESSAGE_TYPE_CODE;
public:
	GetClientSymKeyRequestBuilder(vector<Client>& clientList) : AbstractMessageRequestBuilder(clientList) {}
	virtual unsigned int getUserInpuId() const { return GET_CLIENT_SYM_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get a client sym key."; }
	virtual Request build();
};

class SendClientSymKeyRequestBuilder : public AbstractMessageRequestBuilder {
protected:
	unsigned char messageType = SEND_SYM_KEY_MESSAGE_TYPE_CODE;
public:
	SendClientSymKeyRequestBuilder(vector<Client>& clientList) : AbstractMessageRequestBuilder(clientList) {}
	virtual unsigned int getUserInpuId() const { return SEND_CLIENT_SYM_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Send a sym key to another client."; }
	virtual Request build();
};