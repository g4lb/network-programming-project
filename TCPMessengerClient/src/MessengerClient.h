/*
 * MessengerClient.h
 *
 *  this class is implements the client. each client is in thread ,connects in TCP, and  between two clients the communication is on UDP.
 *  data members:
 *  UDPSocket* udpPeer: holds an object of udp socket.
 * bool running: hold boolean variable, true as long as runing.
 * MessengerClientPeerReader(UDPSocket* udpPeer):running(true): this method returns an object of udp socket.
 * run method: is use to open a new thread.once stopping reading , the variable running will change to false.
 *State clientState: holds an object of state.
 * string currentUserName: holds the current name of the user.
 * string currentRoomName: holds the current name of the room.
 * int myConnectionPort: the number of the connected port.
 * TCPSocket* mainServer: holds an object of TCPSocket.
 * UDPSocket* udpPeer: holds an object of UDPSocket.
 * MessengerClientPeerReader* udpReaderThread: holds an object of MessengerClientPeerReader.
 * map<string,string>* peersInRoom: maps between peers to rooms.
 * pair<string,string>* peerInSeesion: the map holds the peers in sessions.
 * 	void connect(const string& ip):this method is making a connection.
 *  void login(const string& user,const string& password): this method is making a login to the room chat.
 * void reg(const string& user,const string& password);his method is making a register to the room chat.
 * void closeSessionOrExitRoom method: closing session when the client asked to.
 *
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
