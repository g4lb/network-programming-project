/*
 * Socket.cpp
 *
 *  Created on: Jan 1, 2016
 *      Author: parallels
 */

#include "Socket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace npl;
using namespace std;

int Socket::bind(int port) {

	struct sockaddr_in s_in;
	bzero((char*) &s_in, sizeof(s_in));
	s_in.sin_family = (short) AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_in.sin_port = htons(port);

	int rc = ::bind(socket_fd, (struct sockaddr *) &s_in, sizeof(s_in));

	return rc;
}

string Socket::fromAddr() {
	char addr[100];

	::sprintf(addr, "%s:%d", inet_ntoa(peerAddr.sin_addr), peerAddr.sin_port);
	return addr;

}

int Socket::connect(const string& ip, int port) {
	bzero((char*) &peerAddr, sizeof(peerAddr));
	peerAddr.sin_family = (short) AF_INET;
	peerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	peerAddr.sin_port = htons(port);

	int rc = ::connect(socket_fd, (struct sockaddr *) &peerAddr,
			sizeof(peerAddr));

	return rc;
}

