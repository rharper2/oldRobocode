/*
 *  spatialRobocodeCreature.cpp
 *  oneAgentWorld
 *
 *  Created by Robin on 3/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "spatialRobocodeCreature.h"
#include <fstream>
#include "robocodeAlpha.h"
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

void streamout(ostream &pout,crPtr &item) {
	int len;
	pout << (len=item->getDnaLength()) << ' ';
	for (int i=0;i<len;i++) {
		pout << item->getDna()[i] << ' ';
	}
	pout << "-1\n";
}

/************************************************************************
 spatialRobocodeCreature(bool)
 *************************************************************************/

long spatialRobocodeCreature::creatureIdentifierCount = 0;
long spatialRobocodeParasite::parasiteIdentifierCount = 0;
bool spatialRobocodeCreature::stateFlag = false;

typedef boost::shared_ptr<battleLine> battleLinePtr;
list<battleLinePtr> battleFront; // a list of battles to be fought.
list<int> currentFds; // list of clients that have joined
list<int> readyForWork; // the clients that are awaiting work.
map<int,battleLinePtr> sockfdToCritter; // maps clients to the battle they are working out.
map<int,battleLinePtr>::iterator found;

fd_set readfds,testfds;
struct sockaddr_in client_address;
struct sockaddr_in server_address;
socklen_t server_len,client_len;
int server_sockfd, client_sockfd;

#define BUFFERSIZE  200
char inputBuffer[BUFFERSIZE];
char outputBuffer[BUFFERSIZE];

