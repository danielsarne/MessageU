#pragma once
#include <iostream>

#define GET_SYM_KEY_MESSAGE_TYPE_CODE 1
#define SEND_SYM_KEY_MESSAGE_TYPE_CODE 2
#define SEND_TEXT_MESSAGE_TYPE_CODE 3

using namespace std;
class Message
{
public:
	Message(string dstClientID, unsigned char type, string content = ""): dstClientID(dstClientID), type(type), content(content), contentSize(content.size()){}
	Message() {}
	string dstClientID;
	string srcClientID;
	string srcClientName;
	unsigned int id;
	unsigned char type;
	unsigned int contentSize;
	string content;
	string packed();
	friend ostream& operator<<(ostream& outs, const Message& m);

};

