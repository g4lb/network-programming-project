/*
 * Brocker.h
 *
 *  the Brocker class manages an open session between two peers.
 *  The Broker reads the incoming commands and messages from the peers and perform the required operations according to the incoming commands.the class inherited from MThread.
 *  contain inner class:
 *  BrockerHandler:
 *  this class implements the chat room.
 * virtual void onClose(Brocker* brocker, TCPSocket* peerA, TCPSocket* peerB)=0: this method is closing the brocker and returns the peers to the dispacther.
 * virtual void onClientExit(Brocker* brocker, TCPSocket* disconnectingPeer, TCPSocket* peerB)=0: once one of the client enterd exit, the onther peer is returned and the onther one is deleted.
 * data member of the class Brocker:
 * TCPSocket* peerA: holds an objects of TCPSocket.
 * TCPSocket* peerB; holds an objects of TCPSocket.
 * string userNameA, userNameB: holds the name of a user.
 * BrockerHandler* handler: holds an objects of BrockerHandler.
 * bool active: telling if it's active or not.
 * */

#ifndef SRC_BROCKER_H_
#define SRC_BROCKER_H_



#include "../../SocketUtils/src/MThread.h"
#include "../../SocketUtils/src/TCPSocket.h"

namespace npl {

class Brocker:public MThread {
public:
	class BrockerHandler{
	public:
		virtual void onClose(Brocker* brocker, TCPSocket* peerA, TCPSocket* peerB)=0;
		virtual void onClientExit(Brocker* brocker, TCPSocket* disconnectingPeer, TCPSocket* peerB)=0;
	};
private:
	TCPSocket* peerA;
	TCPSocket* peerB;
	string userNameA, userNameB;
	BrockerHandler* handler;
	bool active;
public:
	Brocker(BrockerHandler* handler, TCPSocket* peerA, TCPSocket* peerB,const string& userNameA, const string& userNameB);
	virtual ~Brocker();
	void run();
	void close();
	void closeByExit(TCPSocket* disconnectingPeer);
	string getPeerAName();
	string getPeerBName();
};

} /* namespace npl */

#endif /* SRC_BROCKER_H_ */
