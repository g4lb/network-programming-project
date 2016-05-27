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


void printInstructions(){
	cout<<"Welcome to TCP Messenger Server"<<endl;
	cout<<"Print the users list from server type: lu"<<endl;
	cout<<"Print the connected users list type: lcu"<<endl;
    cout<<"Print the sessions: ls"<<endl;
	cout<<"Print available rooms type: lr"<<endl;
	cout<<"Print users in room type: lru <room name>"<<endl;

	cout<<"To exit type: x"<<endl;
}

int main() {
	printInstructions();

	MessengerServer* server = new MessengerServer();

	while(true){
		string msg;
		string command;
		cin >> command;

		if(command == "x"){
			server->exit();
			break;
		}else if(command == "lu"){
            server->listUsers();
        }
        else if(command == "lcu"){
            server->listConnectedUsers();
        }
        else if(command == "ls"){
            server->listSessions();
        }
        else if(command == "lr"){
            server->listRooms();
        }
        else if(command == "lru"){
            string roomName;
            cin >> roomName;
            server->listRoomUsers(roomName);
        }
        else{
			cout<<"wrong input"<<endl;
		}
	}
	delete server;
	cout<<"Messenger Server was closed"<<endl;
	return 0;
}
