#include "Client.h"
#include "Consts.h"
#include "ServerReplyHandler.h"

Client::Client() {
	SignUpRequestBuilder* signup = new SignUpRequestBuilder;
	GetUserListRequestBuilder* getUserList = new GetUserListRequestBuilder;
	PullMessagesRequestBuilder* pullMessages = new PullMessagesRequestBuilder;
	GetUserSymKeyRequestBuilder* getUserSymKey = new GetUserSymKeyRequestBuilder(userList);
	SendMessageRequestBuilder* sendMessageRequest = new SendMessageRequestBuilder(userList);
	SendUserSymKeyRequestBuilder* sendSymKeyRequest = new SendUserSymKeyRequestBuilder(userList);
	GetUserKeyRequestBuilder* getUserPublicKey = new GetUserKeyRequestBuilder(userList);

	this->requestBuilders = { signup, getUserList, getUserPublicKey, pullMessages, getUserSymKey, sendMessageRequest, getUserSymKey, sendSymKeyRequest };
	this->interactor = new ClientInteractor(this->requestBuilders);
}
Client::~Client() {
	for (auto builder : this->requestBuilders) {
		delete builder;
	}
	delete this->interactor;
}

void Client::run() {
	while (true) {
		try {
			int userInput = this->interactor->getMainMenuUserReply();
			if (userInput == QUIT_APP_INT) {
				cout << "bye bye darling." << endl;
				break;
			}
			Request r = this->interactor->requestBuildersMap[userInput]->build();
			ServerReply sr = this->comm.makeRequest(r);
			switch (sr.code) {
			case (SIGNUP_SUCCESSFULL_CODE):
				SuccessfullSignUpReplyHandler(sr.payload).handle();
				break;
			case (GOT_CLIENT_PUBLIC_KEY):
				UserPublicKeyReplyHandler(sr.payload, userList).handle();
				break;
			case (GOT_CLIENT_LIST_CODE):
				this->userList = UserListReplyHandler(sr.payload).handle();
				break;
			case (GOT_INCOMING_MESSAGES):
				this->messageList = PullMessagesReplyHandler(sr.payload, userList).handle();
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
		catch (exception& e) {
			cerr << "got error: " << e.what() << endl;
		}
	}
}