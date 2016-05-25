/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author:
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
#define SEND_MSG_TO_PEER			    4
#define SESSION_REFUSED				    5
#define SESSION_ESTABLISHED			    6
#define CLIENT_DISCONNECTED_FROM_ROOM   7
#define CLIENT_ENTERED_ROOM             8
#define CHAT_CLOSED_BY_ADMIN            9

#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_TEXT "Connection refused to peer: "
#define SESSION_ESTABLISHED_TEXT "Session established with peer: "
#define CLOSE_SESSION_WITH_PEER_TEXT "Sesion end with peer: "
#define EXIT_TEXT "disconnected: "

namespace npl{
class TCPMessengerProtocol{
public:
	void static sendToServer(int command,const string& data, TCPSocket* sock){

        char commToSend[4];
        *((int*)commToSend) = htonl(command);

		sock->write(commToSend,4);
        cout << "sent command: "<<ntohl(*(int*)commToSend)<<" ";

		if(data.length()>0) {
            const char* dataToSend =data.c_str();
            char lenToSend[4];
            *((int*)lenToSend) = htonl(strlen(dataToSend));

            sock->write(lenToSend, 4);
            sock->write(dataToSend, strlen(dataToSend));

            cout << "len: "<<ntohl(*(int*)lenToSend)<<" data: "<<dataToSend<<endl;
        }
	}

	void static readFromServer(int& command, string& data, TCPSocket* sock){
        char buff[4];
        bzero(buff,4);
		if (sock->read(buff,4) > 0) {
            command = ntohl(*(int *) buff);
            switch (command)
            {
                case EXIT:
                    return;
            }
            if (sock->read(buff, 4) > 0) {
                int len = ntohl(*(int *) buff);

                char dataRead[len];
                bzero(dataRead, len);
                if (sock->read(dataRead, len) > 0) {
                    dataRead[len] = '\0';
                    data = dataRead;
                }
            }
        }
        else
        {
            command=-1;
        }
	}
};
}

#endif /* TCPMESSENGERPROTOCOL_H_ */
