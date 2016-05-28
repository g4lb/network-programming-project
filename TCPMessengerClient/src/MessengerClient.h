/*
 * MessengerClient.h
 *
 *  Created on: Apr 10, 2016
 *      Author: parallels
 */

#ifndef SRC_MESSENGERCLIENT_H_
#define SRC_MESSENGERCLIENT_H_
#include <iostream>
#include <strings.h>
#include "../../SocketUtils/src/TCPMessengerProtocol.h"
#include "../../SocketUtils/src/MThread.h"
#include "../../SocketUtils/src/TCPSocket.h"
#include "ClientState.h"
#include "../../SocketUtils/src/UDPSocket.h"
#include <sstream>
#include <algorithm>
#include <map>

using namespace std;
namespace npl{

class MessengerClient : public MThread {

private:

    class MessengerClientPeerReader : public MThread{
    public:

        UDPSocket* udpPeer;
        bool running;

        MessengerClientPeerReader(UDPSocket* udpPeer):running(true){
            this->udpPeer = udpPeer;
        }

        void run(){
            char buffer[100];
            bzero(buffer,sizeof(buffer));
            while (running){
                udpPeer->read(buffer,100);
                cout << buffer << endl;
                bzero(buffer,sizeof(buffer));
            }
            cout << "Stopped reading from peer"<<endl;
        }
        void stop() {
            running = false;
        }
    };

	State clientState;

    string currentUserName;
    string currentRoomName;
    int myConnectionPort;

	TCPSocket* mainServer;

    UDPSocket* udpPeer;
    MessengerClientPeerReader* udpReaderThread;

    //maps user,IP:PORT
    map<string,string>* peersInRoom;
    //pair of user,IP:PORT
    pair<string,string>* peerInSeesion;

public:
	MessengerClient();
	void connect(const string& ip);
    void login(const string& user,const string& password);
    void reg(const string& user,const string& password);
    void listUsers();
    void listConnectedUsers();
    void listRooms();
    void listRoomUsers(const string& roomName);
    void openOrConnectToRoom(const string& roomName);
    void printClientState();
    void closeCurrentRoom();
    void openSession(const string& ipAndPort);
	void sendToServer(int command, const string& data, TCPSocket* sock);
	void readFromServer(int & command,string& data,TCPSocket* sock);
	void send(const string & msg);
	void closeSessionOrExitRoom();
	void disconnect();
	void run();
	void exit();
private:
    void close();
};

} /* namespace npl */

#endif /* SRC_MESSENGERCLIENT_H_ */
