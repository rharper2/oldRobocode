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


// Population Handler, tailored for robocode robots.
// It assumes the cr_data are wrapped in DeltaWrappedCritter class - this of course enables you to use virtually any system
// for evolving the creatures, just wrap them in something that has the same methods as a DeltaWrappedCritter.

class dwBattleLine;

// DW Handler is defined in the .cpp file. Basically it hands all the stuff neeed to
// hand out battles the the helper apps (java apps that invoke robocode
// to work out the fitness profiles.

namespace DWHandler {
    void initialiseServerStuff(short portNo);
}


class RoboPopHandler {
public:
    RoboPopHandler();
    void fileSave(const char *fname);
    string fileLoad(const char *fname);
    string spatialFileLoad(const char *fname);
    
    
    void rampedptc2();
    void setFileName(string st) { fileName = st; }
    long getPopSize() { return population.size(); }
    string breed(int cycles,int termination = 0);
    vector<wrappedDeltaPtr>::iterator dWpopStart() { return population.begin(); }
    vector<wrappedDeltaPtr>::iterator dWpopEnd() { return population.end(); }
    
    void setGen(int sg) { currentGen = sg; }
    void breed();
    void setPopulationSize(int x) { popSize = x; }\
    void changeDirectory();
    void workOutTheBestFewAndReallyTestThem();
protected:
    int popSize;
    int currentGen;
    
    string fileName;
    BreederBase *breeder;
    vector<wrappedDeltaPtr> population;
    void doTheBattles();
    void saveEm();
};

#endif /* defined(__Robocode__RoboPopHandler__) */
