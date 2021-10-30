#include "UserInteractor.h"
#include "RequestBuilder.h"
#include "Consts.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <exception>
#include <map>

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

UserInteractor::UserInteractor(vector<RequestBuilder*>& requestBuilders): requestBuildersMap(){
	for (const auto& requestBuilder : requestBuilders) {
		this->requestBuildersMap[requestBuilder->getUserInpuId()] = requestBuilder;
	}
}


template<typename T>  
void UserInteractor::getUserStringReply(const string query, T reply) {
	cout << query << "=> ";
	cin >> reply;
}

void UserInteractor::printOptions() {
	cout << "Welcome To MessageU Application." << endl;
	for (const auto& item : this->requestBuildersMap)
	{
		cout << item.first << " : " << item.second->getMenuString() << endl;
	}
}


Client* UserInteractor::getClientFromUser(vector<Client>& clientList) {
	string name = UserInteractor::getNameFromUser();
	while (find_if(clientList.begin(), clientList.end(), [name](Client c1) { return c1.name == name; }) == clientList.end()) {
		cout << "Please Enter a name of a real user, that isn't you." << endl;
		name = UserInteractor::getNameFromUser();
	}
	return &*find_if(clientList.begin(), clientList.end(), [=](Client c1) {return c1.name == name; });
	
}

string UserInteractor::getMessageContentFromUser() {
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

string UserInteractor::getNameFromUser() {
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

int UserInteractor::getMainMenuUserReply() {
	int option;
	this->printOptions();
	while (!getIntFromUser(option) || (this->requestBuildersMap.count(option) == 0 && option != QUIT_APP_INT)) {
		cerr << "Error in input, Must choose option from menu" << endl << "=>";
	} 
	return option;
}