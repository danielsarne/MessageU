#include "UserInteractor.h"
#include "RequestBuilder"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>


UserInteractor::UserInteractor(vector<RequestBuilder> requestBuilders) {
	for (auto const& request : requestBuilders) {
		this->options[request.code] = request.MenuString;
	}
}


string UserInteractor::getUserStringReply(string query) {
	string reply;
	cout << query << "=> ";
	cin >> reply;
	return reply;
}

void UserInteractor::printOptions() {
	cout << "Welcome To MessageU Application." << endl;
	for (const auto& option : this->options)
	{
		cout << option.first << " : " << option.second << endl;
	}
}
int UserInteractor::getMainMenuUserReply() {
	int option;
	this->printOptions();
	cin >> option;
	while (this->options.find(option) == this->options.end()); {
		// TODO : make try catch in case user didn't enter number
		cout << "Error in input, Must choose option from menu" << endl << "=>";
		cin >> option;
	} 
	return option;
}