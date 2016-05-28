/*
 * MessengerClient.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: parallels
 */

#include <map>
#include "MessengerClient.h"

using namespace npl;


MessengerClient::MessengerClient(){
	this->clientState = State::DISCONNECTED;
	this->currentRoomName = "";
    this->peersInRoom = new map<string,string>();
    this->peerInSeesion = NULL;
    this->udpPeer = NULL;
}
void MessengerClient::run() {
    //Main server reader thread
	while (clientState != State::DISCONNECTED) {
		int cmd;
		string str;
		readFromServer(cmd,str,mainServer);
		if (cmd > 0)
		{
            if (cmd == SUCCESS_LOGIN) {
                //Expecting input in format: <user> <ip>:<port>

                std::istringstream splitter(str);
                string myUser;
                string myIp;
                int myPort;
                splitter >> myUser;
                std::getline(splitter,myIp,':');
                splitter >> myPort;


                this->clientState = State::LOGGED_IN;
                this->currentUserName = myUser;
                this->myConnectionPort = myPort;
                cout << "You successfully logged in user: ["<<myUser<<"]"<<endl;
            }
            else if (cmd == SUCCESS_REGISTER){
                //Expecting input in format: <user> <ip>:<port>

                std::istringstream splitter(str);
                string myUser;
                string myIp;
                int myPort;
                splitter >> myUser;
                std::getline(splitter,myIp,':');
                splitter >> myPort;

                this->clientState = State::LOGGED_IN;
                this->currentUserName = myUser;
                this->myConnectionPort = myPort;

                cout << "You successfully registered and logged in user ["<<myUser<<"]"<<endl;
            }
            else if (cmd == LOGIN_REFUSE) {
                cout << "Login was refused by server, check your username and/or password; "
                                "or the user is already logged in"<<endl;
            }
            else if (cmd == REGISTER_REFUSE) {
                cout << "Register was refused by server, your username exists already"<<endl;
            }
			else if (cmd == SESSION_ESTABLISHED)
			{
                //Expecting input in format: <user> <ip>:<port>

                this->clientState = State::IN_SESSION;

                std::istringstream splitter(str);
                string peerUser;
                string peerIpAndPort;
                splitter >> peerUser;
                splitter >> peerIpAndPort;

                cout << SESSION_ESTABLISHED_TEXT << "["<< peerUser<<"]" << endl;
                this->peerInSeesion = new pair<string,string>(peerUser,peerIpAndPort);

                this->udpPeer = new UDPSocket(this->myConnectionPort);

                this->udpReaderThread = new MessengerClientPeerReader(this->udpPeer);
                this->udpReaderThread->start();

			}else if (cmd == SUCCESS_ENTER_ROOM){
                //Expecting input in format: <roomName>

                this->clientState = State::IN_ROOM;
                this->currentRoomName = str;

                this->udpPeer = new UDPSocket(this->myConnectionPort);

                this->udpReaderThread = new MessengerClientPeerReader(this->udpPeer);
                this->udpReaderThread->start();

                cout << "Entered room ["<<str<<"]"<<endl;
            }
            else if (cmd == SESSION_REFUSED) {
                //Expecting input in format: <user>
                cout << SESSION_REFUSED_TEXT << "["<< str <<"]" << endl;
            }
            else if (cmd == CLOSE_SESSION_WITH_PEER){
                //Expecting input in format: NONE
                cout << CLOSE_SESSION_WITH_PEER_TEXT << "["<< this->peerInSeesion->first <<"]" << endl;

                delete this->peerInSeesion;
                if (this->udpReaderThread != NULL) {
                    this->udpReaderThread->stop();

                    delete this->udpReaderThread;
                }
                delete this->udpPeer;

                this->clientState = State::LOGGED_IN;
            }
            else if (cmd == CLIENT_DISCONNECTED_FROM_ROOM){
                //Expecting input in format: <user>
                for (map<string, string>::iterator peer = peersInRoom->begin(); peer != peersInRoom->end(); ++peer)
                {
                    if (peer->first == str){
                        this->peersInRoom->erase(peer);
                        cout << "Client ["<<peer->first<<"] has left the room"<<endl;
                        break;
                    }
                }
            }
            else if (cmd == CLIENT_ENTERED_ROOM){
                //Expecting input in format: <user> <ip>:<port>

                std::istringstream splitter(str);
                string peerUser;
                string peerIpAndPort;
                splitter >> peerUser;
                splitter >> peerIpAndPort;

                this->peersInRoom->insert(make_pair(peerUser,peerIpAndPort));
                cout << "User ["<<str<<"]"<<" has entered the room"<<endl;
            }
            else if (cmd == CHAT_CLOSED_BY_ADMIN or cmd == DISCONNECT_FROM_ROOM_RESPONSE){
                this->peersInRoom->clear();

                if (this->udpReaderThread != NULL) {
                    this->udpReaderThread->stop();

                    delete this->udpReaderThread;
                }
                delete this->udpPeer;

                this->clientState = State::LOGGED_IN;
                if (cmd == CHAT_CLOSED_BY_ADMIN)
                    cout << "Chat room closed by admin"<<endl;
                else if (cmd == DISCONNECT_FROM_ROOM_RESPONSE)
                    cout << "You disconnected from the room" <<endl;
            }
            else if (cmd == LIST_USERS_RESPONSE){
                cout << "****Registered Users****\n"<< str << "***********" << endl;
            }
            else if (cmd == LIST_CONNECTED_USERS_RESPONSE){
                cout << "****Connected Users****\n"<< str << "***********" << endl;
            }else if (cmd == LIST_ROOMS_RESPONSE){
                cout << "****Rooms****\n"<< str << "***********" << endl;
            }else if (cmd == LIST_ROOM_USERS_RESPONSE){
                cout << "****Room Users****\n"<< str << "***********" << endl;
            }
		}
		else
		{
			cout << "Server disconnected" << endl;
            this->close();
		}

	}
}

