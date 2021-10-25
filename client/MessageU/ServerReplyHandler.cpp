#include "ServerReplyHandler.h"
#include "Consts.h"
#include "Base64Wrapper.h"
#include <iostream>
#include <fstream>

using namespace std;
void SuccessfullSignUpReplyHandler::saveClientIDToInfoFile(const string clientID) {
	ofstream infoFile(USER_INFO_FILE_NAME, ios::app);
	infoFile << Base64Wrapper::encode(clientID) << endl;
}

void SuccessfullSignUpReplyHandler::handle() {
	SuccessfullSignUpReplyHandler::saveClientIDToInfoFile(this->uid);
}

ClientListReplyHandler::ClientListReplyHandler(string payload) {
	this->payload = payload;
}

vector<Client> ClientListReplyHandler::handle() {
	vector<Client> appClients;
	string clientPayloadSubstring;
	string uid;
	string clientName;
	const int clientInfoChunkLen = MAX_USERNAME_LEN + 1 + UUID_LEN;
	int clientsCount = this->payload.size() / clientInfoChunkLen;
	for (int i = 0; i < clientsCount; i++) {
		clientPayloadSubstring = this->payload.substr(i * clientInfoChunkLen, clientInfoChunkLen);
		uid = clientPayloadSubstring.substr(0, UUID_LEN);
		clientName = clientPayloadSubstring.substr(UUID_LEN, MAX_USERNAME_LEN + 1);
		appClients.push_back(Client(uid, clientName));
	}
	return appClients;
}