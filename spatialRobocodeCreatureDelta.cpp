/*
 *  spatialRobocodeCreatureDelta.cpp
 *  
 *
 *  Created by Robin on 16 May 2013.
 *  Copyright 2013 Robin Harper
 *
 */

#include "spatialRobocodeCreatureDelta.h"
#include <fstream>
#include "robocodeDelta.h"
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


/* The idea behind this class it that it encapsulates the creatures/parasites that can reside in the locations of a spatial world.
   The algorithm in the spatial world pits creatures against parasites by telling the creature (and then the parasites) to calcScore
   Here we just use the call to the creature and push the mandated battle into a battlelist. At the end of the "calcScore" round all
   the matches wanted by spatial.h will be in the list.
 
   Then when spatial.h tries to retrieve the score of the first creature (aside: spatial.h uses the scores to decide which creatures from the neighbourhood will
   occupy the location for the next genration) then the first time this happens (see getScore), we run all the battles by handing of to 
   client battlerunners (java programs in the robocode directory that are listing to the port set up in initialiseServerStuff(int port)
   default  = 9000, and waiting for them to come back. In theory we can have lots of clients - as long as they have access to the robots.
   We could stream the robots out as well if we don't have shared filestorage, but currently I don't do that.
 
   Anyway the clients come back with a score for the two competitors and its stored appropriately (addToScore if its a competitor in the same "layer")
   (addToLayeredScore if its a competitor in a different layer).
 
   Once all the battles have been run and the scores stored, getScore returns the creatures score. Subsequent calls to getScore just return
   the creatures stored scores (all the battles having been run for this generation).
 
   There are a number of methods that spatial.h requires these classes to have, its kind of documented in spatial.h - I should update.
   Struggling with some of the concepts needed to have a virtual base creature class which can then be substantiated. If I can overcome them
   then thats probably the way to go.
 
   Finally just a reminder, here a low score is bad, high score is good. A creature score is the sum of all the scores it receives in a battle
   a parasite score, on the otherhand, is the worst score it achieves (I believe this results in different behaviour - one aggressive the other defensive).
   the addToScore and addToLayered score and the getScore all achieve this.
 
*/

/* PARTICIPATES:
      I have added some functionality that allows "human" robots to be incorporated into the nodes, using a participates flag. Just don't set 
      any of the nodes to particpates = false if you don't want to use it. We have access to the layer and the node which can be used to set the 
      robot that lives there. The hook is the "inLayerWithLocation" method which is called when a node is set up. You can alter the participates flag there
      and limit the node to "human" robots.
 */

extern vector<int> layerLives;

void streamout(ostream &pout,crPtr &item,int age) {
	int len;
	pout << age << " ";
	pout << (len=item->getDnaLength()) << ' ';
	for (int i=0;i<len;i++) {
		pout << item->getDna()[i] << ' ';
	}
	pout << "-1\n";
}

/************************************************************************
 spatialRobocodeCreatureDelta(bool)
 *************************************************************************/

long spatialRobocodeCreatureDelta::CreatureDeltaIdentifierCount = 0;
long spatialRobocodeParasiteDelta::ParasiteDeltaIdentifierCount = 0;
bool spatialRobocodeCreatureDelta::stateFlag = false;

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

void makeName(long id,bool CreatureDelta) { 
	if (CreatureDelta) sprintf(javaCName,"Critter%ld",id);
	else sprintf(javaPName,"ParasiteDelta%ld",id);
}

// These are the different human written (well gpBot is evolved I think) robots
// that we will add to some of the locations in the top two layers.
// Note they are initially chosen randomly, can mutate into any other
// but otherwise - as between themselves - they will select depending on
// how well they do i.e. what there score is against the parasites (in this case)
// in the 18 node neighbourhood (9 this level and 9 below).
// There is no real rhyme or reason as to the choices made here, the top level
// ones are the top 20 on the rumble at the time I grabbed them
// the others were all just above the rank of the initial SCALPBOT
// after about 160 gens of evolution just against co-evolving robots.

