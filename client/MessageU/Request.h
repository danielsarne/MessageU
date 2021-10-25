#pragma once
#pragma scalar_storage_order little-endian
#include <stdio.h>
#include <string>
using namespace std;

class Request
{
private:
	string clientID;
	unsigned char version;
	unsigned short code;
	unsigned int payloadSize;
	string payload;
public:
	Request(string clientID, unsigned char version, unsigned short code, string payload="");
	/*
	 * The function returns a byte representation of the request
	 */
	string packed();
};

