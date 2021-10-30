#include "RequestBuilder.h"
#include "Base64Wrapper.h"
#include "Request.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"
#include "UserInteractor.h"
#include "Message.h"
#include "Consts.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <boost/format.hpp>


void hexify(const unsigned char* buffer, unsigned int length) {
std::ios::fmtflags f(std::cout.flags());
std::cout << std::hex;
for (size_t i = 0; i < length; i++)
	std::cout << setfill('0') << setw(2) << (0xFF & buffer[i]) << (((i + 1) % 16 == 0) ? "\n" : " ");
std::cout << std::endl;
std::cout.flags(f);
}

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

RequestBuilder::RequestBuilder(unsigned int code): code(code){
}

Request RequestBuilder::build() {
	this->clientID = this->getClientIDFromInfoFile();
	return Request(this->clientID, this->version, this->code);
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

string RequestBuilder::getClientIDFromInfoFile() {
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
	cout << "DEBUG:: signup privateKey:" << privateKey << ";" << endl;
	infoFile << endl;
}

string SignUpRequestBuilder::generateUUID() {
	return generateRandomBytes(UUID_LEN);
	
}

Request SignUpRequestBuilder::build() {
	RSAPrivateWrapper privateKey;
	string name = UserInteractor::getNameFromUser();
	string publicKey = privateKey.getPublicKey();
	cout << "generated public key" << publicKey <<";" << endl;
	this->clientID = generateUUID();
	this->saveGeneratedInfoToFile(name, privateKey.getPrivateKey());
	string payload = name + publicKey;
	return Request(this->clientID, this->version, this->code, payload);
}


Request GetClientKeyRequestBuilder::build() {
	this->clientID = this->getClientIDFromInfoFile();
	string payload;
	Client* c = UserInteractor::getClientFromUser(this->clientList);
	payload = c->uid;
	return Request(this->clientID, this->version, this->code, payload);
}

Request AbstractMessageRequestBuilder::build() {
	this->clientID = this->getClientIDFromInfoFile();
	return Request(this->clientID, this->version, this->code, message.packed());
}

Request GetClientSymKeyRequestBuilder::build() {
	Client* peer = UserInteractor::getClientFromUser(this->clientList);
	this->message = Message(peer->uid, this->messageType);
	return AbstractMessageRequestBuilder::build();
	
}

Request SendClientSymKeyRequestBuilder::build() {
	Client* peer = UserInteractor::getClientFromUser(this->clientList);
	if (peer->publicKey.size() == 0) {
		cout << "DEBUG::error, no public key" << endl;
		throw exception("To send sym key you need to pull clients public asym key.(option 30)");
	}
	if (peer->symKey == nullptr) {
		unsigned char key[AESWrapper::DEFAULT_KEYLENGTH];
		peer->symKey = new unsigned char[AESWrapper::DEFAULT_KEYLENGTH];
		memcpy_s(peer->symKey, AESWrapper::DEFAULT_KEYLENGTH,  AESWrapper::GenerateKey(key, AESWrapper::DEFAULT_KEYLENGTH), AESWrapper::DEFAULT_KEYLENGTH);
		cout << "DEBUG:sent sym key from user. "; hexify(peer->symKey, AESWrapper::DEFAULT_KEYLENGTH); cout << endl;
		cout << "DEBUG:updated key!" << endl;
	}
	RSAPublicWrapper rsapub(peer->publicKey);
	this->message = Message(peer->uid, this->messageType, rsapub.encrypt(reinterpret_cast<char*>(peer->symKey), AESWrapper::DEFAULT_KEYLENGTH));
	cout << "encrypted key" << this->message.content << endl;
	return AbstractMessageRequestBuilder::build();

}

Request SendMessageRequestBuilder::build() {
	Client* peer = UserInteractor::getClientFromUser(this->clientList);
	if (peer->symKey == nullptr) {
		cout << "DEBUG: peer symkey is NULL" << endl;
		throw exception("To send a message you need to get client's sym key.(option 51)");
	}
	// TOO handle it right.s
	cout << "DEBUG: First key." << *peer->symKey << endl;
	AESWrapper aesEncryptor(peer->symKey, AESWrapper::DEFAULT_KEYLENGTH);
	string messageContent = UserInteractor::getMessageContentFromUser();
	/// <summary>
	string encrypted = aesEncryptor.encrypt(messageContent.c_str(), messageContent.size()).c_str();
	cout << "encrypted :" << endl; hexify((const unsigned char *)encrypted.c_str(), encrypted.size()); cout << endl;
	cout << aesEncryptor.decrypt(encrypted.c_str(), encrypted.size()) << endl;
	/// </summary>
	/// <returns></returns>
	
	this->message = Message(peer->uid, this->messageType, aesEncryptor.encrypt(messageContent.c_str(), messageContent.size()));
	return AbstractMessageRequestBuilder::build();
}

