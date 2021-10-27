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
	PullMessagesRequestBuilder pullMessages;
	vector<Client> clientList;
	vector<Message> messageList;
	GetClientKeyRequestBuilder getClientPublicKey(clientList);
	vector<RequestBuilder*> v = {&signup, &getClientList, &getClientPublicKey, &pullMessages};
	UserInteractor u(v);
	string uuid;
	while (true) {
		Request r = u.requestBuildersMap[u.getMainMenuUserReply()]->build();
		ServerReply sr = c.makeRequest(r);
		switch (sr.code) {
		case (SIGNUP_SUCCESSFULL_CODE):
			SuccessfullSignUpReplyHandler(sr.payload).handle();
			cout << "successfull signup." << endl;
			break;
		case (GOT_CLIENT_PUBLIC_KEY):
			ClientPublicKeyReplyHandler(sr.payload).handle();
			cout << "got client public key." << endl;
			break;
		case (GOT_CLIENT_LIST_CODE):
			clientList = ClientListReplyHandler(sr.payload).handle();
			getClientPublicKey = GetClientKeyRequestBuilder(clientList);
			cout << "successfull got client list." << endl;
			break;
		case (GOT_INCOMING_MESSAGES):
			messageList = PullMessagesReplyHandler(sr.payload).handle();
			for (Message m: messageList)
			{
				cout << m << endl;
			}

			cout << "got Messages from server." << endl;

			break;
		case (GENERAL_ERROR_CODE):
			cout << "got a general error code." << endl;
			break;
		default:
			cout << "ALERT: got an unrecognized code" << sr.code << endl;
		}
	}
}

int main() {
	// testRequest();
	testUserInter();
}