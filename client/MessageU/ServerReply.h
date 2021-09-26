#pragma once
#pragma scalar_storage_order little-endian
#include <stdio.h>
using namespace std;
class ServerReply
{
public:
	unsigned char version;
	unsigned short code;
	unsigned int payloadSize;
	string payload;
};

