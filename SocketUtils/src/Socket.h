/*
 * Socket.h
 *
 * the class socket inherited from Fileinterface and reset the variable and methods in order to prepare the socket for using.
 * data members: bool sessionConnected returns true if the
 * bind method: is for making the bind action
 * connect method: uses in the TCP socket .if return number that is bigger the 0 it means that the communication is established.
 * fromAddr method: returns the number of the ip address.
 *
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "FileInterface.h"

using namespace std;
//bla
namespace npl {

class Socket:public FileInterface {
protected:
	struct sockaddr_in peerAddr;
public:
	bool sessionConnected;
	int bind(int port);
	int connect(const string& ip, int port);
	virtual string fromAddr();

};

} /* namespace npl */

#endif /* SOCKET_H_ */
