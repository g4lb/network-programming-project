/*
 * Dispatcher.cpp
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#include "Dispatcher.h"
#define TIMEOUT 0
using namespace std;
using namespace npl;

Dispatcher::Dispatcher() {
	running = false;
    listener = new MultipleTCPSocketListener();

}
void Dispatcher::add(TCPSocket* peer){
	peers.push_back(peer);
	listener->add(peer);
		if(!running){
			running = true;
			start();
		}
}
void Dispatcher::removePeer(TCPSocket* peer){
    peers.erase(std::remove(peers.begin(),peers.end(), peer),peers.end());
    listener->remove(peer);

}
void Dispatcher::listPeers(){
    cout << "Connected clients:" << endl;
    for (int i=0; i<peers.size();i++)
        cout << peers[i]->fromAddr() << endl;
    cout << "********" << endl;

}

TCPSocket* Dispatcher::getPeerByAddress(const string& address){
    for (int i=0;i<peers.size();i++)
        if (peers[i]->fromAddr() == address)
            return peers[i];
    return NULL;
}

void Dispatcher::run(){
    int command;
    string data;
	while(running){
			if(peers.size() == 0){
				running = false;
				break;
			}
			TCPSocket* peer = listener->listen(TIMEOUT);
			if(peer!=NULL){
				command = 0;
				data.clear();
				TCPMessengerProtocol::readFromServer(command, data, peer);
				cout<<"read command from peer: "<< command << " " << data << endl;
				switch(command){
                    case OPEN_SESSION_WITH_PEER: {
                        TCPSocket* peerB = this->getPeerByAddress(data); // find second peer according to the data
                        if (peerB != NULL) {
                            //tell peerB to change sessionIsActive=true
                            TCPMessengerProtocol::sendToServer(SESSION_ESTABLISHED,peerB->fromAddr(),peer);
                            TCPMessengerProtocol::sendToServer(SESSION_ESTABLISHED,peer->fromAddr(),peerB);
                            this->removePeer(peer);
                            this->removePeer(peerB);
                            Brocker* broker = new Brocker(this, peer, peerB);
                        }
                        else{
                            TCPMessengerProtocol::sendToServer(SESSION_REFUSED,data,peer);
                        }

                        break;
                    }
                    case EXIT: {
                        this->removePeer(peer);

                        cout << "Client " << peer->fromAddr() << " has disconnected" << endl;
                        break;
                    }
                    default: {
                        cout << "Problems with client: " << peer->fromAddr() <<". disconnecting that shit"<<endl;
                        removePeer(peer);
                        break;
                    }
				}

			}
		}
}
void Dispatcher::close(){

}
void Dispatcher::onClose(Brocker * brocker, TCPSocket* peerA,TCPSocket* peerB){
	//return peerA and B to the vector
    peers.push_back(peerA);
    peers.push_back(peerB);
    listener->add(peerA);
    listener->add(peerB);
	//remove the brocker from the brockers vector
    brockers.erase(std::remove(brockers.begin(),brockers.end(), brocker),brockers.end());
	//delete the brocker
    delete brocker;

    if (!running){
        running = true;
        start();
    }

}

Dispatcher::~Dispatcher() {
	// TODO Auto-generated destructor stub
}