/*
 * Dispatcher.h
 *
 *  the Dispatcher class is a class that listen to all the peers and handle the incoming commands from the peers.
    the class inherited from MThread, ChatRoom-ChatRoomHandler, Brocker-BrockerHandler classes.
 *  data members:
 *  vector<TCPSocket*> peers: holds a vector of tcp sockets peers.
 *  vector<Brocker*> brockers: holds a vector of brocker .
 *  vector<ChatRoom*> chatRooms: holds a vector of chatRooms.
 *  map<string,string> registeredUsers: holds information of the registered users.
 *  map<string,TCPSocket*> loggedInUsers: holds information of the logged in users.
 *  MultipleTCPSocketListener* listener: holds an object from the type of MultipleTCPSocketListener.
 *  ServerLoader* serverLoader: holds an object of the ServerLoader.
 *  virtual void onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap);this method is delete the chat room and closes the threads.
 *  virtual void onClose(Brocker * brocker, TCPSocket* peerA,TCPSocket* peerB);this method is delete brocker and closes the threads.
 *  virtual void onClientExit(ChatRoom* chatRoom, TCPSocket* peer):in this method ,once client entered exit,
    this peer of clients is deleted.
    bool isLoggedIn(TCPSocket *sock): this method is telling if the specific socket is logged in.
 *  virtual void onClientExit(Brocker * brocker, TCPSocket* peerA,TCPSocket* peerB);in this method ,once client entered exit,the brocker is removed from the brockers vector
    and the peerA and B returned to the vector.


 */

#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_
#define BOLDRED     "\033[1m\033[31m"
#define RESET "\033[0m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDGREEN   "\033[1m\033[32m"

#include <algorithm>
#include <vector>
#include <map>
#include "Brocker.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"
#include "ChatRoom.h"
#include "Users.h"

namespace npl {

class Dispatcher : public MThread, ChatRoom::ChatRoomHandler, Brocker::BrockerHandler {

	vector<TCPSocket*> peers;
	vector<Brocker*> brockers;
	vector<ChatRoom*> chatRooms;
	map<string,string> registeredUsers;
	map<string,TCPSocket*> loggedInUsers;
	MultipleTCPSocketListener* listener;
	Users* users;
	bool running;
	void login(TCPSocket* peer, const string& data);
	void reg(TCPSocket* peer, const string& data);
	void openOrConnectRoom(TCPSocket* peer, const string& data);
	void openSession(TCPSocket* peer, const string& data);
	void listUsers(TCPSocket* peer, const string& data);
	void listConnectedUsers(TCPSocket* peer, const string& data);
	void listRooms(TCPSocket* peer, const string& data);
	void listRoomUsers(TCPSocket* peer, const string& data);
	void exit(TCPSocket* peer, const string& data);
public:
	Dispatcher();

    void listUsers();
    void listConnectedUsers();
    void listSessions();
    void listRooms();
    void listRoomUsers(const string& roomName);

	void add(TCPSocket* peer);
	void removePeer(TCPSocket* peer);
	void run();
	void close();
	virtual ~Dispatcher();
	virtual void onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap);
	virtual void onClose(Brocker * brocker, TCPSocket* peerA,TCPSocket* peerB);
	virtual void onClientExit(ChatRoom* chatRoom, TCPSocket* peer);
	virtual void onClientExit(Brocker * brocker, TCPSocket* peerA,TCPSocket* peerB);
	virtual void onClientDisconnect(ChatRoom* chatRoom, TCPSocket* peer);
	bool isLoggedIn(TCPSocket *sock);

};


} /* namespace npl */

#endif /* SRC_DISPATCHER_H_ */
