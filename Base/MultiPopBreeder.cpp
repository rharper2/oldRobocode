/*
 *  MultiPopBreeder.cpp
 *  GEtestbed
 *
 *  Created by Robin Harper on 28/09/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
// THIS IS A VERSION OF TOURNAMENT BREEDER WHICH HAS ALL SORTS OF CODE
// TO TRY AND GATHER INFORMATION ABOUT THE DESTRUCTIVE/CONSTRUCTIVE
// EFFECT OF MUTATING CHILDREN
// I NEVER GATHERED ANY CONVINING EMPIRICAL EVIDENCE OTHER THAN MUTATION
// SEEMED TO DESTROY A LOT MORE THAN IT CREATED BUT THAT STATS
// INDICATED IT IS STILL BETTER TO HAVE AS A WAY OF PREVENTING STAGNATION
// AND LEADING TO AN OVERALL INCREASE IN THE PROBABILITY OF OBTAINING 
// A SOLUTION - I REMOVED THE CODE IN THE CURRENT VERSION OF TOURNAMENT BREEDER
// AS IT MADE THE CODE MORE DIFFICULT TO FOLLOW

// #define VERBOSEOUTPUT

#include "MultiPopBreeder.h"
#include "myrandom.h"
#include "DataCollector.h"
using namespace std;

bool MultiPopBreeder::DoNewGens = true;
// int MultiPopBreeder::variableCrossoverMutateChance = 1000; // i.e. 1 in 10

MultiPopBreeder::MultiPopBreeder() {
    eliminate = false;
    allowself=false;
    tournamentSize = 7;
    
}

MultiPopBreeder::~MultiPopBreeder() { }


void MultiPopBreeder::listthem(vector<crPtr> &pop,ostream &os,bool showAlt) {
    int inlist = pop.size();
    cout << "In list is " << inlist << "\n";
    vector<crPtr>::iterator p=pop.begin();
    if (inlist  > 200) inlist = 200;
    for (int i=0;i<inlist-1;i++) {
        os << (*p)->get_fitness();
		if (showAlt) {
			os << "(" << (*p)->getAltScore() << ")";
			os << "[" << (*p)->getFuncNos() << "]";
		}
		os << ", ";
        p++;
    }
    os << (*p)->get_fitness();
    if (showAlt) os << "(" << (*p)->getAltScore() << ")";
    os << endl;
}


void MultiPopBreeder::cutdown(vector<crPtr> &pop,int retain) {
    // cout << "Cutdown with list size " << pop.size() << " and retain " << retain << "\n";
    sort(pop.begin(),pop.end(),crPtrGreaterThan); // Do I need a full sort? Or just the shuffle around the top retain?
    pop.resize(retain); //assumes sorted
                        // cout << "After resize we have size " << pop.size() << "\n";
}



int MultiPopBreeder::generate(vector<crPtr> *lowerPop,vector<crPtr> *pop,int gens,int retain,int pop_size,int mutate_chance,
                                int op, bool showAlt, int tf, ostream &os) {
    if ((int) pop->size() != pop_size) {
        cout << "Pop size is " << pop->size() << " but pop_size is " << pop_size << endl;
        exit(1);
    }
    for (int i=0;i<pop_size;i++) {
		//        pop[i]->reEvaluate();
        if (!(*pop)[i]->isValid()) {
            cout << "Population member " << i << "is Not VALID!\n";
           // exit(1);
        }
    }
    
    bool mutated;
    vector<crPtr>::iterator it;
    crPtr c1,c2;
    crPtr p1,p2;
	crPtr candidateP;
    nextgen.reserve(2*pop_size+1);
    cutdown((*pop),pop_size); 

    int gensdone = 0;
    do {

        nextgen.clear(); // clear out the temporary store for the next generation
        int count = 0;
        crPtr t1,t2,p1;
		int totalSize;
		int firstSize;
		if (lowerPop != NULL) {
			firstSize = lowerPop->size();
			totalSize = firstSize + pop->size();
		} else {
			totalSize = pop->size();
			firstSize = 0;
		}

	    while (((int) nextgen.size()) < pop_size) {
            count++;
            // choose the first potential parent1 (p1)
			int firstChosen=randomint(totalSize);
			if (firstChosen < firstSize) p1 = (*lowerPop)[firstChosen];
			else p1 = (*pop)[firstChosen-firstSize];
			// and the first potential p2
			int secondChosen = randomint(totalSize);
			if (secondChosen < firstSize) p2 = (*lowerPop)[secondChosen];
			else p2 = (*pop)[secondChosen-firstSize];
			int candidate;
			// tournament select the actual p1.
            for (int i = 0; i < tournamentSize-1;i++) {
                candidate = randomint(totalSize);
				if (candidate < firstSize) {
					if ((*((*lowerPop)[candidate])) > *p1) p1 = (*lowerPop)[candidate];
				} else {
					if ((*((*pop)[candidate-firstSize])) > *p1) p1 = (*pop)[candidate-firstSize];
				}
				// This works as the ">" and "<" operators are overridden for critters
				// and check their fitness taking into account the higherIsBetter flag
            }
            for (int i = 0; i < tournamentSize-1;i++) {
                candidate = randomint(totalSize);
				if (candidate < firstSize) {
					if ((*((*lowerPop)[candidate])) > *p2) p2 = (*lowerPop)[candidate];
				} else {
					if ((*((*pop)[candidate-firstSize])) > *p2) p2 = (*pop)[candidate-firstSize];
				}
				// This works as the ">" and "<" operators are overridden for critters
				// and check their fitness taking into account the higherIsBetter flag
            }
			// paranoid coding
			
			if (!p1 || !p1->isValid()) {
				cout << "Either no parent or an invalid parent has been chosen, this should not happen.\n";
				exit(1);
			}
			if (!p2 || !p2->isValid()) {
				cout << "Either no parent2 or an invalid parent2 has been chosen, this should not happen.\n";
				exit(1);
			}
			if (p1->getUsefulCodonLength() <=2) {
                cout << "A parent with low codon used.\n";
            }
			if (p2->getUsefulCodonLength() <=2) {
                cout << "A parent with low codon used.\n";
            }
			// \paranoia

            mutated = false; // if we havent done the mutation then do it at the end of the switch
                             // cout << "Going to choose " << first << " and then " << second << "\n";
			switch (op) {
                case singlePointBit: 
                    cr_data::singlePointBitOperator(p1,p2,c1,c2);
                    break;
                case singlePointCodon:
                    cr_data::singlePointCodonOperator(p1,p2,c1,c2);
                    break;
                case twoPointReplacement:
                    cr_data::twoPointReplacementOperator(p1,p2,c1,c2);
                    break;
                case gp:
                    cr_data::gpReplacementOperator(p1,p2,c1,c2);
                    break;
               case variableCrossover: // Limited to just the two crossovers just now
                    cr_data::variableCrossover(p1,p2,c1,c2);
                    break;
                case variable3Crossover: // Limited to just the two crossovers just now
                    cr_data::variable3Crossover(p1,p2,c1,c2);
                    break;
                default:
                    cout << "Default operator in PB\n";
					exit(1); // want to catch this.
                    break;
            }
            if (c1 != NULL) {
                if (c1->isValid() && c1->validate()) {
					if (!mutated) {
                        if (mutate_chance > 0) {
                            c1->smartSureMutate(mutate_chance); // temporarily put this with the switch statement
                            if (c1->isValid() && c1->validate()) {
								if (p1->age > p2->age) c1->age = p1->age+1; else c1->age=p2->age+1;
                                nextgen.push_back(c1);
                            } 
                        } else {
							if (p1->age > p2->age) c1->age = p1->age+1; else c1->age=p2->age+1;
							nextgen.push_back(c1); // valid no mutation to be done, push it in.
						}
                    } 
                    else { // i.e. mutated elsewhere
                        if (c1->isValid() && c1->validate()) {
							if (p1->age > p2->age) c1->age = p1->age+1; else c1->age=p2->age+1;
							nextgen.push_back(c1);
						}
                    }
                }
                c1.reset();
            } 
            if (c2 != NULL) {
                if (c2->isValid() && c2->validate()) {
					if (!mutated) {
                        if (mutate_chance > 0) {
                            c2->smartSureMutate(mutate_chance); // temporarily put this with the switch statement
                            if (c2->isValid() && c2->validate()) {
								if (p1->age > p2->age) c2->age = p1->age+1; else c2->age=p2->age+1;
                                nextgen.push_back(c2);
                            } 
                        } else {
							if (p1->age > p2->age) c2->age = p1->age+1; else c2->age=p2->age+1;
							nextgen.push_back(c2); // valid no mutation to be done, push it in.
						}
                    } 
                    else { // i.e. mutated elsewhere
                        if (c2->isValid() && c2->validate()) {
							if (p1->age > p2->age) c2->age = p1->age+1; else c2->age=p2->age+1;
							nextgen.push_back(c2);
						}
                    }
                }
                c2.reset();
            }
        }
        bool reEval=false;
        if (DoNewGens)  reEval = (cr_data::getInterpreter()->newGen());
        pop->resize(retain);
        it = pop->begin();
        while (it!=pop->end()) {
                if (reEval) (*it)->reEvaluate(); // notify each critter that it will need to be re-evaluated
				(*it)->age++;
                it++;
        }
        pop->insert(pop->end(),nextgen.begin(),nextgen.end());
        if ((int) pop->size() < pop_size) {
            cout << "Wow, the population size at the end was only " << pop->size() << " not pop_size " << endl;
            exit(1);
        }
        cutdown((*pop),pop_size); // cutdown to the retained fittest - cutdown expands if necessary with random critters
                               // Note cutdown performs a sort, query whether a full sort needed.
        if (reEval) cr_data::getInterpreter()->newGenStarted();        
    } while (gensdone++ < gens-1 && (tf ==0 || (*(pop->begin()))->get_fitness() < tf));
    listthem((*pop),os,showAlt);
    return gensdone;
}
