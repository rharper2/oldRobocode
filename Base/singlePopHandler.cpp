/*
 *  singlePopHandler.cpp
 *  
 *
 *  Created by Robin Harper on 13/12/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "singlePopHandler.h"
const int GroupSize = 50;

singlePopHandler::singlePopHandler() {
	breeder = NULL;
    population.clear();
}

//#define VERBOSERAMPED
void singlePopHandler::rampedHalfandHalf(int minMax,int maxMax) {
//	static int mycount = 0;
	int dnaPopSize[120];
	int totalSize = 0;
	int duplicates = 0;
	for (int i=0;i<120;i++) dnaPopSize[i]=0;
    int minTree;
	population.clear();
    population.reserve(popSize+1);
	crPtr tomake;
	bool ok;
    int diff = maxMax-minMax+1; // i.e. number of instances inclusive
    for (int maxD=minMax;maxD<=maxMax;maxD++) {
		  if (maxD == 2) minTree =1;
		  else minTree = 2;
#ifdef VERBOSERAMPED	
		  cout << "Making one\n";
#endif
		  for (int i=0;i<popSize/diff;i++) {
            ok = false;
            while (!ok) {
                tomake.reset(new cr_data);
                if (i%2) {
					bool done = false;
                    do {
#ifdef VERBOSERAMPED	
						cout << "To make: " << maxD << ", " << minTree << "TRUE\n";
#endif
                        tomake->makeValid(maxD,minTree,true);
						done = tomake->isValid();
						if (!done) {
							cout << tomake->getInvalidReason() << endl;
	//						exit(1);
						}
                    } while (!done);
                } else {
                    do {
#ifdef VERBOSERAMPED	
						cout << "To make: " << maxD << ", " << minTree << "FALSE\n";
#endif
                        tomake->makeValid(maxD,minTree,false);
                    } while (!tomake->isValid() && (tomake->getUsefulDna() > 24));
                }
 	//			if (tomake->isValid()) cout << tomake->display()<<endl;
				if (( ok = tomake->isValid()  && tomake->validate())) {
					if (tomake->getUsefulCodonLength() <= 2) {
						cout << "Low codon made!\n";
						if (i%2) cout << "Made with flag true\n"; else cout << "Made with flag false\n";
						cout << "MaxD was " << maxD << " and minTree was " << minTree << endl;
						cout << tomake->display();
						exit(1);
					}
                    if (uniqueInPop(tomake)) {
						int pos = tomake->getUsefulDna()/GroupSize;
						if (pos > 119) {
							cout << "***************Pos was " << pos << "reduced to 119 \n";
							pos = 119;
						}
						if (pos < 0) {
							cout << "Pos was less than 0 " << pos << "made to equal 0\n";
							pos = 0;
						}
						dnaPopSize[pos]++;
						
						
						totalSize += tomake->getUsefulDna();
						population.push_back(tomake);
	//					cout << tomake->display();
						
					} else {
						duplicates++;
						ok = false;
					}
                } else {
        //            cout << "Odd ramped half and half failed to make one, retrying\n";
        //            if (tomake->isValid()) {
         //               cout << "it was valid \n";
          //              cout << tomake->display();
          //              cout << "Score: " << tomake->get_fitness() << endl;
         //           } else cout << "It was not valid , reason: " << tomake->getInvalidReason() << endl;
                }
    //            char ch;
    //            cin >> ch;
            }
        }
    }
    int sofar = population.size();
    for (int i=0;i<(popSize - sofar);i++) {
#ifdef VERBOSERAMPED	
		cout << "Sofar" << sofar << " making another quickly.\n";
#endif
        do {
			tomake.reset(new cr_data);
			tomake->makeValid(minMax,minTree);
		} while (!(tomake->isValid()&&tomake->validate()));
        population.push_back(tomake);
    }
#ifdef VERBOSERAMPED	
    cout << "Ramped half and half, population size " << population.size() << endl;
#endif
   /* vector<crPtr>::iterator p=population.begin();
    sort(population.begin(),population.end(),crPtrGreaterThan); // Do I need a full sort? Or just the shuffle around the top retain?

    int inlist = population.size();
    if (inlist  > 200) inlist = 200;
    for (int i=0;i<inlist-1;i++) {
        cout << (*p)->get_fitness() << ", ";
        p++;
    }
    cout << (*p)->get_fitness() << "\n";
*/
	ofstream fout;
 	fout.open("HandH_OutputfromPop.csv",ios::out);



    fout << "Average dna size " << totalSize/popSize << endl;
	fout << "Duplicates: " << duplicates << endl;
    for (int i=0;i<120;i++)  {
		fout  << i*GroupSize << " , " << (i+1)*GroupSize-1 << " , " << dnaPopSize[i] << endl;
	}
