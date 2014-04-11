//
//  LightlyWrappedCritter.h
//  Robocode
//
//  Created by robin on 11/04/2014.
//
//

#ifndef __Robocode__LightlyWrappedCritter__
#define __Robocode__LightlyWrappedCritter__

#include <iostream>

#endif /* defined(__Robocode__LightlyWrappedCritter__) */
//
//  LightlyWrappedCritter.h
//  Robocode
//
//  Created by Robin Harper on 14/03/2014.
//
//

#ifndef __Robocode__LightlyWrappedCritter__
#define __Robocode__LightlyWrappedCritter__

#include <iostream>
#include "critter.h"
class LightlyWrappedCritter; // forward declaration

typedef boost::shared_ptr<LightlyWrappedCritter> lightlywrappedDeltaPtr;

class LightlyWrappedCritter {
    string theName;
    double storedScore;
    
public:
    
    LightlyWrappedCritter(string name);
    virtual void clearScore();
    virtual void addToScore(double);
    virtual double getScore() { return storedScore; }
    
    virtual bool isBetterThan(double lhs,double rhs);
    virtual bool isBetterThan(double lhsScore,LightlyWrappedCritter *rhs,double rhsScore);
    
    
    string getName();
    
};


#endif /* defined(__Robocode__LightlyWrappedCritter__) */
