#pragma once
#include <stdio.h>
#include <boost/asio.hpp>
#include "ServerReply.h"
#include "Request.h"

using namespace std;
using  boost::asio::ip::tcp;

class ClientCommunicator
{
private:
	tcp::socket socket;
	void sendBytes(string bytes);
	string getBytes();
	/*
	 * The function closes the socket and opens a new one.
     */
	void connect();
	// void sendRequest();
	ServerReply getServerReply();
	void recvReplyHeader();
	/*
	 * After parsing the header will recv the server reply payload
	 */
	void recvReplyPayload(const unsigned int size);
public:
	ClientCommunicator();

	/*
	 * The function gets a request and returns the server reply.
	 */
	ServerReply makeRequest(Request request);
};

