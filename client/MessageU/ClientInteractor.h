#pragma once
#include "RequestBuilder.h"
#include "User.h"
#include <stdio.h>
#include <string>
#include <map>
#include <vector>

using namespace std;
/// <summary>
/// a class that manages user interaction.
/// </summary>
class ClientInteractor
{
private:
	/// <summary>
	/// print user different menu options.
	/// </summary>
	void printOptions();

public:
	ClientInteractor(vector<RequestBuilder*>& requestBuilders);
	/// <summary>
	/// The function prints the queryand gets the user reply.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="query"></param>
	/// <param name="reply"></param>
	template <typename T>
	void getUserStringReply(string query, T reply);
	map<int, RequestBuilder*> requestBuildersMap;

	int getMainMenuUserReply();
	static string getNameFromUser();
	static string getMessageContentFromUser();
	/// <summary>
	/// The function gets a user from user intpu
	/// </summary>
	/// <param name="userList"> a vector of users</param>
	/// <returns> a pointer to the given user.</returns>
	static User* getUserFromUser(vector<User>& userList);
};

