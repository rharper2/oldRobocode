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
namespace DWHandler {
    void initialiseServerStuff(short portNo);
}


class RoboPopHandler {
public:
    RoboPopHandler();
    void fileSave(const char *fname);
    string fileLoad(const char *fname);
    string fileName;
    void rampedptc2();
    void setFileName(string st) { fileName = st; }
    long getPopSize() { return population.size(); }
    string breed(int cycles,int termination = 0);
    vector<wrappedDeltaPtr>::iterator dWpopStart() { return population.begin(); }
    vector<wrappedDeltaPtr>::iterator dWpopEnd() { return population.end(); }
    //void sortIt() { sort(population.begin(),population.end(),crPtrGreaterThan); }
    int currentGen;
    void setGen(int sg) { currentGen = sg; }
    void breed();
    void setPopulationSize(int x) { popSize = x; }
protected:
    int popSize;
    BreederBase *breeder;
    vector<wrappedDeltaPtr> population;
    void doTheBattles();
    void changeDirectory();
    void saveEm();
};

#endif /* defined(__Robocode__RoboPopHandler__) */