void MessengerClient::close(){
    if (this->udpReaderThread != NULL) {
        this->udpReaderThread->stop();

        delete this->udpReaderThread;
    }
    if (this->udpPeer != NULL)
        delete this->udpPeer;
    if (this->clientState != State::DISCONNECTED)
        this->clientState = State::DISCONNECTED;
    this->currentRoomName = "";
    this->currentUserName = "";
    this->myConnectionPort = 0;
    if (this->peerInSeesion != NULL)
        delete this->peerInSeesion;
    if (this->peersInRoom != NULL)
        delete this->peersInRoom;
    if (this->mainServer != NULL) {
        this->mainServer->close();
        delete this->mainServer;
    }

}

void MessengerClient::connect(const string& ip){
	if(clientState == State::DISCONNECTED){
		mainServer = new TCPSocket(ip, MSNGR_PORT);
		if(mainServer != NULL){
			clientState = State::CONNECTED;
			start();
		}
	}
    else
    {
        cout << "You are already connected to a server"<<endl;
    }
}

void MessengerClient::login(const string& user,const string& password) {
    if (clientState == State::CONNECTED){
        sendToServer(LOGIN,user + " " + password,mainServer);
    }
    else{
        cout << "You are not connected to a server" << endl;
    }
}
void MessengerClient::reg(const string& user,const string& password) {
    if (clientState == State::CONNECTED){
        sendToServer(REGISTER,user + " " + password,mainServer);
    }
    else{
        cout << "You are not connected to a server" << endl;
    }
}

