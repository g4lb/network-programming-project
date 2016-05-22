/*
 * UDPSocket.h
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
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
