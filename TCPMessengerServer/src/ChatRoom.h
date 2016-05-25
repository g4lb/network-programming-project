//
// Created by parallels on 5/25/16.
//

#ifndef CPPPROJECT_COPY_CHATROOM_H
#define CPPPROJECT_COPY_CHATROOM_H

#include <vector>
#include "../../SocketUtils/src/MThread.h"
#include "../../SocketUtils/src/TCPSocket.h"

namespace npl {

class ChatRoom:public MThread {
public:
    class Handler{
    public:
        virtual void onClose(ChatRoom* chatRoom, TCPSocket* peerA)=0;
    };
private:
    vector<TCPSocket*> peers;
    TCPSocket * admin;
    Handler* handler;
    string roomName;
    bool active;
public:
    ChatRoom(Handler* handler, TCPSocket* peerA);
    void run();
    void close();
    void addUser(TCPSocket* peer);
    void sendByLoop(int command,const string& data,TCPSocket* sender);
    virtual ~Brocker();
};
} /* namespace npl */

#endif //CPPPROJECT_COPY_CHATROOM_H
