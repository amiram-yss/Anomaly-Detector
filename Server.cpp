/*
 * Server.cpp
 *
 * Author: 314970054 Ariel Barmats &
 *         314985474 Amiram Yasif
 */

#include "Server.h"

string socketIO::read(){
    char c=0;
    size_t i=0;
    string s="";
    while(c!='\n'){
        recv(clientID,&c,sizeof(char),0);
        s+=c;
    }
    return s;
}

void socketIO::write(string text){
    const char* txt=text.c_str();
    send(clientID,txt,strlen(txt),0);
}

void socketIO::write(float f){
    ostringstream ss;
    ss <<f;
    string s(ss.str());
    write(s);
}

void socketIO::read(float* f){
    //recv(clientID,f,sizeof(float),0);
    // it will be already in the string line
}


Server::Server(int port) throw (const char*) {
    isRunning = true;
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
        throw "Socket failed";

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    //Global namespace bind method.
    if(::bind(fd,(struct sockaddr*)&server, sizeof(server)) < 0)
        throw "Failed to bind.";

    if(listen(fd, 3)<0)
        throw "Failed while listening.";
}

void timeoutHandler(int sigNum){
    exit(0);
}

pid_t Server::start(ClientHandler& ch) throw (const char*){
    /*serverListenerThread=new thread([&ch,this](){
		    signal(SIGALRM,timeoutHandler);//
		    while(isRunning){
				socklen_t clientSize=sizeof(client);
				alarm(2);//
                struct timeval timeout;
                timeout.tv_sec = 2;
                timeout.tv_usec = 0;

                if (setsockopt (this->fd, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                                sizeof timeout) < 0) {
                    cout << "Failed to set timeout." << endl;
                }
				int aClient = accept(fd,(struct sockaddr*)&client,&clientSize);
				if(aClient>0){
					new thread([&aClient,this,&ch](){
						ch.handle(aClient);
						close(aClient);
					});
				}
				alarm(0); //
		    }
		    close(fd);
	  });*/
    server_pid = fork();
    //Child process
    if (!server_pid) {
        signal(SIGALRM, timeoutHandler);//

        while (isRunning) {
            socklen_t clientSize = sizeof(client);
            alarm(5);//
            struct timeval timeout;
            /*timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            if (setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                           sizeof timeout) < 0) {
                cout << "Failed to set timeout." << endl;
            }*/
            int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);

            alarm(0); //
            if (aClient > 0) {
                new thread([&aClient, this, &ch]() {
                    ch.handle(aClient);
                    close(aClient);
                });
            }
        }
    }
    return server_pid;
}

void Server::stop(){
    isRunning = false;
    kill(server_pid, SIGALRM);
    //serverListenerThread->join(); // do not delete this!
    //serverListenerThread->detach();
    //close(fd);
}

Server::~Server() {
    // TODO Auto-generated destructor stub
}

