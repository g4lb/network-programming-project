//
// Created by gal on 5/27/16.
//
#include <iostream>
#include "../MessengerServer.h"
#include "../../../TCPMessengerClient/src/MessengerClient.h"
#include <thread>
#include <unistd.h>

using namespace std;
using namespace npl;

void runServer() {

    MessengerServer *a = new MessengerServer();
    cout << "the server is running" << endl;
    sleep(11);
    a->listConnectedUsers();


    sleep(40);
    delete a;
    cout << "Messenger Server was closed" << endl;


}
void runClientOne(){
    MessengerClient * clientOne = new MessengerClient();
    clientOne->connect("127.0.0.1");
    clientOne->login("Ranni","1231");
    sleep(16);
    clientOne->send("msg2");

}
void runClientTwo(){
    MessengerClient * clientTwo = new MessengerClient();
    clientTwo->connect("127.0.0.1");
    clientTwo->login("moshe","1231");
    sleep(10);
    clientTwo->openSession("Ranni");
    sleep(2);
    clientTwo->send("msg1");
}


int main(){

    thread first(runServer);
    sleep(3);
    thread second(runClientOne);
    sleep(2);
    thread third(runClientTwo);

    first.join();
    sleep(3);
    second.join();
    sleep(3);
    third.join();

    return 0;
}