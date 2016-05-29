#include <map>
#include "../Users.h"

using namespace npl;
using namespace std;

void print(map<string,string> map1);
int addNewUsers(Users test,map<string,string> test2);

int main(){

    //get new list from database
    Users a("/home/parallels/github/eliav/users.txt");
    //set a new mapp
    map<string,string> b;
    //add new user and password
    addNewUsers(a,b);
    return 0;
}

void print(map<string,string> map1){
    for (map<string,string>::iterator it=map1.begin();
         it!=map1.end();++it){
        //cout << "******************************" << endl;
        cout << "USERNAME: " << it->first << " PASSWORD: " << it->second << '\n';
    }
    cout << "******************************" << endl;

};
int addNewUsers(Users test,map<string,string> test2){

    //lOADING FROM FILE
    test2 = test.loadAllUserFromFile();
    //ADD TO LIST
    test.addNewUser("a","a534534");
    test.addNewUser("b","b7567");
    test.addNewUser("c","234234");
    test.addNewUser("d","j87878");
    test.addNewUser("e","yt7657567");
    test.addNewUser("f","asdas@#$");
    test.addNewUser("g","dsdsadsa346346");
    test.addNewUser("h","tyiuytiu");

    //GETTING THE LIST
    test2 = test.getUsersAndPasswords();

    //TESTING EXIST CLIENT
    bool a =test.addNewUser("a","passs");
    if(a == false)
        cout << "the client is exist!" << endl;

    //FINAL RESULT + PRINT
    cout << "*********FINAL RESULT:*********" << endl;
    print(test2);



};