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
void SuccessfullSignUpReplyHandler::saveUserIDToInfoFile(const string userID) {
	ofstream infoFile(USER_INFO_FILE_NAME, ios::app);
	infoFile << Base64Wrapper::encode(userID) << endl;
}

void SuccessfullSignUpReplyHandler::handle() {
	SuccessfullSignUpReplyHandler::saveUserIDToInfoFile(this->uid);
	cout << "successfull signup." << endl;
}

UserListReplyHandler::UserListReplyHandler(string payload) {
	this->payload = payload;
}

vector<User> UserListReplyHandler::handle() {
	vector<User> appUsers;
	string userPayloadSubstring;
	string uid;
	string userName;
	const int userInfoChunkLen = MAX_USERNAME_LEN + 1 + UUID_LEN;
	int usersCount = this->payload.size() / userInfoChunkLen;
	for (int i = 0; i < usersCount; i++) {
		userPayloadSubstring = this->payload.substr(i * userInfoChunkLen, userInfoChunkLen);
		uid = userPayloadSubstring.substr(0, UUID_LEN);
		userName = userPayloadSubstring.substr(UUID_LEN, MAX_USERNAME_LEN + 1);
		appUsers.push_back(User(uid, userName));
	}
	cout << "Got user list from server." << endl;
	for (User user : appUsers) {
		cout << "- " << user.name << endl;
	}
	return appUsers;
}


UserPublicKeyReplyHandler::UserPublicKeyReplyHandler(string payload, vector<User>& userList) {
	string userID = payload.substr(0, UUID_LEN);
	this->user = &*find_if(userList.begin(), userList.end(), [=](User c1) {return c1.uid == userID; });
	this->publicKey = payload.substr(UUID_LEN, PUBLIC_KEY_LEN);
}

void UserPublicKeyReplyHandler::handle() {
	this->user->publicKey = this->publicKey;
	cout << "got user public key." << endl;
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
		m.srcUserID = this->payload.substr(cursor, UUID_LEN);
		cursor += UUID_LEN;
		// TODO: handle endianess.
		m.id = reinterpret_cast<unsigned int>(this->payload.substr(cursor, sizeof(m.id)).c_str());
		User* srcUser = &*find_if(userList.begin(), userList.end(), [=](User c1) {return c1.uid == m.srcUserID; });
		m.srcUserName = srcUser->name;
		cursor += sizeof(m.id);

		m.type = this->payload.c_str()[cursor];
		cursor += sizeof(m.type);

		memcpy(&m.contentSize, this->payload.substr(cursor, sizeof(m.contentSize)).c_str(), sizeof(m.contentSize));
		cursor += sizeof(m.contentSize);

		if (m.type == SEND_SYM_KEY_MESSAGE_TYPE_CODE) {
			if (srcUser->publicKey.size() == 0) {
				throw exception("you should pull the public key of the other user to decrypt the message.");
			}
			RSAPrivateWrapper rsapriv(this->getPrivateKeyFromInfoFile());
			m.content = rsapriv.decrypt(this->payload.substr(cursor, m.contentSize));
			srcUser->symKey = new unsigned char[AESWrapper::DEFAULT_KEYLENGTH];
			memcpy(srcUser->symKey, m.content.c_str(), AESWrapper::DEFAULT_KEYLENGTH);
		}
		else if (m.type == GET_SYM_KEY_MESSAGE_TYPE_CODE) {
			// no need for handling.
		}
		else if (srcUser->symKey == nullptr) {
			m.content = "Couldn't decrypt the message!";
		}
		else {
			AESWrapper aes(srcUser->symKey, AESWrapper::DEFAULT_KEYLENGTH);
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
