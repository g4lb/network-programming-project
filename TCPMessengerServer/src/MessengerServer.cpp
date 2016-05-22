/*
 * MessengerServer.cpp
 *
 *  Created on: May 1, 2016
 *      Author: user
 */

#include "MessengerServer.h"

using namespace npl;
using namespace std;

MessengerServer::MessengerServer() {
	dispatcher = new Dispatcher();
	socket = new TCPSocket(MSNGR_PORT);
	start();
}

MessengerServer::~MessengerServer() {

}

void MessengerServer::listPeers(){
	dispatcher->listPeers();
}

void MessengerServer::exit(){
	socket->close();
	waitForThread();
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
