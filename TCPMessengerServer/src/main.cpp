//============================================================================
// Name        : TCPMessengerClient.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "MessengerServer.h"

using namespace std;
using namespace npl;

int main() {
	cout<<"Welcome to TCP Messenger Server"<<endl;
	MessengerServer* server = new MessengerServer();

	while(true){
		string msg;
		string command;
		cin >> command;
		if(command == "lp"){
			server->listPeers();

		}else if(command == "x"){
			server->exit();
			break;
		}else{
			cout<<"wrong input"<<endl;
		}
	}
	delete server;
	cout<<"messenger server was closed"<<endl;
	return 0;
}