#ifdef VERBOSERAMPED	
	for (int i=0;i<120;i++)  {
		cout << "Dna Bracket " << i*GroupSize << " to " << (i+1)*GroupSize-1 << " was: " << dnaPopSize[i] << endl;
	}
#endif
	const long *codons;
		vector<crPtr>::iterator at = population.begin();
	fout << "Length of whole tree, length of lhs, length of rhs ";
	while (at != population.end()) {
		codons = (*at)->getCodons();
		long len1 = (codons[0] >> (CODON + LHSINFO)) & EXPRMASK;
		long len2 = (codons[1] >> (CODON + LHSINFO)) & EXPRMASK;
		long len3 = (codons[1+len2] >> (CODON +LHSINFO)) & EXPRMASK;
		fout << len1 << ", " << len2 << "," << len3 << endl;
		at++;
	}

    fout.close();
//  cout << (*population.begin())->display();
#ifdef VERBOSERAMPED	
  	for (int i=0;i<120;i++)  {
		cout << "Dna Bracket " << i*GroupSize << " to " << (i+1)*GroupSize-1 << " was: " << dnaPopSize[i] << endl;
	}
#endif
}

//#define VERBOSEPTC2

void singlePopHandler::rampedptc2(int startExp,int endExp,int maxDepth) {
    
	int dnaPopSize[120];
	int totalSize = 0;
	int duplicates = 0;
	for (int i=0;i<120;i++) dnaPopSize[i]=0;
    population.clear();
    population.reserve(popSize+1);
	crPtr tomake;
	bool ok;
	int diff = (endExp-startExp)+1;
    for (int expansions=startExp;expansions<=endExp;expansions++) {
#ifdef VERBOSEPTC2	
		cout << "Making one\n";
#endif
		for (int i=0;i<popSize/diff;i++) {
            ok = false;
            while (!ok) {
                tomake.reset(new cr_data);
                bool done = false;
                do {
#ifdef VERBOSEPTC2	
					cout << "To make a critter (" << population.size() << ") with: " << expansions << " expansions, max depth " << maxDepth << "TRUE\n";
					if (population.size() == 48) {
					  cout << "Stop here \n";
					}
#endif
					tomake->makeptc2(expansions,maxDepth);
					done = tomake->isValid();
					if (!done) {
						cout << tomake->getInvalidReason() << endl;
						//						exit(1);
					}
                } while (!done);
				if (( ok = tomake->isValid()  && tomake->validate())) {
					if (tomake->getUsefulCodonLength() <= 2) {
						cout << "Low codon made!\n";
						cout << "Expansions was " << expansions << " and maxDepth was " << maxDepth << endl;
						cout << tomake->display();
						exit(1);
					}
					if (uniqueInPop(tomake)) {
						int pos = tomake->getUsefulDna()/GroupSize;
						if (pos > 119) {
							cout << "***************Pos was " << pos << "reduced to 119 \n";
							pos = 119;
						}
						if (pos < 0) {
							cout << "Pos was less than 0 " << pos << "made to equal 0\n";
							pos = 0;
						}
						dnaPopSize[pos]++;
						
						
						totalSize += tomake->getUsefulDna();
						population.push_back(tomake);
						cout << ".";
	//					cout << tomake->display();
	#ifdef VERBOSEPTC2
			//			entity::displayCodonInfo = true;
						cout << tomake->display();
	#endif
					} else {
						duplicates++;
						ok = false;
					}
				} else { // failed the isValid test
              //      cout << "Odd ptc2 failed to make one, retrying\n";
                    if (tomake->isValid()) {
               //         cout << "it was valid \n";
               //         cout << tomake->display();
               //         cout << "Score: " << tomake->get_fitness() << endl;
                    } else cout << "It was not valid , reason: " << tomake->getInvalidReason() << endl;
                }
            }
        }
	}
	while ( (int) population.size() < popSize) {
#ifdef VERBOSEPTC2	
		cout << "Sofar" << population.size() << " making another quickly.\n";
#endif
		tomake.reset(new cr_data);
		tomake->makeptc2(endExp,maxDepth); // larger trees as more of them
		if (tomake->isValid() && tomake->validate()) {
			population.push_back(tomake);
#ifdef VERBOSEPTC2
			cout << tomake->display();
#endif
		}
	}
#ifdef VERBOSEPTC2	
	cout << "PTC2, population size " << population.size() << endl;
#endif
	ofstream fout;
	fout.open("PTC2_OutputfromPop.csv",ios::out);
	
	
	
	fout << "Average dna size " << totalSize/popSize << endl;
	fout << "Duplicates: " << duplicates << endl;
	for (int i=0;i<120;i++)  {
		fout  << i*GroupSize << " , " << (i+1)*GroupSize-1 << " , " << dnaPopSize[i] << endl;
	}
#ifdef VERBOSEPTC2	
	for (int i=0;i<120;i++)  {
		cout << "Dna Bracket " << i*GroupSize << " to " << (i+1)*GroupSize-1 << " was: " << dnaPopSize[i] << endl;
	}
#endif
	const long *codons;
	vector<crPtr>::iterator at = population.begin();
	fout << "Length of whole tree, length of lhs, length of rhs ";
	while (at != population.end()) {
		codons = (*at)->getCodons();
		long len1 = (codons[0] >> (CODON + LHSINFO)) & EXPRMASK;
		long len2 = (codons[1] >> (CODON + LHSINFO)) & EXPRMASK;
		long len3 = (codons[1+len2] >> (CODON +LHSINFO)) & EXPRMASK;
		fout << len1 << ", " << len2 << "," << len3 << endl;
		at++;
	}
	
	fout.close();
	//  cout << (*population.begin())->display();

}


