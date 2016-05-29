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
	this->clientStatus = Status::DISCONNECTED;
	this->currentRoomName = "";
    this->peersInRoom = new map<string,string>();
    this->peerInSeesion = NULL;
    this->udpPeer = NULL;
}
void MessengerClient::run() {
    //Main server reader thread
	while (clientStatus != Status::DISCONNECTED) {
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


                this->clientStatus = Status::LOGGED_IN;
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

                this->clientStatus = Status::LOGGED_IN;
                this->currentUserName = myUser;
                this->myConnectionPort = myPort;

                cout << BOLDBLUE<< "registered & logged in user ["<<myUser<<"]"<< RESET<<endl;
            }
            else if (cmd == LOGIN_REFUSE) {
                cout << BOLDRED<< "ERROR: please check your username & password "<<RESET<<endl;
            }
            else if (cmd == REGISTER_REFUSE) {
                cout << BOLDRED<<"ERROR: username exist "<< RESET<<endl;
            }
			else if (cmd == SESSION_ESTABLISHED)
			{
                //Expecting input in format: <user> <ip>:<port>

                this->clientStatus = Status::IN_SESSION;

                std::istringstream splitter(str);
                string peerUser;
                string peerIpAndPort;
                splitter >> peerUser;
                splitter >> peerIpAndPort;

                cout << BOLDBLUE<< SESSION_ESTABLISHED_TEXT << "["<< peerUser<<"]" << RESET<<endl;
                this->peerInSeesion = new pair<string,string>(peerUser,peerIpAndPort);

                this->udpPeer = new UDPSocket(this->myConnectionPort);

                this->udpReaderThread = new MessengerClientPeerReader(this->udpPeer);
                this->udpReaderThread->start();

			}else if (cmd == SUCCESS_ENTER_ROOM){
                //Expecting input in format: <roomName>

                this->clientStatus = Status::IN_ROOM;
                this->currentRoomName = str;

                this->udpPeer = new UDPSocket(this->myConnectionPort);

                this->udpReaderThread = new MessengerClientPeerReader(this->udpPeer);
                this->udpReaderThread->start();

                cout << BOLDBLUE <<"Entered room ["<<str<<"]"<< RESET <<endl;
            }
            else if (cmd == SESSION_REFUSED) {
                //Expecting input in format: <user>
                cout << BOLDBLUE << SESSION_REFUSED_TEXT << "["<< str <<"]" << RESET << endl;
            }
            else if (cmd == CLOSE_SESSION_WITH_PEER){
                //Expecting input in format: NONE
                cout << BOLDBLUE << CLOSE_SESSION_WITH_PEER_TEXT << "["<< this->peerInSeesion->first <<"]" << RESET<< endl;

                delete this->peerInSeesion;
                if (this->udpReaderThread != NULL) {
                    this->udpReaderThread->stop();
                    //this->udpReaderThread->waitForThread();
                }
                this->udpPeer->close();

                this->clientStatus = Status::LOGGED_IN;
            }
            else if (cmd == CLIENT_DISCONNECTED_FROM_ROOM){
                //Expecting input in format: <user>
                for (map<string, string>::iterator peer = peersInRoom->begin(); peer != peersInRoom->end(); ++peer)
                {
                    if (peer->first == str){
                        this->peersInRoom->erase(peer->first);
                        cout << BOLDGREEN << "A Client has left the room"<< RESET<<endl;
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
                cout << BOLDBLUE<< "User ["<<str<<"]"<<" entere the room"<< RESET<<endl;
            }
            else if(cmd==ADMIN_LEAVE_ROOM){
                if (this->udpReaderThread != NULL) {
                    this->udpReaderThread->stop();
                }
                this->udpPeer->close();
                this->clientStatus = Status::LOGGED_IN;
                cout << BOLDGREEN << "You close the chat room" << RESET << endl;
            }
            else if (cmd == CHAT_CLOSED_BY_ADMIN or cmd == DISCONNECT_FROM_ROOM_RESPONSE){
                this->peersInRoom->clear();

                if (this->udpReaderThread != NULL) {
                    this->udpReaderThread->stop();
                    //delete this->udpReaderThread;
                }
                this->udpPeer->close();
                this->clientStatus = Status::LOGGED_IN;
                if (cmd == CHAT_CLOSED_BY_ADMIN)
                    cout << BOLDGREEN << "Chat room closed by admin" << RESET << endl;
                else if (cmd == DISCONNECT_FROM_ROOM_RESPONSE)
                    cout << BOLDGREEN<< "You disconnected from the room" <<RESET<<endl;
            }
            else if (cmd == LIST_USERS_RESPONSE){
                cout << BOLDBLUE << "Registered Users:\n"<< str << "--" << RESET<< endl;
            }
            else if (cmd == LIST_CONNECTED_USERS_RESPONSE){
                cout << BOLDBLUE << "Connected Users:\n"<< str << "--" << RESET<< endl;
            }else if (cmd == LIST_ROOMS_RESPONSE){
                cout << BOLDBLUE <<"Rooms:\n"<< str << "--" << RESET<<endl;
            }else if (cmd == LIST_ROOM_USERS_RESPONSE){
                cout << BOLDBLUE << "Room Users:\n"<< str << "--" << RESET<<endl;
            }
		}
		else
		{
			cout << BOLDBLUE <<"Server disconnected" <<RESET<< endl;
            this->close();
		}

	}
}