const char *spatialRobocodeCreatureDelta::getName() {
    if (!participates) {
        if (topLevel) {
            switch(humanRobot) {
                case 0: return "jk.sheldor.nano.Yatagan";
                case 1: return "mld.LittleBlackBook";
                case 2: return "mld.Moebius";
                case 3: return "mld.jdc.nano.LittleBlackBook";
                case 4: return "nat.nano.OcnirpSNG";
                case 5: return "nz.jdc.nano.AralR";
                case 6: return "nz.jdc.nano.AralT";
                case 7: return "nz.jdc.nano.NeophytePRAL";
                case 8: return "nz.jdc.nano.NeophytePattern";
                case 9: return "nz.jdc.nano.NeophyteSRAL";
                case 10: return "nz.jdc.nano.PatternAdept";
                case 11: return "nz.jdc.nano.PralDeGuerre";
                case 12: return "oog.nano.Caligula";
                case 13: return "oog.nano.Fuatisha";
                case 14: return "robar.nano.BlackWidow";
                case 15: return "robar.nano.Pugio";
                case 16: return "sheldor.nano.Sabreur";
                case 17: return "simonton.nano.WeekendObsession_S";
                case 18: return "fromHell.C22H30N2O2S";
                default:    return "wompi.Kowari";
            }
        } else {
            switch(humanRobot) {
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
                case 15: /*return "bots.UberBot";*/
                case 16: return "dggp.haiku.gpBot_0";
                case 17: return "zyx.nano.RedBull";
                case 18: return "zyx.nano.Ant";
                case 19: return "stelo.MirrorNano";
                default: return "supersample.SuperWalls";
            }
        }
        
    }
	else {
        makeName(thisCreatureDeltaNumber,true);
        return javaCName;
    }
}


spatialRobocodeCreatureDelta::spatialRobocodeCreatureDelta() {
    participates = true;
	geneticAge = 0;
	int maxTree;
	bool done = false;
	thisCreatureDeltaNumber = CreatureDeltaIdentifierCount++;
	do {
		maxTree = randomint(8)+2;
		critter.reset(new cr_data);
        // makeptc2 is probabilistic tree creation method 2 - afaik the best way to create intial robots in ge
		critter->makeptc2(maxTree);
	    done = critter->isValid();
	} while (!done);
	isInvalid = false;
	//cout << "New valid spatialRobocodeCreatureDelta CreatureDelta created, with max size " << maxTree << "and id " << thisCreatureDeltaNumber << endl;
	
	
	char fname[200];
	char jname[200];
	char gname[200];
	sprintf(gname,"Gen%d",currentGen);
	//cout << "Thinking of saving " << getName() <<  endl;
	if (!(getCreature()->isValid())) {
		cout << "Weird, newly created CreatureDelta is not valid\n";
		exit(1);
	}
	sprintf(fname,"%s",getName());
	sprintf(jname,"%s.java",fname);
	ofstream robotout;
	robotout.open(jname);
	robocodeDelta::Instance()->print_grammar_individual(robotout,getCreature()->getExpression(),getCreature()->getExpressionLength(),fname,gname);
	robotout.close();
}

spatialRobocodeCreatureDelta::spatialRobocodeCreatureDelta(bool make) {
    participates= true;
	thisCreatureDeltaNumber = CreatureDeltaIdentifierCount++;
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
		//cout << "New valid spatialRobocodeCreatureDelta(true) CreatureDelta created, with max size " << maxTree << "and id " << thisCreatureDeltaNumber << endl;
		char fname[200];
		char jname[200];
		char gname[200];
		sprintf(gname,"Gen%d",currentGen);

		//cout << "Thinking of saving " << getName() << endl;
		if (!(getCreature()->isValid())) {
			cout << "Weird, newly created CreatureDelta is not valid\n";
			exit(1);
		}
		sprintf(fname,"%s",getName());
		sprintf(jname,"%s.java",fname);
		ofstream robotout;
		robotout.open(jname);
		robocodeDelta::Instance()->print_grammar_individual(robotout,getCreature()->getExpression(),getCreature()->getExpressionLength(),fname,gname);
		robotout.close();
	}	
	
}

