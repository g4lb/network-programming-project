/*
 * TCPSocket.h
 *
 *  the TCPSocket implements a tcp connection.the class inherited from socket.
 *  data members:
 *  struct sockaddr_in serv_name: holds the struct of the protocol network.
 *  int connect_sock: holds the number of the socket.
 *  listenAndAccept method: is on listening and once there has been a chance to connect the method returns a socket to communicate through.
 *
 *
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
	virtual ~TCPSocket();
};
}
#endif /* TCPSOCKET_H_ */