void initialiseServerStuff(short portNo=9000) {
	
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
	


int currentGen = 0;

char javaCName[200];
char javaPName[200];

void makeName(long id,bool creature) { 
	if (creature) sprintf(javaCName,"Critter%ld",id);
	else sprintf(javaPName,"Parasite%ld",id);
}

const char *spatialRobocodeCreature::getName() {
	makeName(thisCreatureNumber,true);
	return javaCName;
}

// Need to test ptc2 with this grammar.
spatialRobocodeCreature::spatialRobocodeCreature() {
	geneticAge = 0;
	int maxTree;
	bool done = false;
	thisCreatureNumber = creatureIdentifierCount++;
	do {
		maxTree = randomint(8)+2;
		critter.reset(new cr_data);
		critter->makeptc2(maxTree);
	    done = critter->isValid();
	} while (!done);
	isInvalid = false;
	//cout << "New valid spatialRobocodeCreature creature created, with max size " << maxTree << "and id " << thisCreatureNumber << endl;
	
	
	char fname[200];
	char jname[200];
	char gname[200];
	sprintf(gname,"Gen%d",currentGen);
	//cout << "Thinking of saving " << getName() <<  endl;
	if (!(getCreature()->isValid())) {
		cout << "Weird, newly created creature is not valid\n";
		exit(1);
	}
	sprintf(fname,"%s",getName());
	sprintf(jname,"%s.java",fname);
	ofstream robotout;
	robotout.open(jname);
	robocodeAlpha::Instance()->print_grammar_individual(robotout,getCreature()->getExpression(),getCreature()->getExpressionLength(),fname,gname);
	robotout.close();
}

spatialRobocodeCreature::spatialRobocodeCreature(bool make) {
	thisCreatureNumber = creatureIdentifierCount++;
	compsWon = 0;
	geneticAge =0;
	if (make) {
		int maxTree;
		bool done = false;
		geneticAge = 0;
		do {
			maxTree = randomint(8)+2;
			critter.reset(new cr_data);
			critter->makeptc2(maxTree);
			done = critter->isValid();
		} while (!done);
		isInvalid = false;
		//cout << "New valid spatialRobocodeCreature(true) creature created, with max size " << maxTree << "and id " << thisCreatureNumber << endl;
		char fname[200];
		char jname[200];
		char gname[200];
		sprintf(gname,"Gen%d",currentGen);

		//cout << "Thinking of saving " << getName() << endl;
		if (!(getCreature()->isValid())) {
			cout << "Weird, newly created creature is not valid\n";
			exit(1);
		}
		sprintf(fname,"%s",getName());
		sprintf(jname,"%s.java",fname);
		ofstream robotout;
		robotout.open(jname);
		robocodeAlpha::Instance()->print_grammar_individual(robotout,getCreature()->getExpression(),getCreature()->getExpressionLength(),fname,gname);
		robotout.close();
	}	
	
}

void spatialRobocodeCreature::doCrossover(spatialRobocodeCreature &p2) {
	crPtr c1, c2;
	bool done = false;
	int count = 0;
	if (!isReallyValid()) {
		cout << "Aborting crossover for an invalid p1\n";
		return;
	}
	if (!p2.isReallyValid()) {
		cout << "Aborting crossover for an invalid p2\n";
		return;
	}
	do {
		//		cout << "Creature crossover ";
		cr_data::variableCrossover(critter,p2.getCreature(),c1,c2);
		if ( c1 && c1->isValid()) {
			//		  cout << " success on child1\n";
			isInvalid = false;
			if (p2.geneticAge > geneticAge) geneticAge = p2.geneticAge; 
			done = true;
			critter = c1;
		} else if ( c2 && c2->isValid() ) {
			done = true;
			isInvalid = false;
			critter = c2;
  		    if (p2.geneticAge > geneticAge) geneticAge = p2.geneticAge; 
			//			cout << " success on child2\n";
		} else {
			count++;
			//			cout << " failed: " << count << " ";
			if (count > 10) {
				cout << "Failed to create a valid child after 10 crossover attempts, abandoning it\n";
				done = true;
			}
		}
	} while (!done);
	//cout << "New critter crossed over " << getName() << endl;
}

void spatialRobocodeCreature::doMutation() {
	critter->smartSureMutate(10000);
	// what to do if its invalid etc hmm oh well nothing for just now;
}

void spatialRobocodeCreature::clearScore() {
	storedScore = 0;
	storedLayeredScore = 0;
	//	competitions = 0;
	failedTest = false;
}

void spatialRobocodeCreature::clearLayeredScore() {
	storedLayeredScore = 0;
}


void spatialRobocodeCreature::calcScore(spatialRobocodeParasite &p1,int loc) {
	
	// cout << "A spatialRobocodeCreature is getting its score. ";
	if (!stateFlag) {
		stateFlag = true;
		battleFront.clear();
		cout << "Cleared the battle list.\n";
		system("javac -classpath ../../libs/robocode.jar *.java");
		cout << "Compiled the bots.\n";
		
	}
	battleLinePtr nptr(new battleLine(this,&p1,loc));
	battleFront.push_back(nptr); // this will be autodeleted when battleFront cleared (the power of shared_ptr!)
	//cout << "Scheduled a battle betwen creature " << thisCreatureNumber << " and parasite " << p1.thisParasiteNumber << " loc given as " << loc << endl;
	
	// so spatial.h calls calcScore for every battle it wants that round
	// and our global battleFront will have at the end of this all the many battles scheduled.
	
}

void spatialRobocodeCreature::addToScore(double toAdd) {
	storedScore += toAdd;
	competitions++;
}

void spatialRobocodeCreature::addToLayeredScore(double toAdd) {
	storedLayeredScore += toAdd;
	competitions++;
}
double spatialRobocodeCreature::getScore() {
	if (stateFlag) {
		// So spatial.h told us to calc the scores, but all we have done is scheduled them (in battleFront)
		// here we are going to cause them all to be fought!
		// I am going to assume that all the robots have been saved to the correct directory
		// and compiled.
		
		// at this tender stage just list the battles.
		
		int fd,nread;
		sockfdToCritter.clear(); // none have been assigned
		int result;
		int nearestHundred = (battleFront.size()/100);
		int battlesWeNeed = battleFront.size();
		int battlesSoFar = 0;
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
					battleLinePtr battleToFarm = battleFront.front();
					battleFront.pop_front();
					sprintf(outputBuffer,"Gen%d.%s*,Gen%d.%s*\n",currentGen,battleToFarm->creature->getName(),currentGen,battleToFarm->parasite->getName());
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
					} else 	sockfdToCritter.insert(pair<int,battleLinePtr>(sdf,battleToFarm));
					
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
									battleLinePtr battleLineResult = found->second;
									if (battleLineResult->location == -1) {
										battleLineResult->creature->addToLayeredScore(creatureScore);
										battleLineResult->parasite->addToLayeredScore(parasiteScore);
										battlesSoFar++;
									} else {
										battleLineResult->creature->addToScore(creatureScore);
										battleLineResult->parasite->addToScore(parasiteScore);
										battlesSoFar++;
									}
									//cout << "Battle between " << battleLineResult->creature->getName() << " and parasite " << battleLineResult->parasite->getName() << " scored " << number << endl;
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
		stateFlag = false;
	}
	// both fall through and every other invocation once the scores have been calculated.
	return storedScore+storedLayeredScore;
}

