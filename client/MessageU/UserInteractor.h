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
	void printOptions();

public:
	UserInteractor(vector<RequestBuilder*>& requestBuilders);
	/**
	 * The function prints the query and gets the user reply.
	 */
	template <typename T> 
	void getUserStringReply(string query, T reply);
	map<int, RequestBuilder*> requestBuildersMap;

	int getMainMenuUserReply();
};

