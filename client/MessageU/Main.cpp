#include "ClientCommunicator.h"
#include "Request.h"
#include "ServerReply.h"
#include "UserInteractor.h"
#include "RequestBuilder.h"
#include "ServerReplyHandler.h"
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;
extern string generateRandomBytes(int);

void testComm() {
	ClientCommunicator c;
}

void testRequest() {
	Request req("damn  man", 1, 1001, "payload");
	cout << "\"" << req.packed() << "\"" << endl;
	ClientCommunicator c;
	ServerReply sr = c.makeRequest(req);
	cout << "server reply:" << sr.code << sr.version  << sr.payloadSize << endl;
}

void testUserInter() {
	ClientCommunicator c;
	SignUpRequestBuilder signup;
	GetClientListRequestBuilder getClientList;
	vector<RequestBuilder*> v = {&signup, &getClientList };
	UserInteractor u(v);
	string uuid;
	Request r = u.requestBuildersMap[u.getMainMenuUserReply()]->build();
	cout << r.packed();
	ServerReply sr = c.makeRequest(r);
	switch (sr.code) {
	case (SIGNUP_SUCCESSFULL_CODE):
		SuccessfullSignUpReplyHandler(sr.payload).handle();
		cout << "successfull signup." << endl;
		break;
	case (GOT_CLIENT_LIST_CODE):
		ClientListReplyHandler(sr.payload).handle();
		cout << "successfull got client list." << endl;
		break;
	case (GENERAL_ERROR_CODE):
		cout << "got a general error code." << endl;
		break;
	default:
		cout << "ALERT: got an unrecognized code." << endl;
	}
}

int main() {
	// testRequest();
	testUserInter();
}