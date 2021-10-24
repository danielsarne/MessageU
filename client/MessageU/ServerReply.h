#pragma once
#pragma scalar_storage_order little-endian
#include <stdio.h>

#define SIGNUP_SUCCESSFULL_CODE 2000
#define GENERAL_ERROR_CODE 9000

using namespace std;
class ServerReply
{
public:
	unsigned char version;
	unsigned short code;
	unsigned int payloadSize;
	string payload;
};

