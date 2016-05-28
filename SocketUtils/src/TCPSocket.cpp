/*
 * TCPSocket.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: user
 */

#include "TCPSocket.h"
#include <arpa/inet.h>

using namespace npl;
TCPSocket::TCPSocket(int connect_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr):connect_sock(0) {
		this->socket_fd = connect_sock;
		this->serv_name = serverAddr;
		this->peerAddr = peerAddr;
		sessionConnected = true;
}
TCPSocket::TCPSocket(int port):connect_sock(0) {
	socket_fd = socket (AF_INET, SOCK_STREAM, 0);

    //set SO_REUSEADDR
    int iSetOption = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &iSetOption,
               sizeof(iSetOption));

    if (bind(port) < 0)
	{
		cout << "Error naming channel" << endl;
	}
}

TCPSocket::~TCPSocket()
{
	// TODO Auto-generated destructor stub
}

TCPSocket::TCPSocket(const string & peerIp , int port):connect_sock(0)
{
	socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	if (connect(peerIp,port) < 0)
	{
		cout << "Error establishing communications" << endl;
		close();
	}
	cout<<"connected to the server"<<endl;
}

TCPSocket* TCPSocket::listenAndAccept()
{
	listen(socket_fd,1);
    bzero((char*)&peerAddr, sizeof(peerAddr));

	unsigned int len = sizeof(peerAddr);

	connect_sock = accept(socket_fd, (struct sockaddr *)&peerAddr, &len);

	//TODO: return new TCPSocket object holding the new secondary server socket returned from the accept call
	return (new TCPSocket(connect_sock, serv_name, peerAddr));
}

