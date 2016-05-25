//
// Created by parallels on 5/25/16.
//
#define TIMEOUT 2
#include "ChatRoom.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"

namespace npl {
ChatRoom::ChatRoom(Handler* handler, TCPSocket* peerA){
    this->handler = handler;
    admin = peerA;
    peers.push_back(admin);
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
                peers.erase(std::remove(peers.begin(),peers.end(), sender),peers.end());
                break;
            default:
                cout<<command<<endl;
        }
    }
    close();
}
void ChatRoom::close(){
    handler->onClose(this,admin);
}
void ChatRoom::addUser(TCPSocket* peer){
    peers.push_back(peer);
    if(peers.size()<1){
        sendByLoop(CLIENT_ENTERED_ROOM," ",peer);
    }
}
void ChatRoom::sendByLoop( int command, const string& data,TCPSocket* sender){
    peers.erase(std::remove(peers.begin(),peers.end(), sender),peers.end());
    for (int i = 0; i <peers.size() ; ++i) {
        TCPMessengerProtocol::sendToServer(command,data,peers[i]);
    }
    peers.push_back(sender);
}
virtual ChatRoom::~Brocker(){

}

}/* namespace npl */