void spatialRobocodeCreature::setCreature(boost::shared_ptr<spatialRobocodeCreature> toSetTo) { 
	geneticAge = toSetTo->geneticAge;
	critter = toSetTo->getCreature();
	thisCreatureNumber = toSetTo->thisCreatureNumber;
	isInvalid = toSetTo->isInvalid;
	//cout << "New critter set " << getName() << endl;
}

void spatialRobocodeCreature::makeCopyOfCreature(boost::shared_ptr<spatialRobocodeCreature> /*crPtr*/ tocopy) {
	cr_data *newCopy;
	newCopy = new cr_data(*(tocopy->getCreature())); // dereference the crPtr to refer to the cr_data
	thisCreatureNumber = creatureIdentifierCount++;
	critter.reset(newCopy);
	geneticAge = tocopy->geneticAge;
	isInvalid = false;
	//cout << "New critter copied " << getName() << endl;
}	

bool spatialRobocodeCreature::isReallyValid() {
	if (isInvalid) return false;
	if (critter->isValid()) {
		return true;
	}
	return false;
}

void spatialRobocodeCreature::loadCreature(ifstream &fin) {
	long dnaL;
	int *dna;
	fin >> dnaL;
	if (!fin) return; 
	if (dnaL<10 || dnaL>800000) {
		cout << "Encountered an unusual dna length of " << dnaL << "\n";
	}
	dna = new int[dnaL];
	for (int i=0;i<dnaL;i++) {
		fin >> dna[i];
    }
	int j;
	fin >> j;
	if (j!=-1) { 
		cout << "Potential error, creature \"stream\" not properly terminated\n";
	}
	critter.reset(new cr_data(dna,dnaL));
	isInvalid = false; // assume that its not isInvalid until proven otherwise.
	if (critter->isValid()) {
		cout << "A creature loaded tha t was valid\n";
		if (isReallyValid()) {
				cout << "... and it really is valid\n";
		} else { cout << "odd, its not really valid\n";
		}
	} else {
		cout << "Its not valid\n";
	}
	//	cout << "Loaded a creature!\n";
}


bool spatialRobocodeCreature::isBetterThan(double lhs,double rhs) {
	return lhs > rhs; // high score = good score
}

bool spatialRobocodeCreature::isBetterThan(double lhsScore,spatialRobocodeCreature *rhs,double rhsScore) {
	if (!rhs) {
		cout << "isBetterThan in spatialoneagentworldcreature, called with a null rhs.\n";
		exit(1);
	}
	if (lhsScore == rhsScore) {
		    crPtr lhsC = getCreature();
			if (!lhsC) return false;
			crPtr rhsC = rhs->getCreature();
			if (!rhsC) return true;
			long il = lhsC->getUsefulDna();
			long jl = rhsC->getUsefulDna();
			if ((il > cr_data::COMPARISONIMPACT) || (jl > cr_data::COMPARISONIMPACT)) return il<jl;
	} else return isBetterThan(lhsScore,rhsScore);
	return false;
}

void spatialRobocodeCreature::saveCreature(ofstream &fout)
{
	streamout(fout,critter);
}

bool spatialRobocodeCreature::terminate() {
	return false;
}

