/*
 * MessengerClient.h
 *
 *  Created on: Apr 10, 2016
 *      Author: parallels
 */

#ifndef SRC_MESSENGERCLIENT_H_
#define SRC_MESSENGERCLIENT_H_
#include <iostream>
#include <strings.h>
#include "../../SocketUtils/src/TCPMessengerProtocol.h"
#include "../../SocketUtils/src/MThread.h"
#include "../../SocketUtils/src/TCPSocket.h"

using namespace std;
namespace npl{

class MessengerClient : public MThread{
	bool clientConnected;
	bool sessionIsActive;
	TCPSocket * sock;
protected:
public:
	
	MessengerClient();
	void connect(const string& ip);
	void openSession(const string& ipAndPort);
	void sendToServer(int command, const string& data, TCPSocket* sock);
	void readFromServer(int & command,string& data,TCPSocket* sock);
	void send(const string & msg);
	void closeSession();
	void disconnect();
	void run();
	void exit();
};

} /* namespace npl */

#endif /* SRC_MESSENGERCLIENT_H_ */
