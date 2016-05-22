/*
 * main.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: parallels
 */

#include <iostream>
#include "MessengerClient.h"
using namespace std;
using namespace npl;
void printInstructions(){
	cout<<"Welcome to TCP messenger"<<endl;
	cout<<"Connect to the remote server type: c <ip>"<<endl;
	cout<<"To open session type: o <peer ip> <peer port>"<<endl;
	cout<<"To send a message type: s <message>"<<endl;
	cout<<"To close the session type: cs <peer ip> <peer port>"<<endl;
	cout<<"To disconnect from the server type: d"<<endl;
	cout<<"To exit type: x"<<endl;
}

int main(){
	MessengerClient * client = new MessengerClient();
	printInstructions();
	while(true){
		string msg;
		string command;
		cin >> command;
		if(command == "c"){
			string ip;
			cin >> ip;
			client->connect(ip);
		}else if(command == "o"){
			string ipAndPort;
			cin>>ipAndPort;
			client->openSession(ipAndPort);
		}else if(command == "s"){
			getline(std::cin,msg);
			if(msg.size()>0 && msg[0] == ' ')msg.erase(0,1);
			client->send(msg);
		}else if(command == "cs"){
				client->closeSession();
		}else if(command == "d"){
			client->disconnect();
		}else if(command == "x"){
			client->exit();
			break;
		}else{
			cout<<"wrong input"<<endl;

		}
	}
	cout<<"messenger was closed"<<endl;
	return 0;
}
