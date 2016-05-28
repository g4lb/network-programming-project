/*
 * MessengerServer.cpp
 *
 *  Created on: May 1, 2016
 *      Author: user
 */

#include "MessengerServer.h"

using namespace std;
using namespace npl;

MessengerServer::MessengerServer() {

	dispatcher = new Dispatcher();
	socket = new TCPSocket(MSNGR_PORT);
	start();
}

MessengerServer::~MessengerServer() {

}

void MessengerServer::listUsers(){
	this->dispatcher->listUsers();
}
void MessengerServer::listConnectedUsers(){
	this->dispatcher->listConnectedUsers();
}
void MessengerServer::listSessions(){
	this->dispatcher->listSessions();
}
void MessengerServer::listRooms(){
	this->dispatcher->listRooms();
}
void MessengerServer::listRoomUsers(const string& roomName){
	this->dispatcher->listRoomUsers(roomName);
}

void MessengerServer::exit(){
	socket->close();
	delete socket;
	dispatcher->close();
	delete dispatcher;
}

void MessengerServer::run(){
	while(true){
		TCPSocket* peer = socket->listenAndAccept();
		if(peer == NULL){
			break;
		}
		cout << "Peer " << socket->fromAddr() << " Connected" << endl;
		dispatcher->add(peer);
	}
}
