#pragma once
#include "User.h"
#include "ClientCommunicator.h"
#include "RequestBuilder.h"
#include "ClientInteractor.h"
#include "Message.h"
#include <vector>

class Client
{
private:
	vector<User> userList;
	vector<Message> messageList;
	ClientCommunicator comm;
	vector<RequestBuilder*> requestBuilders;
	ClientInteractor* interactor;
public:
	Client();
	/// <summary>
	/// The function runs the logics of the client.
	/// </summary>
	void run();

};

