#include "ClientCommunicator.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

#define read_from_stream(stream, var) \
        stream.read(reinterpret_cast<char*>(&(var)), sizeof(var))

#define read_from_stream_to_string(stream, var, size) \
        var = string(size, '\0'); \
        stream.read(&var[0], size)

ClientCommunicator::ClientCommunicator() {
	string serverInfoStr;
	ifstream serverInfoFile;
	
	serverInfoFile.open("server.info");
	getline(serverInfoFile, serverInfoStr);
	serverInfoFile.close();

	size_t delimeterIndex = serverInfoStr.find(":");
	this->serverAddr = serverInfoStr.substr(0, delimeterIndex);
	this->serverPort = serverInfoStr.substr(delimeterIndex + 1);
	cout << this->serverAddr << ":" << this->serverPort << endl;
}

void ClientCommunicator::connect() {
	iostream.connect(this->serverAddr, this->serverPort);
}

void ClientCommunicator::close() {
	this->iostream.close();
}

ServerReply ClientCommunicator::getServerReply() {
	ServerReply serverReply;
	read_from_stream(this->iostream, serverReply.version);
	read_from_stream(this->iostream, serverReply.code);
	read_from_stream(this->iostream, serverReply.payloadSize);
	if (serverReply.payloadSize > 0) {
		cout << serverReply.payloadSize << endl;
		read_from_stream_to_string(this->iostream, serverReply.payload, serverReply.payloadSize);
	}
	return serverReply;
}

ServerReply ClientCommunicator::makeRequest(Request request) {
	this->connect();
	this->iostream << request.packed();
	ServerReply serverReply = this->getServerReply();
	this->close();
	return serverReply;
}