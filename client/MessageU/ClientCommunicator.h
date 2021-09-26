#pragma once
#include <stdio.h>
#include <boost/asio.hpp>
#include "ServerReply.h"
#include "Request.h"

using namespace std;
using  boost::asio::ip::tcp;

const int MAX_REPLY_SEGMENT_LEN = 2048;

class ClientCommunicator
{
private:
	string serverAddr;
	string serverPort;
	tcp::iostream iostream;
	
	/*
	 * The function closes the socket and opens a new one.
     */
	void connect();

	void close();

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

