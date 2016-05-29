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
            case CLOSE_ROOM:
            case DISCONNECT_FROM_ROOM : {
                disconnect(sender,data);
                break;
            }
            case EXIT: {
               exit(sender);
                break;
            }
            default: {
                break;
            }
        }
    }
    }
}
void ChatRoom::close(){
    active=false;
    for (map<string,TCPSocket*>::iterator itr = peers.begin(); itr != peers.end() ; ++itr) {
        listener.remove(itr->second);
    }
    handler->onClose(this, this->peers);
}
void ChatRoom::closeByPeer(TCPSocket* peer){
    for (map<string,TCPSocket*>::iterator itr = peers.begin(); itr != peers.end() ; ++itr) {
        if(itr->second == peer){
            peers.erase(itr->first);
            listener.remove(peer);
        }
    }
    handler->onClientExit(this, peer);
}
void ChatRoom::disconnectByPeer(TCPSocket* peer){
    listener.remove(peer);
    for (map<string,TCPSocket*>::iterator itr = peers.begin(); itr != peers.end() ; ++itr) {
        if(itr->second==peer)
            peers.erase(itr->first);
    }
    handler->onClientDisconnect(this, peer);
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
void ChatRoom::exit(TCPSocket* sock){
    string userName;
    if (sock == admin) {
        sendByLoop(CHAT_CLOSED_BY_ADMIN," ", sock);
        closeByPeer(sock);
        close();
    }
    else {
        for (map<string, TCPSocket *>::iterator itr = peers.begin(); itr != peers.end(); ++itr) {
            if (itr->second == sock) {
                userName = itr->first;
                break;
            }
        }
        sendByLoop(CLIENT_DISCONNECTED_FROM_ROOM, userName, sock);
        closeByPeer(sock);
    }
}
void ChatRoom::disconnect(TCPSocket* sock, const string& data){
    string userName;
    if (sock == admin) {
        TCPMessengerProtocol::sendToServer(ADMIN_LEAVE_ROOM,sock->fromAddr(),sock);
        sendByLoop(CHAT_CLOSED_BY_ADMIN, data, sock);
        this->close();
    }
    else {
        for (map<string, TCPSocket *>::iterator itr = peers.begin(); itr != peers.end(); ++itr) {
            if (itr->second == sock) {
                userName = itr->first;
            }
        }
        TCPMessengerProtocol::sendToServer(DISCONNECT_FROM_ROOM_RESPONSE, userName, sock);
        for (map<string, TCPSocket *>::iterator itr = peers.begin(); itr != peers.end(); ++itr) {
            if (itr->second == sock) {
                peers.erase(itr->first);
                listener.remove(sock);
            }
        }
        sendByLoop(CLIENT_DISCONNECTED_FROM_ROOM, userName, sock);
        disconnectByPeer(sock);
    }
}
ChatRoom::~ChatRoom(){

}

