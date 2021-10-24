#pragma once
#include "Request.h"
#include <string>
using namespace std;

#define SIGNUP_CODE 1000
#define CLIENT_LIST_CODE 1001
#define GET_CLIENT_KEY_CODE 1002
#define PULL_MESSAGES_CODE 1004
#define SEND_MESSAGE_CODE 1003


#define GET_SYM_KEY_MESSAGE_TYPE_CODE 1
#define SEND_SYM_KEY_MESSAGE_TYPE_CODE 2
#define SEND_TEXT_MESSAGE_TYPE_CODE 3


#define SIGNUP_USER_INPUT_ID 10
#define CLIENT_LIST_USER_INPUT_ID 20
#define GET_CLIENT_KEY_USER_INPUT_ID 30
#define PULL_MESSAGES_USER_INPUT_ID 40
#define SEND_MESSAGE_USER_INPUT_ID 50
#define GET_CLIENT_SYM_KEY_USER_INPUT_ID 51
#define SEND_CLIENT_SYM_KEY_USER_INPUT_ID 52


#define PUBLIC_KEY_LEN 160
#define UUID_LEN 128



#define MAX_USERNAME_LEN 254
#define USER_INFO_FILE_NAME "me.info"

class RequestBuilder
{
public: 
	RequestBuilder(string clientID, unsigned char version, unsigned int code);
	// in this version the builder gets the other params from the me.info file.
	RequestBuilder(unsigned int code);
	unsigned int code;
	unsigned int version = 2;
	string clientID;
	virtual unsigned int getUserInpuId() const { return 0; }
	virtual string getMenuString() const { return ""; };
	virtual Request build() = 0;
};

class SignUpRequestBuilder : public RequestBuilder {
private:
	string name;


public:
	SignUpRequestBuilder();
	unsigned int code = SIGNUP_CODE;
	virtual Request build();
	virtual unsigned int getUserInpuId() const { return SIGNUP_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Signup to the application"; }
	string getNameFromUser();
	string generatePublicKey();
	string generateUUID();
	/*
	 * The function takes the user info and writes it to the me.info file.
	 */
	void saveToInfoFile(string username, string publicKey, string ClientID);
};

class GetClientKeyRequestBuilder : public RequestBuilder {
public:
	unsigned int code = GET_CLIENT_KEY_CODE;
	virtual unsigned int getUserInpuId() const { return GET_CLIENT_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get a client public key"; }

};

class AbstractMessageRequestBuilder : public RequestBuilder {
private:
	string clientID;
	unsigned char messageType;
	string message;
public:
	unsigned int code = SEND_MESSAGE_CODE;
	virtual Request build();
};

class SendMessageRequestBuilder : public AbstractMessageRequestBuilder {
private:
	unsigned char messageType = SEND_TEXT_MESSAGE_TYPE_CODE;
public:
	virtual Request build();
	virtual unsigned int getUserInpuId() const { return SEND_MESSAGE_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Send a message to another client"; }
};

class GetClientSymKeyRequestBuilder : public AbstractMessageRequestBuilder {
private:
	unsigned char messageType = GET_SYM_KEY_MESSAGE_TYPE_CODE;
public:
	virtual unsigned int getUserInpuId() const { return GET_CLIENT_SYM_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Get a client sym key"; }
};

class SendClientSymKeyRequestBuilder : public AbstractMessageRequestBuilder {
private:
	unsigned char messageType = SEND_SYM_KEY_MESSAGE_TYPE_CODE;
public:
	virtual unsigned int getUserInpuId() const { return SEND_CLIENT_SYM_KEY_USER_INPUT_ID; }
	virtual string getMenuString() const { return "Send a sym key to another client."; }
};