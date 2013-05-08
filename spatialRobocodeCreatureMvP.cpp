/*
 *  spatialRobocodeCreatureMvP.cpp
 *  oneAgentWorld
 *
 *  Created by Robin on 3/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "spatialRobocodeCreatureMvP.h"
#include <fstream>
#include "robocodeBeta.h"
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
 spatialRobocodeCreatureMvP(bool)
 *************************************************************************/

long spatialRobocodeCreatureMvP::creatureIdentifierCount = 0;
long spatialRobocodeMvPParasite::parasiteIdentifierCount = 0;
bool spatialRobocodeCreatureMvP::stateFlag = false;

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

extern ofstream DataOutputFile;

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

const char *spatialRobocodeCreatureMvP::getName() {
	makeName(thisCreatureNumber,true);
	return javaCName;
}


// so originally I just had the best, the ones commented out, trying a layered difficulty approach this time.
// might just use my old highly evolved creatures though. (well 230 odd evolved gens)
// NUMBEROFPARASITES defined in header file
const char *spatialRobocodeMvPParasite::parasiteName[NUMBEROFPARASITES] = {
	/*"jk.mega.DrussGT",
	"davidalves.Phoenix",
	"NDH.GuessFactor",
	"kc.serpent.WaveSerpent",
	"pez.rumble.CassiusClay",
	"voidious.Dookious",
	"oog.Caligula",
	"zyx.mega.YersiniaPestis" */
	// for layer0 - 8 robots
	"sample.Crazy",
	"sample.RamFire",
	"sample.SpinBot",
	"sample.Tracker",
	"sample.TrackFire",
	"sample.VelociRobot",
	"sample.Walls",
	"NDH.GuessFactor*",

	// for layer1 - a further
	"dummy.micro.Sparrow 2.5*",
	"gg.Peryton1_1*",
	"squigsoft.SquigBot2_8*",
	
	// for layer 2 a futher 
	//"oog.Caligula*",
	"davidalves.net.DuelistMini*",
	"apv.NanoLauLectrikTheCannibal*",
	
	"abc.Tron 2.02*",
	"apv.Aspid 1.6.5*",
	"cx.mini.Cigaret*"
	
	
//	"zyx.mega.YersiniaPestis*",
	
	
};


scoreStore storedScores[NUMBEROFPARASITES][SCORESTOSTORE];
parasiteData storedParasiteData[NUMBEROFPARASITES];


// Need to test ptc2 with this grammar.
spatialRobocodeCreatureMvP::spatialRobocodeCreatureMvP() {
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
	//cout << "New valid spatialRobocodeCreatureMvP creature created, with max size " << maxTree << "and id " << thisCreatureNumber << endl;
	
	
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
	robocodeBeta::Instance()->print_grammar_individual(robotout,getCreature()->getExpression(),getCreature()->getExpressionLength(),fname,gname);
	robotout.close();
}

spatialRobocodeCreatureMvP::spatialRobocodeCreatureMvP(bool make) {
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
		//cout << "New valid spatialRobocodeCreatureMvP(true) creature created, with max size " << maxTree << "and id " << thisCreatureNumber << endl;
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
		robocodeBeta::Instance()->print_grammar_individual(robotout,getCreature()->getExpression(),getCreature()->getExpressionLength(),fname,gname);
		robotout.close();
	}	
	
}

