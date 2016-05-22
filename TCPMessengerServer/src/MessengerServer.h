/*
 * MessengerServer.h
 *
 *  Created on: May 1, 2016
 *      Author: user
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

	void listPeers();
	void exit();
	void run();
};

} /* namespace npl */

#endif /* MESSENGERSERVER_H_ */
