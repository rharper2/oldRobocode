/*
 *  singlePopHandler.h
 *  
 *
 *  Created by Robin Harper on 13/12/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef singlePopHandler_type
#define singlePopHandler_type

#include "popHandlerBase.h"
#include "critter.h"
#include <list>
#include <string>


class singlePopHandler: public popHandlerBase {
public:
 singlePopHandler();
 void fileSave(const char *fname) { savePop(fname,population); }
 string fileLoad(const char *fname) { 
        cout << "Calling load pop.\n";
        return loadPop(fname,population); }
 void saveFitness(char *fname, int count = 0,bool resort=false) { fitnessToSave(fname,population,count,resort); }
 
 void randomise();
 void randomise(int categorySize);
 void rampedHalfandHalf(int min,int max);
 void rampedptc2(int startExp,int endExp,int maxDepth=16);

 void setBreeder(BreederBase *bb) { breeder = bb; }
 long getPopSize() { return population.size(); }
 string breed(int cycles,int termination = 0);
 vector<crPtr>::iterator popStart() { return population.begin(); }
 vector<crPtr>::iterator popEnd() { return population.end(); }
 double bestFitness() { return population.front()->get_fitness(); }
 BreederBase *breederSelected() { return breeder; }
 void reEvaluate();
 void sortIt() { sort(population.begin(),population.end(),crPtrGreaterThan); }
 bool uniqueInPop(crPtr tomake);
	crPtr getPopMember(int toGet) { if (toGet > population.size()) { cout << "Error only have " << population.size() << " members" <<endl; return population[0]; } else return population[toGet]; } // need safety checks!

protected:
 BreederBase *breeder;
 vector<crPtr> population;
};
#endif
