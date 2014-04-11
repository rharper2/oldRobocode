//
//  LightHandler.h
//  Robocode
//
//  Created by robin on 11/04/2014.
//
//

#ifndef __Robocode__LightHandler__
#define __Robocode__LightHandler__

#include <iostream>

#endif /* defined(__Robocode__LightHandler__) */
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
#include "LightlyWrappedCritter.h"
#include <list>
#include <string>

class LWBattleLine;
namespace LWHandler {
    void initialiseServerStuff(short portNo);
}


class LightHandler {
public:
    LightHandler();
    string fileName;
    long getPopSize() { return population.size(); }
    void clearPop() { population.clear(); }
    void addRobot(string name);
    //void sortIt() { sort(population.begin(),population.end(),crPtrGreaterThan); }
    int currentGen;
    void changeDirectory();
    void workOutTheBestFewAndReallyTestThem();
protected:
    vector<lightlywrappedDeltaPtr> population;
    void doTheBattles();
    
};

#endif /* defined(__Robocode__RoboPopHandler__) */
