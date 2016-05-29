/**
 * this class holds the status of the client.
 */

#ifndef CPPPROJECT_COPY_CLIENTSTATUS_H
#define CPPPROJECT_COPY_CLIENTSTATUS_H

enum Status
{
    DISCONNECTED = 0,
    CONNECTED    = 1,
    LOGGED_IN    = 2,
    IN_SESSION   = 3,
    IN_ROOM      = 4
};

#endif //CPPPROJECT_COPY_CLIENTSTATUS_H