void spatialRobocodeCreatureDelta::doCrossover(spatialRobocodeCreatureDelta &p2) {
    if (!participates) return;
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
		//		cout << "CreatureDelta crossover ";
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

void spatialRobocodeCreatureDelta::doMutation() {
    if (!participates) {
        humanRobot = randomint(MAXROBOTNUMBER);
    } else {
        critter->smartSureMutate(10000);
	}
    // what to do if its invalid etc hmm oh well nothing for just now;
}

void spatialRobocodeCreatureDelta::clearScore() {
	storedScore = 0;
	storedLayeredScore = 0;
	//	competitions = 0;
	failedTest = false;
}

void spatialRobocodeCreatureDelta::clearLayeredScore() {
	storedLayeredScore = 0;
}


void spatialRobocodeCreatureDelta::calcScore(spatialRobocodeParasiteDelta &p1,int loc) {
	
	// cout << "A spatialRobocodeCreatureDelta is getting its score. ";
	if (!stateFlag) {
		stateFlag = true;
		battleFront.clear();
		cout << "Cleared the battle list.\n";
		system("javac -J-Xmx1024m -classpath ../../libs/robocode.jar *.java");
		cout << "Compiled the bots.\n";
		
	}
	battleLinePtr nptr(new battleLine(this,&p1,loc));
	battleFront.push_back(nptr); // this will be autodeleted when battleFront cleared (the power of shared_ptr!)
	//cout << "Scheduled a battle betwen CreatureDelta " << thisCreatureDeltaNumber << " and ParasiteDelta " << p1.thisParasiteDeltaNumber << " loc given as " << loc << endl;
	
	// so spatial.h calls calcScore for every battle it wants that round
	// and our global battleFront will have at the end of this all the many battles scheduled.
	
}

void spatialRobocodeCreatureDelta::addToScore(double toAdd) {
	storedScore += toAdd;
	competitions++;
}

// Terrible quick hack, that will have to be rethought if we thread this - buffers collide.
char outputB[200];
char outputC[200];

const char *getGenName(int currentGen,spatialRobocodeCreatureDelta *creatureDelta,int loc,int loc2) {
    if (creatureDelta->isParticipating()){
    sprintf(outputB,"Gen%d.%s",currentGen,creatureDelta->getName());
    } else {
        sprintf(outputB,"%s",creatureDelta->getName());
    }
    return outputB;
}

const char *getGenName(int currentGen,spatialRobocodeParasiteDelta *parasiteDelta,int loc2) {
    if (parasiteDelta->isParticipating()){
        sprintf(outputC,"Gen%d.%s",currentGen,parasiteDelta->getName());
    } else {
        sprintf(outputC,"%s",parasiteDelta->getName());
    }
    return outputC;
}

void spatialRobocodeCreatureDelta::addToLayeredScore(double toAdd) {
	storedLayeredScore += toAdd;
	competitions++;
}

// Has all the code to farm battles off to and get results back from the java robocode clients.
double spatialRobocodeCreatureDelta::getScore() {
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
                    // quick hack
                    sprintf(outputBuffer,"%s*,%s*\n",getGenName(currentGen,battleToFarm->CreatureDelta,myLocation,battleToFarm->location),getGenName(currentGen,battleToFarm->ParasiteDelta,battleToFarm->location));
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
                                        cout << "With competitors " << battleLineResult->CreatureDelta->getName() << " and " << battleLineResult->ParasiteDelta->getName() << endl;
										cout << "It should not be possible to get a zero score.\n";
										cout << "The input buffer was " << inputBuffer << endl;
									}
									double totalScore = CreatureDeltaScore + ParasiteDeltaScore;
									if (battleLineResult->location == -1) {
										battleLineResult->CreatureDelta->addToLayeredScore((CreatureDeltaScore*100)/totalScore);
										battleLineResult->ParasiteDelta->addToLayeredScore((ParasiteDeltaScore*100)/totalScore);
										battlesSoFar++;
									} else {
										battleLineResult->CreatureDelta->addToScore((CreatureDeltaScore*100)/totalScore);
										battleLineResult->ParasiteDelta->addToScore((ParasiteDeltaScore*100)/totalScore);
										battlesSoFar++;
									}
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
		stateFlag = false;
	}
	// both fall through and every other invocation once the scores have been calculated.
    /* Temporary lines to give output if not participating, this might help me see how we are going*/
    if (!isParticipating()) { cout << getName() << " scored " << storedScore+storedLayeredScore << " this round\n"; }
	return storedScore+storedLayeredScore;
}

