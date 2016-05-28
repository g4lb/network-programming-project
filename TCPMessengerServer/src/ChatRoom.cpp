//
// Created by parallels on 5/25/16.
//
#define TIMEOUT 2
#include "ChatRoom.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"

using namespace std;
using namespace npl;

ChatRoom::ChatRoom(ChatRoomHandler* handler,const string& roomName,const string& peerName,TCPSocket* peerA){
    this->handler = handler;
    admin = peerA;
    this->roomName = roomName;
    peers.insert(pair<string,TCPSocket*>(peerName,admin));
    active = true;
    start();
    //sleep(1);
}
void ChatRoom::run(){
    listener.add(admin);

    int command = 0;
    string data = "";

    while(active) {
        TCPSocket *sender = listener.listen(TIMEOUT);
        if (sender != NULL){
        TCPMessengerProtocol::readFromServer(command, data, sender);
        switch (command) {
            case CLIENT_DISCONNECTED_FROM_ROOM: {
                if (sender == admin) {
                    active = false;
                    sendByLoop(CHAT_CLOSED_BY_ADMIN, data, sender);
                    close();
                }
                sendByLoop(command, data, sender);
                break;
            }
            case DISCONNECT_FROM_ROOM: {
                string userName;
                if (sender == admin) {
                    active = false;
                    sendByLoop(CHAT_CLOSED_BY_ADMIN, data, sender);
                    this->close();
                }
                for (map<string, TCPSocket *>::iterator itr = peers.begin(); itr != peers.end(); ++itr) {
                    if (itr->second == sender) {
                        userName = itr->first;
                    }
                }
                TCPMessengerProtocol::sendToServer(DISCONNECT_FROM_ROOM_RESPONSE, userName, sender);
                sendByLoop(CLIENT_DISCONNECTED_FROM_ROOM, userName, sender);
                for (map<string, TCPSocket *>::iterator itr = peers.begin(); itr != peers.end(); ++itr) {
                    if (itr->second == sender) {
                        peers.erase(itr->first);
                    }
                }
                break;
            }
            case EXIT: {
                string userName;
                if (sender == admin) {
                    active = false;
                    sendByLoop(CHAT_CLOSED_BY_ADMIN, data, sender);
                    this->close();
                }
                for (map<string, TCPSocket *>::iterator itr = peers.begin(); itr != peers.end(); ++itr) {
                    if (itr->second == sender) {
                        userName = itr->first;
                    }
                }
                TCPMessengerProtocol::sendToServer(DISCONNECT_FROM_ROOM_RESPONSE, userName, sender);
                sendByLoop(CLIENT_DISCONNECTED_FROM_ROOM, userName, sender);
                TCPMessengerProtocol::sendToServer(EXIT, " ", sender);
                for (map<string, TCPSocket *>::iterator itr = peers.begin(); itr != peers.end(); ++itr) {
                    if (itr->second == sender) {
                        peers.erase(itr->first);
                    }
                }
                break;
            }
            default: {
                cout << command << endl;
            }
        }
    }
    close();
    }
}
void ChatRoom::close(){
    active=false;
    handler->onClose(this, this->peers);
}
void ChatRoom::addUser(string userName,TCPSocket* peer){
    //insert the new peer (insert after send so the loop won't send to peer itself
    pair<string,TCPSocket*> newPeer(userName,peer);
    peers.insert(newPeer);
    listener.add(peer);
    //send the new peer to all other peers
    if(peers.size()>1){
        sendByLoop(CLIENT_ENTERED_ROOM,userName+" "+peer->fromAddr(),peer);
    }
    //send the new peer all other peers
    for (map<string,TCPSocket*>::iterator itr = peers.begin(); itr != peers.end() ; ++itr) {
        if ((*itr).first == newPeer.first)
            continue;
        TCPMessengerProtocol::sendToServer(CLIENT_ENTERED_ROOM, (*itr).first + " "+ (*itr).second->fromAddr(), newPeer.second);
    }
}
void ChatRoom::sendByLoop( int command, const string& data,TCPSocket* sender){
    for (map<string,TCPSocket*>::iterator itr = peers.begin(); itr != peers.end() ; ++itr) {
        if(itr->second!=sender){
            TCPMessengerProtocol::sendToServer(command,data,itr->second);
        }
    }
}
string ChatRoom::getRoomName(){
    return this->roomName;
}
string ChatRoom::getUsers(){
    string users;
    for (map<string,TCPSocket*>::iterator itr = peers.begin(); itr != peers.end() ; ++itr) {
        users+=itr->first+"\n";
    }
    return users;
}
ChatRoom::~ChatRoom(){

}

