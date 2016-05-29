cmake_minimum_required(VERSION 3.5)
project(cppproject__copy_)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SERVER_SIDE
    SocketUtils/src/FileInterface.cpp
    SocketUtils/src/FileInterface.h
    SocketUtils/src/MThread.cpp
    SocketUtils/src/MThread.h
    SocketUtils/src/MultipleTCPSocketListener.cpp
    SocketUtils/src/MultipleTCPSocketListener.h
    SocketUtils/src/Socket.cpp
    SocketUtils/src/Socket.h
    SocketUtils/src/TCPMessengerProtocol.h
    SocketUtils/src/TCPSocket.cpp
    SocketUtils/src/TCPSocket.h
    SocketUtils/src/UDPSocket.cpp
    SocketUtils/src/UDPSocket.h
    TCPMessengerServer/src/Brocker.cpp
    TCPMessengerServer/src/Brocker.h
    TCPMessengerServer/src/Dispatcher.cpp
    TCPMessengerServer/src/Dispatcher.h
    TCPMessengerServer/src/main.cpp
    TCPMessengerServer/src/MessengerServer.cpp
    TCPMessengerServer/src/MessengerServer.h
    TCPMessengerServer/src/ChatRoom.cpp
    TCPMessengerServer/src/ChatRoom.h TCPMessengerServer/src/Users.cpp TCPMessengerServer/src/Users.h)

set(CLIENT_SIDE
        SocketUtils/src/FileInterface.cpp
        SocketUtils/src/FileInterface.h
        SocketUtils/src/MThread.cpp
        SocketUtils/src/MThread.h
        SocketUtils/src/MultipleTCPSocketListener.cpp
        SocketUtils/src/MultipleTCPSocketListener.h
        SocketUtils/src/Socket.cpp
        SocketUtils/src/Socket.h
        SocketUtils/src/TCPMessengerProtocol.h
        SocketUtils/src/TCPSocket.cpp
        SocketUtils/src/TCPSocket.h
        SocketUtils/src/UDPSocket.cpp
        SocketUtils/src/UDPSocket.h
        TCPMessengerClient/src/main.cpp
        TCPMessengerClient/src/MessengerClient.cpp
        TCPMessengerClient/src/MessengerClient.h
        TCPMessengerClient/src/ClientStatus.h)

set(serverLoaderTest
        TCPMessengerServer/src/tests/UsersTest.cpp
        TCPMessengerServer/src/Users.cpp
        TCPMessengerServer/src/Users.h)

set(mainTest
        TCPMessengerServer/src/tests/main.cpp
        TCPMessengerServer/src/tests/TcpSocketTest.cpp
        TCPMessengerServer/src/tests/TcpSocketTest.h
        TCPMessengerServer/src/tests/UdpSocketTest.h
        TCPMessengerServer/src/tests/UdpSocketTest.cpp
        SocketUtils/src/FileInterface.cpp
        SocketUtils/src/FileInterface.h
        SocketUtils/src/MThread.cpp
        SocketUtils/src/MThread.h
        SocketUtils/src/MultipleTCPSocketListener.cpp
        SocketUtils/src/MultipleTCPSocketListener.h
        SocketUtils/src/Socket.cpp
        SocketUtils/src/Socket.h
        SocketUtils/src/TCPMessengerProtocol.h
        SocketUtils/src/TCPSocket.cpp
        SocketUtils/src/TCPSocket.h
        SocketUtils/src/UDPSocket.cpp
        SocketUtils/src/UDPSocket.h)

set(TestTwoClients
        TCPMessengerServer/src/Brocker.cpp
        TCPMessengerServer/src/Brocker.h
        TCPMessengerServer/src/Dispatcher.cpp
        TCPMessengerServer/src/Dispatcher.h
        TCPMessengerServer/src/MessengerServer.cpp
        TCPMessengerServer/src/MessengerServer.h
        TCPMessengerServer/src/ChatRoom.cpp
        TCPMessengerServer/src/ChatRoom.h TCPMessengerServer/src/Users.cpp TCPMessengerServer/src/Users.h
        TCPMessengerServer/src/tests/SessionTear.cpp
        TCPMessengerClient/src/MessengerClient.cpp
        TCPMessengerClient/src/MessengerClient.h
        TCPMessengerClient/src/ClientStatus.h
        SocketUtils/src/FileInterface.cpp
        SocketUtils/src/FileInterface.h
        SocketUtils/src/MThread.cpp
        SocketUtils/src/MThread.h
        SocketUtils/src/MultipleTCPSocketListener.cpp
        SocketUtils/src/MultipleTCPSocketListener.h
        SocketUtils/src/Socket.cpp
        SocketUtils/src/Socket.h
        SocketUtils/src/TCPMessengerProtocol.h
        SocketUtils/src/TCPSocket.cpp
        SocketUtils/src/TCPSocket.h
        SocketUtils/src/UDPSocket.cpp
        SocketUtils/src/UDPSocket.h)

add_executable(server ${SERVER_SIDE})
add_executable(client ${CLIENT_SIDE})
add_executable(serverLoaderTest ${serverLoaderTest})
add_executable(mainTest ${mainTest})
add_executable(TestTwoClients ${TestTwoClients})