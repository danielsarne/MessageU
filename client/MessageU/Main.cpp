#include "ClientCommunicator.h"
#include "Request.h"
#include "ServerReply.h"
#include <stdio.h>
#include <iostream>
using namespace std;

void testComm() {
	ClientCommunicator c;
}

void testRequest() {
	Request req("damn man", 1, 1001, "payload");
	cout << "\"" << req.packed() << "\"" << endl;
	ClientCommunicator c;
	ServerReply sr = c.makeRequest(req);
	cout << "server reply:" << sr.code << sr.version  << sr.payloadSize << endl;
}

void testUserInter() {
	return;
}

int main() {
	// testComm();
	// testRequest();
	testUserInter()
}