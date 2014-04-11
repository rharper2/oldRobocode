//
//  LightHandler.cpp
//  Robocode
//
//  Created by robin on 11/04/2014.
//
//

#include "LightHandler.h"
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
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include "robocodeDelta.h"

namespace LWHandler { // avoids collisions with, say spatialRobocodeCreatureDelta
    
    
    class LWBattleLine {
    public:
        LWBattleLine(lightlywrappedDeltaPtr cr,int pa): lightlywrappedDeltaPtr(cr),ParasiteDelta(pa) { }
        lightlywrappedDeltaPtr CreatureDelta;
        int ParasiteDelta;
    };
    
    
    typedef boost::shared_ptr<LWBattleLine> battleLinePtr;
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
    
    string getGenName(int currentGen,wrappedDeltaPtr creatureDelta) {
        char outputB[200];
        sprintf(outputB,"Gen%d.%s",currentGen,creatureDelta->getName().c_str());
        string answer=outputB;
        return answer;
    }
    
    int numberOfParasites = 15;
    int sampleSet = 0;
    void setSampleSet(int sample) {
        switch (sample) {
            case 0:
                sampleSet=0;
                numberOfParasites = 15;
                break;
            case 1:
                sampleSet=1;
                numberOfParasites = 19;
                break;
            case 2:
                sampleSet=2;
                numberOfParasites=16;
                break;
            default:
                cout << "Invalid sample " << sample << endl;
                exit(1);
        }
    }
    
