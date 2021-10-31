#pragma once
#include <iostream>
#include <boost/endian.hpp>

#define GET_SYM_KEY_MESSAGE_TYPE_CODE 1
#define SEND_SYM_KEY_MESSAGE_TYPE_CODE 2
#define SEND_TEXT_MESSAGE_TYPE_CODE 3

using namespace std;
using namespace boost::endian;

/// <summary>
/// a class that represents a message.
/// </summary>
class Message
{
public:
	Message(string dstUserID, unsigned char type, string content = "") : dstUserID(dstUserID), type(type), content(content), contentSize(content.size()) {}
	Message() {}
	string dstUserID;
	string srcUserID;
	string srcUserName;
	little_int32_at id = 0;
	little_int8_at type = 0;
	little_int32_at contentSize = 0;
	string content;

	/// <summary>
	/// returns a string that returns the raw message value.
	/// </summary>
	/// <returns>raw message</returns>
	string packed();
	friend ostream& operator<<(ostream& outs, const Message& m);

};

