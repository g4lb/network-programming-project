/**
 * this class holds the numbers of the possible commands.
 *
 *
 */

#ifndef CPPPROJECT_COPY_CLIENTSTATE_H
#define CPPPROJECT_COPY_CLIENTSTATE_H

enum State
{
    DISCONNECTED = 0,
    CONNECTED    = 1,
    LOGGED_IN    = 2,
    IN_SESSION   = 3,
    IN_ROOM      = 4
};

#endif //CPPPROJECT_COPY_CLIENTSTATE_H
