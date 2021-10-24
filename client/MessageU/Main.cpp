#include "ClientCommunicator.h"
#include "Request.h"
#include "ServerReply.h"
#include "UserInteractor.h"
#include "RequestBuilder.h"
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
extern string generateRandomBytes(int);

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
	ClientCommunicator c;
	SignUpRequestBuilder signup;
	vector<RequestBuilder*> v = {&signup};
	UserInteractor u(v);
	Request r = u.requestBuildersMap[u.getMainMenuUserReply()]->build();
	ServerReply sr = c.makeRequest(r);
	switch (sr.code) {
	case (SIGNUP_SUCCESSFULL_CODE):
		cout << "successfull signup." << endl;
		break;
	case (GENERAL_ERROR_CODE):
		cout << "got a general error code." << endl;
		break;
	}
	// cout << "server reply:" << sr.code << sr.version << sr.payloadSize << endl;
}

int main() {
	// testComm();
	// testRequest();
	testUserInter();
}