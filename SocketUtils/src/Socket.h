/*
 * Socket.h
 *
 *  Created on: Jan 1, 2016
 *      Author: parallels
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
