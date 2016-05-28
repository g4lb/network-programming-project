/*
 * MessengerServer.h
 *
 *  this class has the run method, a thread that in on listening,once one client/user was try to make a connection, it turned to the dispacther.
    the class inherited from MThread.
 *  data members:
 *  TCPSocket* socket: object from type TCP socket.
 *  Dispatcher* dispatcher: object from type Dispatcher.
 *
 */

#ifndef MESSENGERSERVER_H_
#define MESSENGERSERVER_H_

#include "Dispatcher.h"

namespace npl {

class MessengerServer : public MThread{
	TCPSocket* socket;
	Dispatcher* dispatcher;

public:
	MessengerServer();
	virtual ~MessengerServer();


	void listUsers();
	void listConnectedUsers();
	void listSessions();
	void listRooms();
	void listRoomUsers(const string& roomName);
	void exit();
	void run();
};

} /* namespace npl */

#endif /* MESSENGERSERVER_H_ */
