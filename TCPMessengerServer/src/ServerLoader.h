/*
 * this class handle the file of the users. their names, passwords and the register.
 * data members:
 * map<string,string> usersAndPasswords: a map the holds the users and their passwords.
 * map<string,string> getUsersAndPasswords method: this method returns all the users and their passwords.
 * map<string,string> loadAllUserFromFile : this method loads the users that are already exist in the file of the users.
 * bool addNewUser(string user,string password): this method adds a new user to the map and returning the new map.
 */

#ifndef CPPPROJECT_COPY_SERVERLOADER_H
#define CPPPROJECT_COPY_SERVERLOADER_H

#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

namespace npl{

class ServerLoader {

    private:
        map<string,string> usersAndPasswords;
        string path;

    public:
    ServerLoader(string path);
    map<string,string> getUsersAndPasswords();
    map<string,string> loadAllUserFromFile();
    bool addNewUser(string user,string password);

    };
}

#endif //CPPPROJECT_COPY_SERVERLOADER_H