/*
 * Brocker.cpp
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#include "Brocker.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"

#define TIMEOUT 0

namespace npl {

Brocker::Brocker(Handler* Handler, TCPSocket* peerA, TCPSocket* peerB) {
	this->handler = Handler;
	this->peerA=peerA;
	this->peerB = peerB;
	start();
}

Brocker::~Brocker() {
	// TODO Auto-generated destructor stub
}
void Brocker::run(){
	MultipleTCPSocketListener listener;
    TCPSocket* sender, *receiver;

    listener.add(peerA);
    listener.add(peerB);

    int command = 0;
    string data = "";

	while(true){
		sender = listener.listen(TIMEOUT);
        //IF SENDER IS NULL THEN TIMEOUT HANDLING
        if (sender == peerA)
            receiver = peerB;
        else
            receiver = peerA;

        TCPMessengerProtocol::readFromServer(command,data,sender);
        switch (command)
        {
            case SEND_MSG_TO_PEER: {

                TCPMessengerProtocol::sendToServer(command,data,receiver);
                break;
            }
            case CLOSE_SESSION_WITH_PEER:{
                TCPMessengerProtocol::sendToServer(command,data,receiver);
                TCPMessengerProtocol::sendToServer(command,data,sender);
                close();
                break;
            }
        }
	}
	close();
}
void Brocker::close(){
	handler->onClose(this,peerA,peerB);
}

} /* namespace npl */