bool singlePopHandler::uniqueInPop(crPtr tomake) {
	int noInVec = population.size();
	if (!tomake) {
		cout << "No tomake?\n";
	}
	int myExpressionL = tomake->getExpressionLength();
	int *tomakeExp = tomake->getExpression();
	crPtr crCheck;
	vector<function> *madeFunc = tomake->getFunctions();
	for (int i=0;i<noInVec;i++) {
		crCheck = population[i];
		if (crCheck->getUsefulDna() != tomake->getUsefulDna()) continue;
		if (crCheck->getFuncNos() != tomake->getFuncNos()) continue;
		int vecEL = crCheck->getExpressionLength();
		if (myExpressionL != vecEL) continue;
		int *vecExp = crCheck->getExpression();
		bool different = false;
		vector<function> *checkFunc = crCheck->getFunctions();
		for (int fcount =0;fcount < crCheck->getFuncNos();fcount++) {
			int cf = (*checkFunc)[fcount].expLength;
			int mf = (*madeFunc)[fcount].expLength;
			if  (cf != mf) {
				different = true;
				break;
			} else {
				for (int co=0;co<cf;co++) {
					if ( ((*checkFunc)[fcount].expression)[co] != ((*madeFunc)[fcount].expression)[co]) {
						different = true;
						break;
					}
				}
			}
			if (different) {
				break;
			}	
		}	
		
		for (int co=0;co<vecEL;co++) {
			if (tomakeExp[co] != vecExp[co]) {
				different = true;
				break;
			}
		}
/*
	// uncomment if you want to check its working properly.
		if (!different) {
			cout << "These appear to be the same\n";
			cout << tomake->display();
			cout << "And this one.\n";
			cout << crCheck->display();
			return false;
		}
*/
	}
	return true;
}

