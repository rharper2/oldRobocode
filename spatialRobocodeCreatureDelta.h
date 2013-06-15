/*
 *  spatialRobocodeCreatureDelta.h
 *  oneAgentWorld
 *
 *  Created by Robin on 3/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *  spatialRobocodeCreatureDelta.h
 *  to use with oneAgentWorld
 *
 *  Created by Robin on 4/06/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "critter.h"
#include <fstream>
#define MAXROBOTNUMBER 10

class spatialRobocodeParasiteDelta;
class battleLine;

class spatialRobocodeCreatureDelta {
	static long CreatureDeltaIdentifierCount;
	static bool stateFlag;
    crPtr critter;
	int compsWon;
	double storedScore;
	double storedLayeredScore;
	int competitions;
	bool failedTest;
    bool participates;
    int myLocation;
public:

	
	int geneticAge;
	long thisCreatureDeltaNumber;
    int humanRobot;
	bool isInvalid; // only true if we *know* its invalid
    spatialRobocodeCreatureDelta();
    spatialRobocodeCreatureDelta(bool create);
    virtual bool isParticipating();
    virtual void inLayerWithLocation(int layer,int x);
    
    virtual ~spatialRobocodeCreatureDelta() { critter.reset();  } 
    virtual void doCrossover(spatialRobocodeCreatureDelta &p2);
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
	
	virtual void calcScore(spatialRobocodeParasiteDelta &p1,int loc=-1);
	
	// Where a "critter" can gain points when the ParasiteDelta runs the "test"
	// then you can used addToScore or addToLayeredScore (as appropriate)
	// to alter the stored score
	
	virtual void addToScore(double);
	virtual void addToLayeredScore(double);
	
	// Finally use getScores to return the accumulated score
	// plus (or minus) anything you calculate at this point.
    // Note that spatial.h currently stores this in CreatureDeltaScore
	// and it would normally be accessed from there, as the scores in here
	// might change as different layers calculate their score.
	// so if we have a three layered system, assume this node is stored in layer1
	// At one point this score will reflect the competitions between this CreatureDelta
	// And the local ParasiteDeltas in layer0 and layer1
	// Later on in the same update it will reflect the score between this CreatureDelta
	// and the ParasiteDeltas in layer2 and layer1
	
	virtual double getScore();
	
  	// NOTE: this is isBetterThan NOT isBetterThanOrEqual
	// Parsimony control (if they are equal perfer shorter one
	// if its over a certain minimum length) is implemented
	// in spatial.h
	
	virtual bool isBetterThan(double lhs,double rhs);
	virtual bool isBetterThan(double lhsScore,spatialRobocodeCreatureDelta *rhs,double rhsScore);
	
    // virtual double realScore() { return 0; }
    
	
	virtual void setCreature(boost::shared_ptr<spatialRobocodeCreatureDelta> toSetTo);
	virtual void makeCopyOfCreature(boost::shared_ptr<spatialRobocodeCreatureDelta>  tocopy);
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

class spatialRobocodeParasiteDelta {
    int humanrobot;
    bool participates;
	static long ParasiteDeltaIdentifierCount;
    crPtr ParasiteDelta;
	double storedScore;
	double storedLayeredScore;
	int competitions;
	bool failedTest;
public:
	int geneticAge;
	long thisParasiteDeltaNumber;
	bool isInvalid; // only true if we *know* its invalid
	const char *getName();
    virtual bool isParticipating();
    virtual void inLayerWithLocation(int layer,int x);
    spatialRobocodeParasiteDelta();
    spatialRobocodeParasiteDelta(bool create);
    virtual ~spatialRobocodeParasiteDelta();
	virtual void doMutation(int layer=0);
    virtual bool isReallyValid();
    virtual void doCrossover(spatialRobocodeParasiteDelta &p1);
	
	// see comments in spatialRobocodeCreatureDelta for an explanation as to the use of the next four
	// fuctions
	virtual void clearScore();
	virtual void clearLayeredScore();
    virtual void calcScore(spatialRobocodeCreatureDelta &p1,int loc = -1);
	virtual void addToScore(double);
	virtual void addToLayeredScore(double);
	
	virtual double getScore();
    virtual bool isBetterThan(double lhs,double rhs);// returns true if lhs is "better than" the rhs
    virtual void setParasite(boost::shared_ptr<spatialRobocodeParasiteDelta> toSetTo);
    virtual void makeCopyOfParasite(boost::shared_ptr<spatialRobocodeParasiteDelta>  tocopy);
	virtual crPtr getParasite() { return ParasiteDelta; }
	//virtual networkSensorPtr getNetwork();
	void loadParasite(ifstream &istream,int i=0);
	void saveParasite(ofstream &ostream);
	bool terminate() ;
};


class battleLine {
public:
	battleLine(spatialRobocodeCreatureDelta *cr,spatialRobocodeParasiteDelta *pa,int loc): CreatureDelta(cr),ParasiteDelta(pa),location(loc) { }
	spatialRobocodeCreatureDelta *CreatureDelta;
	spatialRobocodeParasiteDelta *ParasiteDelta;
	int location;
};