void spatialRobocodeCreature::display(const char *fname,int no,int para) {
	//networkOneAgent::Instance()->display(theNetwork,fname,para);
	cout << "Err you haven't written display spatialRobocodeCreature yet!\n";
}

/************************************************************************
 spatialRobocodeParasite and spatialRobocodeParasite(bool)
 *************************************************************************/
spatialRobocodeParasite::spatialRobocodeParasite() {
	geneticAge = 0;
	int maxTree;
	bool done = false;
	thisParasiteNumber = parasiteIdentifierCount++;
	do {
		maxTree = randomint(8)+2;
		parasite.reset(new cr_data);
		parasite->makeptc2(maxTree);
	    done = parasite->isValid();
	} while (!done);
	isInvalid = false;
	//cout << "New valid spatialRobocodeParasite creature created, with max size " << maxTree << "and id " << thisParasiteNumber << endl;
	char fname[200];
	char jname[200];
	char gname[200];
	sprintf(gname,"Gen%d",currentGen);

	//cout << "Thinking of saving " << getName() <<  endl;
	if (!(parasite->isValid())) {
		cout << "Weird, newly created parasite is not valid\n";
		exit(1);
	}
	sprintf(fname,"%s",getName());
	sprintf(jname,"%s.java",fname);
	ofstream robotout;
	robotout.open(jname);
	robocodeAlpha::Instance()->print_grammar_individual(robotout,getParasite()->getExpression(),getParasite()->getExpressionLength(),fname,gname);
	robotout.close();
	
}


const char *spatialRobocodeParasite::getName() {
	makeName(thisParasiteNumber,false);
	return javaPName;
}

spatialRobocodeParasite::spatialRobocodeParasite(bool make) {
	thisParasiteNumber = parasiteIdentifierCount++;
	geneticAge =0;
	if (make) {
		int maxTree;
		bool done = false;
		geneticAge = 0;
		do {
			maxTree = randomint(8)+2;
			parasite.reset(new cr_data);
			parasite->makeptc2(maxTree);
			done = parasite->isValid();
		} while (!done);
		isInvalid = false;
		//cout << "New valid spatialRobocodeParasite(true) creature created, with max size " << maxTree << "and id " << thisParasiteNumber << endl;
		char fname[200];
		char jname[200];
		char gname[200];
		sprintf(gname,"Gen%d",currentGen);

		//cout << "Thinking of saving " << getName() <<  endl;
		if (!(parasite->isValid())) {
			cout << "Weird, newly created parasite is not valid\n";
			exit(1);
		}
		sprintf(fname,"%s",getName());
		sprintf(jname,"%s.java",fname);
		ofstream robotout;
		robotout.open(jname);
		robocodeAlpha::Instance()->print_grammar_individual(robotout,getParasite()->getExpression(),getParasite()->getExpressionLength(),fname,gname);
		robotout.close();
	}
}

spatialRobocodeParasite::~spatialRobocodeParasite() {  
	
}

void spatialRobocodeParasite::doCrossover(spatialRobocodeParasite &p2) {
	crPtr c1, c2;
	bool done = false;
	int count = 0;
	if (!isReallyValid()) {
		cout << "Aborting crossover for an invalid p1\n";
		return;
	}
	if (!p2.isReallyValid()) {
		cout << "Aborting crossover for an invalid p2\n";
		return;
	}
	do {
		//		cout << "Creature crossover ";
		cr_data::variableCrossover(parasite,p2.getParasite(),c1,c2);
		if ( c1 && c1->isValid()) {
			//		  cout << " success on child1\n";
			isInvalid = false;
			if (p2.geneticAge > geneticAge) geneticAge = p2.geneticAge; 
			done = true;
			parasite = c1;
		} else if ( c2 && c2->isValid() ) {
			done = true;
			isInvalid = false;
			parasite = c2;
  		    if (p2.geneticAge > geneticAge) geneticAge = p2.geneticAge; 
			//			cout << " success on child2\n";
		} else {
			count++;
			//			cout << " failed: " << count << " ";
			if (count > 10) {
				cout << "Failed to create a valid child after 10 crossover attempts, abandoning it\n";
				done = true;
			}
		}
	} while (!done);
	return; 
}

