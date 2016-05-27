/*
 * Brocker.cpp
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#include "Brocker.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"

#define TIMEOUT 2

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
            string a,b;
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
                case SEND_MSG_TO_PEER: {
                    TCPMessengerProtocol::sendToServer(command, data, reciver);
                    break;
                }
                case CLOSE_SESSION_WITH_PEER: {
                    TCPMessengerProtocol::sendToServer(command, b, sender);
                    TCPMessengerProtocol::sendToServer(command, a, reciver);
                    active = false;
                    break;
                }
                case EXIT: {
                    TCPMessengerProtocol::sendToServer(command, reciver->fromAddr(), sender);
                    TCPMessengerProtocol::sendToServer(command, sender->fromAddr(), reciver);
                    active = false;
                    TCPMessengerProtocol::sendToServer(EXIT, " ", sender);
                    break;
                }
                default: {
                    cout << command << endl;
                    break;
                }
            }
        }
        close();
    }

    void Brocker::close() {
        handler->onClose(this, peerA, peerB);
    }
}