    string getGenName(int parasite) {
        switch (sampleSet) {
            case 0:{
                
                switch (parasite) {
                    case 0: return "supersample.SuperBoxBot*";
                    case 1: return "supersample.SuperCorners*";
                    case 2: return "supersample.SuperCrazy*";
                    case 3: return "supersample.SuperMercutio*";
                    case 4: return "supersample.SuperRamFire*";
                    case 5: return "supersample.SuperSpinBot*";
                    case 6: return "supersample.SuperTracker*";
                    case 7: return "supersample.SuperTrackFire*";
                    case 8: return "supersample.SuperWalls*";
                    case 9: return "supersample.SuperBoxBot*";
                    case 10:return "sample.Corners";
                    case 11:return "sample.SittingDuck";
                    case 12:return "sample.SpinBot";
                    case 13:return "sample.RamFire";
                    case 14:return "sample.SpinBot";
                    default:    return "Bum parasite";
                }
            }
            case 1:
            {
                switch (parasite) {
                    case 0: return "supersample.SuperBoxBot*";
                    case 1: return "supersample.SuperCorners*";
                    case 2: return "supersample.SuperCrazy*";
                    case 3: return "supersample.SuperMercutio*";
                    case 4: return "supersample.SuperRamFire*";
                    case 5: return "supersample.SuperSpinBot*";
                    case 6: return "supersample.SuperTracker*";
                    case 7: return "supersample.SuperTrackFire*";
                    case 8: return "supersample.SuperWalls*";
                    case 9: return "supersample.SuperBoxBot*";
                    case 10: return "blir.nano.Cabbage*";
                    case 11: return "sgp.nano.FurryLeech*";
                    case 12: return "pez.nano.LittleEvilBrother*";
                    case 13: return "NG.LegatusLegionis*";
                    case 14: return "projectx.ProjectNano*";
                    case 15: return "dggp.haiku.gpBot_0*";
                    case 16: return "zyx.nano.RedBull*";
                    case 17: return "zyx.nano.Ant*";
                    case 18: return "stelo.MirrorNano*";
                    default:    return "Bum parasite";
                }
                
            }
            case 2: {
                switch (parasite) {
                    case 0: return "mld.LittleBlackBook*";
                    case 1: return "mld.Moebius*";
                    case 2: return "mld.jdc.nano.LittleBlackBook*";
                    case 3: return  "nz.jdc.nano.AralR*";
                    case 4: return   "nz.jdc.nano.AralT*";
                    case 5: return   "nz.jdc.nano.NeophytePattern*";
                    case 6: return   "nz.jdc.nano.PatternAdept*";
                    case 7: return   "nz.jdc.nano.PralDeGuerre*";
                    case 8: return   "oog.nano.Caligula*";
                    case 9: return  "robar.nano.Pugio*";
                    case 10: return   "simonton.nano.WeekendObsession_S*";
                    case 11: return   "fromHell.C22H30N2O2S*";
                    case 12: return  "wompi.Kowari*";
                    case 13: return "gg.Peryton1_1*";
                    case 14: return "jk.sheldor.nano.Yatagan*";
                    case 15: return "dummy.micro.Sparrow*";
                    default:    return "Bum parasite";
                }
            default: return "Bum set";
            }
                
                
                
        }
    }
    
    
    
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

RoboPopHandler::RoboPopHandler() {
    popSize = 300;
    
}


/* Hmm not sure where we set the generation - the conrolling program on here.
 Just now I am assuming currentGen is set correctly
 */

vod addRobot(string name) {
    LightlyWrappedCritter *lwc = new LightlyWrappedCritter(name);
    lightlywrappedDeltaCritter toadd;
    toadd.reset(lwc);
    population.push_back(toadd);
}


void RoboPopHandler::changeDirectory() {
	// clean up so robocode doesnt take ages loading all the old robots.
	if (currentGen != 0) {
		system("tar -czf robots.tgz *.java *.class");
		system("rm *.class *.java");
	}
    
	char dirname[200];
	sprintf(dirname,"/Development/robocode/robots/Gen%d",currentGen);
	printf("Creating directory %s\n",dirname);
	int j;
	j=mkdir(dirname,0775);
	if (!j) {
		int en=errno;
		printf("Mkdir returned %d, with errno %d\n",j,en);
	}
	chdir(dirname);
}

// goes through the world (and each layer recursively)
// saving the phenotype (as a .java) file of each critter and parasite
// into the current working directory (which has been set up by changeDirectory)


// Assumes that population contains the robots you want to give a really good workout!
// Oh and assumes you have properly set Generation. so watchout.
// Currently selects the best set on set0, then does 50 runs on the two top bots with set0 set1 and set2
void RoboPopHandler::reallyTestThem() {
    LWHandler::setSampleSet(0);
    // clear their scores
    for_each(population.begin(),population.end(),[] (wrappedDeltaPtr pt) { pt->clearScore(); });
    
    fileSave(fileName.c_str());
    // Schedule each populaiton member to battle each parasite. <- this could be made more sophisticated.
    for_each(population.begin(),population.end(),[&] (lightlywrappedDeltaPtr pt) {
        for (int i=0;i<LWHandler::numberOfParasites;i++) {
            LWHandler::battleLinePtr nptr(new LWHandler::LWBattleLine(pt,i));
            LWHandler::battleFront.push_back(nptr); // this will be autodeleted when battleFront cleared (the power of shared_ptr!)
        }
    });
    
    doTheBattles();
    
    // use this to work out the top (two) we are interested in.
    std::sort(population.begin(),population.end(), [](wrappedDeltaPtr a, wrappedDeltaPtr b){ return b->getScore() < a->getScore(); });
    population.resize(2);
    cout << "There can be only, err, two\n";
    // get rid of the dross
   	system("rm *.class *.java");
    saveEm(); // works as we are down to a few in the population
    LWHandler::battleFront.clear();
    cout << "Cleared the battle list.\n";
    system("javac -J-Xmx1024m -classpath ../../libs/robocode.jar *.java");
    cout << "Compiled the bots.\n";
    // quick and dirty
    char  fname[200];
    sprintf(fname,"ResultsofTopNowWith%dSets.csv",3);
    ofstream of(fname);
    of.open("LightWrapWrapUp.csv");
    for (int set=0;set<3;set++) {
        of << "Set " << set << endl;
        
        LWHandler::setSampleSet(set);
        for_each(population.begin(),population.end(),[&] (lightywrappedDeltaPtr pt) {
            of << pt->getName() << ",";
        });
        for (int i=0;i<50;i++) {
            for_each(population.begin(),population.end(),[] (lightlywrappedDeltaPtr pt) { pt->clearScore(); });
            for_each(population.begin(),population.end(),[&] (lightlywrappedDeltaPtr pt) {
                for (int i=0;i<LWHandler::numberOfParasites;i++) {
                    LWHandler::battleLinePtr nptr(new LWHandler::LWBattleLine(pt,i));
                    LWHandler::battleFront.push_back(nptr); // this will be autodeleted when battleFront cleared (the power of shared_ptr!)
                }
            });
            doTheBattles();
            for_each(population.begin(),population.end(),[&] (lightywrappedDeltaPtr pt) {
                of << pt->getScore() << ",";
            });
            of << endl;
        }
        of << endl;
    }
    of.close();
}


/*
 Assumes all the scheduled battles are in LWHandler::battleFront
 */
using namespace LWHandler;

void RoboPopHandler::doTheBattles() {
    int fd,nread;
    LWHandler::sockfdToCritter.clear(); // none have been assigned
    int result;
    int nearestHundred = (LWHandler::battleFront.size()/100);
    int battlesWeNeed = LWHandler::battleFront.size();
    int battlesSoFar = 0;
    cout << "About to schedule " << battlesWeNeed << " matches.\n";
    while (! (LWHandler::battleFront.empty() && LWHandler::sockfdToCritter.empty())) { // so as long as there are battles or we await outcomes
        if (LWHandler::battleFront.size()/100 < nearestHundred) {
            nearestHundred--;
            cout << "Down to about " << (nearestHundred+1)*100 << " matches\n";
        }
        if (!LWHandler::battleFront.empty()) { // more battles to send out
            while ( (!(LWHandler::battleFront.empty())) && (!(LWHandler::readyForWork.empty()))) { // so whilst we still have battles and workers
                int sdf = LWHandler::readyForWork.front();
                LWHandler::readyForWork.pop_front();
                LWHandler::battleLinePtr battleToFarm = LWHandler::battleFront.front();
                LWHandler::battleFront.pop_front();
                // quick hack
                sprintf(LWHandler::outputBuffer,"%s*,%s\n",getGenName(currentGen,battleToFarm->CreatureDelta).c_str(),getGenName(battleToFarm->ParasiteDelta).c_str());
                //cout << "Sending instructions as follows: " << outputBuffer;
                int cr;
                if ( (cr = write(sdf,LWHandler::outputBuffer,strlen(LWHandler::outputBuffer))) != strlen(LWHandler::outputBuffer)) {
                    perror("Write error, writing battle robot names on socket ");
                    LWHandler::battleFront.push_back(battleToFarm); // put it back in the battleFront
                    // lets see what happens, maybe I need to close the fd.
                    close(sdf);
                    FD_CLR(sdf,&LWHandler::readfds);
                    list<int>::iterator at = find<list<int>::iterator,int>(LWHandler::currentFds.begin(),LWHandler::currentFds.end(),sdf);
                    if (at != LWHandler::currentFds.end()) LWHandler::currentFds.erase(at);
                    
                    else { cout << "Strange couldn't find it in currentFds\n";
                    }
                    
                    //exit(1);
                } else 	LWHandler::sockfdToCritter.insert(pair<int,LWHandler::battleLinePtr>(sdf,battleToFarm));
                
            }
        }
        LWHandler::testfds=LWHandler::readfds;
        // so wait for something to happen (maybe should timeout this
        result = select(FD_SETSIZE,&LWHandler::testfds,(fd_set *) 0,(fd_set *) 0,(struct timeval *) 0);
        if (result < 1) {
            perror("Error in spatialRobocodeCreater, select returned less than 1 whilst I was trying to do all my battle stuff.");
            //				exit(1);
        }
        list<int>::iterator at = LWHandler::currentFds.begin();
        while (at!= LWHandler::currentFds.end()) {  // loop through the fds looking for one that has been set.
            if (FD_ISSET((*at),&LWHandler::testfds)) {
                fd = *at;
                if (fd == LWHandler::server_sockfd) {
                    // it was ths server, which means we have a client connecting
                    LWHandler::client_len = sizeof(LWHandler::client_address);
                    LWHandler::client_sockfd = accept(LWHandler::server_sockfd,(struct sockaddr *) &LWHandler::client_address,(socklen_t *) &client_len);
                    FD_SET(client_sockfd,&readfds);
                    currentFds.push_back(client_sockfd);
                    printf("Adding client on fd %d.\n",client_sockfd);
                    //readyForWork.push_back(fd);
                    list<int>::iterator y = currentFds.begin();
                    cout << "Current fds: ";
                    while (y!=currentFds.end()) { cout << (*y) << " "; y++; }
                    cout << endl;
                }
                else { // okay it was not the server, so presumably its a client
                    ioctl(fd,FIONREAD,&nread);
                    if (nread == 0) {
                        close(fd);
                        if (( found = sockfdToCritter.find(fd)) != sockfdToCritter.end()) {
                            // well the socket terminated but it was supposed to be doing something for me!
                            battleFront.push_back(found->second); // so put the battle it was processing back on the battlefront
                            sockfdToCritter.erase(found); // take it off the fds that have been assigned battles
                        }
                        at = currentFds.erase(at); // remove from the list of currentFds
                        readyForWork.remove(fd); // just in case its in the ready for work list.
                        FD_CLR(fd,&readfds); // take out of the fds we are polling
                        printf("Removing client with fd, %d.\n",fd);
                    } else { // established socket with data.
                        int no = read(fd,inputBuffer,BUFFERSIZE-1);
                        if (no < 0) {
                            perror("Read error in get score, failed to read from a socket that said it had info.");
                            exit(1);
                        }
                        inputBuffer[no] = '\0'; // null terminate it.
                        if (!strcmp("Next\n",inputBuffer)) {
                            sprintf(outputBuffer,"ack\n");
                            write(fd,outputBuffer,strlen(outputBuffer));
                            readyForWork.push_back(fd);
                            if ((found = sockfdToCritter.find(fd)) != sockfdToCritter.end()) {
                                // hey how can it be ready, its supposed to be something for me.
                                battleFront.push_back(found->second);
                                sockfdToCritter.erase(found);
                                cout << "Received a NEXT from fd " << fd << " when I thought it was still doing a battle for me.\n";
                            }
                        } else
                        { // okay looks like we have a score
                            if ((found = sockfdToCritter.find(fd)) == sockfdToCritter.end()) {
                                // but we dont seem to be waiting for a score
                                cout << "File descriptor " << fd << " gave me something which was not a NEXT but I wasn't expecting a score from it.\n";
                                cout << "It gave me : " << inputBuffer << endl;
                            } else {
                                char *currPtr;
                                long CreatureDeltaScore = strtol(inputBuffer,&currPtr,10);
                                long ParasiteDeltaScore;
                                if (*currPtr != ',') {
                                    cout << "Strange expected a comma after the number.\n";
                                } else {
                                    currPtr++;
                                    ParasiteDeltaScore = strtol(currPtr,NULL,10);
                                }
                                battleLinePtr battleLineResult = found->second;
                                if ((ParasiteDeltaScore == 0) || (CreatureDeltaScore == 0)) {
                                    cout << "Got a ParasiteDelta score of " << ParasiteDeltaScore << " and a CreatureDeltaScore of " << CreatureDeltaScore << endl;
                                    cout << "With competitors " << (battleLineResult->CreatureDelta)->getName() << " and " << getGenName(battleLineResult->ParasiteDelta) << endl;
                                    cout << "It should not be possible to get a zero score.\n";
                                    cout << "The input buffer was " << inputBuffer << endl;
                                }
                                double totalScore = CreatureDeltaScore + ParasiteDeltaScore;
                                battleLineResult->CreatureDelta->addToScore((CreatureDeltaScore*100)/totalScore);
                                //cout << "Battle between " << battleLineResult->CreatureDelta->getName() << " and ParasiteDelta " << battleLineResult->ParasiteDelta->getName() << " scored " << number << endl;
                                sockfdToCritter.erase(found);
                                readyForWork.push_back(fd);
                                
                            }
                        }
                    } // end of a client which has data (rather than closed
                } // end of presumably its a client
            } // end of if FD_ISSET
            at++;
        } // end of looping through the currentfds while.
    }
    cout << "All the battles done, we believed we had to carry out " << battlesWeNeed << " battles and we got " << battlesSoFar << " results.\n";
}




