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


ClientPublicKeyReplyHandler::ClientPublicKeyReplyHandler(string payload) {
	this->publicKey = payload.substr(UUID_LEN, PUBLIC_KEY_LEN);
}

string ClientPublicKeyReplyHandler::handle() {
	return this->publicKey;
}

vector<Message> PullMessagesReplyHandler::handle() {
	vector<Message> messages;
	int cursor = 0;
	Message m;
	while (cursor < this->payload.size()) {
		m.srcClientID = this->payload.substr(cursor, UUID_LEN);
		cursor += UUID_LEN;
		// TODO: handle endianess.
		m.srcClientID = reinterpret_cast<unsigned int>(this->payload.substr(cursor, sizeof(m.id)).c_str());
		cursor += sizeof(m.id);
		m.type = this->payload.c_str()[cursor];
		cursor += sizeof(m.type);
		m.id = reinterpret_cast<unsigned int>(this->payload.substr(cursor, sizeof(m.id)).c_str());
		cursor += sizeof(m.contentSize);
		m.content = this->payload.substr(cursor, m.contentSize);
		cursor += m.contentSize;
		cout << "Message:" << m.srcClientID << m.id << m.type << endl;
		messages.push_back(m);
	}
	return messages;
}
