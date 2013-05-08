/*
 *  popHandlerBase.cpp
 *  
 *
 *  Created by Robin Harper on 15/11/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "popHandlerBase.h"
#include <list>

using namespace std;

popHandlerBase::popHandlerBase() {
 popSize = 200;
 eliteStrategy = 20;
 mutationRate = 200;
 combiningOp = 3;
 altScore = false;
}


void popHandlerBase::streamout(ostream &pout,crPtr &item) {
 int len;
 pout << (len=item->getDnaLength()) << ' ';
 for (int i=0;i<len;i++) {
  pout << item->getDna()[i] << ' ';
 }
 pout << "-1\n";
}


void popHandlerBase::savePop(const char *currFileName,vector<crPtr> &population)
{
 ofstream fout;
 if (population.size() == 0) {
    cout << "Nothing to save\n";
    return;
 }
 fout.open(currFileName,ios::out);
 vector<crPtr>::iterator it = population.begin();
 while (it != population.end()) {
  streamout(fout,*it);
  it++;
 };
 fout.close();
}


/* Need to change the cr_data system so it takes and claims ownership of the dna
   passed to it, save me copying over the dna passed. */
   
string popHandlerBase::loadPop(const char *currFileName,vector<crPtr> &population) {
 ifstream fin(currFileName);
  cout << "Opened file\n";
 ostringstream outs;
 if (!fin) {
  cout <<  "Error trying to open " << currFileName << " for loading\n";
  return outs.str();
 } else { // the following isnt very robust if we dont have a well formed file
  population.clear();
  long dnaL;
  int *dna;
  int j;
  int count = 0;
  while (fin) {
   // playing with the idea of a cr_data constructor cr_data(fin) but not done as
   // alerting re ill-formed critters might be tricky in gui 
   fin >> dnaL;
   if (!fin) break; 
   if (dnaL<10 || dnaL>80000) {
    outs << "Encountered an unusual dna length of " << dnaL << "\n";
   }
   dna = new int[dnaL];
 //  cout << "Read dnaL as " << dnaL << "- count" << count << "\n";
   for (int i=0;i<dnaL;i++) {
    fin >> dna[i];
    }
   do {
    fin >> j;
   } while (fin && j != -1);
   count++;
   crPtr encap(new cr_data(dna,dnaL));
   population.push_back(encap);
   delete [] dna;
  }
  fin.close();
  outs << "OK";
  return outs.str();
 } 
}

void popHandlerBase::fitnessToSave(char *fname,vector<crPtr> &population,int count,bool toSort) {
    ofstream of;
    int pos = 0;
    vector<crPtr>::iterator at = population.begin();
    if (toSort) {
        sort(population.begin(),population.end(),crPtrGreaterThan);
    }
    if (count !=0 ) {
        of.open(fname,ios::app);
    } else {
        of.open(fname);
    }
    of << count << ", " << pos++ << ", ";
    of << (*at)->get_fitness();
    while (at != population.end()) {
        of << ", " << (*at)->get_fitness();
        at++;
    }
    of << "\n";
    if (altScore) {
        vector<crPtr>::iterator at = population.begin();
        if (toSort) {
            sort(population.begin(),population.end(),crPtrGreaterThan);
        }
        of << count << ", " << pos++ << ", ";
        of << (*at)->getAltScore();
        while (at != population.end()) {
            of << ", " << (*at)->getAltScore();
            at++;
        }
        of << "\n";
	} 
    
    of << count << ", " << pos++ << ", ";
    at = population.begin();
    of <<  (*at)->getUsefulDna();
    while (at != population.end()) {
        of << ", " << (*at)->getUsefulDna();
        at++;
    }
    of << "\n";
    of << count << ", " << pos++ << ", ";
    at = population.begin();
    of <<  (*at)->getDnaLength();
    while (at != population.end()) {
        of << ", " << (*at)->getDnaLength();
        at++;
    }
    of << "\n";
    of << count << ", " << pos++ << ", ";
    at = population.begin();
    of << (*at)->getFuncNos();
    //  	  of << at->getMutationRate(mutationRate);
    while (at != population.end()) {
        of << ", " << (*at)->getFuncNos();
        //of << ", " <<at->getMutationRate(mutationRate);
        at++;
    }
    of << "\n";
    of << count << ", " << pos++ << ", ";
    at = population.begin();
    of << (*at)->getCrossMethod();
    //  	  of << at->getMutationRate(mutationRate);
    while (at != population.end()) {
        of << ", " << (*at)->getCrossMethod();
        //of << ", " <<at->getMutationRate(mutationRate);
        at++;
    }
        of << "\n";
    of << count << ", " << pos++ << ", ";
    at = population.begin();
    of << (*at)->getMutationRate(mutationRate);
    while (at != population.end()) {
        //of << ", " << (*at)->getFuncNos();
        of << ", " << (*at)->getMutationRate(mutationRate);
        at++;
    }

    of << "\n";
    of.close();
}





