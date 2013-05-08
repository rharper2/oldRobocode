/*
 *  popHandlerBase.h
 *  test2
 *
 *  Created by Robin Harper on 13/12/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

//************************
//
// Base class for the pop(ulation)Handlers
// These implement the storing/saving/loading of a population
// of critters as well as their breeding strategies.
//
//**************************

#ifndef popHandlerBase_type
#define popHandlerBase_type

#include <iostream>
#include <vector>
#include "critter.h"
#include "BreederBase.h"
#include <string>
#include <sstream>

class popHandlerBase {
public:
 popHandlerBase();
 virtual ~popHandlerBase() {}
 virtual void fileSave(const char *fName) =0;
 virtual string fileLoad(const char *fname) =0;
 virtual void saveFitness(char *fname,int count = 0,bool resort = false) =0;
 
 virtual void setPopulationSize(int size=200) { popSize = size; }
 virtual void setEliteStrategy(int size = 10) { eliteStrategy = size; }
 virtual void setCombiningMethod(int op=twoPointReplacement) { combiningOp = op; cout << "Operator set to: " << op << endl;} //enum defined in BreederBase.h
 
 virtual long getPopSize() =0; //{ return popSize; }
 
 virtual void randomise() = 0;
 bool altScore; 
 virtual void setBreeder(BreederBase *bb) =0;
 virtual BreederBase *breederSelected() =0;
 virtual void setMutationRate(int mutate=200) { mutationRate = mutate; };
 
 virtual string breed(int cycles, int termination = 0) =0;
  
 virtual vector<crPtr>::iterator popStart() =0;
 virtual vector<crPtr>::iterator popEnd() =0;
 virtual void sortIt() = 0;
 virtual double bestFitness() = 0;
 virtual void reEvaluate() { };
protected:
 void savePop(const char *fName,vector<crPtr> &popToSave);
 string loadPop(const char *fName, vector<crPtr> &popToLoad);
 void fitnessToSave(char *fname, vector<crPtr> &pop, int count = 0,bool toSort = false);
 void streamout(ostream &,crPtr &);
 int popSize;
 int eliteStrategy;
 int combiningOp;
 int mutationRate;
 
};

#endif
