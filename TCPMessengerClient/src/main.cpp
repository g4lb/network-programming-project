/*
 * main.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: parallels
 */

#define BOLDRED     "\033[1m\033[31m"
#define RESET "\033[0m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDGREEN   "\033[1m\033[32m"

#include <iostream>
#include "MessengerClient.h"

using namespace std;
using namespace npl;
void printInstructions(){
	cout<< BOLDBLUE << "------->" << RESET << BOLDGREEN << "Welcome to TCP messenger"<< RESET<< BOLDBLUE<< "<-------" << RESET<<endl;
	cout<<BOLDBLUE<<"Connect to the remote server type: c <ip>"<<endl;
	cout<<"Log in type: login <user> <password>"<<endl;
	cout<<"Register type: register <user> <password>"<<endl;
	cout<<"Print the users list from server type: lu"<<endl;
	cout<<"Print the connected users list type: lcu"<<endl;
	cout<<"Print available rooms type: lr"<<endl;
	cout<<"Print users in room type: lru <room name>"<<endl;
	cout<<"To open session with peer type: o <username>"<<endl;
	cout<<"To enter a chat room(or create if not exist) type: or <room name>"<<endl;
	cout<<"To send a message type: s <message>"<<endl;
	cout<<"Print user status type: l"<<endl;
	cout<<"To close the session/exit a room type: cs"<<endl;
	cout<<"To close a room(if you are the admin) type: cr"<<endl;
	cout<<"To disconnect from the server type: d"<<endl;
	cout<<"To exit type: x"<< RESET<<endl;
}

int main(){
	MessengerClient * client = new MessengerClient();
	printInstructions();

    string msg;
    string command;

	while(true){
	    msg.clear();
        command.clear();

		cin >> command;
		if(command == "c"){
			string ip;
			cin >> ip;
			client->connect(ip);
		}else if (command == "login") {
            string user;
            cin >> user;
            string password;
            cin >> password;
            client->login(user, password);
        }else if (command == "register"){
            string user;
            cin >> user;
            string password;
            cin >> password;
            client->reg(user, password);
        }else if (command == "lu") {
            client->listUsers();
        }else if (command =="lcu"){
            client->listConnectedUsers();

        }else if (command =="lr"){
            client->listRooms();

        }else if (command =="lru"){
            string roomName;
            cin >> roomName;
            client->listRoomUsers(roomName);
        }else if (command =="or"){
            string roomName;
            cin >> roomName;
            client->openOrConnectToRoom(roomName);
        }else if (command =="l"){
            client->printClientStatus();
        }else if (command =="cr"){
            client->closeCurrentRoom();
        }
        else if(command == "o"){
			string peerUser;
			cin>>peerUser;
			client->openSession(peerUser);
		}else if(command == "s"){
			cin >> msg;
			if(msg.length()>0 && msg[0] == ' ')
				msg.erase(0,1);
			client->send(msg);
		}else if(command == "cs"){
				client->closeSessionOrExitRoom();
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
