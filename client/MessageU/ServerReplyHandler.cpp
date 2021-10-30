#include "ServerReplyHandler.h"
#include "Consts.h"
#include "RequestBuilder.h"
#include "Base64Wrapper.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"
#include <iostream>
#include <iomanip>
#include <fstream>


using namespace std;
void SuccessfullSignUpReplyHandler::saveClientIDToInfoFile(const string clientID) {
	ofstream infoFile(USER_INFO_FILE_NAME, ios::app);
	infoFile << Base64Wrapper::encode(clientID) << endl;
}

void SuccessfullSignUpReplyHandler::handle() {
	SuccessfullSignUpReplyHandler::saveClientIDToInfoFile(this->uid);
	cout << "successfull signup." << endl;
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
	cout << "Got client list from server." << endl;
	for (Client client : appClients) {
		cout << "- " << client.name << endl;
	}
	return appClients;
}


ClientPublicKeyReplyHandler::ClientPublicKeyReplyHandler(string payload, vector<Client>& clientList) {
	string clientID = payload.substr(0, UUID_LEN);
	this->client = &*find_if(clientList.begin(), clientList.end(), [=](Client c1) {return c1.uid == clientID; });
	this->publicKey = payload.substr(UUID_LEN, PUBLIC_KEY_LEN);
}

void ClientPublicKeyReplyHandler::handle() {
	this->client->publicKey = this->publicKey;
	cout << "got client public key." << endl;
}

string PullMessagesReplyHandler::getPrivateKeyFromInfoFile() {
	string hexFormatKey = getLineFromIndex(USER_INFO_FILE_NAME, 1);
	std::string privateKey;
	for (unsigned int i = 0; i < hexFormatKey.size(); i += 2)
	{
		std::string byte = hexFormatKey.substr(i, 2);
		char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
		privateKey.push_back(chr);
	}
	return privateKey;

}

vector<Message> PullMessagesReplyHandler::handle() {
	vector<Message> messages;
	unsigned int cursor = 0;
	Message m;
	while (cursor < this->payload.size()) {
		m.srcClientID = this->payload.substr(cursor, UUID_LEN);
		cursor += UUID_LEN;
		// TODO: handle endianess.
		m.id = reinterpret_cast<unsigned int>(this->payload.substr(cursor, sizeof(m.id)).c_str());
		Client* srcClient = &*find_if(clientList.begin(), clientList.end(), [=](Client c1) {return c1.uid == m.srcClientID; });
		m.srcClientName = srcClient->name;
		cursor += sizeof(m.id);

		m.type = this->payload.c_str()[cursor];
		cursor += sizeof(m.type);

		memcpy(&m.contentSize, this->payload.substr(cursor, sizeof(m.contentSize)).c_str(), sizeof(m.contentSize));
		cursor += sizeof(m.contentSize);

		if (m.type == SEND_SYM_KEY_MESSAGE_TYPE_CODE) {
			if (srcClient->publicKey.size() == 0) {
				throw exception("you should pull the public key of the other client to decrypt the message.");
			}
			RSAPrivateWrapper rsapriv(this->getPrivateKeyFromInfoFile());
			m.content = rsapriv.decrypt(this->payload.substr(cursor, m.contentSize));
			srcClient->symKey = new unsigned char[AESWrapper::DEFAULT_KEYLENGTH];
			memcpy(srcClient->symKey, m.content.c_str(), AESWrapper::DEFAULT_KEYLENGTH);
		}
		else if (m.type == GET_SYM_KEY_MESSAGE_TYPE_CODE) {
			// no need for handling.
		}
		else if (srcClient->symKey == nullptr) {
			m.content = "Couldn't decrypt the message!";
		}
		else {
			AESWrapper aes(srcClient->symKey, AESWrapper::DEFAULT_KEYLENGTH);
			m.content = aes.decrypt(this->payload.substr(cursor, m.contentSize).c_str(), m.contentSize);
		}
		cursor += m.contentSize;

		messages.push_back(m);
	}
	// print messages for user.
	for (Message m : messages)
	{
		cout << m << endl;
	}
	cout << "got Messages from server." << endl;
	return messages;
}
