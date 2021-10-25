#include "RequestBuilder.h"
#include "Base64Wrapper.h"
#include "Request.h"
#include "RSAWrapper.h"
#include "Consts.h"
#include <iostream>
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

RequestBuilder::RequestBuilder(unsigned int code): code(code){
}

Request RequestBuilder::build() {
	this->clientID = this->getClientIDFromInfoFile();
	return Request(this->clientID, this->version, this->code);
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
	username.insert(username.size() - 1, MAX_USERNAME_LEN - username.size() + 1, '\0');
	return username;
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
string RequestBuilder::getPrivateKeyFromInfoFile() {
	string privateKey;
	string privateKeyHex = getLineFromIndex(USER_INFO_FILE_NAME, 1);
	for (auto iter = privateKey.begin(); iter + 2 != privateKey.end(); iter += 2) {
		string chr = string(iter, iter + 2);
		privateKey.push_back(stoi(chr, nullptr, 16));
	}
	return privateKey;
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
	infoFile << endl;
}

string SignUpRequestBuilder::generateUUID() {
	return generateRandomBytes(UUID_LEN);
	
}

Request SignUpRequestBuilder::build() {
	RSAPrivateWrapper privateKey;
	string name = getNameFromUser();
	string publicKey = privateKey.getPublicKey();
	this->clientID = generateUUID();
	this->saveGeneratedInfoToFile(name, privateKey.getPrivateKey());
	string payload = name + publicKey;
	return Request(this->clientID, this->version, this->code, payload);
}