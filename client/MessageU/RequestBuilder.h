#pragma once
#include "Request.h"
#include <string>
using namespace std;
#define SIGNUP_CODE 1000
#define CLIENT_LIST_CODE 1001
#define GET_CLIENT_KEY_CODE 1002
#define SEND_MESSAGE_CODE 1003
#define PULL_MESSAGES 1004

class RequestBuilder
{
public:
	unsigned int code;
	string menuString;
	Request build;
};

class SignUpRequestBuilder : RequestBuilder {
private:
	string name;
	
public:
	unsigned int code = SIGNUP_CODE;
	string menuString = "Signup to the application";
	SignUpRequestBuilder();

};

class GetClientKeyRequestBuilder : RequestBuilder {
public:
	unsigned int code = GET_CLIENT_KEY_CODE;
	string menuString = "Get a client public key";

};

class SendMessageRequestBuilder : RequestBuilder {
private:
	string clientID;
	unsigned char MessageType;
	string message;
public:
	unsigned int code = SEND_MESSAGE_CODE;
	string menuString = "Send a message to another client";
};

