/*
 * Dispatcher.cpp
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#include <sstream>
#include "Dispatcher.h"
#include "ServerLoader.h"

#define TIMEOUT 2
#define USERS_PATH "users.txt"
using namespace std;
using namespace npl;

Dispatcher::Dispatcher() {
    //read users map
    ServerLoader sl(USERS_PATH);
    this->registeredUsers = sl.loadAllUserFromFile();

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
                    case LOGIN: {
                        std::istringstream splitter(data);
                        string peerUser;
                        string peerPassword;
                        splitter >> peerUser;
                        splitter >> peerPassword;

                        bool loginSuccess = false;

                        for (map<string, string>::iterator itr = registeredUsers.begin();
                             itr != registeredUsers.end(); ++itr) {
                            if ((itr->second == peerPassword) && (itr->first == peerUser)) {
                                loggedInUsers.insert(pair<string, TCPSocket *>(peerUser, peer));
                                TCPMessengerProtocol::sendToServer(SUCCESS_LOGIN, peerUser, peer);
                                loginSuccess = true;
                                break;
                            }
                        }
                        if (!loginSuccess)
                            TCPMessengerProtocol::sendToServer(LOGIN_REFUSE," ", peer);
                        break;
                    }
                    case REGISTER:{
                        std::istringstream splitter1(data);
                        string peerUser1;
                        string peerPassword1;
                        splitter1 >> peerUser1;
                        splitter1 >> peerPassword1;

                        bool alreadyExist = false;

                        for (map<string,string>::iterator itr = registeredUsers.begin(); itr != registeredUsers.end() ; ++itr) {
                            if(itr->second==peerUser1){
                                TCPMessengerProtocol::sendToServer(REGISTER_REFUSE,data, peer);
                                alreadyExist = true;
                                break;
                            }
                        }
                        if (!alreadyExist){
                            registeredUsers.insert(pair<string,string>(peerUser1,peerPassword1));
                            loggedInUsers.insert(pair<string,TCPSocket*>(peerUser1,peer));
                            TCPMessengerProtocol::sendToServer(SUCCESS_REGISTER,peerUser1, peer);
                        }
                        break;
                    }

                    case OPEN_OR_CONNECT_TO_ROOM: {
                        for (map<string, TCPSocket *>::iterator itr = loggedInUsers.begin();
                             itr != loggedInUsers.end(); ++itr){
                            if (itr->second==peer) {
                                ChatRoom *room = new ChatRoom(this,itr->first, peer);
                                TCPMessengerProtocol::sendToServer(SUCCESS_ENTER_ROOM, itr->first, peer);
                                this->removePeer(peer);
                                chatRooms.push_back(room);
                                break;
                            }
                        }

                        break;
                    }
                    case OPEN_SESSION_WITH_PEER: {
                        if (isLogedIn(peer)){
                            TCPSocket* peerB = this->getPeerByAddress(data); // find second peer according to the data
                            if (peerB != NULL) {
                                //get the username from the peers
                                string peerName,peerBName;
                                for (map<string, TCPSocket *>::iterator itr = loggedInUsers.begin();
                                     itr != loggedInUsers.end(); ++itr) {
                                    if (itr->second == peer) {
                                        peerName=itr->first;
                                    }
                                    if (itr->second==peerB) {
                                        peerBName=itr->first;
                                    }
                                }
                                //tell peerB to change sessionIsActive=true
                                TCPMessengerProtocol::sendToServer(SESSION_ESTABLISHED,peerBName+" "+peerB->fromAddr(),peer);
                                TCPMessengerProtocol::sendToServer(SESSION_ESTABLISHED,peerName+" "+peer->fromAddr(),peerB);
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
                        TCPMessengerProtocol::sendToServer(NOT_CONNECTED_TO_SERVER," ", peer);
                        break;
                    }
                    case LIST_USERS:{
                        string users;
                        for (map<string, string>::iterator itr = registeredUsers.begin();
                             itr != registeredUsers.end(); ++itr) {
                            users += itr->first+"/n";
                        }
                        TCPMessengerProtocol::sendToServer(LIST_USERS_RESPONSE,users,peer);
                        break;
                    }
                    case LIST_CONNECTED_USERS:{
                        string connectedUsers;
                        for (map<string, TCPSocket *>::iterator itr = loggedInUsers.begin();
                             itr != loggedInUsers.end(); ++itr) {
                            connectedUsers += itr->first+"/n";
                        }
                        TCPMessengerProtocol::sendToServer(LIST_CONNECTED_USERS_RESPONSE,connectedUsers,peer);
                        break;
                    }
                    case LIST_ROOMS:{
                        string rooms;
                        for (int i = 0; i <chatRooms.size(); ++i) {
                            rooms+=chatRooms[i]->getRoomName();
                        }
                        TCPMessengerProtocol::sendToServer(LIST_ROOMS_RESPONSE,rooms,peer);
                        break;
                    }
                    case LIST_ROOM_USERS:{
                        for (int i = 0; i <chatRooms.size() ; ++i) {
                            if(chatRooms[i]->getRoomName()==data){
                                TCPMessengerProtocol::sendToServer(LIST_ROOM_USERS_RESPONSE,chatRooms[i]->getUsers(),peer);
                                break;
                            }
                        }
                        break;
                    }
                    case EXIT: {
                        if (isLogedIn(peer)) {
                            this->removePeer(peer);
                            TCPMessengerProtocol::sendToServer(EXIT, " ", peer);
                            cout << "Client " << peer->fromAddr() << " has disconnected" << endl;
                            if (peers.size() == 0) {
                                running = false;
                            }
                            break;
                        }
                        TCPMessengerProtocol::sendToServer(NOT_CONNECTED_TO_SERVER," ", peer);
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
    this->brockers.erase(std::remove(brockers.begin(),brockers.end(), brocker),brockers.end());
    //return peerA and B to the vector
    this->add(peerA);
    this->add(peerB);
    //delete the brocker
    brocker->waitForThread();

}
void Dispatcher::onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap){
    //remove the brocker from the brockers vector
    chatRooms.erase(std::remove(chatRooms.begin(),chatRooms.end(), chatRoom),chatRooms.end());
    //return the peers to the vector
    for (map<string,TCPSocket*>::iterator itr = peersMap.begin(); itr != peersMap.end() ; ++itr) {
        peers.push_back(itr->second);
    }

    //delete the brocker
    chatRoom->waitForThread();
}
bool Dispatcher::isLogedIn(TCPSocket* sock){
    for (map<string, TCPSocket *>::iterator itr = loggedInUsers.begin();
         itr != loggedInUsers.end(); ++itr) {
        if(itr->second==sock){
            return true;
        }
    }
    return false;
}
Dispatcher::~Dispatcher() {
	// TODO Auto-generated destructor stub
}