void spatialRobocodeCreatureDelta::setCreature(boost::shared_ptr<spatialRobocodeCreatureDelta> toSetTo) { 
	geneticAge = toSetTo->geneticAge;
	critter = toSetTo->getCreature();
	thisCreatureDeltaNumber = toSetTo->thisCreatureDeltaNumber;
	isInvalid = toSetTo->isInvalid;
	//cout << "New critter set " << getName() << endl;
}

void spatialRobocodeCreatureDelta::makeCopyOfCreature(boost::shared_ptr<spatialRobocodeCreatureDelta> /*crPtr*/ tocopy) {
    if (!participates) humanRobot = tocopy->humanRobot;
	cr_data *newCopy;
	newCopy = new cr_data(*(tocopy->getCreature())); // dereference the crPtr to refer to the cr_data
	thisCreatureDeltaNumber = CreatureDeltaIdentifierCount++;
	critter.reset(newCopy);
	geneticAge = tocopy->geneticAge;
	isInvalid = false;
	//cout << "New critter copied " << getName() << endl;
}	

bool spatialRobocodeCreatureDelta::isReallyValid() {
	if (isInvalid) return false;
	if (critter->isValid()) {
		return true;
	}
	return false;
}

void spatialRobocodeCreatureDelta::loadCreature(ifstream &fin) {
	long dnaL;
	int *dna;
	fin >> geneticAge;
	cout << "CreatureDelta Age:" << geneticAge << endl;
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
		cout << "Potential error, CreatureDelta \"stream\" not properly terminated\n";
	}
	critter.reset(new cr_data(dna,dnaL));
	isInvalid = false; // assume that its not isInvalid until proven otherwise.
	if (critter->isValid()) {
		//cout << "A CreatureDelta loaded tha t was valid\n";
		if (isReallyValid()) {
				//cout << "... and it really is valid\n";
		} else { cout << "A valid creature delta was loaded but oddly, its not really valid\n";
		}
	} else {
		cout << "An invalid creature delta was loaded\n";
	}
	//	cout << "Loaded a CreatureDelta!\n";
}


bool spatialRobocodeCreatureDelta::isBetterThan(double lhs,double rhs) {
	return lhs > rhs; // high score = good score
}