void spatialRobocodeCreatureMvP::doCrossover(spatialRobocodeCreatureMvP &p2) {
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

void spatialRobocodeCreatureMvP::doMutation() {
	critter->smartSureMutate(10000);
	// what to do if its invalid etc hmm oh well nothing for just now;
}

void spatialRobocodeCreatureMvP::clearScore() {
	storedScore = 0;
	storedLayeredScore = 0;
	//	competitions = 0;
	failedTest = false;
}

void spatialRobocodeCreatureMvP::clearLayeredScore() {
	storedLayeredScore = 0;
}


void spatialRobocodeCreatureMvP::calcScore(spatialRobocodeMvPParasite &p1,int loc) {
	
	// cout << "A spatialRobocodeCreatureMvP is getting its score. ";
	if (!stateFlag) {
		stateFlag = true;
		battleFront.clear();
		cout << "Cleared the battle list.\n";
		system("javac -J-Xmx512m -classpath ../../libs/robocode.jar *.java");
		cout << "Compiled the bots.\n";
		
	}
	battleLinePtr nptr(new battleLine(this,&p1,loc));
	battleFront.push_back(nptr); // this will be autodeleted when battleFront cleared (the power of shared_ptr!)
	//cout << "Scheduled a battle betwen creature " << thisCreatureNumber << " and parasite " << p1.thisParasiteNumber << " loc given as " << loc << endl;
	
	// so spatial.h calls calcScore for every battle it wants that round
	// and our global battleFront will have at the end of this all the many battles scheduled.
	
}

void spatialRobocodeCreatureMvP::addToScore(double toAdd) {
	storedScore += toAdd;
	competitions++;
}

void spatialRobocodeCreatureMvP::addToLayeredScore(double toAdd) {
	storedLayeredScore += toAdd;
	competitions++;
}
double spatialRobocodeCreatureMvP::getScore() {
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
		int rematchesSoFar = 0;
		int layerBattlesSoFar = 0;
		cout << "About to schedule " << battlesWeNeed << " matches.\n";
		double totalCritterScore = 0;
		double totalParasiteScore = 0;
		for (int i=0;i<NUMBEROFPARASITES;i++) storedParasiteData[i].clear();
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
					sprintf(outputBuffer,"Gen%d.%s*,%s\n",currentGen,battleToFarm->creature->getName(),battleToFarm->parasite->getName());
					//cout << "Sending instructions as follows: " << outputBuffer;
					//cout << "It was " << (battleToFarm->rematch?"a":"not a") << " rematch " << endl;
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
				printf("Error in spatialRobocodeMvPCreater, select returned less than 1 whilst I was trying to do all my battle stuff.");
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
									// make the score a percentage, as otherwise some parasites
									// get surprisingly high scores for surprisingly not so good behavoiur
									// well what happens is they need to do a lot more damage (as the critter regens by hitting them)
									// so they get a better bullet score
									int totalScore = parasiteScore + creatureScore;
									double parasitePercent = (parasiteScore*100)/totalScore;
									double creaturePercent = (creatureScore*100)/totalScore;
									battleLinePtr battleLineResult = found->second;
									// code to update the best score found so far information
									int parasite = battleLineResult->parasite->getParasite();
									// sanity check
									if (parasite < 0 || parasite > (NUMBEROFPARASITES-1)) {
										cout << "Error get parasite returned " << parasite << endl;
										exit(1);
									}
									// ok safe to use it to index array
									if (battleLineResult->location != -1) storedParasiteData[parasite].add(creatureScore,parasiteScore,battleLineResult->creature->getName()); // update averaging data
									
									// lets keep tabs on the best so far. The problem with this is sometimes creatures just get lucky
									// so if we do get a score, we reschedule for a rematch - so they have to get lucky twice!
									bool notBest = true;
									if (creaturePercent > (storedScores[parasite][0]).percentage) {
										notBest = false;
										if (battleLineResult->rematch) { // it was  a rematch
											cout << "We have a rematch which is a new best result, the parasite it won against was " << parasite << " the creature was " << battleLineResult->creature->getName() << endl;
											cout << "The current percentage of the match is " << creaturePercent << " and the stored percentage was " << (storedScores[parasite][0]).percentage << endl;
											// we have a winner!
											for (int i = SCORESTOSTORE -1;i > 0;i--) {
												storedScores[parasite][i]=storedScores[parasite][i-1];
											}
											storedScores[parasite][0].setScore(creatureScore,parasiteScore,creaturePercent,currentGen,battleLineResult->creature->getName(),battleLineResult->creature->geneticAge);
										} else {
											// schedule rematch
											cout << "Scheduling a rematch for " << battleLineResult->creature->getName() << " v " << spatialRobocodeMvPParasite::parasiteName[parasite] << "(" << parasite <<")" << endl;
											cout << "The current percentage of the match beign rescheduled is " << creaturePercent << " and the stored percentage was " << (storedScores[parasite][0]).percentage << endl;
											
											battleLineResult->rematch = true; // note that its a rematch
											battleFront.push_front(battleLineResult); // and push it back into the battle.
										}
									}
									if (!battleLineResult->rematch) { //dont score the rematch - as it will have the original score.
										// so we have stored the best creature against that parasite (so far)
										if (battleLineResult->location == -1) {
											// Using percent now, instead of scores because, creatures that allow themselves
											// to be hit (and therefore give the parasite more energy
											// can achieve a higher score (but lower percentage score) as the parasite takes
											// more bullets to kill it and therefore a higher "raw" bullet damage score is achieved.
											battleLineResult->creature->addToLayeredScore(creaturePercent);
											battleLineResult->parasite->addToLayeredScore((parasiteScore*100)/totalScore);
											battlesSoFar++;
										} else {
											battleLineResult->creature->addToScore(creaturePercent);
											battleLineResult->parasite->addToScore((parasiteScore*100)/totalScore);
											totalCritterScore +=creatureScore;
											totalParasiteScore += ((parasiteScore*100)/totalScore);
											layerBattlesSoFar++;
											battlesSoFar++;
										}
									} else {
										rematchesSoFar++;
										if (notBest) {
											cout << "We have a rematch which did not have the best result, parasite " << parasite << " against " << battleLineResult->creature->getName() << endl;
											cout << "The percentage of the re-match was " << creaturePercent << " and the stored percentage was " << (storedScores[parasite][0]).percentage << endl;
										}	
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
		if (layerBattlesSoFar > 0) {
			cout << "We had " << layerBattlesSoFar << " battles within the topmost layer " << endl;
			cout << "The average critterScore over all of these battles was " << totalCritterScore/layerBattlesSoFar << endl;
			cout << "And the average parasiteScore over all of the battles was " << totalParasiteScore/layerBattlesSoFar << endl;
			cout << "There were an additional " << rematchesSoFar << " scheduled.\n";
			cout << "For each of the parasites we gathered the following data:\n";
			if (DataOutputFile) DataOutputFile << currentGen;
			for (int i=0;i<NUMBEROFPARASITES;i++) {
				cout << "***************************************\nParasite: " << spatialRobocodeMvPParasite::parasiteName[i] << endl;
				if (storedParasiteData[i].matchesSoFar == 0) cout << "No matches against that parasite this round.\n"; else {
				cout << "Average Score of competitors " << storedParasiteData[i].scoreSoFar/storedParasiteData[i].matchesSoFar << endl;
				cout << "Average score of parasite    " << storedParasiteData[i].pScoreSoFar/storedParasiteData[i].matchesSoFar << endl;
				cout << "Average percentage score by competitors " << storedParasiteData[i].cPercentSoFar/storedParasiteData[i].matchesSoFar << endl;
				cout << "Average percentage score by parasite " << storedParasiteData[i].pPercentSoFar/storedParasiteData[i].matchesSoFar << endl;
				cout << "It participated in " << storedParasiteData[i].matchesSoFar << endl;
				cout << "The best score achieved against it (this round) was " << storedParasiteData[i].highestSinceClear << endl;
					cout << "By " << storedParasiteData[i].contender << endl;
				cout << "The best percentage achieved against it (this round) was " << storedParasiteData[i].highestPSinceClear << endl;
				}
				if (DataOutputFile) {
					if (storedParasiteData[i].matchesSoFar == 0) {
					    DataOutputFile << ", -, -, 0, -, -, -, -, -";
					} else {
					DataOutputFile  << ", " << storedParasiteData[i].scoreSoFar/storedParasiteData[i].matchesSoFar 
									<< ", " << storedParasiteData[i].pScoreSoFar/storedParasiteData[i].matchesSoFar
									<< ", " << storedParasiteData[i].matchesSoFar
									<< ", " << storedParasiteData[i].cPercentSoFar/storedParasiteData[i].matchesSoFar 
									<< ", " << storedParasiteData[i].pPercentSoFar/storedParasiteData[i].matchesSoFar
									<< ", " << storedParasiteData[i].highestSinceClear
									<< ", " << storedParasiteData[i].highestPSinceClear
									<< ", " << storedParasiteData[i].contender;
					}
				}
			}
			if (DataOutputFile) DataOutputFile << endl;
		}
		stateFlag = false;
	}
	// both fall through and every other invocation once the scores have been calculated.
	return storedScore+storedLayeredScore;
}

void spatialRobocodeCreatureMvP::setCreature(boost::shared_ptr<spatialRobocodeCreatureMvP> toSetTo) { 
	geneticAge = toSetTo->geneticAge;
	critter = toSetTo->getCreature();
	thisCreatureNumber = toSetTo->thisCreatureNumber;
	isInvalid = toSetTo->isInvalid;
	//cout << "New critter set " << getName() << endl;
}

void spatialRobocodeCreatureMvP::makeCopyOfCreature(boost::shared_ptr<spatialRobocodeCreatureMvP> /*crPtr*/ tocopy) {
	cr_data *newCopy;
	newCopy = new cr_data(*(tocopy->getCreature())); // dereference the crPtr to refer to the cr_data
	thisCreatureNumber = creatureIdentifierCount++;
	critter.reset(newCopy);
	geneticAge = tocopy->geneticAge;
	isInvalid = false;
	//cout << "New critter copied " << getName() << endl;
}	

bool spatialRobocodeCreatureMvP::isReallyValid() {
	if (isInvalid) return false;
	if (critter->isValid()) {
		return true;
	}
	return false;
}

void spatialRobocodeCreatureMvP::loadCreature(ifstream &fin) {
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
		cout << "A creature loaded that was valid\n";
		if (isReallyValid()) {
				cout << "... and it really is valid\n";
		} else { cout << "odd, its not really valid\n";
		}
	} else {
		cout << "Its not valid\n";
	}
	//	cout << "Loaded a creature!\n";
}


