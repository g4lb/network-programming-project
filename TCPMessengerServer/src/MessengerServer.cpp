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
void MessengerServer::listSessions(){
	this->dispatcher->listSessions();
}

MessengerServer::~MessengerServer() {

}
void MessengerServer::listUsers(){
	this->dispatcher->listUsers();
}
void MessengerServer::listConnectedUsers(){
	this->dispatcher->listConnectedUsers();
}
void MessengerServer::exit(){
	socket->close();
	delete socket;
	dispatcher->close();
	delete dispatcher;
}
void MessengerServer::listRoomUsers(const string& roomName){
	this->dispatcher->listRoomUsers(roomName);
}
void MessengerServer::run(){
	while(true){
		TCPSocket* peer = socket->listenAndAccept();
		if(peer == NULL){
			break;
		}
		cout << BOLDBLUE << "The peer " << socket->fromAddr() << " Connected" << RESET<< endl;
		dispatcher->add(peer);
	}
}
void MessengerServer::listRooms(){
	this->dispatcher->listRooms();
}
