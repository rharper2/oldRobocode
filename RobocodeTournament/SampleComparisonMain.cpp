/*
 *  SampleComparisonMain.cpp
 *  RobocodeTournament
 *
 *  Created by Robin on 4/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *  MultiMain.cpp
 *  RobocodeTournament
 *
 *  Created by Robin on 4/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include <iostream>
#include <string>
#include <dirent.h>
#include "myrandom.h"
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <list>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <string.h>


using namespace std;

int generation = 190;

bool verbose;


#define NUMBEROFSAMPLES 17

class results {
public:
	string name;
	float matches[NUMBEROFSAMPLES];
	results(string n): name(n) { };
};

typedef boost::shared_ptr<results> resultPtr;

vector<resultPtr> resultVector;

class battle {
public:
	string c1,c2;
	int oppNumber;
	resultPtr resultStore;
	battle(string first,string second,int opp, resultPtr res): c1(first),c2(second),oppNumber(opp), resultStore(res) { };
};

vector<string> battlers;
typedef boost::shared_ptr<battle> battlePtr;

list<battlePtr> battleFront; // a list of battles to be fought.
list<int> currentFds; // list of clients that have joined
list<int> readyForWork; // the clients that are awaiting work.
map<int,battlePtr> sockfdToCritter; // maps clients to the battle they are working out.
map<int,battlePtr>::iterator found;

fd_set readfds,testfds;
struct sockaddr_in client_address;
struct sockaddr_in server_address;
socklen_t server_len,client_len;
int server_sockfd, client_sockfd;


void initialiseServerStuff(short portNo=9001) {
	
    server_sockfd = socket(AF_INET,SOCK_STREAM,0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(portNo);
    server_len = sizeof(server_address);
    
    bind(server_sockfd,(struct sockaddr *)&server_address,server_len);
    
    listen(server_sockfd,5);
    FD_ZERO(&readfds);
    FD_SET(server_sockfd,&readfds);
	currentFds.clear();
	readyForWork.clear();
	currentFds.push_back(server_sockfd); // server is one of the fds.
}

void changeDir();
void changeDir(int);

void readInBattlers(int gen) {
	changeDir(gen);
	char name[80];
	system("tar -xvzf robotsThetaAdvanced.tgz '*.class'");
	DIR *dp;
	string inputString;
	int pos;
	battlers.clear();
	if ((dp = opendir("./")) == NULL) {
		cout << "Cant open current directory!\n";
		exit(1);
	}
	struct dirent *entry;
	while (( entry = readdir(dp)) != NULL) {
		inputString = entry->d_name;
		if ((pos = inputString.find(".class")) != string::npos) {
			inputString.erase(pos,6);
			sprintf(name,"Gen%d.%s",gen,inputString.c_str());
			
			battlers.push_back(name);
			cout << "Found a battler " << name << endl;
		}
	}
	cout << "At the end of that, we have " << battlers.size() << " battlers. \n";
}

void readInWinners() {
    for (int toread = 150;toread < 174;toread++) {
        
    changeDir(toread);
    ifstream in;
	in.open("TopDeltawinners.txt");
	if (!in) {
		cout << "Error in reading Top10Winners file.\n";
		exit(1);
	}
	char name[80];
    char command[80];
	while (!in.eof()) {
		string read;
		in >> read;
		if (!in.eof()) {
			cout << "Just read a winner as " << read << endl;
			sprintf(name,"%s",read.c_str());
			cout << "It has become " << name << endl;
           /* string named = name;
            int pos = named.find(".");
            string directory = named.substr(3,pos);
            int gen;
            gen = atoi(directory.c_str());
            cout << "The gen is " << gen << endl;
            changeDir(gen);
           */
            int gen = toread;
            //string justname = named.substr(pos+1);
            string justname = name;
            cout << "justname is " << justname << endl;
            sprintf(command,"tar -xzvf robotsThetaAdvanced.tgz %s.class",justname.c_str());
            system(command);
            sprintf(name,"Gen%d.%s",gen,justname.c_str());
			battlers.push_back(name);
		}
	}
	in.close();
	cout << "At the end of that, we have " << battlers.size() << " battlers. \n";
    }
}

void setUpBattleFront() {
	// for each battler, set up a result store and a battle with each of the six sample bots.
		
	vector<string>::iterator at;
	at=battlers.begin();
	while (at!=battlers.end()) {
		resultPtr result(new results(*at));
		resultVector.push_back(result);
		battlePtr nbp(new battle(*at,"sample.Crazy",0,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"sample.RamFire",1,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"sample.SpinBot",2,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"sample.Tracker",3,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"sample.TrackFire",4,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"sample.VelociRobot",5,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"sample.Walls",6,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"NDH.GuessFactor 1.0*",7,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"gg.Peryton1_1*",8,result));
		battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperBoxBot*",9,result));
        battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperCorners*",11,result));
        battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperCrazy*",12,result));
        battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperMercutio*",13,result));
        battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperRamFire*",14,result));
        battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperSpinBot*",15,result));
        battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperTrackFire*",16,result));
        battleFront.push_back(nbp);
		nbp.reset(new battle(*at,"supersample.SuperWalls*",10,result));
        battleFront.push_back(nbp);
		at++;
		//cout << "Set up a battle between " << nbp->c1 << " and " << nbp->c2 << endl;
	}
}

void changeDir() { changeDir(generation); }

