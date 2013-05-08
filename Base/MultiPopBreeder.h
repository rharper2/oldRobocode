/*
 *  MultiPopBreeder.h
 *  GEtestbed
 *
 *  Created by Robin Harper on 06/06/06.
 *  Copyright (c) 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MultiPopBreeder_Header
#define MultiPopBreeder_Header

#include "myrandom.h"
#include "params.h"
#include <list>
#include <vector>
#include "critter.h"
#include "BreederBase.h"


class MultiPopBreeder: public BreederBase
{
public:
          MultiPopBreeder();
          ~MultiPopBreeder();
          void cutdown(vector<crPtr> &pop,int retain);
          void listthem(vector<crPtr> &pop,ostream &os,bool);
		  // to maintain compatibility with breeder base - really just grabbing the enums from it.
          int generate(vector<crPtr> &pop,int gens,int retain,int pop_size,int mutate_chance,int op=twoPointReplacement,bool showAlt = false,  int terminate_fitness =0, ostream &os=cout) {
			return 0; //generate(NULL,&pop,retain,pop_size,mutate_chance,op,showAlt,terminate_fitness,os);  
		  }
          int generate(vector<crPtr> *lowerPop,vector<crPtr> *pop,int gens,int retain,int pop_size,int mutate_chance,int op=twoPointReplacement,bool showAlt = false,  int terminate_fitness =0, ostream &os=cout);
          void setEliminate(bool el) { eliminate = el; }
          void setAllowSelf(bool as) { allowself = as; }
          static bool DoNewGens;
          int tournamentSize;
private:
          bool eliminate;
          bool allowself;
          vector<crPtr> nextgen;
};

#endif

