/*
 *  ProbabilityBreeder.cpp
 *  GEtestbed
 *
 *  Created by Robin Harper on 28/09/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#include "ProbabilityBreeder.h"
#include "myrandom.h"

using namespace std;

bool ProbabilityBreeder::DoNewGens = true;
int ProbabilityBreeder::variableCrossoverMutateChance = 1000; // i.e. 1 in 10

ProbabilityBreeder::ProbabilityBreeder() {
    eliminate = false;
    allowself=false;
    //        for (int i=0;i<4;i++)
    //           for (int j=0;j<3;j++)
    //               varStats[i][j]=0;
}

ProbabilityBreeder::~ProbabilityBreeder() { }

/*
 void ProbabilityBreeder::copyOver(list<cr_data> *src,list<cr_data> *dest) {
     dest->clear();
     list<cr_data>::iterator at = src->begin();
     while (at != src->end())
         dest->push_back(*(at++));
 }
 */

void ProbabilityBreeder::listthem(vector<crPtr> &pop,ostream &os) {
    int inlist = pop.size();
    cout << "In list is " << inlist << "\n";
    vector<crPtr>::iterator p=pop.begin();
    if (inlist  > 200) inlist = 200;
    for (int i=0;i<inlist-1;i++) {
        os << (*p)->get_fitness() << ", ";
        p++;
    }
    os << (*p)->get_fitness() << "\n";
}


void ProbabilityBreeder::cutdown(vector<crPtr> &pop,int retain) {
    // cout << "Cutdown with list size " << pop.size() << " and retain " << retain << "\n";
    sort(pop.begin(),pop.end(),crPtrGreaterThan); // Do I need a full sort? Or just the shuffle around the top retain?
    pop.resize(retain); //assumes sorted
                        // cout << "After resize we have size " << pop.size() << "\n";
}

long ProbabilityBreeder::calctotal(vector<crPtr> &li) {
    vector<crPtr>::iterator it;
    long total_fitness = 0;
    it = li.begin();
    while (it!=li.end()) {
        if (*it) total_fitness += (*it)->get_fitness();
        it++;
    }
    return total_fitness;
}

crPtr &ProbabilityBreeder::findit(vector<crPtr> &pop,long fitness) {
    long count = 0;
    vector<crPtr>::iterator it = pop.begin();
    while (it != pop.end()) {
        count= (*it)->get_fitness() + count;
        if (count > fitness) break;
        it++;
    }
    if (it != pop.end()) 
        return *it;
    else { 
        cout << "**********ProbabilityBreeder findit ran off the end!\n***********";
        return *(pop.begin());
    }
}

