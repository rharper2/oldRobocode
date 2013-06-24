/*
 *  spatialRobocodeCreatureMvP.h
 *  oneAgentWorld
 *
 *  Created by Robin on 3/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *  spatialRobocodeCreatureMvP.h
 *  to use with oneAgentWorld
 *
 *  Created by Robin on 4/06/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "critter.h"
#include <fstream>

#define SCORESTOSTORE 3
#define NUMBEROFPARASITES 16


class spatialRobocodeMvPParasite;
class battleLine;

class spatialRobocodeCreatureMvP {
	static long creatureIdentifierCount;
	static bool stateFlag;
    crPtr critter;
	int compsWon;
	double storedScore;
	double storedLayeredScore;
	int competitions;
	bool failedTest;

public:
    // added these to stay compatibile with the new spatial
    virtual bool isParticipating() {return false;};
    virtual void inLayerWithLocation(int layer,int x) {};
    
	
	int geneticAge;
	long thisCreatureNumber;
	bool isInvalid; // only true if we *know* its invalid
    spatialRobocodeCreatureMvP();
    spatialRobocodeCreatureMvP(bool create);
    virtual ~spatialRobocodeCreatureMvP() { critter.reset();  } 
    virtual void doCrossover(spatialRobocodeCreatureMvP &p2);
    virtual void doMutation();
	const char *getName();
	// Works as follows, clear scores clears the scores ready for the scores
	// in a generation to be calculated
	// Layered Scores (for layered systems) are the scores achieved against different layers.
	// Either the layer below, or later, the layer above.
	
	virtual void clearScore();
	virtual void clearLayeredScore();
    
	// Then calcScore takes an opponent and works out and culmlatively stores
	// the score achieved, loc is the location in your layer
	// or if the competitor is in a different layer it is -1
	// calc Score will then call either addToScore or addToLayeredScore
	// as appropriate.
	
	virtual void calcScore(spatialRobocodeMvPParasite &p1,int loc=-1);
	
	// Where a "critter" can gain points when the parasite runs the "test"
	// then you can used addToScore or addToLayeredScore (as appropriate)
	// to alter the stored score
	
	virtual void addToScore(double);
	virtual void addToLayeredScore(double);
	
	// Finally use getScores to return the accumulated score
	// plus (or minus) anything you calculate at this point.
    // Note that spatial.h currently stores this in creatureScore
	// and it would normally be accessed from there, as the scores in here
	// might change as different layers calculate their score.
	// so if we have a three layered system, assume this node is stored in layer1
	// At one point this score will reflect the competitions between this creature
	// And the local parasites in layer0 and layer1
	// Later on in the same update it will reflect the score between this creature
	// and the parasites in layer2 and layer1
	
	virtual double getScore();
	
  	// NOTE: this is isBetterThan NOT isBetterThanOrEqual
	// Parsimony control (if they are equal perfer shorter one
	// if its over a certain minimum length) is implemented
	// in spatial.h
	
	virtual bool isBetterThan(double lhs,double rhs);
	virtual bool isBetterThan(double lhsScore,spatialRobocodeCreatureMvP *rhs,double rhsScore);
	
    // virtual double realScore() { return 0; }
    
	
	virtual void setCreature(boost::shared_ptr<spatialRobocodeCreatureMvP> toSetTo);
	virtual void makeCopyOfCreature(boost::shared_ptr<spatialRobocodeCreatureMvP>  tocopy);
	virtual bool isReallyValid();
	
    crPtr getCreature() { return critter; }
	
    void display() { if (critter) cout << critter->display(); }
	void display(const char *name,int no,int para);
	
	void loadCreature(ifstream &istream);
	void saveCreature(ofstream &ostream);
	
	// you can put logic in here to check if the critter has in fact 
	// statisfied your termination conditions (if any).
	
	bool terminate();
	
	//virtual networkSensorPtr getNetwork();
	
};

class spatialRobocodeMvPParasite {
	static long parasiteIdentifierCount;
    int parasite;
	double storedScore;
	double storedLayeredScore;
	int competitions;
	bool failedTest;
	
public:
    // added these to stay compatibile with the new spatial
    virtual bool isParticipating() {return false;};
    virtual void inLayerWithLocation(int layer,int x) {};
    
	static const char *parasiteName[];
	int geneticAge;
	long thisParasiteNumber;
	bool isInvalid; // only true if we *know* its invalid
	const char *getName();
    spatialRobocodeMvPParasite();
    spatialRobocodeMvPParasite(bool create);
    virtual ~spatialRobocodeMvPParasite();
    virtual void doMutation(int layer=0);
    virtual bool isReallyValid();
    virtual void doCrossover(spatialRobocodeMvPParasite &p1);
	
	// see comments in spatialRobocodeCreatureMvP for an explanation as to the use of the next four
	// fuctions
	virtual void clearScore();
	virtual void clearLayeredScore();
    virtual void calcScore(spatialRobocodeCreatureMvP &p1,int loc = -1);
	virtual void addToScore(double);
	virtual void addToLayeredScore(double);
	
	virtual double getScore();
    virtual bool isBetterThan(double lhs,double rhs);// returns true if lhs is "better than" the rhs
    virtual void setParasite(boost::shared_ptr<spatialRobocodeMvPParasite> toSetTo);
    virtual void makeCopyOfParasite(boost::shared_ptr<spatialRobocodeMvPParasite>  tocopy);
	virtual int getParasite() { return parasite; }
	//virtual networkSensorPtr getNetwork();
	void loadParasite(ifstream &istream,int ll=0);
	void saveParasite(ofstream &ostream);
	bool terminate() ;
};


class battleLine {
public:
	battleLine(spatialRobocodeCreatureMvP *cr,spatialRobocodeMvPParasite *pa,int loc,bool rem = false): creature(cr),parasite(pa),location(loc), rematch(rem) { }
	spatialRobocodeCreatureMvP *creature;
	spatialRobocodeMvPParasite *parasite;
	int location;
	bool rematch;
};

class parasiteData {
public:
    // added these to stay compatibile with the new spatial
    virtual bool isParticipating() {return false;};
    virtual void inLayerWithLocation(int layer,int x) {};

	double scoreSoFar;
	double pScoreSoFar;
	double cPercentSoFar;
	double pPercentSoFar;
	double highestSinceClear;
	double highestPSinceClear;
	int matchesSoFar;
	string contender;
	parasiteData() { clear(); contender = "None"; }
	void clear() { scoreSoFar = pScoreSoFar = highestSinceClear = cPercentSoFar = pPercentSoFar = highestPSinceClear = 0.0; matchesSoFar = 0; }
	void add(double scoreC,double scoreP, string Name) { 
		double cPer = (scoreC*100)/(scoreC+scoreP);
		scoreSoFar += scoreC; pScoreSoFar += scoreP; 
		cPercentSoFar += cPer;
		pPercentSoFar += (scoreP*100)/(scoreC + scoreP);
		if (scoreC > highestSinceClear) {
			highestSinceClear = scoreC;
			contender = Name;
		}
		if (cPer > highestPSinceClear) highestPSinceClear = cPer;
		matchesSoFar++; 
	}
};

class scoreStore {
	public:
	string name;
	double score, percentage, parasiteScore;
	int generation;
	int age;
	scoreStore() { }
	scoreStore(string critter,double sc, double pS, double per,int gen,int ag): name(critter),score(sc), parasiteScore(pS), percentage(per), generation(gen), age(ag) { }
	void setScore(double sc, double pS, double per, int gen,string critter,int ag ) { score = sc; parasiteScore = pS; percentage = per; generation = gen;name = critter;age=ag; }
};

