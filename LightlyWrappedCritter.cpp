//
//  LightlyWrappedCritter.cpp
//  Robocode
//
//  Created by robin on 11/04/2014.
//
//

#include "LightlyWrappedCritter.h"


char javaCName[200];

LightlyWrappedCritter::LightlyWrappedCritter(string Name) {
    theName = Name;
}

string DeltaWrappedCritter::getName()
{
    return theName;
}

void LightlyWrappedCritter::clearScore() {
    storedScore = 0;
}

void LightlyWrappedCritter::addToScore(double ats) {
    storedScore += ats;
    if (theId==0) cout << "adding " << ats << " its now " << storedScore << endl;
}

bool LightlyWrappedCritter::isBetterThan(double lhsScore,DeltaWrappedCritter *rhs,double rhsScore) {
        return lhsScore < rhsScore;
}

bool LightlyWrappedCritter::isBetterThan(double lhs,double rhs) {
	return lhs > rhs;
}

	
}



