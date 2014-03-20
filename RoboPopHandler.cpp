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

namespace DWHandler { // avoids collisions with, say spatialRobocodeCreatureDelta
    
    
    class dwBattleLine {
    public:
        dwBattleLine(wrappedDeltaPtr cr,int pa): CreatureDelta(cr),ParasiteDelta(pa) { }
        wrappedDeltaPtr CreatureDelta;
        int ParasiteDelta;
    };

    
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
    
    string getGenName(int currentGen,wrappedDeltaPtr creatureDelta) {
        char outputB[200];
        sprintf(outputB,"Gen%d.%s",currentGen,creatureDelta->getName().c_str());
        string answer=outputB;
        return answer;
    }

#define numberOfParasites 19
    string getGenName(int parasite) {
        switch (parasite) {
            case 0: return "supersample.SuperBoxBot";
            case 1: return "supersample.SuperCorners";
            case 2: return "supersample.SuperCrazy";
            case 3: return "supersample.SuperMercutio";
            case 4: return "supersample.SuperRamFire";
            case 5: return "supersample.SuperSpinBot";
            case 6: return "supersample.SuperTracker";
            case 7: return "supersample.SuperTrackFire";
            case 8: return "supersample.SuperWalls";
            case 9: return "supersample.SuperBoxBot";
            case 10: return "blir.nano.Cabbage";
            case 11: return "sgp.nano.FurryLeech";
            case 12: return "pez.nano.LittleEvilBrother";
            case 13: return "NG.LegatusLegionis";
            case 14: return "projectx.ProjectNano";
            case 15: return "dggp.haiku.gpBot_0";
            case 16: return "zyx.nano.RedBull";
            case 17: return "zyx.nano.Ant";
            case 18: return "stelo.MirrorNano";
            default:    return "Bum parasite";
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

void RoboPopHandler::rampedptc2() {
    population.clear();
    for (int i=0;i<popSize;i++) {
        wrappedDeltaPtr tp;
        tp.reset(new DeltaWrappedCritter(true));
        population.push_back(tp);
    }
}

/* Hmm not sure where we set the generation - the conrolling program on here.
   Just now I am assuming currentGen is set correctly
 */



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

void RoboPopHandler::saveEm() {
	char fname[200];
	char jname[200];
	char gname[200];
	sprintf(gname,"Gen%d",currentGen);
    for_each(population.begin(),population.end(),[&] (wrappedDeltaPtr pt) {
        sprintf(fname,"%s",pt->getName().c_str());
        sprintf(jname,"%s.java",fname);
        ofstream robotout;
        robotout.open(jname);
        robocodeDelta::Instance()->print_grammar_individual(robotout,pt->getCreature()->getExpression(),pt->getCreature()->getExpressionLength(),fname,gname);
        robotout.close();
    });
    
}



void RoboPopHandler::breed() {
    // clear their scores
    for_each(population.begin(),population.end(),[] (wrappedDeltaPtr pt) { pt->clearScore(); });
    // Compile the bots
   
    changeDirectory();
    fileSave(fileName.c_str());
    saveEm();
    DWHandler::battleFront.clear();
    cout << "Cleared the battle list.\n";
    system("javac -J-Xmx1024m -classpath ../../libs/robocode.jar *.java");
    cout << "Compiled the bots.\n";

    // Schedule each populaiton member to battle each parasite. <- this could be made more sophisticated.
    for_each(population.begin(),population.end(),[&] (wrappedDeltaPtr pt) {
        for (int i=0;i<numberOfParasites;i++) {
            DWHandler::battleLinePtr nptr(new DWHandler::dwBattleLine(pt,i));
            DWHandler::battleFront.push_back(nptr); // this will be autodeleted when battleFront cleared (the power of shared_ptr!)
        }
    });
    
    doTheBattles();
    
    // So recap - for the "currentGeneration" we have compiled the robots, battled them against the parasites and stored their scores
    
    // So - sort them and then breed a new population to replace the old one.
    // reverse as we want top near the beginning.
    std::sort(population.begin(),population.end(), [](wrappedDeltaPtr a, wrappedDeltaPtr b){ return b->getScore() < a->getScore(); });
    
    // since we have a sorted population. Lets show the fittest 20
    cout << "Top 20 scores were:\n";
    for (int i=0;i<20;i++) {
        cout << population[i]->getScore() << (i==19 ? "\n":", ");
    }
    
    // could invoke the generic breeders, but I'll just implement a simple tournament breeder now.
    int tournamentSize=5;
    int mutate_chance=2000; // 0.2
    int eliteRetain = 10; // keep this number of the best ones
    
    vector<wrappedDeltaPtr> nextgen;
    nextgen.clear();
    crPtr c1,c2;
    wrappedDeltaPtr cw1,cw2;
    crPtr p1,p2;
    nextgen.reserve(popSize+1); // really only need pop_size+1, but what the heck..
    crPtr t1,t2;
    while (((int) nextgen.size()) < (popSize-eliteRetain)) { // this is how many we need to make.
        int firstChosen=popSize-1;
        int secondChosen=popSize-1;
        int canditate;
        
        // Population is ordered best first, so we just need to roll the dice tournamentSize times and pick the lowest one.
        for (int i = 0; i < tournamentSize;i++) {
                canditate = randomint(popSize);
                if (canditate < firstChosen) firstChosen = canditate;
        }
        for (int i = 0; i < tournamentSize;i++) {
                canditate = randomint(popSize);
                if (canditate < secondChosen)  secondChosen = canditate;
        }
        
        // cout << "Going to choose " << first << " and then " << second << "\n";
        
        // Obviously you can use any operator.
        cr_data::variableCrossover(p1=population[firstChosen]->getCreature(),p2=population[secondChosen]->getCreature(),c1,c2);
        
        if (c1 != NULL) {
            if (c1->isValid()) {
               c1->smartSureMutate(mutate_chance);
               if (c1->isValid()) { // is it still valid after mutation?
                    DeltaWrappedCritter *newChild1= new DeltaWrappedCritter(false);
                    cw1.reset(newChild1);
                    cw1->replaceWithCopyOfCreature(c1);
                    nextgen.push_back(cw1);
                }
            }
            c1.reset();  // its okay to reset it as nextgen has a copy of the smart pointer.
            cw1.reset();
        }
        if (c2 != NULL) {
            if (c2->isValid()) {
                
                // NOTE I am only mutating valid children, which might not be right
                // some operators will create invalid children - do I want to mutate them
                // in the hope they become valid?
                
                c2->smartSureMutate(mutate_chance);
                if (c2->isValid()) { // is it still valid after mutation?
                    DeltaWrappedCritter *newChild2= new DeltaWrappedCritter(false);
                    cw2.reset(newChild2);
                    cw2->replaceWithCopyOfCreature(c2);
                    nextgen.push_back(cw2);
                }
            }
            c2.reset();  // its okay to reset it as nextgen has a copy of the smart pointer.
            cw2.reset();
        }
    }
    population.resize(eliteRetain); // get rid of any that aren't saved by being elite.
    population.insert(population.end(),nextgen.begin(),nextgen.end());
    population.resize(popSize); // sometimes we can get one extra because we create two children per pop.
    
    // So we have a new population, but remember their scores aren't valid yet. Lets just reset them to zero so we don't accidentally use them.
    for_each(population.begin(),population.end(),[] (wrappedDeltaPtr pt) { pt->clearScore(); });
   
}

/*
 Assumes all the scheduled battles are in DWHandler::battleFront
 */
using namespace DWHandler;

void RoboPopHandler::doTheBattles() {
    int fd,nread;
    DWHandler::sockfdToCritter.clear(); // none have been assigned
    int result;
    int nearestHundred = (DWHandler::battleFront.size()/100);
    int battlesWeNeed = DWHandler::battleFront.size();
    int battlesSoFar = 0;
    cout << "About to schedule " << battlesWeNeed << " matches.\n";
    while (! (DWHandler::battleFront.empty() && DWHandler::sockfdToCritter.empty())) { // so as long as there are battles or we await outcomes
        if (DWHandler::battleFront.size()/100 < nearestHundred) {
            nearestHundred--;
            cout << "Down to about " << (nearestHundred+1)*100 << " matches\n";
        }
        if (!DWHandler::battleFront.empty()) { // more battles to send out
            while ( (!(DWHandler::battleFront.empty())) && (!(DWHandler::readyForWork.empty()))) { // so whilst we still have battles and workers
                int sdf = DWHandler::readyForWork.front();
                DWHandler::readyForWork.pop_front();
                DWHandler::battleLinePtr battleToFarm = DWHandler::battleFront.front();
                DWHandler::battleFront.pop_front();
                // quick hack
                sprintf(DWHandler::outputBuffer,"%s*,%s*\n",getGenName(currentGen,battleToFarm->CreatureDelta).c_str(),getGenName(battleToFarm->ParasiteDelta).c_str());
                //cout << "Sending instructions as follows: " << outputBuffer;
                int cr;
                if ( (cr = write(sdf,DWHandler::outputBuffer,strlen(DWHandler::outputBuffer))) != strlen(DWHandler::outputBuffer)) {
                    perror("Write error, writing battle robot names on socket ");
                    DWHandler::battleFront.push_back(battleToFarm); // put it back in the battleFront
                    // lets see what happens, maybe I need to close the fd.
                    close(sdf);
                    FD_CLR(sdf,&DWHandler::readfds);
                    list<int>::iterator at = find<list<int>::iterator,int>(DWHandler::currentFds.begin(),DWHandler::currentFds.end(),sdf);
                    if (at != DWHandler::currentFds.end()) DWHandler::currentFds.erase(at);
                    
                    else { cout << "Strange couldn't find it in currentFds\n";
                    }
                    
                    //exit(1);
                } else 	DWHandler::sockfdToCritter.insert(pair<int,DWHandler::battleLinePtr>(sdf,battleToFarm));
                
            }
        }
        DWHandler::testfds=DWHandler::readfds;
        // so wait for something to happen (maybe should timeout this
        result = select(FD_SETSIZE,&DWHandler::testfds,(fd_set *) 0,(fd_set *) 0,(struct timeval *) 0);
        if (result < 1) {
            perror("Error in spatialRobocodeCreater, select returned less than 1 whilst I was trying to do all my battle stuff.");
            //				exit(1);
        }
        list<int>::iterator at = DWHandler::currentFds.begin();
        while (at!= DWHandler::currentFds.end()) {  // loop through the fds looking for one that has been set.
            if (FD_ISSET((*at),&DWHandler::testfds)) {
                fd = *at;
                if (fd == DWHandler::server_sockfd) {
                    // it was ths server, which means we have a client connecting
                    DWHandler::client_len = sizeof(DWHandler::client_address);
                    DWHandler::client_sockfd = accept(DWHandler::server_sockfd,(struct sockaddr *) &DWHandler::client_address,(socklen_t *) &client_len);
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




