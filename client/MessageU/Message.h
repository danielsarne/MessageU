#pragma once
#include <iostream>
using namespace std;
class Message
{
public:
	string srcClientID;
	unsigned int id;
	unsigned char type;
	unsigned int contentSize;
	string content;
	friend ostream& operator<<(ostream& outs, const Message& m) { cout << "Message haahhaha"<< endl; return outs; }

};

