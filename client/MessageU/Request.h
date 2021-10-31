#pragma once
#include <stdio.h>
#include <string>
#define append_to_string(str, var) \
	for(int i = 0; i < sizeof(var); i++) { \
		str +=  reinterpret_cast<char *>(&(var))[i];	\
	}

using namespace std;

/// <summary>
/// a class That represents a request.
/// </summary>
class Request
{
private:
	string userID;
	unsigned char version;
	unsigned short code;
	unsigned int payloadSize;
public:
	Request(string userID, unsigned char version, unsigned short code, string payload = "");
	string payload;
	// <summary>
	/// The function returns a byte representation of the request
	/// </summary>
	/// <returns>raw request</returns>
	string packed();
};

