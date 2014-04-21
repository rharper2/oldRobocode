//
//  DeltaWrappedCritter.cpp
//  Robocode
//
//  Created by Robin Harper on 14/03/2014.
//
//

#include "DeltaWrappedCritter.h"
#include "robocodeDelta.h"

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

// If make is true we need to actually create the underlying wrapped critter
// If make is false, we can create a light-weight version of the class
// i.e. no critter. This will normally be because we will copy a critter in
// for instance a child made or a copy taken.
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
    if (theId==0) cout << "adding " << ats << " its now " << storedScore << endl;
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

// the spatially bred critters have an age parameter saved.
// This method allows us to read them in here, but we just through away the genetic age.
void DeltaWrappedCritter::loadSpatialCreature(ifstream &fin) {
	long dnaL;
	int *dna;
    int geneticAge;
	fin >> geneticAge; // we dont care just now, so throw away.
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
// crosses over this DeltaWrappedCritter with the supplied one, returning up to two valid children.
void DeltaWrappedCritter::variableCrossover(wrappedDeltaPtr secondParent,wrappedDeltaPtr &cw1,wrappedDeltaPtr &cw2) {
    crPtr c1,c2;
    cr_data::variableCrossover(getCreature(),getCreature(),c1,c2);
    int mutate_chance=2000; // 0.2
    cw1=NULL;
    if (c1 != NULL) {
        if (c1->isValid()) {
            c1->smartSureMutate(mutate_chance);
            if (c1->isValid()) { // is it still valid after mutation?
                 DeltaWrappedCritter *newChild1= new DeltaWrappedCritter(false);
                 cw1.reset(newChild1);
                 cw1->replaceWithCopyOfCreature(c1);
            }
         }
    }
    cw2=NULL;
    if (c2 != NULL) {
        if (c2->isValid()) {
            
                // NOTE I am only mutating valid children, which might not be right
                // some operators will create invalid children - do I want to mutate them
                // in the hope they become valid?
                                       
                c2->smartSureMutate(mutate_chance);
                if (c2->isValid()) { // is it still valid after mutation?
                     DeltaWrappedCritter *newChild2= new DeltaWrappedCritter(false);
                     cw2.reset(newChild2);
                     cw2->replaceWithCopyOfCreature(c2);
                }
        }
    }
}


// printout the robot into the ofstream, with the fname and gname supplied.
// robocodeDelta is the grammar we are using just now, which contains the static method need to translate the genome to a printed java program.
void DeltaWrappedCritter::printItOut(ofstream &roboout,const char *fname,const char *gname) {
    robocodeDelta::Instance()->print_grammar_individual(roboout,getCreature()->getExpression(),getCreature()->getExpressionLength(),fname,gname);
}


