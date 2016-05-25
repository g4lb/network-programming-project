//
// Created by parallels on 5/25/16.
//
#define TIMEOUT 2
#include "ChatRoom.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"

using namespace std;
using namespace npl;

ChatRoom::ChatRoom(ChatRoomHandler* handler, string peerName,TCPSocket* peerA){
    this->handler = handler;
    admin = peerA;
    peers[peerName]=admin;
    active = true;
    start();
}
void ChatRoom::run(){
    MultipleTCPSocketListener listener;

    listener.add(admin);

    int command = 0;
    string data = "";

    while(active){
        TCPSocket* sender = listener.listen(TIMEOUT);

        TCPMessengerProtocol::readFromServer(command,data,sender);
        switch (command)
        {
            case CLIENT_DISCONNECTED_FROM_ROOM:
                if(sender==admin){
                    active = false;
                    sendByLoop(CHAT_CLOSED_BY_ADMIN,data,sender);
                    close();
                }
                sendByLoop(command,data,sender);
                break;
            case EXIT:
                if(sender==admin){
                    active = false;
                    sendByLoop(CHAT_CLOSED_BY_ADMIN,data,sender);
                    close();
                }
                sendByLoop(CLIENT_DISCONNECTED_FROM_ROOM,data,sender);
                for (map<string,TCPSocket*>::iterator itr = peers.begin(); itr != peers.end() ; ++itr) {
                    if(itr->second==sender){
                        peers.erase(itr->first);
                    }
                }
                break;
            default:
                cout<<command<<endl;
        }
    }
    close();
}
void ChatRoom::close(){
   //handler->onClose(this,admin);
}
void ChatRoom::addUser(string userName,TCPSocket* peer){
    if(peers.size()>1){
        sendByLoop(CLIENT_ENTERED_ROOM,userName+" "+peer->fromAddr(),peer);
    }
    peers[userName]=peer;
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
        users+=itr->first;
    }
    return users;
}
ChatRoom::~ChatRoom(){

}