void MessengerClient::close(){
    cout << BOLDBLUE<< "Closing All..\n";

    this->currentRoomName = "";
    this->currentUserName = "";
    this->myConnectionPort = 0;
    if (this->peerInSeesion != NULL)
        delete this->peerInSeesion;
    if (this->peersInRoom != NULL)
        this->peersInRoom->clear();

    if (this->udpReaderThread != NULL) {
        this->udpReaderThread->stop();
        delete this->udpReaderThread;
    }
    if (this->udpPeer != NULL) {
        this->udpPeer->close();
    }
    if (this->clientStatus != Status::DISCONNECTED) {
        this->clientStatus = Status::DISCONNECTED;
    }
    if (this->mainServer != NULL) {
        this->mainServer->close();
    }
    cout<<"Done."<<RESET<<endl;

    ::exit(0);
}

void MessengerClient::connect(const string& ip){
	if(clientStatus == Status::DISCONNECTED){
		mainServer = new TCPSocket(ip, MSNGR_PORT);
		if(mainServer != NULL){
			clientStatus = Status::CONNECTED;
			start();
		}
	}
    else
    {
        cout << BOLDRED<<"You are already connected to a server"<<RESET<<endl;
    }
}

void MessengerClient::login(const string& user,const string& password) {
    if (clientStatus == Status::CONNECTED){
        sendToServer(LOGIN,user + " " + password,mainServer);
    }
    else{
        cout << BOLDRED<<"You are not connected to a server" << RESET<<endl;
    }
}
void MessengerClient::reg(const string& user,const string& password) {
    if (clientStatus == Status::CONNECTED){
        sendToServer(REGISTER,user + " " + password,mainServer);
    }
    else{
        cout << BOLDRED<<"You are not connected to a server" << RESET<<endl;
    }
}