void singlePopHandler::randomise(int maxSize) {
	int categorySize = 500;
	int minSize = 2000;
    int categories = (maxSize-minSize)/categorySize;
	int maxPerCategory = popSize/categories +1;
	int dnaPopSize[categories];
	for (int i=0;i<categories;i++) dnaPopSize[i]=0;
	population.clear();
    population.reserve(popSize+1);
	crPtr tomake;
	bool ok;
	int duplicates;
	int count =0;
    int ivcount = 0;
	int tooSmall = 0;
    long totalSize = 0;
	long categoryfull = 0;
	for (int i =0;i<popSize;i++) {
		ok = false;
		do {
            count++;
			tomake.reset(new cr_data());
			int critterSize;
			int critterCategory;
			if (tomake->isValid() && ((critterSize = tomake->getUsefulDna()) > minSize)) {
				ok= false;
				//cout << "Rolled one with a dna of " << tomake->getUsefulDna() << " = " << critterSize << endl;
				critterCategory = (critterSize-minSize)/categorySize;
				if (critterCategory >= categories) { 
					ok = false;
					categoryfull++;
					//critterCategory = categories-1; 
				}
			    else if (dnaPopSize[critterCategory] < maxPerCategory) { // so we have room in that category
					if (uniqueInPop(tomake)) {
					    if (! (i%50)) {
							cout << "50 for a total of " << i << " randomly created\n";
						}
						totalSize += tomake->getUsefulDna();
						population.push_back(tomake);
                        cout << tomake->display();

						ok=true;
						dnaPopSize[critterCategory]++;
					} else { 
						// not unique
						ok = false;
						duplicates++;
					}
				} else {
				//    cout << "Category " << critterCategory << " is full\n";
				    ok = false;
					categoryfull++;
				}
            }
            else {
                if (tomake->isValid()) {
                    if (tomake->getUsefulDna() <= 24) cout << "Failed on too small useful dna.\n";
                    tooSmall++;
                } else {
                    cout << "One failed to be valid, " << tomake->getInvalidReason() << "\n";   
                    ivcount++;
                }
            }
			
        } while (!ok);
		//        cout << i << " created. " << endl;
	}
//	sort(population.begin(),population.end(),crPtrGreaterThan);
	ofstream fout;
 	fout.open("OutputfromPop.csv",ios::out);
	
	
	
//    cout << "Took " << count << " tries to generate " << popSize << "critters thats: " << (((double) count) /(double) popSize) << " tries per.\n";
//    cout << "Invalid " << ivcount << endl;
//    cout << "Size failed " << tooSmall << "\n";
//    cout << "Average dna size " << totalSize/popSize << endl;
//	cout << "Duplicates: " << duplicates << endl;
	fout << "Took " << count << " tries to generate " << popSize << "critters thats: " << (((double) count) /(double) popSize) << " tries per.\n";
    fout << "Invalid " << ivcount << endl;
    fout << "Size failed " << tooSmall << "\n";
    fout << "Average dna size " << totalSize/popSize << endl;
	fout << "Duplicates: " << duplicates << endl;
	fout << "Category full: " << categoryfull << endl;
	int sizes[200];
	for (int i=0;i<199;i++) sizes[i]=0;
	vector<crPtr>::iterator at = population.begin();
	while (at != population.end()) {
		int len = (*at)->getUsefulDna()/50;
		if (len > 199) len = 199;
		sizes[len]++;
		at++;
	}
	
	
   for (int i=0;i<199;i++)  {
		fout  << i*50 << " , " << (i+1)*50-1 << " , " << sizes[i] << endl;
	}
	for (int i=0;i<120;i++)  {
//		cout << "Dna Bracket " << i*GroupSize << " to " << (i+1)*GroupSize-1 << " was: " << dnaPopSize[i] << endl;
	}
	const long *codons;
	at = population.begin();
	fout << "Length of whole tree, length of lhs, length of rhs ";
	while (at != population.end()) {
		codons = (*at)->getCodons();
		long len1 = (codons[0] >> (CODON + LHSINFO)) & EXPRMASK;
		long len2 = (codons[1] >> (CODON + LHSINFO)) & EXPRMASK;
		long len3 = (codons[1+len2] >> (CODON +LHSINFO)) & EXPRMASK;
		fout << len1 << ", " << len2 << "," << len3 << endl;
		at++;
	}
    fout.close();
	
}


