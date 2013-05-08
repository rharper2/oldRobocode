/*
 *  TournamentBreeder.cpp
 *  GEtestbed
 *
 *  Created by Robin Harper on 28/09/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

// THERE IS A VERSION OF TOURNAMENT BREEDER IN A FILE CALLED SOMETHING LIKE
// TOURNAMENTBREEDERWITHSTATS.H WHICH CONTAINS A LOT OF DATA GATHERING 
// CODE EXAMINING THE EFFECT OF MUTATION AND THE CROSSOVER


#include "TournamentBreeder.h"
#include "myrandom.h"
#include "DataCollector.h"
using namespace std;

bool TournamentBreeder::DoNewGens = true;

TournamentBreeder::TournamentBreeder() {
    eliminate = false;
    allowself=false;
    tournamentSize = 7;
    
}

TournamentBreeder::~TournamentBreeder() { }


void TournamentBreeder::listthem(vector<crPtr> &pop,ostream &os,bool showAlt) {
    int inlist = pop.size();
    cout << "In list is " << inlist << "\n";
    vector<crPtr>::iterator p=pop.begin();
    if (inlist  > 200) inlist = 200;
    for (int i=0;i<inlist-1;i++) {
		if (!(*p)->isValid()) {
			cerr << "INVALID IN LISTTHEM\n";
		}
	
        os << (*p)->get_fitness();
		if (showAlt) {
			os << "(" << (*p)->getAltScore() << ")";
			os << "[" << (*p)->getFuncNos() << "]";
			os << "{" << (*p)->getCrossMethod()%2 << "}";
		}
		os << ", ";
        p++;
    }
    os << (*p)->get_fitness();
    if (showAlt) os << "(" << (*p)->getAltScore() << ")";
    os << endl;
}


void TournamentBreeder::cutdown(vector<crPtr> &pop,int retain) {
    // cout << "Cutdown with list size " << pop.size() << " and retain " << retain << "\n";
    sort(pop.begin(),pop.end(),crPtrGreaterThan); // Do I need a full sort? Or just the shuffle around the top retain?
    pop.resize(retain); //assumes sorted
    // cout << "After resize we have size " << pop.size() << "\n";
}



int TournamentBreeder::generate(vector<crPtr> &pop,int gens,int retain,int pop_size,int mutate_chance,
                                int op, bool showAlt, int tf, ostream &os) {
    if ((int) pop.size() != pop_size) {
        cout << "Pop size is " << pop.size() << " but pop_size is " << pop_size << endl;
        exit(1);
    }
    for (int i=0;i<pop_size;i++) {
		//        pop[i]->reEvaluate();
        if (!pop[i]->isValid()) {
            cout << "Population member " << i << "is Not VALID!\n";
			// sometimes (depending on the operator) non-valid children being created might be an error
           // exit(1);
        }
    }
    
    dataCollector *dc = dataCollector::Instance();
    dc->resetCounters();
    bool mutated;
    vector<crPtr>::iterator it;
    crPtr c1,c2;
    crPtr p1,p2;
    nextgen.reserve(pop_size+3); // really only need pop_size+1, but what the heck..
    cutdown(pop,pop_size); 
    int gensdone = 0;
    do {
        nextgen.clear(); // clear out the temporary store for the next generation
        int count = 0;
        crPtr t1,t2,p1;
        
        while (((int) nextgen.size()) < pop_size) {
            count++;
            int firstChosen=pop_size;
            int secondChosen=pop_size;
            int canditate;
            for (int i = 0; i < tournamentSize;i++) {
                canditate = randomint(pop_size);
                if ((canditate < firstChosen) && pop[canditate]->isValid()) firstChosen = canditate;
            }
            for (int i = 0; i < tournamentSize;i++) {
                canditate = randomint(pop_size);
                if ((canditate < secondChosen) && pop[canditate]->isValid()) secondChosen = canditate;
            }
			if (firstChosen == pop_size) {
				cout << "Odd couldn't chose a first candidate, must be riddled with invalid choices\n";
				return 0;
			}
			if (secondChosen == pop_size) {
				cout << "Odd couldn't choose a second candidate, must be riddled with invalid choices\n";
				return 0;
			}
            if (pop[firstChosen]->getUsefulCodonLength() <=2) {
                cout << "A parent with low codon used.\n";
				// This might indicate something has really gone wrong
				// or the population might have converged to a very simple
				// creature
            }
			if (pop[secondChosen]->getUsefulCodonLength() <=2) {
                cout << "A parent with low codon used.\n";
            }
           mutated = false; // if we havent done the mutation then do it at the end of the switch
           // cout << "Going to choose " << first << " and then " << second << "\n";
		   switch (op) {
                case singlePointBit: 
                    cr_data::singlePointBitOperator(p1=pop[firstChosen],p2=pop[secondChosen],c1,c2);
                    break;
                case singlePointCodon:
                    cr_data::singlePointCodonOperator(p1=pop[firstChosen],p2=pop[secondChosen],c1,c2);
                    break;
// The commented out blocks represent choices I once tried,
// but currently I restrict my crossover choice.

/*                case singlePointMatched:
                    cr_data::singlePointMatchedOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case headlessBit:
                    cr_data::headlessBitOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case taillessBit:
                    cr_data::taillessBitOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case twoPointBit: 
                    cr_data::twoPointBitOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case twoPointCodon:
                    cr_data::twoPointCodonOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case twoPointMatched:
                    cr_data::twoPointMatchedOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
 */
                case twoPointReplacement: // this was a precursor to gp - some additional code to alert me
										  // if this is creating small children - it really shouldn't 
										  
                    cr_data::twoPointReplacementOperator(p1=pop[firstChosen],p2=pop[secondChosen],c1,c2);
                    if ((c1 && c1->isValid() && c1->getUsefulCodonLength() <= 2) || 
                        (c2 &&c2->isValid() && c2->getUsefulCodonLength() <= 2) ) {
                        cout << "Short child from Two Point\n";
                        cout << pop[firstChosen]->display();
                        cout << pop[secondChosen]->display();
                        if (c1) cout << c1->display();
                        else cout << "C1 not valid\n";
                        if (c2) cout << c2->display();
                        else cout << "C2 not valid\n";
                        exit(1);
                    }
                        break;

