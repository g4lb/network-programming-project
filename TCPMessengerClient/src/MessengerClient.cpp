/*
 * MessengerClient.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: parallels
 */

#include "MessengerClient.h"

using namespace npl;

MessengerClient::MessengerClient(){
	clientConnected = false;
}
void MessengerClient::run() {
	while (clientConnected) {
		int cmd;
		string str;
		readFromServer(cmd,str,sock);
		if (cmd > 0)
		{
			//cout<<"got msg from server: "<<str<< "  ,cmd number: "<<cmd<<endl<<flush;
			if (cmd == SESSION_ESTABLISHED)
			{
                cout << SESSION_ESTABLISHED_TEXT << str << endl;
				sessionIsActive=true;
			}
            if (cmd == SEND_MSG_TO_PEER)
                cout << str << endl;
            if (cmd == SESSION_REFUSED)
                cout << SESSION_REFUSED_TEXT << str << endl;
            if (cmd == CLOSE_SESSION_WITH_PEER){
                cout << CLOSE_SESSION_WITH_PEER_TEXT << str << endl;
                sessionIsActive = false;
            }
		}
		else
		{
			cout << "Server disconnected" << endl;
			clientConnected=false;
		}

	}
}
void MessengerClient::connect(const string& ip){
	if(!clientConnected){
		clientConnected = true;
		sock = new TCPSocket(ip, MSNGR_PORT);
		if(sock!=NULL){
			start();
		}
	}
}
void MessengerClient::openSession(const string& ipAndPort){
	if(clientConnected){
		sendToServer(OPEN_SESSION_WITH_PEER,ipAndPort,sock);
	}
	else
		cout<<"client is not connected"<<endl;
}
void MessengerClient::closeSession(){
	if(sessionIsActive){
		sendToServer(CLOSE_SESSION_WITH_PEER," ",sock);
	}
	else
		cout<<"sesion is already close"<<endl;
}
void MessengerClient::disconnect(){
	if(sessionIsActive){
		sendToServer(CLOSE_SESSION_WITH_PEER," ",sock);
	}
	if (clientConnected){
		cout<<"disconnected"<<endl;
		sendToServer(EXIT,"",sock);
		clientConnected = false;
	}
	else
		cout<<"client is already disconnected"<<endl;

}
void MessengerClient::exit(){
	if(sessionIsActive){
		sendToServer(CLOSE_SESSION_WITH_PEER," ",sock);
		}
	if(clientConnected){
		sendToServer(EXIT,"exit",sock);
		clientConnected = false;
	}
	cout<<"disconnected from server"<<endl;
	delete sock;
}
void MessengerClient::sendToServer(int command, const string& data, TCPSocket* sock){
	TCPMessengerProtocol::sendToServer(command,data,sock);
}
void MessengerClient::readFromServer(int & command,string& data,TCPSocket* sock){
	TCPMessengerProtocol::readFromServer(command,data,sock);
}
void MessengerClient::send(const string & msg){
    if (sessionIsActive && clientConnected)
	    sendToServer(SEND_MSG_TO_PEER,msg,sock);
}





