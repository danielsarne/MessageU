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


ClientPublicKeyReplyHandler::ClientPublicKeyReplyHandler(string payload, vector<Client>& clientList){
	string clientID = payload.substr(0, UUID_LEN);
	this->client = &*find_if(clientList.begin(), clientList.end(), [=](Client c1) {return c1.uid == clientID; });
	this->publicKey = payload.substr(UUID_LEN, PUBLIC_KEY_LEN);
	cout << "DEBUG:got public Key:" << this->publicKey<< ";"<< endl;
}

void ClientPublicKeyReplyHandler::handle() {
	this->client->publicKey = this->publicKey;
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
	cout << "DEBUG private Key from file:" << privateKey << ";" << endl;
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
		Client *srcClient =  &*find_if(clientList.begin(), clientList.end(), [=](Client c1) {return c1.uid == m.srcClientID; });
		m.srcClientName = srcClient->name;
		cursor += sizeof(m.id);
		m.type = this->payload.c_str()[cursor];
		cout << "TYPE:" << (int)m.type << endl;
		cursor += sizeof(m.type);
		memcpy(&m.contentSize, this->payload.substr(cursor, sizeof(m.contentSize)).c_str(), sizeof(m.contentSize));
		cout << "DEBUG: substring for content:" << m.contentSize << endl;
		cursor += sizeof(m.contentSize);
		if (m.type == SEND_SYM_KEY_MESSAGE_TYPE_CODE) {
			cout << "DEBUG: trying to get sym key" << endl;
			if (srcClient->publicKey.size() == 0) {
				throw exception("you should pull the public key of the other client to decrypt the message.");
			}
			cout << "DEBUG: unencrypted symkey" << this->payload.substr(cursor, m.contentSize) << endl;
			RSAPrivateWrapper rsapriv(this->getPrivateKeyFromInfoFile());
			m.content = rsapriv.decrypt(this->payload.substr(cursor, m.contentSize));
			const char* contentPtr = m.content.c_str();
			srcClient->symKey = new unsigned char[AESWrapper::DEFAULT_KEYLENGTH];
			memcpy(srcClient->symKey, contentPtr, AESWrapper::DEFAULT_KEYLENGTH);
			cout << "DEBUG:got sym key from user. "; hexify(srcClient->symKey, AESWrapper::DEFAULT_KEYLENGTH); cout << endl;
		}
		else if (srcClient->symKey == nullptr) {
			cout << "DEBUG:: Couldn't decrypt the message!" << endl;
			m.content = "Couldn't decrypt the message!";
		}
		else {
			cout << "DEBUG: trying to read message" << endl;
			AESWrapper aes(srcClient->symKey, AESWrapper::DEFAULT_KEYLENGTH);
			cout << "encrypted :" << endl; hexify((const unsigned char*)this->payload.substr(cursor, m.contentSize).c_str(), m.contentSize); cout << endl;

			m.content = aes.decrypt(this->payload.substr(cursor, m.contentSize).c_str(), m.contentSize);
		}
		cursor += m.contentSize;
		messages.push_back(m);
	}
	return messages;
}
