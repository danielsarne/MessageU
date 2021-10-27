#include "UserInteractor.h"
#include "RequestBuilder.h"
#include "Consts.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>


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

string UserInteractor::getNameFromUser() {
	string username;
	cout << "enter username ==>";
	cin >> username;
	while (username.size() > MAX_USERNAME_LEN) {
		cout << "Username should be shorter then " << MAX_USERNAME_LEN << "chars." << endl;
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
	cin >> option;
	while (this->requestBuildersMap.count(option) == 0) {
		// TODO : make try catch in case user didn't enter number
		cout << "Error in input, Must choose option from menu" << endl << "=>";
		cin >> option;
	} 
	return option;
}