int ProbabilityBreeder::generate(vector<crPtr> &pop,int gens,int retain,int pop_size,int mutate_chance,
                                 int tf, bool showAlt,int op,ostream &os) {
    //    sort(pop.begin(),pop.end(),crPtrGreaterThan); // I need to work out if this sort is really necessary.
    pop.reserve(2*pop_size + retain + 1); // there is a risk a second child is valid and takes it one above.
    long total_fitness;
    long first,second;
    bool mutated;
    vector<crPtr>::iterator it;
    crPtr c1,c2;
    nextgen.reserve(2*pop_size+1);
    cutdown(pop,pop_size); // cutdown to the retained fittest - cutdown expands if necessary with random critters
    int gensdone = 0;
    do {
        nextgen.clear(); // clear out the temporary store for the next generation
        total_fitness = calctotal(pop); // query selection strategy here
                                        //                   cout << "total_fitness = " << total_fitness << "\n";
        if (total_fitness <=0) {
            cout << "total_fitness = " << total_fitness << "- it needs to be greater than 0 so aborting.\n";
            return 0;
        }
        int count = 0;
        crPtr t1,t2,p1;
        //       int tryCount = 0;
        //       int noGot = 0;
        //                    while ((SizeSwitch?count:(int) nextgen.size()) < pop_size) {
        while (((int) nextgen.size()) < pop_size) {
            count++;
            first = rand()%total_fitness;
            second = rand()%total_fitness;
            mutated = false; // if we havent done the mutation then do it at the end of the switch
                             // cout << "Going to choose " << first << " and then " << second << "\n";
            switch (op) {
                case singlePointBit: 
                    cr_data::singlePointBitOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case singlePointCodon:
                    cr_data::singlePointCodonOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
/*                case singlePointMatched:
                    cr_data::singlePointMatchedOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case headlessBit:
                    cr_data::headlessBitOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case taillessBit:
                    cr_data::taillessBitOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case twoPointBit: 
                    cr_data::twoPointBitOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case twoPointCodon:
                    cr_data::twoPointCodonOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case twoPointMatched:
                    cr_data::twoPointMatchedOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case twoPointReplacement:
                    cr_data::twoPointReplacementOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case twoPointRSaga:
                    cr_data::twoPointReplacementSAGAOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case twoPointGutless:
                    cr_data::twoPointGutlessOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
 */
                case gp:
                    cr_data::gpReplacementOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
 /*
                case variableCrossover: // Limited to just the two crossovers just now
                    cr_data::variableCrossover(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case randomCrossover:
                {   
                    crPtr p1 = findit(pop,first);
                    crPtr p2 = findit(pop,second);
                    int cross = -1;
                    if (!p1) {
                        cout << "Findit returned a null parent in Probability Breeder\n";
                        exit(1);
                    }
                    if (!p2) {
                        cout << "Findit returned a null p2 parent in Probability Breeder\n";
                        exit(1);
                    }
                    if (!p1->isValid()) {
                        cout << "Findit it returned and invalid parent in Probability Breeder\n";
                        exit(1);
                    }
                    if (!p2->isValid()) {
                        cout << "Findit it returned and invalid p2 parent in Probability Breeder\n";
                        exit(1);
                    }
                    switch(randomint(2)) {
                        case 0: // single point Bit
                            cr_data::singlePointBitOperator(p1,p2,c1,c2);
                            cross = 0;
                            break;
                        case 1: // two Point Replacement
                            cr_data::twoPointReplacementOperator(p1,p2,c1,c2);
                            cross =1;
                            break;
                    }
                    if (c1 != NULL) {
                        c1->setCrossMethod(cross); // just noting that this was created with cross type.
                    }
                    if (c2 != NULL ) {
                        c2->setCrossMethod(cross); // just noting that this was created with cross type.
                    }
                    
                    
                    
                    
                    //                    cr_data::randomCrossover(findit(pop,first),findit(pop,second),c1,c2);
                }
                    break;
                case uniformCrossover:
                    cr_data::uniformCodonOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
                case mutateOnly:
                    cr_data::mutateOnly(findit(pop,first),findit(pop,second),c1,c2);
                    break;
 */
                default:
                    cout << "Default operator in PB\n";
                    cr_data::singlePointCodonOperator(findit(pop,first),findit(pop,second),c1,c2);
                    break;
            }
            if (c1 != NULL) {
                if (!mutated)
                    if (mutate_chance > 0) c1->mutate(mutate_chance); // temporarily put this with the switch statement
                if (c1->isValid()) {
                    nextgen.push_back(c1);
                }
                c1.reset();
            } 
            if (c2 != NULL) {
                if (!mutated)
                    if(mutate_chance > 0) c2->mutate(mutate_chance);
                if (c2->isValid()) {
                    nextgen.push_back(c2);
                }
                c2.reset();
            }
        }
        bool reEval=false;
        if (DoNewGens)  reEval = (cr_data::getInterpreter()->newGen());
        
        it = pop.begin();
        int tc = 0;
        while (tc < retain && it != pop.end()) {
            tc++;it++;
        }
        if (it!=pop.end()) pop.erase(it,pop.end());
      if (reEval)  {
            it = pop.begin();
            while (it!=pop.end()) {
                (*it)->reEvaluate(); // notify each critter that it will need to be re-evaluated
                it++;
            }
        }
   it = nextgen.begin();
        while (it!=nextgen.end()) {
            pop.push_back(*it++);
        }
        
        
               cutdown(pop,pop_size); // cutdown to the retained fittest - cutdown expands if necessary with random critters
        // Note cutdown performs a sort, query whether a full sort needed.
        
        if (reEval) cr_data::getInterpreter()->newGenStarted();        
        } while (gensdone++ < gens-1 && (tf ==0 || (*pop.begin())->get_fitness() < tf));
    listthem(pop,os);
    return gensdone;
    }
