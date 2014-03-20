//
//  DeltaWrappedCritter.cpp
//  Robocode
//
//  Created by Robin Harper on 14/03/2014.
//
//

#include "DeltaWrappedCritter.h"

long DeltaWrappedCritter::id_count = 0;


namespace dWCritter {
    void streamout(ostream &pout,crPtr &item) {
        int len;
        pout << (len=item->getDnaLength()) << ' ';
        for (int i=0;i<len;i++) {
            pout << item->getDna()[i] << ' ';
        }
        pout << "-1\n";
    }
}

DeltaWrappedCritter::DeltaWrappedCritter(bool make) {
    storedScore = 0;
    theId  = id_count++;
    if (make) {
		int maxTree;
		bool done = false;
		do {
			maxTree = randomint(8)+2;
			critter.reset(new cr_data);
			critter->makeptc2(maxTree);
			done = critter->isValid();
		} while (!done);
		
	}	
}


char javaCName[200];

string DeltaWrappedCritter::getName()
{
	sprintf(javaCName,"Critter%ld",theId);
    string answer = javaCName;
    return answer;
}

void DeltaWrappedCritter::clearScore() {
    storedScore = 0;
}

void DeltaWrappedCritter::addToScore(double ats) {
    storedScore += ats;
}

bool DeltaWrappedCritter::isBetterThan(double lhsScore,DeltaWrappedCritter *rhs,double rhsScore) {
    if (lhsScore == rhsScore) {
        return critter->getUsefulDna() <= rhs->getCreature()->getUsefulDna();
    }
    return lhsScore < rhsScore;
}


void DeltaWrappedCritter::replaceWithCopyOfCreature(crPtr  tocopy) {
    critter.reset(new cr_data(*tocopy)); // dereference the crPtr to refer to the cr_data
    storedScore = 0;
};


bool DeltaWrappedCritter::isBetterThan(double lhs,double rhs) {
	return lhs > rhs;
}

void DeltaWrappedCritter::saveCreature(ofstream &fout)
{
    dWCritter::streamout(fout,critter);
}


void DeltaWrappedCritter::loadCreature(ifstream &fin) {
	long dnaL;
	int *dna;
	
    fin >> dnaL;
	if (!fin) return;
	if (dnaL<10 || dnaL>800000) {
		cout << "Encountered an unusual dna length of " << dnaL << "\n";
	}
	dna = new int[dnaL];
	for (int i=0;i<dnaL;i++) {
		fin >> dna[i];
    }
	int j;
	fin >> j;
	if (j!=-1) {
		cout << "Potential error, CreatureDelta \"stream\" not properly terminated\n";
	}
	critter.reset(new cr_data(dna,dnaL));
	if (critter->isValid()) {
		//cout << "A ParasiteDelta was loaded that was valid\n";
	} else {
		cout << "An invalid DeltaWrappedCritter was loaded\n";
	}
	
}