void MessengerClient::listUsers(){
    if (clientState == State::LOGGED_IN || clientState == State::IN_ROOM || clientState == State::IN_SESSION){
        sendToServer(LIST_USERS," ",mainServer);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::listConnectedUsers(){
    if (clientState == State::LOGGED_IN || clientState == State::IN_ROOM || clientState == State::IN_SESSION){
        sendToServer(LIST_CONNECTED_USERS," ",mainServer);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::listRooms(){
    if (clientState == State::LOGGED_IN || clientState == State::IN_ROOM || clientState == State::IN_SESSION){
        sendToServer(LIST_ROOMS," ",mainServer);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::listRoomUsers(const string& roomName){
    if (clientState == State::LOGGED_IN || clientState == State::IN_ROOM || clientState == State::IN_SESSION){
        sendToServer(LIST_ROOM_USERS,roomName,mainServer);
    }
    else{
        cout << "You are not logged in" << endl;
    }
}
void MessengerClient::openOrConnectToRoom(const string& roomName){
    if (clientState == State::LOGGED_IN)
        sendToServer(OPEN_OR_CONNECT_TO_ROOM,roomName,mainServer);
    else if (clientState == State::IN_SESSION)
        cout << "You are already in a session. use command: cs"<<endl;
    else if (clientState == State::IN_ROOM)
        cout << "You are already in a room. use command: cs"<<endl;
    else
        cout << "You are not logged in" << endl;

}
void MessengerClient::printClientState(){
    if (clientState == State::IN_ROOM)
        cout << "Client in room ["<<this->currentRoomName<<"]"<<endl;
    else if (clientState == State::IN_SESSION)
        cout << "Client is in session"<<endl;
    else if (clientState == State::LOGGED_IN)
        cout << "Client is logged in to server" <<endl;
    else if (clientState == State::CONNECTED)
        cout << "Client is connected to server" <<endl;
    else if (clientState == State::DISCONNECTED)
        cout << "Client is not connected to any server"<<endl;
}
void MessengerClient::closeCurrentRoom(){
    if (clientState == State::IN_ROOM){
        sendToServer(CLOSE_ROOM,this->currentRoomName,mainServer);
    }
    else{
        cout << "You are not in a room" << endl;
    }
}

void MessengerClient::openSession(const string& peerUser){
	if(clientState == State::LOGGED_IN)
		sendToServer(OPEN_SESSION_WITH_PEER,peerUser,mainServer);
    else if (clientState == State::IN_SESSION)
        cout << "You are already in a session. use command: cs"<<endl;
    else if (clientState == State::IN_ROOM)
        cout << "You are already in a room. use command: cs"<<endl;
    else
        cout << "You are not logged in" << endl;
}
void MessengerClient::closeSessionOrExitRoom(){
	if(clientState == State::IN_SESSION)
		sendToServer(CLOSE_SESSION_WITH_PEER," ",mainServer);
	else if (clientState == State::IN_ROOM)
        sendToServer(DISCONNECT_FROM_ROOM,this->currentRoomName,mainServer);
	else
		cout<<"Not in session or room"<<endl;
}
void MessengerClient::disconnect(){
    if (clientState != State::DISCONNECTED) {
        sendToServer(EXIT, " ", mainServer);
        close();
        cout << "You have disconnected from server"<<endl;
    }
    else
        cout<<"Client is already disconnected"<<endl;

}
void MessengerClient::exit(){
    if (clientState != State::DISCONNECTED) {
        sendToServer(EXIT, " ", mainServer);
    }
	close();
    cout << "You have disconnected from server"<<endl;
}
void MessengerClient::sendToServer(int command, const string& data, TCPSocket* mainServer){
	TCPMessengerProtocol::sendToServer(command,data,mainServer);
}
void MessengerClient::readFromServer(int & command,string& data,TCPSocket* mainServer){
	TCPMessengerProtocol::readFromServer(command,data,mainServer);
}
void MessengerClient::send(const string & msg){

    string peerIp;
    int peerPort;

    if (clientState == State::IN_SESSION) {

        std::istringstream splitter(this->peerInSeesion->second);

        std::getline(splitter, peerIp, ':');
        splitter >> peerPort;

        //send peer in session the msg
        this->udpPeer->sendTo(">["+currentUserName+"] " + msg, peerIp, peerPort);
    }
    else if (clientState == State::IN_ROOM) {

        std::istringstream splitter;

        //iterate over all peers in room and send msg
        for (map<string, string>::iterator peer = peersInRoom->begin(); peer != peersInRoom->end(); ++peer)
        {
            splitter.clear();
            splitter.str(peer->second);
            std::getline(splitter, peerIp, ':');
            splitter >> peerPort;

            this->udpPeer->sendTo(">["+currentUserName+"] " + msg, peerIp, peerPort);
        }
    }
    else{
        cout << "You are not in a room or a session"<<endl;
    }
}





