//============================================================================
// Name        : Lab1UDPSocket
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>
#include "UdpSocketTest.h"
#include "../../../SocketUtils/src/UDPSocket.h"

using namespace std;
using namespace npl;


bool UdpSocketTest::test(){
	cout << "UDP Socket test" << endl;

	UDPSocket* sSocket = new UDPSocket(3346);
	UDPSocket* cSocket = new UDPSocket();
	string message = "Test 1234567890";
	cSocket->sendTo(message,"127.0.0.1", 3346);
	char buffer[100];
	memset((void*)buffer,0,100);
	int rc = sSocket->recv(buffer, 100);
	cout<<"send msg:"<<message<<endl;
	cout<<"recv msg:"<<buffer<<endl;
	if (rc != (int)message.length()){
		cerr<<"FAIL1: receive different message length"<<endl;
		return false;
	}
	if (message != buffer){
		cerr<<"FAIL2: receive different message"<<endl;
		return false;
	}
	message = "Test Reply 129012901290";
	sSocket->reply(message);
	memset((void*)buffer,0,100);
	rc = cSocket->recv(buffer, 100);
	cout<<"send msg:"<<message<<endl;
	cout<<"recv msg:"<<buffer<<endl;
	if (rc != (int)message.length()){
		cerr<<"FAIL1: receive different message length"<<endl;
		return false;
	}
	if (message != buffer){
		cerr<<"FAIL2: receive different message"<<endl;
		return false;
	}
	sSocket->close();
	cSocket->close();
	delete sSocket;
	delete cSocket;
	return true;
}
