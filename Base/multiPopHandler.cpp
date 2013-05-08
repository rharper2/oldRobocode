/*
 *  multiPopHandler.cpp
 *  
 *
 *  Created by Robin Harper on 13/12/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "multiPopHandler.h"
const int GroupSize = 50;

int multiPopHandler::initialisationMethod = 1;

multiPopHandler::multiPopHandler() {
	breeder = NULL;
	ageLimits.push_back(20);
	ageLimits.push_back(40);
	ageLimits.push_back(80);
	ageLimits.push_back(180);
	ageLimits.push_back(320);
	ageLimits.push_back(500);
	ageLimits.push_back(720);
	ageLimits.push_back(980);
	ageLimits.push_back(-1);
    allThePopulations.clear();
}

void multiPopHandler::fileSave(const char *fname) {
	int populations = allThePopulations.size();
	// since this is only really a testbed, I didn't want to make any changes to popHandlerBase
	// so the way I am going to do this is to save the meta (about populations) data to the fname
	// and save each population to a different (concocted) filename
	// it means we will have multiple files (one for each of the populations + 1 containing 
	// other info, but thats ok really.
	char fileName[100];
	if (strlen(fname)>90) {
		cout << "Choose a shorted name for fileSave (or increase the buffer space in multPopHandler, but " << fname << " seems a bit excessive.\n";
		exit(1);
	}
	
	ofstream fout;
	sprintf(fileName,"%s.data",fname);
	fout.open(fileName,ios::out);
	fout << populations;
	fout.close(); // its all we really need, the number of populations.
	for (int i=0;i<populations;i++) {
		sprintf(fileName,"%s%d.pop",fname,i);
		savePop(fileName,( *(allThePopulations[i])));
	}
}

string multiPopHandler::fileLoad(const char *fname) {
	char fileName[100];
	if (strlen(fname)>90) {
		cout << "Choose a shorted name for fileLoad (or increase the buffer space in multPopHandler, but " << fname << " seems a bit excessive.\n";
		exit(1);
	}
	ifstream fin;
	sprintf(fileName,"%s.data",fname);
	cout << "Checking " << fileName << " for number of layers.\n";
	int pops;
	fin >> pops;
	cout << "Loading " << pops << " Layers\n";
	fin.close();
	pops=9;
	if (pops < 0) {
		cout << "Reading a number of populations to load of less than 0 i.e. " << pops << " - this is not sensible.\n";
		exit(1);
	}
	if (pops > 10) {
		cout << "Sensibility check in fileLoad, getting a number of saved populations of " << pops << " which is outside ";
		cout << "my current expected ranges, if its correct you will need to change fileLoad in multiPopHandler.cpp\n";
		exit(1);
	}
	if (pops==0) {
		cout << "Hmm told to load populations, but zero have been saved.\n";
		exit(1);
	}
	allThePopulations.clear(); // ahh the joy of shared pointers.
	for (int i=0;i<pops;i++) {
		populationPtr newPop;
		newPop.reset(new vector<crPtr>);
		allThePopulations.push_back(newPop);
		sprintf(fileName,"%s%d.pop",fname,i);
		cout << "Loading " << fileName << endl;
		loadPop(fileName,(*(allThePopulations[i])));
	}
	return "ok";
}

		
void multiPopHandler::saveFitness(char *fname, int count,bool resort) {
    ofstream of;
    int pos = 0;
	for (int i=allThePopulations.size()-1;i<0;i--) {
		// saving the oldest layer first will make it easier to extract information
		vector<crPtr>::iterator at = (*(allThePopulations[i])).begin();
		if (resort) {
			sort((*(allThePopulations[i])).begin(),(*(allThePopulations[i])).end(),crPtrGreaterThan);
		}
		if (count !=0 ) {
			of.open(fname,ios::app);
		} else {
			of.open(fname);
		}
		of << count << ", " << i << ", " << pos++ << ", ";
		of << (*at)->get_fitness();
		while (at != (*(allThePopulations[i])).end()) {
			of << ", " << (*at)->get_fitness();
			at++;
		}
		of << "\n";
		if (altScore) {
			vector<crPtr>::iterator at = (*(allThePopulations[i])).begin();
			if (resort) {
				sort((*(allThePopulations[i])).begin(),(*(allThePopulations[i])).end(),crPtrGreaterThan);
			}
			of << count << ", " << i << ", " << pos++ << ", ";
			of << (*at)->getAltScore();
			while (at != (*(allThePopulations[i])).end()) {
				of << ", " << (*at)->getAltScore();
				at++;
			}
			of << "\n";
		} 
		
		of << count << ", " << i << ", " << pos++ << ", ";
		at = (*(allThePopulations[i])).begin();
		of <<  (*at)->getUsefulDna();
		while (at != (*(allThePopulations[i])).end()) {
			of << ", " << (*at)->getUsefulDna();
			at++;
		}
		of << "\n";
		of << count << ", " << i << ", " << pos++ << ", ";
		at = (*(allThePopulations[i])).begin();
		of <<  (*at)->getDnaLength();
		while (at != (*(allThePopulations[i])).end()) {
			of << ", " << (*at)->getDnaLength();
			at++;
		}
		of << "\n";
		of << count << ", " << i << ", " << pos++ << ", ";
		at = (*(allThePopulations[i])).begin();
		of << (*at)->getFuncNos();
		//  	  of << at->getMutationRate(mutationRate);
		while (at != (*(allThePopulations[i])).end()) {
			of << ", " << (*at)->getFuncNos();
			//of << ", " <<at->getMutationRate(mutationRate);
			at++;
		}
		of << "\n";
		of << count << ", " << i << ", " << pos++ << ", ";
		at = (*(allThePopulations[i])).begin();
		of << (*at)->getCrossMethod();
		//  	  of << at->getMutationRate(mutationRate);
		while (at != (*(allThePopulations[i])).end()) {
			of << ", " << (*at)->getCrossMethod();
			//of << ", " <<at->getMutationRate(mutationRate);
			at++;
		}
        of << "\n";
		of << count << ", " << i << ", " << pos++ << ", ";
		at = (*(allThePopulations[i])).begin();
		of << (*at)->getMutationRate(mutationRate);
		while (at != (*(allThePopulations[i])).end()) {
			//of << ", " << (*at)->getFuncNos();
			of << ", " << (*at)->getMutationRate(mutationRate);
			at++;
		}
		
		of << "\n";
	}
    of.close();
}




// #define VERBOSERAMPED
void multiPopHandler::rampedHalfandHalf(int minMax,int maxMax) {
	populationPtr thePointer;
	thePointer.reset(new vector<crPtr>);
	allThePopulations.clear();
	allThePopulations.push_back(thePointer);
	rampedHalfandHalf(*thePointer,minMax,maxMax);
}


void multiPopHandler::rampedHalfandHalf(vector<crPtr> &population,int minMax,int maxMax) {
	int dnaPopSize[120];
	int totalSize = 0;
	int duplicates = 0;
	for (int i=0;i<120;i++) dnaPopSize[i]=0;
    int minTree;
	//population.clear();
    population.reserve(popSize+1);
	crPtr tomake;
	bool ok;
	int popToDo = popSize - population.size();
    int diff = maxMax-minMax+1; // i.e. number of instances inclusive
    for (int maxD=minMax;maxD<=maxMax;maxD++) {
		  if (maxD == 2) minTree =1;
		  else minTree = 2;
#ifdef VERBOSERAMPED	
		  cout << "Making one\n";
#endif
		  for (int i=0;i<popToDo/diff;i++) {
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
// 				cout << tomake->display();
				if ( ok = tomake->isValid()) {
					if (tomake->validate()) {
					if (tomake->getUsefulCodonLength() <= 2) {
						cout << "Low codon made!\n";
						if (i%2) cout << "Made with flag true\n"; else cout << "Made with flag false\n";
						cout << "MaxD was " << maxD << " and minTree was " << minTree << endl;
						cout << tomake->display();
						exit(1);
					}
                    if (uniqueInPop(population,tomake)) {
						int pos = tomake->getUsefulDna()/GroupSize;
						if (pos > 119) {
							//cout << "***************Pos was " << pos << "reduced to 119 \n";
							pos = 119;
						}
						if (pos < 0) {
							//cout << "Pos was less than 0 " << pos << "made to equal 0\n";
							pos = 0;
						}
						dnaPopSize[pos]++;
						
						
						totalSize += tomake->getUsefulDna();
						population.push_back(tomake);
						cout << ".";
						if (!(population.size()%50)) cout << endl;
				//		cout << tomake->display();
						
					} else {
						duplicates++;
						ok = false;
					}
					} else {
						//cout << "Failed to validate freshly created individual, scrapping it\n";
						ok=false;
					}
                } else {
                    cout << "Odd ramped half and half failed to make one, retrying\n";
                    if (tomake->isValid()) {
                        cout << "it was valid \n";
                        cout << tomake->display();
                        cout << "Score: " << tomake->get_fitness() << endl;
                    } else cout << "It was not valid , reason: " << tomake->getInvalidReason() << endl;
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
		} while (!(tomake->isValid() && tomake->validate()));
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

// #define VERBOSERAMPED

void multiPopHandler::rampedptc2(int startExp,int endExp,int maxDepth) {
	populationPtr thePointer;
	thePointer.reset(new vector<crPtr>);
	allThePopulations.clear();
	allThePopulations.push_back(thePointer);
	rampedptc2(*thePointer,startExp,endExp,maxDepth);
}

void multiPopHandler::rampedptc2(vector<crPtr> &population,int startExp,int endExp,int maxDepth) {
    
	int dnaPopSize[120];
	int totalSize = 0;
	int duplicates = 0;
	for (int i=0;i<120;i++) dnaPopSize[i]=0;
 //   population.clear();
    population.reserve(popSize+1);
	crPtr tomake;
	bool ok;
	int diff = (endExp-startExp)+1;
	int popToMake = popSize - population.size();
    for (int expansions=startExp;expansions<=endExp;expansions++) {
#ifdef VERBOSERAMPED	
		cout << "Making one\n";
#endif
		for (int i=0;i<popToMake/diff;i++) {
            ok = false;
            while (!ok) {
                tomake.reset(new cr_data);
                bool done = false;
                do {
#ifdef VERBOSERAMPED	
					cout << "To make a critter (" << population.size() << ") with: " << expansions << " expansions, max depth " << maxDepth << "TRUE\n";
					if (population.size() == 7) {
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
				if (( ok = tomake->isValid() /* && tomake->get_fitness()>0*/)) {
					if (tomake->getUsefulCodonLength() <= 2) {
						cout << "Low codon made!\n";
						cout << "Expansions was " << expansions << " and maxDepth was " << maxDepth << endl;
						cout << tomake->display();
						exit(1);
					}
					if (uniqueInPop(population,tomake)) {
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
	#ifdef VERBOSERAMPED
			//			entity::displayCodonInfo = true;
						cout << tomake->display();
	#endif
					} else {
						duplicates++;
						ok = false;
					}
				} else { // failed the isValid test
                    cout << "Odd ptc2 failed to make one, retrying\n";
                    if (tomake->isValid()) {
                        cout << "it was valid \n";
                        cout << tomake->display();
                        cout << "Score: " << tomake->get_fitness() << endl;
                    } else cout << "It was not valid , reason: " << tomake->getInvalidReason() << endl;
                }
            }
        }
	}
	while ( (int) population.size() < popSize) {
#ifdef VERBOSERAMPED	
		cout << "Sofar" << population.size() << " making another quickly.\n";
#endif
		tomake.reset(new cr_data);
		tomake->makeptc2(endExp,maxDepth); // larger trees as more of them
		if (tomake->isValid()) {
			population.push_back(tomake);
#ifdef VERBOSERAMPED
			cout << tomake->display();
#endif
		}
	}
