#pragma once
#include "RequestBuilder.h"
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

class UserInteractor
{
private:
	map<int, string> options;
	void printOptions();
public:
	UserInteractor(vector<RequestBuilder> requestBuilders);
	/**
	 * The function prints the query and gets the user reply.
	 */
	string getUserStringReply(string query);

	int getMainMenuUserReply();
};