void singlePopHandler::randomise() {
	int dnaPopSize[200];
	for (int i=0;i<199;i++) dnaPopSize[i]=0;
	population.clear();
    population.reserve(popSize+1);
	crPtr tomake;
	bool ok;
	int duplicates;
	int count =0;
    int ivcount = 0;
	int tooSmall = 0;
    long totalSize = 0;
	for (int i =0;i<popSize;i++) {
		ok = false;
		do {
            count++;
			tomake.reset(new cr_data());
			if ((ok = tomake->isValid() && (tomake->getUsefulDna() > 24))) {
				if (uniqueInPop(tomake)) {
//					cout << "One randomly created with dna size of " << tomake->getUsefulDna() << "\n";
					totalSize +=tomake->getUsefulDna();
					population.push_back(tomake);
					//			cout << tomake->display();
					int pos = tomake->getUsefulDna()/GroupSize;
					if (pos > 199) {
						cout << "***************Pos was " << pos << "reduced to 199 \n";
						pos = 199;
					}
					if (pos < 0) {
						cout << "Pos was less than 0 " << pos << "made to equal 0\n";
						pos = 0;
					}
					dnaPopSize[pos]++;
					//cout << tomake->display();
				} else 
				{ 
					// not unique
					ok = false;
					duplicates++;
				}
            }
            else {
                if (tomake->isValid()) {
  //                  if (tomake->getUsefulDna() <= 24) cout << "Failed on too small useful dna.\n";
                    tooSmall++;
                } else {
  //                  cout << "One failed to be valid, " << tomake->getInvalidReason() << "\n";   
                    ivcount++;
                }
            }
			
        } while (!ok);
		//        cout << i << " created. " << endl;
	}
//	sort(population.begin(),population.end(),crPtrGreaterThan);
	ofstream fout;
 	fout.open("OutputfromPop.csv",ios::out);
	
	
	
//    cout << "Took " << count << " tries to generate " << popSize << "critters thats: " << (((double) count) /(double) popSize) << " tries per.\n";
//    cout << "Invalid " << ivcount << endl;
//    cout << "Size failed " << tooSmall << "\n";
//    cout << "Average dna size " << totalSize/popSize << endl;
//	cout << "Duplicates: " << duplicates << endl;
	fout << "Took " << count << " tries to generate " << popSize << "critters thats: " << (((double) count) /(double) popSize) << " tries per.\n";
    fout << "Invalid " << ivcount << endl;
    fout << "Size failed " << tooSmall << "\n";
    fout << "Average dna size " << totalSize/popSize << endl;
	fout << "Duplicates: " << duplicates << endl;
   for (int i=0;i<120;i++)  {
		fout  << i*GroupSize << " , " << (i+1)*GroupSize-1 << " , " << dnaPopSize[i] << endl;
	}
	for (int i=0;i<120;i++)  {
//		cout << "Dna Bracket " << i*GroupSize << " to " << (i+1)*GroupSize-1 << " was: " << dnaPopSize[i] << endl;
	}
    fout.close();
	
}

string singlePopHandler::breed(int cycle,int fitness) {
	if (breeder == NULL) return "ERROR NO BREEDER SELECTED.\n";
	vector<crPtr>::iterator pop;
	ostringstream outs;
	if (cr_data::getInterpreter()->newGen()) {
			pop = population.begin();
			while (pop!=population.end()) {
				(*pop)->reEvaluate(); // notify each critter that it will need to be re-evaluated
				pop++;
			}
	}
	breeder->generate(population,cycle,eliteStrategy,popSize,mutationRate,combiningOp,altScore,fitness,outs);
	return outs.str();
}

void singlePopHandler::reEvaluate() {
	vector<crPtr>::iterator pop = population.begin();
	while (pop!=population.end()) {
		(*pop)->reEvaluate();
		pop++;
	}
}
				