bool spatialRobocodeCreatureMvP::isBetterThan(double lhs,double rhs) {
	return lhs > rhs; // high score = good score
}

bool spatialRobocodeCreatureMvP::isBetterThan(double lhsScore,spatialRobocodeCreatureMvP *rhs,double rhsScore) {
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

void spatialRobocodeCreatureMvP::saveCreature(ofstream &fout)
{
	streamout(fout,critter);
}

bool spatialRobocodeCreatureMvP::terminate() {
	return false;
}

void spatialRobocodeCreatureMvP::display(const char *fname,int no,int para) {
	//networkOneAgent::Instance()->display(theNetwork,fname,para);
	cout << "Err you haven't written display spatialRobocodeCreatureMvP yet!\n";
}

/************************************************************************
 spatialRobocodeMvPParasite and spatialRobocodeMvPParasite(bool)
 *************************************************************************/
spatialRobocodeMvPParasite::spatialRobocodeMvPParasite() {
	geneticAge = 0;
	int maxTree;
	bool done = false;
	thisParasiteNumber = parasiteIdentifierCount++;
	parasite= randomint(NUMBEROFPARASITES-8);
}


const char *spatialRobocodeMvPParasite::getName() {
	if (parasite < 0 || parasite >= NUMBEROFPARASITES) {
		cout << "MvPParasite asked for name, when parasite was " << parasite << " (should be from 0 to 8)\n";
		exit(1);
	} 
	return parasiteName[parasite];
}

spatialRobocodeMvPParasite::spatialRobocodeMvPParasite(bool make) {
	thisParasiteNumber = parasiteIdentifierCount++;
	geneticAge =0;
	parasite = randomint(NUMBEROFPARASITES-8);
}

spatialRobocodeMvPParasite::~spatialRobocodeMvPParasite() {  
	
}

void spatialRobocodeMvPParasite::doCrossover(spatialRobocodeMvPParasite &p2) {
	return; 
}

void spatialRobocodeMvPParasite::doMutation(int layer) {
	int maxParasite;
	switch (layer) {
		case 0: maxParasite = NUMBEROFPARASITES -8;
			break;
		case 1: maxParasite = NUMBEROFPARASITES -5;
			break;
		case 2: 
			maxParasite = NUMBEROFPARASITES - 3;
			break;
		default:
			maxParasite = NUMBEROFPARASITES;
			break;
	}
	parasite=randomint(maxParasite);
	// changed it from below, any parasite can mutate into any other one (valid for layer)
	/*if (randomint(2)) {
		parasite++;
		if (parasite >= maxParasite) parasite =0;
	} else {
		parasite--;
		if (parasite < 0) parasite = maxParasite-1;
	}*/
}

bool spatialRobocodeMvPParasite::isReallyValid() {
	return true;
}


void spatialRobocodeMvPParasite::clearScore() {
	storedScore=0;
	storedLayeredScore=0;
	competitions=0;
}

void spatialRobocodeMvPParasite::clearLayeredScore() {
	storedLayeredScore=0;
}



void spatialRobocodeMvPParasite::calcScore(spatialRobocodeCreatureMvP &c1,int loc) {
	//    cout << "An animal creatures is getting its score. ";
	return;
	// parasite dont calc scores
}


void spatialRobocodeMvPParasite::addToScore(double toAdd) {
	//competitions++;
	//storedScore += toAdd;
	if (storedScore ==0) storedScore=toAdd;
	else if (toAdd < storedScore) storedScore=toAdd;
	return;
}

void spatialRobocodeMvPParasite::addToLayeredScore(double toAdd) {
	//competitions++;
	//storedScore += toAdd;
	if (storedLayeredScore == 0) storedLayeredScore = toAdd;
	else if (toAdd < storedLayeredScore) {
		storedLayeredScore=toAdd;
	}
}

// So the Lower the better, we need to return the "worst"
// score i.e. the highest.

double spatialRobocodeMvPParasite::getScore() {
	if ((storedLayeredScore == 0) || (storedScore < storedLayeredScore)) {
		//cout << "Parasite " << getName() << "returning score " << storedScore << "rather than " << storedLayeredScore << endl;
		return storedScore;
	} else {
			//cout << "Parasite " << getName() << "returning score " << storedLayeredScore << " rather than " << storedScore << endl;
			return storedLayeredScore;
		}
}



void spatialRobocodeMvPParasite::setParasite(boost::shared_ptr<spatialRobocodeMvPParasite> toSetTo) {
	geneticAge = toSetTo->geneticAge;
	parasite = toSetTo->parasite;
	thisParasiteNumber = toSetTo->thisParasiteNumber;
	isInvalid = toSetTo->isInvalid;
	
}
void spatialRobocodeMvPParasite::makeCopyOfParasite(boost::shared_ptr<spatialRobocodeMvPParasite> tocopy) {
	thisParasiteNumber = parasiteIdentifierCount++;
	parasite = tocopy->parasite;
	geneticAge = tocopy->geneticAge;
	isInvalid = false;
}

bool spatialRobocodeMvPParasite::terminate() {
	return false;
}

bool spatialRobocodeMvPParasite::isBetterThan(double lhs,double rhs) {
	return lhs > rhs;
}

void spatialRobocodeMvPParasite::saveParasite(ofstream &fout)
{
	
}


void spatialRobocodeMvPParasite::loadParasite(ifstream &fin,int layer) {
	int maxParasite;
	switch (layer) {
		case 0: maxParasite = NUMBEROFPARASITES -8;
			break;
		case 1: maxParasite = NUMBEROFPARASITES -5;
			break;
		case 2: 
			maxParasite = NUMBEROFPARASITES - 3;
			break;
		default:
			maxParasite = NUMBEROFPARASITES;
			break;
	}
	parasite=randomint(maxParasite);
}	
	

