#include "Users.h"

using namespace npl;


map<string,string> Users::getUsersAndPasswords() {
    return this->usersAndPasswords;
};

Users::Users(string path){
    this->path = path;
};

bool Users::addNewUser(string user,string password){
    string nameuser = user;
    string passworduser = password;

    for (map<string, string>::iterator it = this->usersAndPasswords.begin();
         it != this->usersAndPasswords.end(); ++it) {
        if (it->first == nameuser){
            cout << BOLDRED << "ERROR: The client " << "'"<< it->first << "'" << " is connected" << RESET <<  endl;
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
map<string,string> Users::loadAllUserFromFile(){

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
