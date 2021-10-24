#include "RequestBuilder.h"
#include "Request.h"
#include "RSAWrapper.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <boost/beast/core/detail/base64.hpp>

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

RequestBuilder::RequestBuilder(unsigned int code): clientID(clientID), code(code){
}

SignUpRequestBuilder::SignUpRequestBuilder(): RequestBuilder(this->code) {

}

string SignUpRequestBuilder::getNameFromUser() {
	string username;
	cout << "enter username ==>";
	cin >> username;
	while (username.size() > MAX_USERNAME_LEN) {
		cout << "Username should be shorter then " << MAX_USERNAME_LEN << "chars." << endl;
		cout << "enter username ==>";
		cin >> username;
	}	
	return username;
}

void SignUpRequestBuilder::saveToInfoFile(string username, string publicKey, string clientID) {
	filesystem::path infoPath(USER_INFO_FILE_NAME);
	ofstream infoFile(infoPath);
	infoFile << username << endl;
	infoFile << publicKey << endl;
	infoFile << clientID << endl;

	// TODO write clientID and publickey in hex and base64.
}

string SignUpRequestBuilder::generatePublicKey() {
	return "";
}

string SignUpRequestBuilder::generateUUID() {
	return generateRandomBytes(UUID_LEN);
	
}

Request SignUpRequestBuilder::build() {
	RSAPrivateWrapper privateKey;
	string name = getNameFromUser();
	string publicKey = privateKey.getPublicKey();
	this->clientID = generateUUID();
	this->saveToInfoFile(name, privateKey.getPrivateKey(), this->clientID);
	string payload = name + publicKey;
	return Request(this->clientID, this->version, this->code, payload);
}