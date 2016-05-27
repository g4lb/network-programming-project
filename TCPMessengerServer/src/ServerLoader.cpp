#include "ServerLoader.h"

using namespace npl;


map<string,string> ServerLoader::getUsersAndPasswords() {
    return this->usersAndPasswords;
};

ServerLoader::ServerLoader(string path){
    this->path = path;
};

bool ServerLoader::addNewUser(string user,string password){
    string nameuser = user;
    string passworduser = password;

    for (map<string, string>::iterator it = this->usersAndPasswords.begin();
         it != this->usersAndPasswords.end(); ++it) {
        if (it->first == nameuser){
            cout << "sorry the user " << "'"<< it->first << "'" << " is connected" << endl;
            return false;
        }
    }
    this->usersAndPasswords.insert(pair<string,string>(nameuser,passworduser));

    ofstream out;
    out.open(this->path ,ios::app | ios::out);
    out << nameuser << endl;
    out.flush();
    out << passworduser<< endl;
    out.flush();
    out.close();
    return true;
};
map<string,string> ServerLoader::loadAllUserFromFile(){

    ifstream nameFileout;
    nameFileout.open(this->path, ios::in);
    if (nameFileout) {
        string user;
        string password;
        map<string, string> map;

        while (std::getline(nameFileout, user) && getline(nameFileout, password)) {
            this->usersAndPasswords.insert(pair<string, string>(user, password));
        }
    }
    nameFileout.close();

    return this->usersAndPasswords;
};