bool spatialRobocodeCreatureDelta::isBetterThan(double lhsScore,spatialRobocodeCreatureDelta *rhs,double rhsScore) {
	if (!rhs) {
		cout << "isBetterThan in spatialoneagentworldCreatureDelta, called with a null rhs.\n";
		exit(1);
	}
	if (lhsScore == rhsScore) {
        // not sure about keeping this in - it doesn't seem to be necessary and may not be a good selective pressure.
        // however for robocode v. unlikely robots in nodes that compare will have identical scores.
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

void spatialRobocodeCreatureDelta::saveCreature(ofstream &fout)
{
	streamout(fout,critter,geneticAge);
}

bool spatialRobocodeCreatureDelta::terminate() {
	return false;
}

void spatialRobocodeCreatureDelta::display(const char *fname,int no,int para) {
	//networkOneAgent::Instance()->display(theNetwork,fname,para);
	cout << "Err you haven't written display spatialRobocodeCreatureDelta yet!\n";
}

bool spatialRobocodeCreatureDelta::isParticipating() {
    return participates;
}


void spatialRobocodeCreatureDelta::inLayerWithLocation(int layer,int x) {
    myLocation = x;
    // you can set the partipates flag here for certain nodes, if you want.
    // some code left in comments to show an example of how to do it.
    /*
     topLevel = false;
     if (layer == layerLives.size()-2) {
        if (x>=27 && x<=29) participates = false;
        if (x>=39 && x<=41) participates = false;
        if (x>=51 && x<=53) participates = false;
        
        if (x>=105 && x<=107) participates = false;
        if (x>=117 && x<=119) participates = false;
        if (x>=129 && x<=131) participates = false;
        humanRobot=randomint(MAXROBOTNUMBER);
            // need some code to change some of the locations to not participating and human robots.
    } else {
        if (layer == layerLives.size()-1) {
            topLevel = true;
            if (x>=7 && x<=9) participates = false;
            if (x>=13 && x<=15) participates = false;
            
            if (x>=61 && x<=64) participates = false;
            if (x>=73 && x<=75) participates = false;
            humanRobot=randomint(MAXROBOTNUMBER);
        }
    } 
    if (!participates) cout << "Set location " << x << " in layer " << layer << " to not participate (creature), assigned robot " << humanRobot << " which is " << getName() << "\n";
     */
}



/************************************************************************
 spatialRobocodeParasiteDelta and spatialRobocodeParasiteDelta(bool)
 *************************************************************************/

// All the critter comments also apply to this class.

spatialRobocodeParasiteDelta::spatialRobocodeParasiteDelta() {
    participates=true;
	geneticAge = 0;
	int maxTree;
	bool done = false;
	thisParasiteDeltaNumber = ParasiteDeltaIdentifierCount++;
	do {
		maxTree = randomint(8)+2;
		ParasiteDelta.reset(new cr_data);
		ParasiteDelta->makeptc2(maxTree);
	    done = ParasiteDelta->isValid();
	} while (!done);
	isInvalid = false;
	char fname[200];
	char jname[200];
	char gname[200];
	sprintf(gname,"Gen%d",currentGen);

	if (!(ParasiteDelta->isValid())) {
		cout << "Weird, newly created ParasiteDelta is not valid\n";
		exit(1);
	}
	sprintf(fname,"%s",getName());
	sprintf(jname,"%s.java",fname);
	ofstream robotout;
	robotout.open(jname);
	robocodeDelta::Instance()->print_grammar_individual(robotout,getParasite()->getExpression(),getParasite()->getExpressionLength(),fname,gname);
	robotout.close();
	
}



const char *spatialRobocodeParasiteDelta::getName() {
    if (!participates) {
        if (topLevel) {
            switch(humanrobot) {
                case 0: return "jk.sheldor.nano.Yatagan";
                case 1: return "mld.LittleBlackBook";
                case 2: return "mld.Moebius";
                case 3: return "mld.jdc.nano.LittleBlackBook";
                case 4: return "nat.nano.OcnirpSNG";
                case 5: return "nz.jdc.nano.AralR";
                case 6: return "nz.jdc.nano.AralT";
                case 7: return "nz.jdc.nano.NeophytePRAL";
                case 8: return "nz.jdc.nano.NeophytePattern";
                case 9: return "nz.jdc.nano.NeophyteSRAL";
                case 10: return "nz.jdc.nano.PatternAdept";
                case 11: return "nz.jdc.nano.PralDeGuerre";
                case 12: return "oog.nano.Caligula";
                case 13: return "oog.nano.Fuatisha";
                case 14: return "robar.nano.BlackWidow";
                case 15: return "robar.nano.Pugio";
                case 16: return "sheldor.nano.Sabreur";
                case 17: return "simonton.nano.WeekendObsession_S";
                case 18: return "fromHell.C22H30N2O2S";
                default:    return "wompi.Kowari";
            }
        } else {
            switch(humanrobot) {
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
                case 15: /*return "bots.UberBot";*/
                case 16: return "dggp.haiku.gpBot_0";
                case 17: return "zyx.nano.RedBull";
                case 18: return "zyx.nano.Ant";
                case 19: return "stelo.MirrorNano";
                default: return "supersample.SuperWalls";
            }
        }
    }
	else {
        
        makeName(thisParasiteDeltaNumber,false);
        return javaPName;
    }
}

spatialRobocodeParasiteDelta::spatialRobocodeParasiteDelta(bool make) {
    participates=true;
	thisParasiteDeltaNumber = ParasiteDeltaIdentifierCount++;
	geneticAge =0;
	if (make) {
		int maxTree;
		bool done = false;
		geneticAge = 0;
		do {
			maxTree = randomint(8)+2;
			ParasiteDelta.reset(new cr_data);
			ParasiteDelta->makeptc2(maxTree);
			done = ParasiteDelta->isValid();
		} while (!done);
		isInvalid = false;
		char fname[200];
		char jname[200];
		char gname[200];
		sprintf(gname,"Gen%d",currentGen);

		if (!(ParasiteDelta->isValid())) {
			cout << "Weird, newly created ParasiteDelta is not valid\n";
			exit(1);
		}
		sprintf(fname,"%s",getName());
		sprintf(jname,"%s.java",fname);
		ofstream robotout;
		robotout.open(jname);
		robocodeDelta::Instance()->print_grammar_individual(robotout,getParasite()->getExpression(),getParasite()->getExpressionLength(),fname,gname);
		robotout.close();
	}
}

spatialRobocodeParasiteDelta::~spatialRobocodeParasiteDelta() {  
	
}

void spatialRobocodeParasiteDelta::doCrossover(spatialRobocodeParasiteDelta &p2) {
    if (!participates) return;
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
		cr_data::variableCrossover(ParasiteDelta,p2.getParasite(),c1,c2);
		if ( c1 && c1->isValid()) {
			isInvalid = false;
			if (p2.geneticAge > geneticAge) geneticAge = p2.geneticAge; 
			done = true;
			ParasiteDelta = c1;
		} else if ( c2 && c2->isValid() ) {
			done = true;
			isInvalid = false;
			ParasiteDelta = c2;
  		    if (p2.geneticAge > geneticAge) geneticAge = p2.geneticAge; 
		} else {
			count++;
			if (count > 10) {
				cout << "Failed to create a valid child after 10 crossover attempts, abandoning it\n";
				done = true;
			}
		}
	} while (!done);
	return; 
}   

void spatialRobocodeParasiteDelta::doMutation(int i) {
    if (!participates) humanrobot = randomint(MAXROBOTNUMBER);
	else ParasiteDelta->smartSureMutate(10000);
}

bool spatialRobocodeParasiteDelta::isReallyValid() {
	if (isInvalid) return false;
	if (ParasiteDelta->isValid()) {
		return true;
	}
	return false;
}


void spatialRobocodeParasiteDelta::clearScore() {
	storedScore=0;
	storedLayeredScore=0;
	competitions=0;
}

void spatialRobocodeParasiteDelta::clearLayeredScore() {
	storedLayeredScore=0;
}



void spatialRobocodeParasiteDelta::calcScore(spatialRobocodeCreatureDelta &c1,int loc) {
	return;
}


// For parasites they are judged by their worst score, here its their lowest.
void spatialRobocodeParasiteDelta::addToScore(double toAdd) {
	//competitions++;
	//storedScore += toAdd;
	if (storedScore ==0) storedScore=toAdd;
	else if (toAdd < storedScore) storedScore=toAdd;
	return;
}

// Ditto keep the worst (lowest) layered score.
void spatialRobocodeParasiteDelta::addToLayeredScore(double toAdd) {
	//competitions++;
	//storedScore += toAdd;
	if (storedLayeredScore == 0) storedLayeredScore = toAdd;
	else if (toAdd < storedLayeredScore) {
		storedLayeredScore=toAdd;
	}
}

// So here, the higher score is the better score, we want to return the worst score the parasite has
// i.e. the lowest.

double spatialRobocodeParasiteDelta::getScore() {
	if ((storedLayeredScore == 0) || (storedScore < storedLayeredScore)) {
		//cout << "ParasiteDelta " << getName() << "returning score " << storedScore << "rather than " << storedLayeredScore << endl;
        if (!isParticipating()) { cout << "Parasite: " << getName() << " scored " << storedScore<< " this round\n"; }
 		return storedScore;
	} else {
			//cout << "ParasiteDelta " << getName() << "returning score " << storedLayeredScore << " rather than " << storedScore << endl;
            if (!isParticipating()) { cout << "Parasite: " << getName() << " scored " << storedScore<< " this round\n"; }
			return storedLayeredScore;
		}
}



void spatialRobocodeParasiteDelta::setParasite(boost::shared_ptr<spatialRobocodeParasiteDelta> toSetTo) {
	geneticAge = toSetTo->geneticAge;
	ParasiteDelta = toSetTo->getParasite();
	thisParasiteDeltaNumber = toSetTo->thisParasiteDeltaNumber;
	isInvalid = toSetTo->isInvalid;
	
}
void spatialRobocodeParasiteDelta::makeCopyOfParasite(boost::shared_ptr<spatialRobocodeParasiteDelta> /*crPtr*/ tocopy) {
	cr_data *newCopy;
	newCopy = new cr_data(*(tocopy->getParasite())); // dereference the crPtr to refer to the cr_data
	thisParasiteDeltaNumber = ParasiteDeltaIdentifierCount++;
	ParasiteDelta.reset(newCopy);
	geneticAge = tocopy->geneticAge;
	isInvalid = false;
}

bool spatialRobocodeParasiteDelta::terminate() {
	return false;
}

bool spatialRobocodeParasiteDelta::isBetterThan(double lhs,double rhs) {
	return lhs > rhs;
}

void spatialRobocodeParasiteDelta::saveParasite(ofstream &fout)
{
	streamout(fout,ParasiteDelta,geneticAge);
}


void spatialRobocodeParasiteDelta::loadParasite(ifstream &fin,int i) {
	long dnaL;
	int *dna;
	fin >> geneticAge;
	cout << "ParasiteDelta Age:" << geneticAge << endl;
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
		cout << "Potential error, CreatureDelta \"stream\" not properly terminated\n";
	}
	ParasiteDelta.reset(new cr_data(dna,dnaL));
	isInvalid = false; // assume that its not isInvalid until proven otherwise.
	if (ParasiteDelta->isValid()) {
		//cout << "A ParasiteDelta was loaded that was valid\n";
		if (isReallyValid()) {
		//	cout << "... and it really is valid\n";
		} else { cout << "odd, A valid Parasite was loaded but its not really valid\n";
		}
	} else {
		cout << "An invalid Parasite Delta was loaded\n";
	}
	//	cout << "Loaded a ParasiteDelta!\n";
	
}


