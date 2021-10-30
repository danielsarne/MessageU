#pragma once
#include <stdio.h>
#include <boost/asio.hpp>
#include "ServerReply.h"
#include "Request.h"

using namespace std;
using  boost::asio::ip::tcp;

const int MAX_REPLY_SEGMENT_LEN = 2048;

/// <summary>
/// a class that holds client-server  communcication functionallity.
/// </summary>
class ClientCommunicator
{
private:
	string serverAddr;
	string serverPort;
	tcp::iostream iostream;

	/// <summary>
	/// The function closes the socket and opens a new one.
	/// </summary>
	void connect();

	void close();

	ServerReply getServerReply();
	void recvReplyHeader();

	/// <summary>
	/// After parsing the header will recv the server reply payload
	/// </summary>
	/// <param name="size"></param>
	void recvReplyPayload(const unsigned int size);
public:
	ClientCommunicator();
	/// <summary>
	/// The function gets a request and returns the server reply.
	/// </summary>
	/// <param name="request">sends the requests and returns the servers reply</param>
	/// <returns></returns>
	ServerReply makeRequest(Request request);
};

