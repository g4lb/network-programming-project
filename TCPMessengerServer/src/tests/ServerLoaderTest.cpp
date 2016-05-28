#include <map>
#include "../ServerLoader.h"

using namespace npl;
using namespace std;

void listAllUsers(map<string,string> map1);

int main(){

    ServerLoader test("/home/parallels/github/eliav/users.txt");

    map<string,string> mapTest = test.loadAllUserFromFile();
    listAllUsers(mapTest);

    if(test.addNewUser("Ranni","1231"))
        cout <<  "true" << endl;
    else
        cout<< "flase" << endl;

    if(test.addNewUser("moshe","1231"))
        cout <<  "true" << endl;
    else
        cout<< "flase" << endl;
    if(test.addNewUser("tyu","1231"))
        cout <<  "true" << endl;
    else
        cout<< "flase" << endl;

    if(test.addNewUser("iop","1231"))
        cout <<  "true" << endl;
    else
        cout<< "flase" << endl;
    if(test.addNewUser("iop","1231"))
        cout <<  "true" << endl;
    else
        cout<< "flase" << endl;

    if(test.addNewUser("Ranni","1231"))
        cout <<  "true" << endl;
    else
        cout<< "flase" << endl;

    mapTest = test.getUsersAndPasswords();

    cout << "the final map is:" << endl;
    listAllUsers(mapTest);

    return 0;
}

void listAllUsers(map<string,string> map1){

    cout << "numbers of clients: " << map1.size() << endl;

    for (map<string,string>::iterator it=map1.begin(); it!=map1.end(); ++it)
        cout << it->first << " => " << it->second << '\n';

};