void spatialRobocodeParasite::doMutation() {
	parasite->smartSureMutate(10000);
}

bool spatialRobocodeParasite::isReallyValid() {
	if (isInvalid) return false;
	if (parasite->isValid()) {
		return true;
	}
	return false;
}


void spatialRobocodeParasite::clearScore() {
	storedScore=0;
	storedLayeredScore=0;
	competitions=0;
}

void spatialRobocodeParasite::clearLayeredScore() {
	storedLayeredScore=0;
}



void spatialRobocodeParasite::calcScore(spatialRobocodeCreature &c1,int loc) {
	//    cout << "An animal creatures is getting its score. ";
	return;
	// parasite dont calc scores
}


void spatialRobocodeParasite::addToScore(double toAdd) {
	//competitions++;
	//storedScore += toAdd;
	if (storedScore ==0) storedScore=toAdd;
	else if (toAdd < storedScore) storedScore=toAdd;
	return;
}

void spatialRobocodeParasite::addToLayeredScore(double toAdd) {
	//competitions++;
	//storedScore += toAdd;
	if (storedLayeredScore == 0) storedLayeredScore = toAdd;
	else if (toAdd < storedLayeredScore) {
		storedLayeredScore=toAdd;
	}
}

// So the Lower the better, we need to return the "worst"
// score i.e. the highest.

double spatialRobocodeParasite::getScore() {
	if ((storedLayeredScore == 0) || (storedScore < storedLayeredScore)) {
		//cout << "Parasite " << getName() << "returning score " << storedScore << "rather than " << storedLayeredScore << endl;
		return storedScore;
	} else {
			//cout << "Parasite " << getName() << "returning score " << storedLayeredScore << " rather than " << storedScore << endl;
			return storedLayeredScore;
		}
}



void spatialRobocodeParasite::setParasite(boost::shared_ptr<spatialRobocodeParasite> toSetTo) {
	geneticAge = toSetTo->geneticAge;
	parasite = toSetTo->getParasite();
	thisParasiteNumber = toSetTo->thisParasiteNumber;
	isInvalid = toSetTo->isInvalid;
	
}
void spatialRobocodeParasite::makeCopyOfParasite(boost::shared_ptr<spatialRobocodeParasite> /*crPtr*/ tocopy) {
	cr_data *newCopy;
	newCopy = new cr_data(*(tocopy->getParasite())); // dereference the crPtr to refer to the cr_data
	thisParasiteNumber = parasiteIdentifierCount++;
	parasite.reset(newCopy);
	geneticAge = tocopy->geneticAge;
	isInvalid = false;
}

bool spatialRobocodeParasite::terminate() {
	return false;
}

bool spatialRobocodeParasite::isBetterThan(double lhs,double rhs) {
	return lhs > rhs;
}

void spatialRobocodeParasite::saveParasite(ofstream &fout)
{
	streamout(fout,parasite);
}


void spatialRobocodeParasite::loadParasite(ifstream &fin) {
	long dnaL;
	int *dna;
	fin >> dnaL;
	if (!fin) return; 
	if (dnaL<10 || dnaL>800000) {
		cout << "Encountered an unusual dna length of " << dnaL << "\n";
	}
	dna = new int[dnaL];
	for (int i=0;i<dnaL;i++) {
		fin >> dna[i];
    }
	int j;
	fin >> j;
	if (j!=-1) { 
		cout << "Potential error, creature \"stream\" not properly terminated\n";
	}
	parasite.reset(new cr_data(dna,dnaL));
	isInvalid = false; // assume that its not isInvalid until proven otherwise.
	if (parasite->isValid()) {
		cout << "A parasite was loaded that was valid\n";
		if (isReallyValid()) {
			cout << "... and it really is valid\n";
		} else { cout << "odd, its not really valid\n";
		}
	} else {
		cout << "Its not valid\n";
	}
	//	cout << "Loaded a parasite!\n";
	
}

