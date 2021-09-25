#include "ClientCommunicator.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

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

void ClientCommunicator::sendBytes(string bytes) {
	this->iostream << bytes;
}

string ClientCommunicator::getBytes(int length) {
	string buf(length, '\0');
	this->iostream.read(&buf[0], length);
	return buf;
}

void ClientCommunicator::test() {
	this->connect();
	this->sendBytes("Hello little fucker 8======D");
	cout << "receieved: " << this->getBytes(100) << endl;
	this->close();
}


