#pragma once
#include <stdio.h>

#define SIGNUP_SUCCESSFULL_CODE 2000
#define GOT_CLIENT_LIST_CODE 2001
#define GOT_CLIENT_PUBLIC_KEY 2002
#define MESSAGE_SENT_SUCCESSFULLY 2003
#define GOT_INCOMING_MESSAGES 2004
#define GENERAL_ERROR_CODE 9000

using namespace std;
/// <summary>
/// A class that represents the server reply for a request.
/// </summary>
class ServerReply
{
public:
	unsigned char version;
	unsigned short code;
	unsigned int payloadSize;
	string payload;
};

