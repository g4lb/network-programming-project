/*
 * Dispatcher.h
 *
 *  Created on: May 8, 2016
 *      Author: parallels
 */

#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include <algorithm>
#include <vector>
#include <map>
#include "Brocker.h"
#include "../../SocketUtils/src/MultipleTCPSocketListener.h"
#include "../../SocketUtils/src/TCPMessengerProtocol.h"
#include "ChatRoom.h"

namespace npl {

class Dispatcher : public MThread, ChatRoom::ChatRoomHandler,Brocker::BrockerHandler {
	vector<TCPSocket*> peers;
	vector<Brocker*> brockers;
	vector<ChatRoom*> chatRooms;
	map<string,string> registeredUsers;
	map<string,TCPSocket*> loggedInUsers;
	MultipleTCPSocketListener* listener;
	bool running;
public:
	Dispatcher();
	void add(TCPSocket* peer);
	TCPSocket* getPeerByAddress(const string& address);
	void removePeer(TCPSocket* peer);
	void listPeers();
	void run();
	void close();
	virtual ~Dispatcher();
	void onClose(Brocker * brocker, TCPSocket* peerA,TCPSocket* peerB);
	void onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap);

};


} /* namespace npl */

#endif /* SRC_DISPATCHER_H_ */
