//
//  DeltaWrappedCritter.h
//  Robocode
//
//  Created by Robin Harper on 14/03/2014.
//
//
// Wraps the internals of whatever representation we are using for our robocode programs from the population handler
// Basically you just need to implement the public methods here.
// A lot of them will be the same (isBetterThan, getscore etc
// But you will need to tailor things like loading and saving creatures
// variableCrossover breeding and also "printing" the java program which is the creature.

#ifndef __Robocode__DeltaWrappedCritter__
#define __Robocode__DeltaWrappedCritter__

#include <iostream>
#include "critter.h"
class DeltaWrappedCritter; // forward declaration

typedef boost::shared_ptr<DeltaWrappedCritter> wrappedDeltaPtr;

class DeltaWrappedCritter {
    crPtr critter;
    static long id_count;
    long theId;
    int compsWon;
    double storedScore;
protected:
    // we use this within the class
    // Not exposed so that if we change the underlying representaiton this is the only class we need to change.
    crPtr getCreature() { return critter; }
public:
    
    DeltaWrappedCritter() {DeltaWrappedCritter(true);}
    DeltaWrappedCritter(bool make=true);
    virtual void clearScore();
    virtual void addToScore(double);
    virtual double getScore() { return storedScore; }
    
    virtual bool isBetterThan(double lhs,double rhs);
    virtual bool isBetterThan(double lhsScore,DeltaWrappedCritter *rhs,double rhsScore);
    
    
    virtual void replaceWithCopyOfCreature(crPtr tocopy);
    
    
    void display() { if (critter) cout << critter->display(); }
    
    void loadCreature(ifstream &istream);
    void loadSpatialCreature(ifstream &istream);
    void saveCreature(ofstream &ostream);
    string getName();
    void printItOut(ofstream &roboout,const char *fname,const char *gname);
     // crosses over this DeltaWrappedCritter with the supplied one, returning up to two valid children.
    // It is prefectly okay to return a null c1 or c2.
    void variableCrossover(wrappedDeltaPtr secondParent,wrappedDeltaPtr &c1,wrappedDeltaPtr &c2);

};


#endif /* defined(__Robocode__DeltaWrappedCritter__) */
