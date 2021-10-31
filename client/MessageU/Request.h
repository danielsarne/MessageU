#pragma once
#include <stdio.h>
#include <string>
#include <boost/endian.hpp>
#define append_to_string(str, var) \
	for(int i = 0; i < sizeof(var); i++) { \
		str +=  reinterpret_cast<char *>(&(var))[i];	\
	}

using namespace std;
using namespace boost::endian;

/// <summary>
/// a class That represents a request.
/// </summary>
class Request
{
private:
	string userID;
	little_uint8_at version;
	little_uint16_at code;
	little_uint32_at payloadSize;

public:
	Request(string userID, unsigned char version, unsigned short code, string payload = "");
	string payload;
	// <summary>
	/// The function returns a byte representation of the request
	/// </summary>
	/// <returns>raw request</returns>
	string packed();
};

