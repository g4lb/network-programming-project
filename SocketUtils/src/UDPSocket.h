/*
 * UDPSocket.h
 *
 *the UDPSocket implements a udp connection.the class inherited from socket.
 * recv method: receive information from the server.returns number that is bigger the 0 it means that there was information the receive.
 * sendTo method: send information to the server.returns number that is bigger the 0 it means that there was information to send.
 * replay method: replays to last one that send a message.
 * close method: closing the udp socket.
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include "Socket.h"

using namespace std;
namespace npl{

class UDPSocket: public Socket {

public:
	UDPSocket(int port);
	UDPSocket();
	int recv(char* buffer, int length);
	int sendTo(const string& msg, const string& ip, int port);
	int reply(const string& msg);
	void close();

};
}
#endif /* UDPSOCKET_H_ */
