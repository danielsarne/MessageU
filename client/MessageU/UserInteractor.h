#pragma once
#include "RequestBuilder.h"
#include "Client.h"
#include <stdio.h>
#include <string>
#include <map>
#include <vector>

using namespace std;
/// <summary>
/// a class that manages user interaction.
/// </summary>
class UserInteractor
{
private:
	/// <summary>
	/// print client different menu options.
	/// </summary>
	void printOptions();

public:
	UserInteractor(vector<RequestBuilder*>& requestBuilders);
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
	/// The function gets a client from user intpu
	/// </summary>
	/// <param name="clientList"> a vector of clients</param>
	/// <returns> a pointer to the given client.</returns>
	static Client* getClientFromUser(vector<Client>& clientList);
};

