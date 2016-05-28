/*
 * UDPSocket.cpp
 *
 *  Created on: Feb 8, 2013
 *      Author: Eliav Menachi
 */

#include "UDPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace npl;

UDPSocket::UDPSocket(int port){
	socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
	bind(port);
	}
UDPSocket::UDPSocket(){
	socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
	}

int UDPSocket::recv(char* buffer, int length){
	printf("UDP server receive ...\n");
	int rec;
	socklen_t fsize = sizeof(peerAddr);
	rec = recvfrom(socket_fd, buffer, length, 0, (struct sockaddr*)&peerAddr, &fsize);
	if (rec < 0){
		cout << "Error receive from!" << endl;
	}
	return rec;
}

int UDPSocket::sendTo(const string& msg,const string& ip, int port){
	int send;
	struct sockaddr_in s_in;
	bzero((char*)&s_in, sizeof(s_in));

	s_in.sin_family = (short)AF_INET;
	s_in.sin_addr.s_addr = inet_addr(ip.data());
	s_in.sin_port = htons(port);

	send = sendto(socket_fd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&s_in, sizeof(s_in));

	if (send < 0)
		cout << "Error send to" << endl;

	return send;
}

int UDPSocket::reply(const string& msg){
	int reply;
	socklen_t peerAddrSize = sizeof(peerAddr);
	reply =  sendto(socket_fd , msg.c_str() , msg.length(), 0 , (struct sockaddr*)&peerAddr , peerAddrSize);
	if (reply < 0){
		cout << "Error reply" << endl;
	}
	return reply;
}

void UDPSocket::close(){
	cout<<"closing socket"<<endl;
	shutdown(socket_fd,SHUT_RDWR);
	::close(socket_fd);
}





