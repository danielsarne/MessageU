#include "ClientInteractor.h"
#include "RequestBuilder.h"
#include "Consts.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <exception>

bool getIntFromUser(int& output) {
	try {
		string input;
		cin >> input;
		output = std::stoi(input);
	}
	catch (std::invalid_argument) {
		return false;
	}
	return true;
}

ClientInteractor::ClientInteractor(vector<RequestBuilder*>& requestBuilders) : requestBuildersMap() {
	for (const auto& requestBuilder : requestBuilders) {
		this->requestBuildersMap[requestBuilder->getUserInpuId()] = requestBuilder;
	}
}


template<typename T>
void ClientInteractor::getUserStringReply(const string query, T reply) {
	cout << query << "=> ";
	cin >> reply;
}

void ClientInteractor::printOptions() {
	cout << "Welcome To MessageU Application." << endl;
	for (const auto& item : this->requestBuildersMap)
	{
		cout << item.first << " : " << item.second->getMenuString() << endl;
	}
}


User* ClientInteractor::getUserFromUser(vector<User>& userList) {
	string name = ClientInteractor::getNameFromUser();
	vector<User>::iterator iter = find_if(userList.begin(), userList.end(), [name](User user) {return user.name == name; });
	while (iter == userList.end()) {
		cout << "Please Enter a name of a real user, that isn't you." << endl;
		name = ClientInteractor::getNameFromUser();
	}
	return &*iter;
}

string ClientInteractor::getMessageContentFromUser() {
	string content;
	cout << "enter a message ==>";
	cin >> content;
	while (content.size() > UINT_MAX) {
		cout << "Message Should be shorter than " << UINT_MAX << " chars." << endl;
		cout << "enter a message ==>";
		getline(cin, content);
	}

	return content;
}

string ClientInteractor::getNameFromUser() {
	string username;
	cout << "enter username ==>";
	cin >> username;
	while (username.size() > MAX_USERNAME_LEN) {
		cout << "Username should be shorter then " << MAX_USERNAME_LEN << " chars." << endl;
		cout << "enter username ==>";
		cin >> username;
	}
	// add padding to name with 0 as described in the protocol.
	username.append(MAX_USERNAME_LEN - username.size() + 1, '\0');
	return username;
}

int ClientInteractor::getMainMenuUserReply() {
	int option;
	this->printOptions();
	while (!getIntFromUser(option) || (this->requestBuildersMap.count(option) == 0 && option != QUIT_APP_INT)) {
		cerr << "Error in input, Must choose option from menu" << endl << "=>";
	}
	return option;
}