//
//  DeltaWrappedCritter.h
//  Robocode
//
//  Created by Robin Harper on 14/03/2014.
//
//

#ifndef __Robocode__DeltaWrappedCritter__
#define __Robocode__DeltaWrappedCritter__

#include <iostream>
#include "critter.h"
class DeltaWrappedCritter; // forward declaration

typedef boost::shared_ptr<DeltaWrappedCritter> wrappedDeltaPtr;

class DeltaWrappedCritter {
    crPtr critter;
    
    int compsWon;
    double storedScore;
public:
    
    DeltaWrappedCritter() {DeltaWrappedCritter(true);}
    DeltaWrappedCritter(bool make=true);
    virtual void clearScore();
    virtual void addToScore(double);
    
    virtual bool isBetterThan(double lhs,double rhs);
    virtual bool isBetterThan(double lhsScore,DeltaWrappedCritter *rhs,double rhsScore);
    
    
    virtual void makeCopyOfCreature(boost::shared_ptr<DeltaWrappedCritter>  tocopy);
    
    crPtr getCreature() { return critter; }
    
    void display() { if (critter) cout << critter->display(); }
    
    void loadCreature(ifstream &istream);
    void saveCreature(ofstream &ostream);
    
};


#endif /* defined(__Robocode__DeltaWrappedCritter__) */
