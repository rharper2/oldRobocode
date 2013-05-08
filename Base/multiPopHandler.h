/*
 *  multiPopHandler.h
 *  
 *
 *  Created by Robin Harper on 13/12/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef multiPopHandler_type
#define multiPopHandler_type
#include "popHandlerBase.h"
#include "critter.h"
#include <list>
#include <string>
#include "MultiPopBreeder.h"

typedef vector<crPtr> populationType;
typedef boost::shared_ptr<populationType> populationPtr;

enum {
	rampedPTC2 = 0,
	rampedHalfAndHalf = 1,
	randomiseIt = 2
};


class multiPopHandler: public popHandlerBase {
public:
 multiPopHandler();
 void fileSave(const char *fname); 
 string fileLoad(const char *fname);
 void saveFitness(char *fname, int count = 0,bool resort=false);
 
 void randomise();
 void randomise(int categorySize);
 void rampedHalfandHalf(int min,int max);
 void rampedptc2(int startExp,int endExp,int maxDepth=16);
 void randomise(vector<crPtr> &);
 void randomise(vector<crPtr> &,int categorySize);
 void rampedHalfandHalf(vector<crPtr> &,int min,int max);
 void rampedptc2(vector<crPtr> &,int startExp,int endExp,int maxDepth=16);

 void enforceAgeLimit();
 void display();
 void setBreeder(BreederBase *bb) { breeder = dynamic_cast<MultiPopBreeder *> (bb);
									if (!breeder) {
											cout << "Dynamic cast of breeder failed, for MultiPopHandlers, your breeder must be of type MultiPopBreeder.\n";
											exit(1);
									}
								   }
 long getPopSize() { return allThePopulations[allThePopulations.size()-1]->size(); }
 string breed(int cycles,int termination = 0);
 vector<crPtr>::iterator popStart() { return allThePopulations[allThePopulations.size()-1]->begin(); }
 vector<crPtr>::iterator popEnd() { return allThePopulations[allThePopulations.size()-1]->end(); }
 double bestFitness() { return allThePopulations[allThePopulations.size()-1]->front()->get_fitness(); }
 MultiPopBreeder *breederSelected() { return breeder; }
 void reEvaluate();
 void sortIt() { sort(allThePopulations[allThePopulations.size()-1]->begin(),allThePopulations[allThePopulations.size()-1]->end(),crPtrGreaterThan); }
 bool uniqueInPop(vector<crPtr> &pop,crPtr tomake);
 static int initialisationMethod;

protected:
 MultiPopBreeder *breeder;
 vector<populationPtr> allThePopulations; // so a vector of shared pointers, which each point to a vector of critter pointers (crPtr)
 vector<int> ageLimits;
 
};
#endif
