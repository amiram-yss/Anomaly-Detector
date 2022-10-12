/*
 * Server.h
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#pragma once

#include <iostream>
#include <sys/socket.h>  // The header file socket.h includes a number of definitions of structures needed for sockets.
#include <netinet/in.h>  // The header file in.h contains constants and structures needed for internet domain addresses.

#include <pthread.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sstream>

#include "CLI.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};

// you can add helper classes

class socketIO:public DefaultIO{
    int clientID;
public:

    socketIO(int clientID):clientID(clientID){}

    virtual string read();
    virtual void write(string text);

    virtual void write(float f);

    virtual void read(float* f);

};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        socketIO sio(clientID);
        CLI cli(&sio);
        cli.start();
    }
};


/**
 * implement on Server.cpp
 * Server shuts down in 2 seconds.
 */

class Server {
    // you may add data members
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    pid_t server_pid;

    thread* serverListenerThread;

    volatile bool isRunning;

public:
    Server(int port) throw (const char*);
    virtual ~Server();

    pid_t start(ClientHandler& ch)throw(const char*);

    void stop();
};