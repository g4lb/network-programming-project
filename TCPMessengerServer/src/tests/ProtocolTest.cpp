/*
 * ProtocolTest.cpp
 *
 *  Created on: May 1, 2016
 *      Author: user
 */

#include "ProtocolTest.h"
#include "../../../SocketUtils/src/TCPSocket.h"
#include <unistd.h>
#include <map>

namespace npl {

ProtocolTest::ProtocolTest() {
	// TODO Auto-generated constructor stub

}

ProtocolTest::~ProtocolTest() {
	// TODO Auto-generated destructor stub
}

bool ProtocolTest::test(){
	TCPSocket* server = new TCPSocket(4545);
	start();
	TCPSocket* peer = server->listenAndAccept();

	map<string,string> rec_data;
	int command;
	bool rc = TCPMessengerProtocol::sendToServer(command,rec_data,peer);
	if(!rc){
		cerr<<"fail test"<<endl;
		return false;
	}
	//check the data...
	map<string,string>::iterator iter = rec_data.find(KEY_USER);
	if (iter == rec_data.end() || iter->second.compare("kuku1234") != 0){
		cerr<<"fail test"<<endl;
		return false;
	}
	iter = rec_data.find(KEY_PASSWORD);
	if (iter == rec_data.end() || iter->second.compare("1234567890") != 0){
		cerr<<"fail test"<<endl;
		return false;
	}
	return true;
}

void ProtocolTest::run(){
	sleep(1);
	TCPSocket* client = new TCPSocket("127.0.0.1",4545);
	map<string,string> data;
	data.insert(pair<string,string>(KEY_USER,"kuku1234"));
	data.insert(pair<string,string>(KEY_PASSWORD,"1234567890"));
	TCPMessengerProtocol::sendToServer(COMMAND_USER_AND_PWD,data,client);
}
} /* namespace npl */
