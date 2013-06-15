/*
 *  Main.cpp
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


#define TARFILE "robotsThetaAdvanced.tgz"
#define OUTPUTFILE "TopDeltaWinners.txt"

#define STARTGEN 135
#define ENDGEN 137


using namespace std;

vector<int> needToCheck;

int generation = 0;

class battle {
public:
	string c1,c2;
	battle(string first,string second): c1(first),c2(second) { };
};

vector<string> battlers,winners;
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




void readInBattlers() {
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
			battlers.push_back(inputString);
			//cout << "Found a battler " << inputString << endl;
		}
	}
	cout << "At the end of that, we have " << battlers.size() << " battlers. \n";
}

void setUpBattleFront() { // will not empty the battlers vector as it fills up the battle front.
	if (battlers.size() %2 != 0 ) { // we have an odd number
		int freepass = randomint(battlers.size());
		winners.push_back(battlers[freepass]);
		if (freepass != battlers.size()-1)
			battlers[freepass]=battlers[battlers.size()-1];
		battlers.pop_back();
	} // changed this to choose the contestants randomly.
	while (battlers.size() > 0) {
		int first = randomint(battlers.size());
		int second;
		do {
			second = randomint(battlers.size());
		} while  (second == first);
		battlePtr nbp(new battle(battlers[first],battlers[second]));
		battleFront.push_back(nbp);
	
		// okay remove the people that have been pushed into the battlefront
		// by swapping them with the end one and poping it off the vector
		// need to be slightly careful in case they are the end ones
		// in which case we just need to pop them.
		// These are all pretty cheap operations as we are just moving boost pointers.
		
		if (second != battlers.size()-1) {
			if (first != battlers.size() -1) {
				battlers[first]=battlers[battlers.size()-1];
			}
			battlers.pop_back();
			if (second < battlers.size() -1) {
				battlers[second]=battlers[battlers.size()-1];
			}
			battlers.pop_back();
		} else { // the second contender was the last one on the list, so pop it and get rid of the first one.
			battlers.pop_back(); // we got rid of the second one, just the first to deal with
			if (first != battlers.size() -1) {
				battlers[first]=battlers[battlers.size()-1];
			}
			battlers.pop_back();
		}
	}
}

	
	
	

void changeDir() {
	char dirname[200];
	sprintf(dirname,"/Development/robocode/robots/Gen%d",generation);
	chdir(dirname);
    char command[200];
    sprintf(command,"tar -xf %s *.class",TARFILE);
//	system("tar -xf robotsMvPAdvanced.tgz *.class");
    system(command);
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
				sprintf(outputBuffer,"Gen%d.%s*,Gen%d.%s*\n",generation,battleToFarm->c1.c_str(),generation,battleToFarm->c2.c_str());
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
							//battleFront.push_back(found->second); // so put the battle it was processing back on the battlefront
                            // So we used tojust reschedule it, but we are getting robots that crash the whole system
                            // so we need to just arbitrarily get rid of them, ill give both the robots a poor score
                            // bit tough on the competitor but its a bit of a shitty situation all round.
                            // (we dont know which one caused the crash)
                            // here we just don't push a winner.
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
								if (creatureScore > parasiteScore) winners.push_back(battleResult->c1);
								else if (parasiteScore > creatureScore) winners.push_back(battleResult->c2); 
								else {
									cout << "Draw (just so you know) ";
									if (randomint(2)) { 
										cout << " selecting first \n";
										winners.push_back(battleResult->c1);
									} else {
										cout << " selecting second.\n";
										winners.push_back(battleResult->c2);
									}
								}
								//cout << "Battle between " << battleResult->c1 << " and parasite " << battleResult->c2 << " scored " << creatureScore << " v" << parasiteScore << endl;
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
	cout << "Size of battlers " << battlers.size() << " size of winners " << winners.size();
}
	

void saveResults() {
	if (winners.size() > 0) {
	ofstream of(OUTPUTFILE);
	cout << "Saving results of " << winners.size() << " winners " << endl;
	for (int i=0;i<winners.size();i++) {
		of << winners[i] << endl;
		cout << "Saving " << winners[i] << endl;
	}
		of.close();}
	else { 
		cout << "saveResults called with a zero winner size!\n";
        needToCheck.push_back(generation);
	}
}

void doATournament() {
	readInBattlers();
	while (battlers.size() > 32) {
		winners.clear();
		setUpBattleFront();
		doBattles();
		battlers = winners;
	}
	
	//cout << "We ended with " << winners.size() << " winners " << " they were: \n";
	
	//for (int i=0;i<winners.size();i++) {
	//	string winer1 = winners[i];
	//	cout << winer1 << endl;
	//}
	saveResults();
}


void cleanUp() { // assumes in current directory
		system("rm *.class");
}
	

int main (int argc, char * const argv[]) {
    // insert code here...
	generation = 0;
    needToCheck.clear();
	initialiseServerStuff();
	for (generation =STARTGEN;generation < ENDGEN;generation++) { 
		changeDir();
		doATournament();
		cleanUp();
	}
    if (!needToCheck.size()) {
        cout << "We need to check the following generations, something odd happened...  ";
        for (int i=0;i<needToCheck.size();i++) {
            cout << needToCheck[i] << ", ";
        }
        cout << endl;
    }
    std::cout << "Hello, World!\n";
    return 0;
}
