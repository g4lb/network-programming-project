/*
 * TCPMessengerProtocol.h
 *
 *  this class holds the protocol of the tcp messenger, holds the names of the commands that the clients would like to commit.
 *  sendToServer method: with this method we will send the number of command we want to perform and if needed, a data.
 *  readFromServer method: reading the answer from the server about the command we asked to perform.
 *
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

#include "TCPSocket.h"
/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346

#define CLOSE_SESSION_WITH_PEER 	    1
#define OPEN_SESSION_WITH_PEER 		    2
#define EXIT						    3
#define SESSION_REFUSED				    5
#define SESSION_ESTABLISHED			    6
#define CLIENT_DISCONNECTED_FROM_ROOM   7
#define CLIENT_ENTERED_ROOM             8
#define LOGIN                           9
#define REGISTER                        10
#define LIST_USERS                      11
#define LIST_CONNECTED_USERS            12
#define LIST_ROOMS                      13
#define LIST_ROOM_USERS                 14
#define OPEN_OR_CONNECT_TO_ROOM         15
#define CLOSE_ROOM                      16
#define CHAT_CLOSED_BY_ADMIN            17
#define DISCONNECT_FROM_ROOM            18
#define SUCCESS_LOGIN                   19
#define SUCCESS_REGISTER                20
#define LOGIN_REFUSE                    21
#define REGISTER_REFUSE                 22
#define SUCCESS_ENTER_ROOM              23
#define LIST_USERS_RESPONSE             24
#define LIST_CONNECTED_USERS_RESPONSE   25
#define LIST_ROOMS_RESPONSE             26
#define LIST_ROOM_USERS_RESPONSE        27
#define DISCONNECT_FROM_ROOM_RESPONSE   28


#define SESSION_REFUSED_TEXT "Connection refused to peer: "
#define SESSION_ESTABLISHED_TEXT "Session established with peer: "
#define CLOSE_SESSION_WITH_PEER_TEXT "Sesion end with peer: "

namespace npl{
class TCPMessengerProtocol{
public:
	void static sendToServer(int command,const string& data, TCPSocket* sock){
        try {
            char commToSend[4];
            *((int *) commToSend) = htonl(command);

            sock->write(commToSend, 4);
            cout << "sent command: " << ntohl(*(int *) commToSend) << " ";

            if (data.length() > 0) {
                const char *dataToSend = data.c_str();
                char lenToSend[4];
                *((int *) lenToSend) = htonl(strlen(dataToSend));

                sock->write(lenToSend, 4);
                sock->write(dataToSend, strlen(dataToSend));

                cout << "len: " << ntohl(*(int *) lenToSend) << " data: " << dataToSend << endl;
            }
        }
        catch (...){
            cout <<"Broken pipe"<<endl;
        }
	}

	void static readFromServer(int& command, string& data, TCPSocket* sock){
        try {


            char buff[4];
            bzero(buff, 4);
            if (sock->read(buff, 4) > 0) {
                command = ntohl(*(int *) buff);

                if (sock->read(buff, 4) > 0) {
                    int len = ntohl(*(int *) buff);

                    char dataRead[len+1];
                    bzero(dataRead, len);
                    if (sock->read(dataRead, len) > 0) {
                        dataRead[len] = '\0';
                        data = dataRead;
                    }
                }
            }
            else {
                command = -1;
            }
        }
        catch (...){
            command = -1;
            data = "";
            cout <<"Broken pipe"<<endl;
        }
	}
};
}

#endif /* TCPMESSENGERPROTOCOL_H_ */
