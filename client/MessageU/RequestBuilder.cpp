#include "RequestBuilder.h"
#include "Base64Wrapper.h"
#include "Request.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"
#include "ClientInteractor.h"
#include "Message.h"
#include "Consts.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <boost/format.hpp>


string generateRandomBytes(int len) {
	string randomString;
	char randomByteString;
	srand(unsigned int(time(0)));
	for (int i = 0; i < len; i++) {
		randomByteString = rand()% 255 + 1;
		randomString.push_back(randomByteString);
	}
	return randomString;
}

RequestBuilder::RequestBuilder(unsigned short code): code(code){
}

Request RequestBuilder::build() {
	this->userID = this->getUserIDFromInfoFile();
	return Request(this->userID, this->version, this->code);
}


string getLineFromIndex(string filename, int index) {
	string line;
	ifstream file(filename);
	for (int lineno = 0; getline(file, line); lineno++) {
		if (lineno == index) {
			return line;
		}
	}
	throw exception("error reading from file.");
}

string RequestBuilder::getUserIDFromInfoFile() {
	return Base64Wrapper::decode(getLineFromIndex(USER_INFO_FILE_NAME, 2));
}

string RequestBuilder::getUsernameFromInfoFile() {
	return getLineFromIndex(USER_INFO_FILE_NAME, 0);
}


void SignUpRequestBuilder::saveGeneratedInfoToFile(const string username, const string privateKey) {
	ofstream infoFile(USER_INFO_FILE_NAME);
	infoFile << username << endl;
	for (const unsigned char& ch : privateKey) {
		infoFile << setfill('0') << setw(2) << hex << int(ch);
	}
	infoFile << endl;
}

string SignUpRequestBuilder::generateUUID() {
	return generateRandomBytes(UUID_LEN);
	
}

Request SignUpRequestBuilder::build() {
	RSAPrivateWrapper privateKey;
	string name = ClientInteractor::getNameFromUser();
	string publicKey = privateKey.getPublicKey();
	this->userID = generateUUID();
	this->saveGeneratedInfoToFile(name, privateKey.getPrivateKey());
	string payload = name + publicKey;
	return Request(this->userID, this->version, this->code, payload);
}


Request GetUserKeyRequestBuilder::build() {
	this->userID = this->getUserIDFromInfoFile();
	string payload;
	User* c = ClientInteractor::getUserFromUser(this->userList);
	payload = c->uid;
	return Request(this->userID, this->version, this->code, payload);
}

Request AbstractMessageRequestBuilder::build() {
	this->userID = this->getUserIDFromInfoFile();
	return Request(this->userID, this->version, this->code, message.packed());
}

Request GetUserSymKeyRequestBuilder::build() {
	User* peer = ClientInteractor::getUserFromUser(this->userList);
	this->message = Message(peer->uid, this->messageType);
	return AbstractMessageRequestBuilder::build();
	
}

Request SendUserSymKeyRequestBuilder::build() {
	User* peer = ClientInteractor::getUserFromUser(this->userList);
	if (peer->publicKey.size() == 0) {
		throw exception("To send sym key you need to pull users public asym key.(option 30)");
	}
	if (peer->symKey == nullptr) {
		unsigned char key[AESWrapper::DEFAULT_KEYLENGTH];
		peer->symKey = new unsigned char[AESWrapper::DEFAULT_KEYLENGTH];
		memcpy_s(peer->symKey, AESWrapper::DEFAULT_KEYLENGTH,  AESWrapper::GenerateKey(key, AESWrapper::DEFAULT_KEYLENGTH), AESWrapper::DEFAULT_KEYLENGTH);
	}
	RSAPublicWrapper rsapub(peer->publicKey);
	this->message = Message(peer->uid, this->messageType, rsapub.encrypt(reinterpret_cast<char*>(peer->symKey), AESWrapper::DEFAULT_KEYLENGTH));
	return AbstractMessageRequestBuilder::build();

}

Request SendMessageRequestBuilder::build() {
	User* peer = ClientInteractor::getUserFromUser(this->userList);
	if (peer->symKey == nullptr) {
		throw exception("To send a message you need to get user's sym key.(option 51)");
	}
	AESWrapper aesEncryptor(peer->symKey, AESWrapper::DEFAULT_KEYLENGTH);
	string messageContent = ClientInteractor::getMessageContentFromUser();
	string encrypted = aesEncryptor.encrypt(messageContent.c_str(), messageContent.size()).c_str();
	this->message = Message(peer->uid, this->messageType, aesEncryptor.encrypt(messageContent.c_str(), messageContent.size()));
	return AbstractMessageRequestBuilder::build();
}

