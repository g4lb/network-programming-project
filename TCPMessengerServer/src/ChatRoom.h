//
// Created by parallels on 5/25/16.
//

#ifndef CPPPROJECT_COPY_CHATROOM_H
#define CPPPROJECT_COPY_CHATROOM_H

#include <vector>
#include <map>
#include "../../SocketUtils/src/MThread.h"
#include "../../SocketUtils/src/TCPSocket.h"

namespace npl {

class ChatRoom:public MThread {
public:
    class ChatRoomHandler{
    public:
        virtual void onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap)=0;
    };
private:
    map<string,TCPSocket*> peers;
    TCPSocket * admin;
    ChatRoomHandler* handler;
    string roomName;
    bool active;
public:
    ChatRoom(ChatRoomHandler* handler, string peerName,TCPSocket* peerA);
    void run();
    void close();
    void addUser(string userName,TCPSocket* peer);
    void sendByLoop(int command,const string& data,TCPSocket* sender);
    string getRoomName();
    string getUsers();
    virtual ~ChatRoom();
};
} /* namespace npl */

#endif //CPPPROJECT_COPY_CHATROOM_H
