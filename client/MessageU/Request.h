#pragma once
#include <stdio.h>
#include <string>
#define append_to_string(str, var) \
	for(int i = 0; i < sizeof(var); i++) { \
		str +=  reinterpret_cast<char *>(&(var))[i];	\
	}

using namespace std;

class Request
{
private:
	string clientID;
	unsigned char version;
	unsigned short code;
	unsigned int payloadSize;
public:
	Request(string clientID, unsigned char version, unsigned short code, string payload = "");
	/*
	 * The function returns a byte representation of the request
	 */
	string payload;
	string packed();
};