void changeDir(int gen) {
	char dirname[200];
	sprintf(dirname,"/Development/robocode/robots/Gen%d",gen);
	chdir(dirname);
	//	system("tar -xf robotsAdvanced.tgz *.class");
}


void doBattles() {
#define BUFFERSIZE 200
	int fd,nread;
	sockfdToCritter.clear(); // none have been assigned
	int result;
	int nearestHundred = (battleFront.size()/100);
	int battlesWeNeed = battleFront.size();
	int battlesSoFar = 0;
	char outputBuffer[BUFFERSIZE],inputBuffer[BUFFERSIZE];
	
	cout << "About to schedule " << battlesWeNeed << " matches.\n";
	while (! (battleFront.empty() && sockfdToCritter.empty())) { // so as long as there are battles or we await outcomes
		if (battleFront.size()/100 < nearestHundred) {
			nearestHundred--;
			cout << "Down to about " << (nearestHundred+1)*100 << " matches\n";
		}
		if (!battleFront.empty()) { // more battles to send out
			while ( (!(battleFront.empty())) && (!(readyForWork.empty()))) { // so whilst we still have battles and workers
				int sdf = readyForWork.front();
				readyForWork.pop_front();
				battlePtr battleToFarm = battleFront.front();
				battleFront.pop_front();
				sprintf(outputBuffer,"%s*,%s\n",battleToFarm->c1.c_str(),battleToFarm->c2.c_str());
				//cout << "Sending instructions as follows: " << outputBuffer;
				int cr;
				if ( (cr = write(sdf,outputBuffer,strlen(outputBuffer))) != strlen(outputBuffer)) {
					perror("Write error, writing battle robot names on socket ");
					battleFront.push_back(battleToFarm); // put it back in the battleFront
					// lets see what happens, maybe I need to close the fd.
					close(sdf);
					FD_CLR(sdf,&readfds);
					list<int>::iterator at = find<list<int>::iterator,int>(currentFds.begin(),currentFds.end(),sdf);
					if (at != currentFds.end()) currentFds.erase(at);
					
					else { cout << "Strange couldn't find it in currentFds\n";
					}
					
					//exit(1);
				} else 	sockfdToCritter.insert(pair<int,battlePtr>(sdf,battleToFarm));
				
			}
		}
		testfds=readfds;
		// so wait for something to happen (maybe should timeout this
		result = select(FD_SETSIZE,&testfds,(fd_set *) 0,(fd_set *) 0,(struct timeval *) 0); 
		if (result < 1) {
			perror("Error in spatialRobocodeCreater, select returned less than 1 whilst I was trying to do all my battle stuff.");
			//				exit(1);
		}
		list<int>::iterator at = currentFds.begin();
		while (at!= currentFds.end()) {  // loop through the fds looking for one that has been set.
			if (FD_ISSET((*at),&testfds)) { 
				fd = *at; 
				if (fd == server_sockfd) {
					// it was ths server, which means we have a client connecting
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd,(struct sockaddr *) &client_address,(socklen_t *) &client_len);
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
								long creatureScore = strtol(inputBuffer,&currPtr,10);
								long parasiteScore;
								if (*currPtr != ',') {
									cout << "Strange expected a comma after the number.\n";
								} else {
									currPtr++;
									parasiteScore = strtol(currPtr,NULL,10);
								}
								if ((parasiteScore == 0) || (creatureScore == 0)) {
									cout << "Got a parasite score of " << parasiteScore << " and a creatureScore of " << creatureScore << endl;
									cout << "It should not be possible to get a zero score.\n";
									cout << "The input buffer was " << inputBuffer << endl;
								}
								battlePtr battleResult = found->second;
								battlesSoFar++;
								battleResult->resultStore->matches[battleResult->oppNumber] = ((float) creatureScore)/((float)(creatureScore+parasiteScore));
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


void saveResults(const char *filename) {
	ofstream of(filename);
	cout << "Saving results of " << resultVector.size() << " battlers " << endl;
	of << "Name, Crazy, RamFire, Spin Bot, Tracker, TrackFire, VelociRobot, Walls, GuessFactor, Pertyton, SBB, SCrazy, SMercuito, SRamFire, SSping, SuperTF, SuperWalls, Cigaret \n";
	for (int i=0;i<resultVector.size();i++) {
		of << resultVector[i]->name;
		for (int j=0;j<NUMBEROFSAMPLES;j++) 
			of << ", " << resultVector[i]->matches[j] ;
		of << endl;
	}
	of.close();
}
void cleanUp() { // assumes in current directory
	system("rm *.class");
}
void doTheTournament() {
	readInWinners();
	
	setUpBattleFront();
	doBattles();
	chdir("~");
	saveResults("PvMGenTournament2.csv");
	
	
}

void doAGenTournament(int gen) {
	char filename[200];
	sprintf(filename ,"PvMTournamentofGen%d.csv",gen);
	readInBattlers(gen);
	setUpBattleFront();
	doBattles();
	saveResults(filename);
}




int main (int argc, char * const argv[]) {
    // insert code here...
	generation = 3;
	verbose = false;
	initialiseServerStuff();
	//doTheTournament();
	//for (int i=165;i>130;i--) {
	//	doAGenTournament(i);
		cleanUp();
	//}
   //doAGenTournament(154);
	doTheTournament();
    std::cout << "Goodbye, World!\n";
    return 0;
}
