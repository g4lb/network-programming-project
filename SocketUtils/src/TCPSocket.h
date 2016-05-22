/*
 * TCPSocket.h
 *
 *  Created on: Mar 21, 2016
 *      Author: user
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_


#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include "Socket.h"

using namespace std;
namespace npl{
class TCPSocket : public Socket {
	struct sockaddr_in serv_name;
	int connect_sock;
	TCPSocket(int connect_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr);
public:
	TCPSocket(int port);
	TCPSocket(const string & peerIp , int port);
	TCPSocket* listenAndAccept();
	int reply(const string& msg);
	virtual ~TCPSocket();
};
}
#endif /* TCPSOCKET_H_ */
