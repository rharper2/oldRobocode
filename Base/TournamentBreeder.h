/*
 *  TournamentBreeder.h
 *  GEtestbed
 *
 *  Created by Robin Harper on 06/06/06.
 *  Copyright (c) 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TournamentBreeder_Header
#define TournamentBreeder_Header

#include "myrandom.h"
#include "params.h"
#include <list>
#include <vector>
#include "critter.h"
#include "BreederBase.h"


class TournamentBreeder : public BreederBase
{
public:
          TournamentBreeder();
          ~TournamentBreeder();
          void cutdown(vector<crPtr> &pop,int retain);
          void listthem(vector<crPtr> &pop,ostream &os,bool);
          int generate(vector<crPtr> &pop,int gens,int retain,int pop_size,int mutate_chance,int op=twoPointReplacement,bool showAlt = false,  int terminate_fitness =0, ostream &os=cout);
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

