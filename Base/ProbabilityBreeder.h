/*
 *  ProbabilityBreeder.h
 *  GEtestbed
 *
 *  Created by Robin Harper on 28/09/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROBABILITYBREEDER
#define PROBABILITYBREEDER

#include "myrandom.h"
#include "params.h"
#include <list>
#include <vector>
#include "critter.h"
#include "BreederBase.h"


class ProbabilityBreeder : public BreederBase
{
public:
          ProbabilityBreeder();
          ~ProbabilityBreeder();
          void cutdown(vector<crPtr> &pop,int retain);
          void listthem(vector<crPtr> &pop,ostream &os);
           int generate(vector<crPtr> &pop,int gens,int retain,int breeders,int mutate_chance, 
		int op = variableCrossover,
		bool showAlt = false, 
		int terminate_fitness =0,
		ostream &os=cout);
 //         void EliminateIdentical(bool);
 //         void AllowSelfPollination(bool);
          void setEliminate(bool el) { eliminate = el; }
          void setAllowSelf(bool as) { allowself = as; }
          static bool DoNewGens;
          static int variableCrossoverMutateChance;
private:
          bool eliminate;
          bool allowself;
          vector<crPtr> nextgen;
          long calctotal(vector<crPtr> &);
          crPtr &findit(vector<crPtr> &,long);
};

#endif

