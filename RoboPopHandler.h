//
//  RoboPopHandler.h
//  Robocode
//
//  Created by Robin Harper on 14/03/2014.
//
//

#ifndef __Robocode__RoboPopHandler__
#define __Robocode__RoboPopHandler__

#include <iostream>
#include "popHandlerBase.h"
#include "critter.h"
#include "DeltaWrappedCritter.h"
#include <list>
#include <string>

class dwBattleLine;



class RoboPopHandler: public popHandlerBase {
public:
    RoboPopHandler();
    void fileSave(const char *fname);
    string fileLoad(const char *fname);
    
    void rampedptc2(int startExp,int endExp,int maxDepth=16);
    
    long getPopSize() { return population.size(); }
    string breed(int cycles,int termination = 0);
    vector<wrappedDeltaPtr>::iterator dWpopStart() { return population.begin(); }
    vector<wrappedDeltaPtr>::iterator dWpopEnd() { return population.end(); }
    //void sortIt() { sort(population.begin(),population.end(),crPtrGreaterThan); }
    
    
protected:
    BreederBase *breeder;
    vector<wrappedDeltaPtr> population;
};

#endif /* defined(__Robocode__RoboPopHandler__) */