void MessengerClient::listUsers(){
    if (clientStatus == Status::LOGGED_IN || clientStatus == Status::IN_ROOM || clientStatus == Status::IN_SESSION){
        sendToServer(LIST_USERS," ",mainServer);
    }
    else{
        cout << BOLDRED<<"You are not logged in" << RESET<<endl;
    }
}
void MessengerClient::listConnectedUsers(){
    if (clientStatus == Status::LOGGED_IN || clientStatus == Status::IN_ROOM || clientStatus == Status::IN_SESSION){
        sendToServer(LIST_CONNECTED_USERS," ",mainServer);
    }
    else{
        cout << BOLDRED<<"You are not logged in" << RESET<<endl;
    }
}
void MessengerClient::listRooms(){
    if (clientStatus == Status::LOGGED_IN || clientStatus == Status::IN_ROOM || clientStatus == Status::IN_SESSION){
        sendToServer(LIST_ROOMS," ",mainServer);
    }
    else{
        cout <<BOLDRED<< "You are not logged in" << RESET<<endl;
    }
}
void MessengerClient::listRoomUsers(const string& roomName){
    if (clientStatus == Status::LOGGED_IN || clientStatus == Status::IN_ROOM || clientStatus == Status::IN_SESSION){
        sendToServer(LIST_ROOM_USERS,roomName,mainServer);
    }
    else{
        cout << BOLDRED<<"You are not logged in" << RESET<<endl;
    }
}
void MessengerClient::openOrConnectToRoom(const string& roomName){
    if (clientStatus == Status::LOGGED_IN)
        sendToServer(OPEN_OR_CONNECT_TO_ROOM,roomName,mainServer);
    else if (clientStatus == Status::IN_SESSION)
        cout << BOLDRED<< "You are already in a session. use command: cs"<<RESET<<endl;
    else if (clientStatus == Status::IN_ROOM)
        cout <<BOLDRED<< "You are already in a room. use command: cs"<<RESET<<endl;
    else
        cout << BOLDRED<<"You are not logged in" << RESET<<endl;

}
void MessengerClient::printClientStatus(){
    if (clientStatus == Status::IN_ROOM)
        cout << BOLDBLUE << "Client in room ["<<this->currentRoomName<<"]"<<RESET<<endl;
    else if (clientStatus == Status::IN_SESSION)
        cout << BOLDBLUE <<"Client is in session"<<RESET<<endl;
    else if (clientStatus == Status::LOGGED_IN)
        cout << BOLDBLUE <<"Client is logged in to server" <<RESET<<endl;
    else if (clientStatus == Status::CONNECTED)
        cout << BOLDBLUE <<"Client is connected to server" <<RESET<<endl;
    else if (clientStatus == Status::DISCONNECTED)
        cout << BOLDBLUE <<"Client is not connected to any server"<<RESET<<endl;
}
void MessengerClient::closeCurrentRoom(){
    if (clientStatus == Status::IN_ROOM){
        sendToServer(CLOSE_ROOM,this->currentRoomName,mainServer);
    }
    else{
        cout << BOLDRED<< "You are not in a room" << RESET<<endl;
    }
}

void MessengerClient::openSession(const string& peerUser){
	if(clientStatus == Status::LOGGED_IN)
		sendToServer(OPEN_SESSION_WITH_PEER,peerUser,mainServer);
    else if (clientStatus == Status::IN_SESSION)
        cout << BOLDRED<<"You are already in a session. use command: cs"<< RESET<< endl;
    else if (clientStatus == Status::IN_ROOM)
        cout << BOLDRED<<"You are already in a room. use command: cs"<<RESET<<endl;
    else
        cout << BOLDRED<<"You are not logged in" << RESET<<endl;
}
void MessengerClient::closeSessionOrExitRoom(){
	if(clientStatus == Status::IN_SESSION)
		sendToServer(CLOSE_SESSION_WITH_PEER," ",mainServer);
	else if (clientStatus == Status::IN_ROOM)
        sendToServer(DISCONNECT_FROM_ROOM,this->currentRoomName,mainServer);
	else
		cout<< BOLDRED<<"Not in session or room"<<RESET<<endl;
}
void MessengerClient::disconnect(){
    if (clientStatus != Status::DISCONNECTED) {
        sendToServer(EXIT, " ", mainServer);
        close();
        cout << BOLDBLUE<< "You have disconnected from server"<<RESET<<endl;
    }
    else
        cout<< BOLDBLUE<<"Client is already disconnected"<< RESET<<endl;

}
void MessengerClient::exit(){
    if (clientStatus != Status::DISCONNECTED) {
        sendToServer(EXIT, " ", mainServer);
    }
	close();
    cout << BOLDBLUE<< "You have disconnected from server"<< RESET<<endl;
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

    if (clientStatus == Status::IN_SESSION) {

        istringstream splitter(this->peerInSeesion->second);
        getline(splitter, peerIp, ':');
        splitter >> peerPort;

        //send peer in session the msg
        this->udpPeer->sendTo(">["+currentUserName+"] " + msg, peerIp, peerPort);
    }
    else if (clientStatus == Status::IN_ROOM) {

        istringstream splitter;

        //iterate over all peers in room and send msg
        for (map<string, string>::iterator peer = peersInRoom->begin(); peer != peersInRoom->end(); ++peer)
        {
            splitter.clear();
            splitter.str(peer->second);
            getline(splitter, peerIp, ':');
            splitter >> peerPort;

            this->udpPeer->sendTo(">["+currentUserName+"] " + msg, peerIp, peerPort);
        }
    }
    else{
        cout << BOLDRED<< "You are not in a room or a session"<< RESET<<endl;
    }
}