#ifdef VERBOSERAMPED	
	cout << "ptc2, population size " << population.size() << endl;
#endif
	ofstream fout;
	fout.open("ptc2_OutputfromPop.csv",ios::out);
	
	
	
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

}


bool multiPopHandler::uniqueInPop(vector<crPtr> &population,crPtr tomake) {
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

void multiPopHandler::randomise(int maxSize) {
	populationPtr thePointer;
	thePointer.reset(new vector<crPtr>);
	allThePopulations.clear();
	allThePopulations.push_back(thePointer);
	randomise(*thePointer,maxSize);
}


void multiPopHandler::randomise(vector<crPtr> &population,int maxSize) {
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
	int popToDo = popSize - population.size();
	for (int i =0;i<popToDo;i++) {
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
					if (uniqueInPop(population,tomake)) {
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


void multiPopHandler::randomise() {
	populationPtr thePointer;
	thePointer.reset(new vector<crPtr>);
	allThePopulations.clear();
	allThePopulations.push_back(thePointer);
	randomise(*thePointer);
}


void multiPopHandler::randomise(vector<crPtr> &population) {
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
			if (ok = tomake->isValid() && (tomake->getUsefulDna() > 24)) {
				if (uniqueInPop(population,tomake)) {
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

string multiPopHandler::breed(int cycle,int fitness) {
	ostringstream outs;
	if (breeder == NULL) return "ERROR NO BREEDER SELECTED.\n";
	int sizeOfPops = allThePopulations.size();
	for (int i= 0;i<sizeOfPops;i++) {
		if (i==0) {
			breeder->generate(NULL,(&(*(allThePopulations[i]))),cycle,eliteStrategy,popSize,mutationRate,combiningOp,altScore,fitness,outs);
		} else {
			breeder->generate((&(*(allThePopulations[i-1]))),(&(*(allThePopulations[i]))),cycle,eliteStrategy,popSize,mutationRate,combiningOp,altScore,fitness,outs);
		}
		vector<crPtr>::iterator pop;
		if (cr_data::getInterpreter()->newGen()) {
			pop = allThePopulations[i]->begin();
			while (pop!=allThePopulations[i]->end()) {
				(*pop)->reEvaluate(); // notify each critter that it will need to be re-evaluated
				pop++;
			}
		}
	}
	enforceAgeLimit();
	return outs.str();
}

void multiPopHandler::display() {
	cout << "There are " << allThePopulations.size() << " current populations.\n";
	cout << "\n************************\n";
	for (int i=0;i<allThePopulations.size();i++) {
		cout << "Population " << i << " with agelimit " << ageLimits[i]<<endl;
		populationPtr thePop = allThePopulations[i];
		vector<crPtr>::iterator pop;
		pop = thePop->begin();
		while (pop!=thePop->end()) {
			cout << (*pop)->get_fitness() << "[" << (*pop)->age << "], ";
			pop++;
		}
		cout << "\n************************\n";
	}
}

void multiPopHandler::enforceAgeLimit() {
	int latest = allThePopulations.size()-1;
	vector<crPtr>::iterator pop;
	vector<crPtr> *currentPop;
	currentPop = &(*(allThePopulations[latest]));
	pop = currentPop->begin();
	int agelimit = ageLimits[latest];
	if (agelimit != -1) { // the latest layer has an age limit
		while (pop != currentPop->end()) {
			if ( (*pop)->age > agelimit) { // need to create a new layer
					// and put it at the beginning of the age vectors
					populationPtr thePointer;
					thePointer.reset(new vector<crPtr>);
					switch (initialisationMethod) {
				case rampedPTC2:
					rampedptc2(*thePointer,20,40,200);
					break;
				case rampedHalfAndHalf:
					rampedHalfandHalf(*thePointer,2,5);
					break;
				case randomiseIt:
					randomise(*thePointer);
					break;
				default:
					cout << "Initialisation method " << initialisationMethod << " not supported\n";
					exit(1);
				}
//rampedptc2(*thePointer,20,60,200);
					allThePopulations.insert(allThePopulations.begin(),thePointer);
					return;
			} else {
				pop++;
			}
		}
	}
	// so we have dealt with the top layer, if it had critters
	// older than its limit, a new bottom layer has been created
	// and, effectively, all the other layers have moved up one.
	// otherwise, we need to look at the bottom layers and kill
	// (or promote) and critters that are too old.
	latest--;
	while (latest>=0) {
		currentPop = &(*(allThePopulations[latest]));
		pop = currentPop->begin();
		int agelimit = ageLimits[latest];
		populationPtr layerAbove = allThePopulations[latest+1];
		double leastFitInLayerAbove;
		if (layerAbove->size() > 0 ) leastFitInLayerAbove = layerAbove->back()->get_fitness();
		else {
			crPtr lastinThisLayer =((*currentPop)[currentPop->size() -  1]);
			leastFitInLayerAbove = lastinThisLayer->get_fitness();
		}
		while (pop != currentPop->end()) {
			if ((*pop)->age > agelimit) {
				if ((*pop)->get_fitness() > leastFitInLayerAbove) {
					layerAbove->push_back(*pop);
				}
				pop = currentPop->erase(pop);
			} else { pop++; }
		}
		latest--;
	}
	for (int i=0;i<allThePopulations.size();i++) {
			cout << "Sorting layer " << i << " its size is " << allThePopulations[i]->size() << endl;
		    if (allThePopulations[i]->size() > popSize) {
				sort(allThePopulations[i]->begin(),allThePopulations[i]->end(),crPtrGreaterThan); // Do I need a full sort? Or just the shuffle around the top retain?
				allThePopulations[i]->resize(popSize); //assumes sorted
			} else if (allThePopulations[i]->size() < popSize) {
				switch (initialisationMethod) {
				case rampedPTC2:
					rampedptc2((*(allThePopulations[i])),20,40,200);
					break;
				case rampedHalfAndHalf:
					rampedHalfandHalf((*(allThePopulations[i])),2,5);
					break;
				case randomiseIt:
					randomise((*(allThePopulations[i])));
					break;
				default:
					cout << "Initialisation method " << initialisationMethod << " not supported\n";
					exit(1);
				}
				
			}
			cout << "Sorted layer " << i << " its size is " << allThePopulations[i]->size() << endl;

	}
}
					


void multiPopHandler::reEvaluate() {
	for (int i=0;i<allThePopulations.size();i++) {
		vector<crPtr>::iterator pop = allThePopulations[i]->begin();
		while (pop!=allThePopulations[i]->end()) {
			(*pop)->reEvaluate();
			pop++;
		}
	}
}
				
