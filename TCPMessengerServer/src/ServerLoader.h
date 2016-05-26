
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