/*
 * Brocker.cpp
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#include "Brocker.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"

#define TIMEOUT 120

namespace npl {

Brocker::Brocker(Handler* Handler, TCPSocket* peerA, TCPSocket* peerB) {
	this->handler = Handler;
	this->peerA=peerA;
	this->peerB = peerB;
    active = true;
	start();
}

Brocker::~Brocker() {
	// TODO Auto-generated destructor stub
}
void Brocker::run(){
	MultipleTCPSocketListener listener;
    TCPSocket* sender, *reciver;

    listener.add(peerA);
    listener.add(peerB);

    int command = 0;
    string data = "";

	while(active){
		sender = listener.listen(TIMEOUT);
        //IF SENDER IS NULL THEN TIMEOUT HANDLING
        if (sender == peerA)
            reciver = peerB;
        else
            reciver = peerA;

        TCPMessengerProtocol::readFromServer(command,data,sender);
        switch (command)
        {
            case SEND_MSG_TO_PEER:
                TCPMessengerProtocol::sendToServer(command,data,reciver);
                break;
            case CLOSE_SESSION_WITH_PEER:
                TCPMessengerProtocol::sendToServer(command,reciver->fromAddr(),sender);
                TCPMessengerProtocol::sendToServer(command,sender->fromAddr(),reciver);
                active = false;
                break;
            case EXIT:
                TCPMessengerProtocol::sendToServer(command,reciver->fromAddr(),sender);
                TCPMessengerProtocol::sendToServer(command,sender->fromAddr(),reciver);
                active = false;
                TCPMessengerProtocol::sendToServer(EXIT," ",sender);
                break;
            default:
                cout<<command<<endl;
        }
	}
	close();
}
void Brocker::close(){
	handler->onClose(this,peerA,peerB);
}

} /* namespace npl */