bool spatialRobocodeParasiteDelta::isParticipating() {
    return participates;
}


void spatialRobocodeParasiteDelta::inLayerWithLocation(int layer,int x) {
// used to set the participates flag to false if you want occupation by non-evolving bots.
    /*
    if (layer == layerLives.size()-2) {
        if (x>=58 && x<=60) participates = false;
        if (x>=70 && x<=72) participates = false;
        if (x>=82 && x<=84) participates = false;
        
        if (x>=85 && x<=87) participates = false;
        if (x>=97 && x<=99) participates = false;
        if (!participates) humanrobot= randomint(MAXROBOTNUMBER);
        // need some code to change some of the locations to not participating and human robots.
    } else if (layer == layerLives.size()-1) {
            topLevel = true;
            if (x>=53 && x<=55) participates = false;
            if (x>=66 && x<=68) participates = false;
            if (x>=78 && x<=80) participates = false;
        
            if (x>=121 && x<=123) participates = false;
            if (x>=133 && x<=135) participates = false;
            if (!participates) humanrobot= randomint(MAXROBOTNUMBER);
            // need some code to change some of the locations to not participating and human robots.
        }

    if (!participates) cout << "Set location " << x << " in layer " << layer << "to not participate (parasite) assigned robot " << humanrobot << " which is " << getName() << "\n";
  */
}


