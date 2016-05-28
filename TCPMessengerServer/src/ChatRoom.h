/*
 * this class is the chat room,the class inherited from MThread.
 * ChatRoomHandler class is: is the same functionality of the brocker but handle the room itself.
 * methods of ChatRoomHandler class:
 * virtual void onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap)=0 :
 * virtual void onClientExit(ChatRoom* chatRoom, TCPSocket* peer)=0 :
 * in chat room class:
 * data members:
 * map<string,TCPSocket*> peers : map that holds the peers.
 * TCPSocket * admin: holds an object of the TCPSocket, this will be the user admin.
 * ChatRoomHandler* handler: object fromn the type ChatRoomHandler.
 * string roomName: holds the name of the room.
 *  MultipleTCPSocketListener listener: object from the type MultipleTCPSocketListener.
 *  bool active: if active or not.
 *  void run(): opening a thread.
 *  void close();
 *  void closeByPeer(TCPSocket* peer): closing a specific peer.
 *  void addUser(string userName,TCPSocket* peer): adding a user.
 *  void sendByLoop(int command,const string& data,TCPSocket* sender): once one user send a messege, this method sends the messege to all the usres in the room.
 *  string getRoomName();getting the room name.
 *  string getUsers(): getting the user
 *
 */

#ifndef CPPPROJECT_COPY_CHATROOM_H
#define CPPPROJECT_COPY_CHATROOM_H

#include <vector>
#include <map>
#include "../../SocketUtils/src/MThread.h"
#include "../../SocketUtils/src/TCPSocket.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"

namespace npl {

class ChatRoom:public MThread {
public:
    class ChatRoomHandler{
    public:
        virtual void onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap)=0;
        virtual void onClientExit(ChatRoom* chatRoom, TCPSocket* peer)=0;
    };
private:
    map<string,TCPSocket*> peers;
    TCPSocket * admin;
    ChatRoomHandler* handler;
    string roomName;
    MultipleTCPSocketListener listener;
    bool active;
public:
    ChatRoom(ChatRoomHandler* handler, const string& roomName,const string& peerName,TCPSocket* peerA);
    void run();
    void close();
    void closeByPeer(TCPSocket* peer);
    void addUser(string userName,TCPSocket* peer);
    void sendByLoop(int command,const string& data,TCPSocket* sender);
    string getRoomName();
    string getUsers();
    virtual ~ChatRoom();
};
} /* namespace npl */

#endif //CPPPROJECT_COPY_CHATROOM_H
