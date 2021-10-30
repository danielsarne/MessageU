#include "Request.h"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
	
Request::Request(string clientID, unsigned char version, unsigned short code, string payload) : clientID(clientID), version(version), code(code), payload(payload), payloadSize(payload.length()){
}

string Request::packed() {
	string packedRequest = "";
	packedRequest += this->clientID;
	append_to_string(packedRequest, this->version);
	append_to_string(packedRequest, this->code);
	append_to_string(packedRequest, this->payloadSize);
	if (this->payloadSize > 0) {
		packedRequest += this->payload;
	}
	return packedRequest;
}