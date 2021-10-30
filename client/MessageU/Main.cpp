#include "ClientCommunicator.h"
#include "Request.h"
#include "ServerReply.h"
#include "UserInteractor.h"
#include "RequestBuilder.h"
#include "ServerReplyHandler.h"
#include "RSAWrapper.h"
#include "Base64Wrapper.h"
#include "Consts.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
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
	vector<Client> clientList;
	ClientCommunicator c;
	SignUpRequestBuilder signup;
	GetClientListRequestBuilder getClientList;
	PullMessagesRequestBuilder pullMessages;
	GetClientSymKeyRequestBuilder getClientSymKey(clientList);
	SendMessageRequestBuilder sendMessageRequest(clientList);
	SendClientSymKeyRequestBuilder sendSymKeyRequest(clientList);

	vector<Message> messageList;
	GetClientKeyRequestBuilder getClientPublicKey(clientList);
	vector<RequestBuilder*> v = {&signup, &getClientList, &getClientPublicKey, &pullMessages, &getClientSymKey, &sendMessageRequest, &getClientSymKey, &sendSymKeyRequest};
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
			ClientPublicKeyReplyHandler(sr.payload, clientList).handle();
			cout << "got client public key." << endl;
			break;
		case (GOT_CLIENT_LIST_CODE):
			clientList = ClientListReplyHandler(sr.payload).handle();
			getClientPublicKey = GetClientKeyRequestBuilder(clientList);
			cout << "successfull got client list." << endl;
			break;
		case (GOT_INCOMING_MESSAGES):
			messageList = PullMessagesReplyHandler(sr.payload, clientList).handle();
			for (Message m: messageList)
			{
				cout << m << endl;
			}
			cout << "got Messages from server." << endl;
			break;
		case(MESSAGE_SENT_SUCCESSFULLY):
			cout << "message was sent successfully." << endl;
			break;
		case (GENERAL_ERROR_CODE):
			cout << "got a general error code." << endl;
			break;
		default:
			cout << "ALERT: got an unrecognized code" << sr.code << endl;
		}
	}
}

string getPrivateKeyFromInfoFile() {
	string hexFormatKey = getLineFromIndex(USER_INFO_FILE_NAME, 1);
	std::string privateKey;
	for (int i = 0; i < hexFormatKey.size(); i += 2)
	{
		std::string byte = hexFormatKey.substr(i, 2);
		char chr = (char)(int)strtol(byte.c_str(), nullptr, 16);
		privateKey.push_back(chr);
	}
	return privateKey;

}

int rsa_example()
{
	std::cout << std::endl << std::endl << "----- RSA EXAMPLE -----" << std::endl << std::endl;

	// plain text (could be binary data as well)
	unsigned char plain[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
	std::cout << "plain:" << std::endl;

	// 1. Create an RSA decryptor. this is done here to generate a new private/public key pair
	RSAPrivateWrapper rsapriv;

	// 2. get the public key
	std::string pubkey = rsapriv.getPublicKey();	// you can get it as std::string ...

	char pubkeybuff[RSAPublicWrapper::KEYSIZE];
	rsapriv.getPublicKey(pubkeybuff, RSAPublicWrapper::KEYSIZE);	// ...or as a char* buffer

	// 3. create an RSA encryptor
	RSAPublicWrapper rsapub(pubkey);
	std::string cipher = rsapub.encrypt((const char*)plain, sizeof(plain));	// you can encrypt a const char* or an std::string 
	std::cout << "cipher:" << std::endl;


	// 4. get the private key and encode it as base64 (base64 in not necessary for an RSA decryptor.)
	std::string base64key = Base64Wrapper::encode(rsapriv.getPrivateKey());

	// 5. create another RSA decryptor using an existing private key (decode the base64 key to an std::string first)
	RSAPrivateWrapper rsapriv_other(Base64Wrapper::decode(base64key));

	std::string decrypted = rsapriv_other.decrypt(cipher);		// 6. you can decrypt an std::string or a const char* buffer
	std::cout << "decrypted:" << std::endl;

	return 0;
}

string getPrivateKeyFromInfoFileDemo(string filename) {
	string privateKey;
	string privateKeyHex = getLineFromIndex(filename, 1);
	for (auto iter = privateKey.begin(); iter + 2 != privateKey.end(); iter += 2) {
		string chr = string(iter, iter + 2);
		privateKey.push_back(stoi(chr, nullptr, 16));
	}
	return privateKey;
}

void testCryptoFunctions() {
	RSAPrivateWrapper rsapriv;
	std::ofstream file("test_enc.txt");
	std::string pubkey = rsapriv.getPublicKey();
	std::string privkey = rsapriv.getPrivateKey();
	file << " aaaaa" << endl;
	file << privkey << endl;
	string privkeyFromFile(getPrivateKeyFromInfoFileDemo("test_enc.txt"));
	cout << "public key length:" << pubkey.size() << endl;
	cout << "private key generated:" << privkey<< endl;
	cout << "private key from file: " << privkeyFromFile << endl;
}

int main() {
	// testRequest();
	// string priv = getPrivateKeyFromInfoFile();
	// cout << priv.size() << endl;
	// testCryptoFunctions();
	testUserInter();
}