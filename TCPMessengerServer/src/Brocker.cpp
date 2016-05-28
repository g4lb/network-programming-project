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

    Brocker::Brocker(BrockerHandler *handler, TCPSocket *peerA, TCPSocket *peerB,const string& userNameA, const string& userNameB) {
        this->handler = handler;
        this->peerA = peerA;
        this->peerB = peerB;
        this->userNameA = userNameA;
        this->userNameB = userNameB;
        active = true;
        start();
    }

    Brocker::~Brocker() {
        // TODO Auto-generated destructor stub
    }

    void Brocker::run() {
        MultipleTCPSocketListener listener;
        TCPSocket *sender, *reciver;

        listener.add(peerA);
        listener.add(peerB);

        int command = 0;
        string data = "";

        while (active) {
            sender = listener.listen(TIMEOUT);
            //IF SENDER IS NULL THEN TIMEOUT HANDLING
            if (sender != NULL) {
                string a, b;
                if (sender == peerA) {
                    reciver = peerB;
                    a = userNameA;
                    b = userNameB;
                }
                else {
                    reciver = peerA;
                    b = userNameA;
                    a = userNameB;
                }
                TCPMessengerProtocol::readFromServer(command, data, sender);
                switch (command) {
                    case CLOSE_SESSION_WITH_PEER: {
                        TCPMessengerProtocol::sendToServer(command, b, sender);
                        TCPMessengerProtocol::sendToServer(command, a, reciver);
                        close();
                        break;
                    }
                    case EXIT: {
                        TCPMessengerProtocol::sendToServer(CLOSE_SESSION_WITH_PEER, a, reciver);
                        closeByExit(sender);
                        break;
                    }
                    default: {
                        cout << command << endl;
                        break;
                    }
                }
            }
        }
    }

    void Brocker::close() {
        active=false;
        handler->onClose(this, peerA, peerB);
    }
    void Brocker::closeByExit(TCPSocket* disconnectingPeer){
        active=false;

        if(disconnectingPeer == this->peerA)
            handler->onClientExit(this, peerA, peerB);
        else
            handler->onClientExit(this, peerB, peerA);
    }
    string Brocker::getPeerAName(){
        return this->userNameA;
    }
    string Brocker::getPeerBName(){
        return this->userNameB;
    }
}
