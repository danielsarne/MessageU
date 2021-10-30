#pragma once
#include <iostream>

#define GET_SYM_KEY_MESSAGE_TYPE_CODE 1
#define SEND_SYM_KEY_MESSAGE_TYPE_CODE 2
#define SEND_TEXT_MESSAGE_TYPE_CODE 3

using namespace std;
/// <summary>
/// a class that represents a message.
/// </summary>
class Message
{
public:
	Message(string dstClientID, unsigned char type, string content = "") : dstClientID(dstClientID), type(type), content(content), contentSize(content.size()) {}
	Message() {}
	string dstClientID;
	string srcClientID;
	string srcClientName;
	unsigned int id = 0;
	unsigned char type = 0;
	unsigned int contentSize = 0;
	string content;
	/// <summary>
	/// returns a string that returns the raw message value.
	/// </summary>
	/// <returns>raw message</returns>
	string packed();
	friend ostream& operator<<(ostream& outs, const Message& m);

};

