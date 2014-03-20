//
//  RoboPopHandler.cpp
//  Robocode
//
//  Created by Robin Harper on 14/03/2014.
//
//

#include "RoboPopHandler.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <list>
#include <map>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

namespace DWHandler { // avoids collisions with, say spatialRobocodeCreatureDelta
    
    typedef boost::shared_ptr<dwBattleLine> battleLinePtr;
    list<battleLinePtr> battleFront; // a list of battles to be fought.
    list<int> currentFds; // list of clients that have joined
    list<int> readyForWork; // the clients that are awaiting work.
    map<int,battleLinePtr> sockfdToCritter; // maps clients to the battle they are working out.
    map<int,battleLinePtr>::iterator found;
    
    fd_set readfds,testfds;
    struct sockaddr_in client_address;
    socklen_t server_len,client_len;
    int server_sockfd, client_sockfd;
    
#define BUFFERSIZE  200
    char inputBuffer[BUFFERSIZE];
    char outputBuffer[BUFFERSIZE];
    
    
    
#define PORT "9000"  // the port users will be connecting to
    
#define BACKLOG 100	 // how many pending connections queue will hold
    
    void sigchld_handler(int s)
    {
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }
    
    // get sockaddr, IPv4 or IPv6:
    void *get_in_addr(struct sockaddr *sa)
    {
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }
    
    void initialiseServerStuff(short portNo=9000)
    {
        struct addrinfo hints, *servinfo, *p;
        struct sigaction sa;
        int yes=1;
        int rv;
        
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // use my IP
        
        if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }
        
        // loop through all the results and bind to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((server_sockfd = socket(p->ai_family, p->ai_socktype,
                                        p->ai_protocol)) == -1) {
                perror("server: socket");
                continue;
            }
            
            if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                           sizeof(int)) == -1) {
                perror("setsockopt");
                exit(1);
            }
            
            if (bind(server_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(server_sockfd);
                perror("server: bind");
                continue;
            }
            
            break;
        }
        
        if (p == NULL)  {
            fprintf(stderr, "server: failed to bind\n");
            exit(2);
        }
        
        freeaddrinfo(servinfo); // all done with this structure
        
        if (listen(server_sockfd, BACKLOG) == -1) {
            perror("listen");
            exit(1);
        }
        
        sa.sa_handler = sigchld_handler; // reap all dead processes
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }
        
        printf("server: waiting for connections...\n");
        
        FD_ZERO(&readfds);
        FD_SET(server_sockfd,&readfds);
        currentFds.clear();
        readyForWork.clear();
        currentFds.push_back(server_sockfd); // server is one of the fds.
    }
}

void RoboPopHandler::fileSave(const char *fname) {
    ofstream fout;
    if (population.size() == 0) {
        cout << "Nothing to save\n";
        return;
    }
    fout.open(fname,ios::out);
    vector<wrappedDeltaPtr>::iterator it = population.begin();
    while (it != population.end()) {
        (*it)->saveCreature(fout);
        it++;
    };
    fout.close();
}

string RoboPopHandler::fileLoad(const char *fname){
    // might want to save popSize, but we will just assume its correct
    ifstream fin(fname);
    cout << "Opened file\n";
    ostringstream outs;
    if (!fin) {
        cout <<  "Error trying to open " << fname << " for loading\n";
        return outs.str();
    } else { // the following isnt very robust if we dont have a well formed file
        population.clear();
        for (int i=0;i<popSize;i++) {
            wrappedDeltaPtr tp;
            tp.reset(new DeltaWrappedCritter(false));
            tp->loadCreature(fin);
            population.push_back(tp);
        }
    }
    fin.close();
    outs << "OK";
    return outs.str();
}

void RoboPopHandler::rampedptc2(int startExp,int endExp,int maxDepth) {
    population.clear();
    for (int i=0;i<popSize;i++) {
        wrappedDeltaPtr tp;
        tp.reset(new DeltaWrappedCritter(true));
        population.push_back(tp);
    }
}

string RoboPopHandler::breed(int cycles,int termination ) {
    /*
     So what is to do:
     1) Go through and run the battles to get the scores.
     2) Sort depending on the score you got.
     3) Use tournament selection to pick the best ones and breed.
     */
     
    return "good idea";
}




