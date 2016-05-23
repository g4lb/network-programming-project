/*
 * Dispatcher.cpp
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#include "Dispatcher.h"
#define TIMEOUT 2
using namespace std;
using namespace npl;

Dispatcher::Dispatcher() {
	running = false;
    listener = new MultipleTCPSocketListener();

}
void Dispatcher::add(TCPSocket* peer){
    if (std::find(peers.begin(),peers.end(),peer) == peers.end()) // add to vector if not exist already
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
                            //remove peers from the dispatcher responsibility
                            this->removePeer(peer);
                            this->removePeer(peerB);
                            //give responsibility of the peers to a new brocker
                            Brocker* broker = new Brocker(this, peer, peerB);
                            //keep reference of brocker in brockers vector
                            brockers.push_back(broker);
                        }
                        else
                            //if peer does not exist in peers list - refuse the session
                            TCPMessengerProtocol::sendToServer(SESSION_REFUSED,data,peer);
                        break;
                    }
                    case EXIT: {
                        this->removePeer(peer);
                        TCPMessengerProtocol::sendToServer(EXIT," ",peer);
                        cout << "Client " << peer->fromAddr() << " has disconnected" << endl;
                        if(peers.size() == 0){
                            running = false;
                        }
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
	//remove the brocker from the brockers vector
    brockers.erase(std::remove(brockers.begin(),brockers.end(), brocker),brockers.end());
	//return peerA and B to the vector
    this->add(peerA);
    this->add(peerB);
	//delete the brocker
    brocker->waitForThread();

}

Dispatcher::~Dispatcher() {
	// TODO Auto-generated destructor stub
}