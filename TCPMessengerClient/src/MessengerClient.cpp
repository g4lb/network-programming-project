/*
 * MessengerClient.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: parallels
 */

#include <map>
#include "MessengerClient.h"

using namespace npl;

MessengerClient::MessengerClient(){
	clientState = State::DISCONNECTED;
	currentRoomName = "";
}
void MessengerClient::run() {
	while (clientState == State::CONNECTED) {
		int cmd;
		string str;
		readFromServer(cmd,str,sock);
		if (cmd > 0)
		{
			if (cmd == SESSION_ESTABLISHED)
			{
                cout << SESSION_ESTABLISHED_TEXT << str << endl;
				//sessionIsActive=true;
			}
            if (cmd == SEND_MSG_TO_PEER)
                cout << str << endl;
            if (cmd == SESSION_REFUSED)
                cout << SESSION_REFUSED_TEXT << str << endl;
            if (cmd == CLOSE_SESSION_WITH_PEER){
                cout << CLOSE_SESSION_WITH_PEER_TEXT << str << endl;
                //sessionIsActive = false;
            }
		}
		else
		{
			cout << "Server disconnected" << endl;
			//clientConnected=false;
		}

	}
}
void MessengerClient::connect(const string& ip){
	if(clientState == State::DISCONNECTED){
		sock = new TCPSocket(ip, MSNGR_PORT);
		if(sock!=NULL){
			clientState = State::CONNECTED;
			start();
		}
	}
    else
    {
        cout << "You are already connected to a server"<<endl;
    }
}

void MessengerClient::login(const string& user,const string& password) {
    if (clientState == State::CONNECTED){
        sendToServer(LOGIN,user + "@" + password,sock);
    }
    else{
        cout << "You are not connected to a server" << endl;
    }
}
void MessengerClient::reg(const string& user,const string& password) {
    if (clientState == State::CONNECTED){
        sendToServer(REGISTER,user + "@" + password,sock);
    }
    else{
        cout << "You are not connected to a server" << endl;
    }
}

void MessengerClient::listUsers(){
    if (clientState == State::LOGGED_IN){
        sendToServer(LIST_USERS,"",sock);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::listConnectedUsers(){
    if (clientState == State::LOGGED_IN){
        sendToServer(LIST_CONNECTED_USERS,"",sock);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::listRooms(){
    if (clientState == State::LOGGED_IN){
        sendToServer(LIST_ROOMS,"",sock);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::listRoomUsers(const string& roomName){
    if (clientState == State::LOGGED_IN){
        sendToServer(LIST_ROOM_USERS,roomName,sock);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::openOrConnectToRoom(const string& roomName){
    if (clientState == State::LOGGED_IN){
        sendToServer(OPEN_OR_CONNECT_TO_ROOM,roomName,sock);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::printClientState(){
    if (clientState == State::IN_ROOM)
        cout << "Client in room ["<<this->currentRoomName<<"]"<<endl;
    else if (clientState == State::IN_SESSION)
        cout << "Client is in session"<<endl;
    else if (clientState == State::LOGGED_IN)
        cout << "Client is logged in to server ["<<this->sock->fromAddr()<<"]"<<endl;
    else if (clientState == State::CONNECTED)
        cout << "Client is connected to server ["<<this->sock->fromAddr()<<"]"<<endl;
    else if (clientState == State::DISCONNECTED)
        cout << "Client is not connected to any server"<<endl;
}
void MessengerClient::closeCurrentRoom(){
    if (clientState == State::IN_ROOM){
        sendToServer(CLOSE_ROOM,this->currentRoomName,sock);
    }
    else{
        cout << "You are not in a room" << endl;
    }
}

void MessengerClient::openSession(const string& ipAndPort){
	if(clientState == State::LOGGED_IN){
		sendToServer(OPEN_SESSION_WITH_PEER,ipAndPort,sock);
	}
	else
		cout<<"client is not connected"<<endl;
}
void MessengerClient::closeSession(){
	if(clientState == State::IN_SESSION){
		sendToServer(CLOSE_SESSION_WITH_PEER," ",sock);
	}
	else
		cout<<"sesion is already close"<<endl;
}
void MessengerClient::disconnect(){
	if(clientState == State::IN_SESSION){
		sendToServer(CLOSE_SESSION_WITH_PEER," ",sock);
	}
	if (clientConnected){
		cout<<"disconnected"<<endl;
		sendToServer(EXIT,"",sock);
		clientConnected = false;
	}
	else
		cout<<"client is already disconnected"<<endl;

}
void MessengerClient::exit(){
	if(sessionIsActive){
		sendToServer(CLOSE_SESSION_WITH_PEER," ",sock);
		}
	if(clientConnected){
		sendToServer(EXIT,"exit",sock);
		clientConnected = false;
	}
	cout<<"disconnected from server"<<endl;
	delete sock;
}
void MessengerClient::sendToServer(int command, const string& data, TCPSocket* sock){
	TCPMessengerProtocol::sendToServer(command,data,sock);
}
void MessengerClient::readFromServer(int & command,string& data,TCPSocket* sock){
	TCPMessengerProtocol::readFromServer(command,data,sock);
}
void MessengerClient::send(const string & msg){
    if (sessionIsActive && clientConnected)
	    sendToServer(SEND_MSG_TO_PEER,msg,sock);
}