// Saga is an interesting operator, I might return to it one day.
/*             case twoPointRSaga:
                    cr_data::twoPointReplacementSAGAOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case twoPointGutless:
                    cr_data::twoPointGutlessOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
*/

                case gp:
                    cr_data::gpReplacementOperator(p1=pop[firstChosen],p2=pop[secondChosen],c1,c2);
                    break;
                case variableCrossover: // Limited to just the two crossovers just now
                    cr_data::variableCrossover(p1=pop[firstChosen],p2=pop[secondChosen],c1,c2);
                    break;
                case variable3Crossover: // Limited to just the two crossovers just now
                    cr_data::variable3Crossover(p1=pop[firstChosen],p2=pop[secondChosen],c1,c2);
                    break;
/*
                case randomCrossover:
                    cr_data::randomCrossover(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case uniformCrossover:
                    cr_data::uniformCodonOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
                case mutateOnly:
                    cr_data::mutateOnly(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
*/
                default:
                    cout << "Default operator in PB\n";
                    cr_data::singlePointCodonOperator(pop[firstChosen],pop[secondChosen],c1,c2);
                    break;
            }

            if (c1 != NULL) {
                if (!c1->isValid()) {
				//	                    cout << c1->getInvalidReason();
				//	                    cout << "Invalid child created?\n";
					// I left this in in case you want to find out why a particular operator is creating invalid
					// children (if it is).
                } else { // so we know its valid

// NOTE I am only mutating valid children, which might not be right
// some operators will create invalid children - do I want to mutate them
// in the hope they become valid?

                    if (!mutated) { // and that we have not mutated it.
                        if (mutate_chance > 0) {
                            c1->smartSureMutate(mutate_chance); 
							// There are other mutations you can use. Such as:
                            //c1->qmutate(mutate_chance); 
                            if (c1->isValid()) { // is it still valid after mutation?

							    // **** this section isn't strictly necessary but it allowed me to catch a silly error once.
                                c1->reEvaluate(); // ok lets get its score
                                if (!c1 || !c1->isValid()) { // something has gone wrong if its no longer valid - array overrun perhaps?
                                    cout << "Trying to push back non-valid child, c1.\n";
                                    exit(1);
                                }
								// *** end of redundant code

                                nextgen.push_back(c1);
                            } else {
								// okay it became invalid when we mutated it, you might want to do something here.
                            }
                        } else {
                            // mutate chance = 0
                            if (!c1->isValid()) {
                                cout << "Trying to push back non-valid child1 (Mutate = 0).\n";
                                exit(1);
                            }
                            nextgen.push_back(c1); // we haven't checked its score yet, but thats ok as it will be checked when its sorted
							// (Where we checked its score above - it won't be double checked, the score is remembered)
                        }
                    } // end of if !mutated
                    else { // i.e. mutated elsewhere
                        if (c1->isValid()) nextgen.push_back(c1);
                    }
                }
                c1.reset();
            } 
            if (c2 != NULL) {
                if (!c2->isValid()) {
					//                 cout << "Invalid child (child2) created?\n";
					//                 cout << c2->getInvalidReason();
                } else {
                    if (!mutated) {
                        if(mutate_chance > 0) {
                            c2->smartSureMutate(mutate_chance);
                            //  c2->qmutate(mutate_chance);
                          if (c2->isValid()) {

                                c2->reEvaluate();
                                if (!c2 || !c2->isValid()) {
                                    cout << "Trying to push back non-valid child2.\n";
                                    exit(1);
                                }

                                nextgen.push_back(c2);
                            } else {
                            }
                        }
                        else {
                            if (!c2->isValid()) {
                                cout << "Trying to push back non-valid child2.\n";
                                exit(1);
                            }
                            nextgen.push_back(c2);
                        }
                    } // end of if !mutated
                    else { // i.e. mutated elsewhere
                        if (c2->isValid()) nextgen.push_back(c2);
                        
                    }
                }
                c2.reset();
            }
        }
        bool reEval=false;
		// If the interpreter requires a re-evaluation every generation then we need to do taht
		// This might be the case where the testing data changes every so often.
		
        if (DoNewGens)  reEval = (cr_data::getInterpreter()->newGen());
        pop.resize(retain);
        // okay we might be retaining some "elites" from the original.
		if (reEval)  {
            it = pop.begin();
            while (it!=pop.end()) {
                (*it)->reEvaluate(); // notify each critter that it will need to be re-evaluated
                it++;
            }
        }
//		cout << "Retained " << pop.size();
        pop.insert(pop.end(),nextgen.begin(),nextgen.end());
//		cout << " merged in the children " << pop.size();
        if ((int) pop.size() < pop_size) {
            cout << "Wow, the population size at the end was only " << pop.size() << " not pop_size " << endl;
            exit(1);
        }
        cutdown(pop,pop_size); // cutdown to the retained fittest - cutdown expands if necessary with random critters
                               // Note cutdown performs a sort, query whether a full sort needed.
//        cout << " and trimmed down to " << pop.size() << endl;
        if (reEval) cr_data::getInterpreter()->newGenStarted();        
    } while (gensdone++ < gens-1 && (tf ==0 || (*pop.begin())->get_fitness() < tf));
    listthem(pop,os,showAlt);
    return gensdone;
}
