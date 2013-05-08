/*
 *  critter.cpp
 *  GEtestbed
 *
 *  Created by Robin Harper on 30/08/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

// The header files has extensive comments on what most of the methods are for.


#include "critter.h"
#include <sstream>

grammarBase *cr_data::grammar = 0;
interpreterBase *cr_data::interpreter = 0;
int cr_data::serialNo = 0;
int cr_data::invalid_count = 0;
bool cr_data::fullDnaToBeUsed = true;
#ifdef DEBUGCRITTERALLOC
set<int> cr_data::extantCritters;
#endif
// This is still slightly! experimental
// If you switch it on check its not broken before using.
// The idea was to allow the actual codons to change the
// mutation rate, taking advantage of the polymorphism
// of the codons. That is, if you have a grammar with 4 choices
// in a particular non-terminal, then codons 4,8,12 etc
// all code the same.
// So this method says that low codons lower the mutation rate (e.g. 4)
// whereas high codons, (e.g. 252) raise the mutation rate.
// Early experiments convinced me to lower the overall mutation rate
// but once I did that, it became unclear how well this was working and
// why it was producing the results it did - I tried delinking it 
// and keeping a shadow set of dna which solely influenced the mutation
// but couldn't get the same smooth results I seemed to get with this,
// then I gave up - not sure if I will ever return to it.
bool cr_data::DnaInfluencedMutation = false;

// Comparison impact is where two critters
// who have otherwise equal fitness will start 
// to rank ahead of each other if their useful dna
// is less than that value.

int cr_data::COMPARISONIMPACT= 2000;
int cr_data::USEFULMANDATORY=  30000;
int cr_data::INVALIDTHRESHOLD= 40000;


// A switch to determine if you want high
// fitness individuals or low fitness individuals.
// See header file for more detail
bool cr_data::higherIsBetter = true;

// The saga part of this is 
int cr_data::SAGAApplicationMinimum = 2;
// bool cr_data::SAGAUseGrammar = false;
//bool cr_data::newMethodMutation = false;
//int	 cr_data::MaxMut = 200;
int cr_data::variableCrossoverMutateChance = 1000; // i.e. 1 in 10

#ifdef PROTECTIONENABLED
bool cr_data::USEPROTECTION = false;
#endif


// chance in 100 to constrain a tree-style crossover to crossing over
// a non-leaf node. E.G set to 90 to mean that 90% of the time
// any crossover is forced to choose non-leaf nodes
int cr_data::chanceOfTreeCrossover=90; 

// this is the size that a parse-tree will consider something not to be a leaf
// based on the number of codons used to fully decode this expansion.
int cr_data::minSizeofNonLeaf=4; 

// If we are doing something like fair crossover set to a non-zero number (normaly above 1)
// specifies the maximum eligible mathching tree in parent 2
// so if we a choose, say a 8 codon tree and this is 1.5 then we can matchi it with a tree
// of up to 12 codons.
double cr_data::maxMatchingTree=1.5;

// complimentary min paramater .e.g 0.5 would mean that we don't consider trees of less than 4 codons
// if the first parents tree was 8 codons long- again 0 disables.
double cr_data::minMatchingTree=0.5;




// The < and > than operators were at one point defined to favour critters who have 
// shorter dna lengths (fitness being equal) - this lead to premature convergence on low
// level fitness with miniscule dna lengths - need to explore further 
// As at 18/08/2005 pretty much settled on the use of COMPARISON IMPACT
// which causes the length favouring only to be enabled when a certain complexity has been
// reached. The way runs seem to work is that a certain fitness level is achieved and the 
// average length shrinks until a combination (with a larger length) is more fit and bang the 
// size increases ... shrinks ... and (often) evolution continues.

// A final thought - which I will mull over is to favour the individual which has a UsefulDna closest
// to its actual dna.


const bool operator<( const cr_data &o1,const  cr_data &o2) {
    //cout << "< than called \n";
    double i,j;
    i = o1.get_fitness();
    j = o2.get_fitness();
    if (i==j) {
        long il = o1.getUsefulDna();
        long jl = o2.getUsefulDna();
        if ((il > cr_data::COMPARISONIMPACT) || (jl > cr_data::COMPARISONIMPACT)) return il>jl;
    }
    if (cr_data::higherIsBetter) return i<j;
    else return i>j;
}


const bool operator>(const cr_data &o1,const  cr_data &o2) {
    //cout << "> than called\n";
    double i,j;
    i = o1.get_fitness();
    j = o2.get_fitness();
    if (i==j) {
        long il = o1.getUsefulDna();
        long jl = o2.getUsefulDna();
        if ((il > cr_data::COMPARISONIMPACT) || (jl > cr_data::COMPARISONIMPACT)) return il<jl;
    }
    if (cr_data::higherIsBetter) return i>j;
    else return i<j;
}

const bool operator==(const cr_data &o1, const cr_data &o2) {
    return o1.get_fitness() == o2.get_fitness();
}

const bool operator!=(const cr_data &o1, const cr_data &o2) {
    return o1.get_fitness() != o2.get_fitness();
}

// Two helper functions because critters are stored as 
// shared_ptr using the boost library 
// (basically pointers that delete the thing they point
// to when nothing else points to it.
// so this allows us to compare two shared_ptrs and
// have them call the correct critter compare stuff
// which also allows us to use containers of the pointers
// to critters.

bool crPtrGreaterThan(const crPtr &lhs,const crPtr &rhs) {
    return ( operator>(*lhs,*rhs) );
}

bool crPtrLessThan(const crPtr &lhs,const crPtr &rhs) {
    return ( operator<(*lhs,*rhs));
}


cr_data::cr_data() {
	mySerial = serialNo++;
    changed = true;
    fitness = 0.0;
    altFitness = 0;
    tested=false;
    //		  for (int i=0;i<MaxMut;i++) MutationCodons[i]=randomint(256);
    int binaryString[NUMBITS];
    long binaryStringLength;
    binaryStringLength = NUMBITS;
    randomise(binaryStringLength,binaryString);
    theEntity = new entity(binaryStringLength,binaryString);
    crossMethod = randomint(4); // This codes in a crossover preference methond
    age=0; // Can be used for anything - see for instance Age Level Protection System ALPD of Gregory Hornby

#ifdef DEBUGCRITTERALLOC
	set<int>::iterator at = extantCritters.find(mySerial);
	if (at != extantCritters.end()) {
		cout << "Critter with serialNo " << mySerial << " already exists\n";
		exit(1);
	}
	extantCritters.insert(mySerial);
#endif	
}

cr_data::cr_data(int* ibits,long nobits) {
	mySerial = serialNo++;
    changed = true;
    fitness = 0.0;
    altFitness = 0;
    tested=false;
    //		  for (int i=0;i<MaxMut;i++) MutationCodons[i]=randomint(256);
		  theEntity = new entity(nobits,ibits);
          crossMethod = randomint(4);
          age=0;

#ifdef DEBUGCRITTERALLOC
		set<int>::iterator at = extantCritters.find(mySerial);
	    if (at != extantCritters.end()) {
		cout << "Critter with serialNo " << mySerial << " already exists\n";
		exit(1);
	}
	extantCritters.insert(mySerial);
#endif
}

cr_data::cr_data(const cr_data &tocopy) {
    changed = tocopy.changed;
    fitness = tocopy.fitness;
	mySerial = serialNo++;
    altFitness = tocopy.altFitness;
    //		  for (int i=0;i<MaxMut;i++) MutationCodons[i]=tocopy.MutationCodons[i];
		  tested=false; 
          // Initially I copied over the "parsed state of the cr_data and the entity
          // I did this as there might be a fair bit of copying of individuals as I was using
          // containers of cr_data, but then I moved to shared pointers and so only pointers are
          // copied. The hesitation I have of passing over parsed individuals, is that we really want
          // to re-run the interpreter as I think I might start storing things like the completed
          // neural network with the creature and there might not be a copy procedure for them.
          theEntity = new entity(*tocopy.theEntity); // We know entity has a safe copy constuctor;
          crossMethod = tocopy.crossMethod;
          age=tocopy.age; // note its a copy you can always reset the age if you want
		  
#ifdef DEBUGCRITTERALLOC
		  	set<int>::iterator at = extantCritters.find(mySerial);
	if (at != extantCritters.end()) {
		cout << "Critter with serialNo " << mySerial << " already exists\n";
		exit(1);
	}
	extantCritters.insert(mySerial);
#endif
}

cr_data& cr_data::operator=(const cr_data &op2) {
    changed = op2.changed;
    fitness = op2.fitness;
    altFitness = op2.altFitness;
	mySerial = serialNo++;
    tested = false;
    //		  for (int i=0;i<MaxMut;i++) MutationCodons[i]=op2.MutationCodons[i];
		  if (theEntity) delete theEntity;
          theEntity = new entity(*op2.theEntity);
          crossMethod = op2.crossMethod;
          age=op2.age;
          return *this;
}


cr_data::~cr_data() {
#ifdef DEBUGCRITTERALLOC
	set<int>::iterator at = extantCritters.find(mySerial);
	if (at == extantCritters.end()) {
		cout << "Error failed to find serial number " << mySerial << " in extant critters\n";
		exit(1);
	}
	extantCritters.erase(mySerial);
		cout << "Deleted " << mySerial << endl;
		cout << "Extant critter size " << extantCritters.size() << endl;
#endif

    if (theEntity) {
        delete theEntity;
        theEntity = NULL;
    } else cout << "TheEntity was already null\n";
}


void cr_data::randomise(long len,int *bS) {
	for (int i=0;i<len;i++) {
#ifdef PROTECTIONENABLED
		if (i%CODON == 0) bS[i] = 0;
		else 
#endif
			bS[i] = randomint(2);
	}
#ifdef VERBOSE 
    fprintf(stdout,"The Randomised Binary String is : ");
    for(int i=0;i<NUMBITS;i++) fprintf(stdout,"%d",bS[i]);
    fprintf(stdout,"\n");
#endif
}


void cr_data::set_fitness(double fit) {
    // This is really just to facilitate information gathering in some very
    // odd circumstances you will almost NEVER want to set the fitness this way
    // You want to set it through get_fitness. (i.e. you want the critter 
    // to get (calculate its own) fitness rather than you (the user) set it.
    
    tested = true;
    changed = false;
    fitness = fit;
}

const double cr_data::get_fitness() const {
    if (tested && !changed) {
        return fitness;
    } else {
        if (isValid()) {
                //                   cout << display();
            fitness = interpreter->getScore(theEntity->getExpression(),theEntity->getFunctions(),altFitness);
			if (fitness < 0) {
				//cout << display();
				//reEvaluate();
				//	changed=true;
				//	theEntity->resetChanged();
				//	isValid();
				//       fitness = interpreter->getScore(theEntity->getExpression(),theEntity->getFunctions(),altFitness);
				
				//				}
				//      tested = true;
				//  } else {
				//       */
				fitness = 0.0;
				altFitness = 0;
				tested=true;
			} else {
				tested = true;
			}
			changed = false;
			return fitness;
		} else {	
			fitness = 0.0;
			altFitness = 0;
			tested = true;
			return fitness;
		}
    }
}


// This is provided to make things easier if you one fitness which guides evolution
// And another which you want for --- well something else.
// For instance regression problems normally use something like 
// Average error to guide critter ranking but when looking to see
// if a successful critter has been evolved they might look at the number of hits
// e.g. the number of test-data samples which have an error of less than 0.001.

const int cr_data::getAltScore() const {
    if (tested && !changed) {
        return altFitness;
    } else {
        if (isValid()) {
            //                       cout << display();
            fitness = interpreter->getScore(theEntity->getExpression(),theEntity->getFunctions(),altFitness);
            tested = true;
        } else {
            fitness = 0;
            altFitness = 0;
            tested=false;
        }
        changed = false;
        return altFitness;
    }
}


void cr_data::reEvaluate() {
    changed = true; 
    theEntity->resetChanged();
}




// They were just taking up too much (text) room so I moved them to their own subdirectory and their own files.
#include "../operators/singlePointCodon.cpp" 
#include "../operators/singlePointBit.cpp"
//#include "../operators/singlePointMatched.cpp"
#include "../operators/twoPointReplacement.cpp"
//#include "../operators/tailAndHeadless.cpp"
//#include "../operators/twoPointBit.cpp"
//#include "../operators/twoPointCodon.cpp"
//#include "../operators/twoPointMatched.cpp"
//#include "../operators/twoPointGutless.cpp"
//#include "../operators/singlePointSaga.cpp"
//#include "../operators/twoPointReplacementSaga.cpp"
//#include "../operators/uniformCodon.cpp"
#include "../operators/gpReplacement.cpp"
#include "../operators/variableCrossover.cpp"
#include "../operators/variable3Crossover.cpp"
//#include "../operators/randomCrossover.cpp"

void cr_data::mutate(int chance) {
    bool mutated=false;
    if (DnaInfluencedMutation) chance = calcMutateChance(chance);
    if (theEntity) 
        mutated = theEntity->mutate(chance);
    changed = changed || mutated;
    tested = tested || mutated;
}
void cr_data::qmutate(int chance) {
    double realChance = ((double) chance)/10000.0;
    
    bool mutated=false;
    if (DnaInfluencedMutation) chance = calcMutateChance(chance);
    if (theEntity) 
        mutated = theEntity->qmutate(realChance);
    changed = changed || mutated;
    tested = tested || mutated;
}

void cr_data::smartMutate(int chance) {
    double realChance = ((double) chance)/10000.0;
    
    bool mutated=false;
    if (theEntity) 
        mutated = theEntity->smartMutate(realChance);
    changed = changed || mutated;
    tested = tested || mutated;
}

void cr_data::smartSureMutate(int chance) {
    double realChance = ((double) chance)/10000.0;
    
    bool mutated=false;
    if (theEntity) 
        mutated = theEntity->smartSureMutate(realChance);
    changed = changed || mutated;
    tested = tested || mutated;
}

void cr_data::mutate() {
    bool mutated=false;
    if (theEntity) 
        mutated = theEntity->mutate();
    changed = changed || mutated;
}

bool cr_data::isValid() const {
    bool valid;
    valid = false;
    valid = theEntity->validate(grammar);
	//if (valid) valid = interpreter->validate(theEntity->getExpression(),theEntity->getFunctions());
//	if (!valid)   
//		cout << theEntity->display(grammar);
    if (changed) {
        changed = false;
        tested = false;
    }
    return valid;
}

bool cr_data::validate() {
	bool ok = interpreter->validate(theEntity->getExpression(),theEntity->getFunctions());
//	if (!ok) 
//		cout << display() << endl;
	return ok;
}

// This is designed to be used to do something like
// Koza's half and half methond to create starting individuals
// of a certain maxDepth (in parse tree terms) 
// maxCreate governing whether or not you force them to be
// of maxDepth.
// It requires some hooks into the grammar that are not
// generically implemented yet (24/07/06) and so you need
// to check that the grammar in question allows this to work.
bool cr_data::makeValid(int maxDepth,int minDepth,bool maxCreate)  {
    bool valid;
    valid = false;
    valid = theEntity->makeValid(grammar,maxDepth,minDepth,maxCreate);
    changed = false;
    return valid;
}


bool cr_data::makeptc2(int expectedExpansions,int maxDepth)  {
    bool valid;
    valid = false;
    valid = theEntity->makeptc2(grammar,expectedExpansions,maxDepth);
    changed = false;
    return valid;
}

const string cr_data::display() const {
    if (changed) {
        theEntity->validate(grammar);
        changed = false;
    }  
 //   entity::displayCodonInfo = true;
    return theEntity->display(grammar);
    
}

// Bit of a historical one this as I initially tested it
// on some problem domains that had multiplemaps,
// its useful though as sometimes you might want to distinguish betweeen possible
// test cases when displaying the "Map" or the solution
// so it really depends on what the interpreter makes of it,
// but its supposed to display the critters progress in a human understandable way.
const string cr_data::displayMap(int map) const {
    ostringstream outs;
    
    if (isValid()) {
        interpreter->displayMaps(outs,map,theEntity->getExpression(),theEntity->getFunctions());
        outs << '\n';
        outs << '\0';
    } else outs << "The individual was not valid - no map available.\n";
    return outs.str();
}


bool cr_data::isEqual(cr_data *cr) {
    return theEntity->isEqual(cr->theEntity);
}


int cr_data::calcMutateChance(int mutate) {
	if (!theEntity) return mutate;
    long *cods = theEntity->getCodons();
    int max = theEntity->getNumberOfCodons();
    for (int i=theEntity->getUsefulCodonLength();i<max;i++) {
        if (cods[i] <12) mutate--;
        else if ((cods[i] > 115) && (cods[i]<128)) mutate++;
    }
	if (mutate<1) mutate = 1;
    return mutate;
	
}

int cr_data::getMutationRate(int mutate) {
	return calcMutateChance(mutate);
}

/****************************************************************************
* July 2006 I have moved all variable crosover stuff out of here
* and into the pop Handlers,
* I intend to move them into individual crossover operators of their own
* See paper on self-selecing crossover opeator for technical details
* HISTORIC - code described moved as discussed above
* 16 September 2005
* This new section is a crossover operator which can
* change depending on evolution, it requires
* a new variable in cr_data - crossMethod (I must get
* more orginal in my variable names!) and this determines
* which crossover operator is used. Upon crossover parent a
* determines the crossMethod and this is inherited by both childern. 
* Both of them have a chance to mutate
* which is passed in as a paramater (chance in 10,000) to the crossover call.
* and thats it!
* Well some thought went into whether it would be better to have child1
* determined by crossmethod of parent a and child2 by crossMethod of parent b
* or even just for child2 to inherit crossMethod of parent b even although
* it was created by the crossMethod of parent a - but I have left it for
* just now.
*********************************************************************/



/*
void cr_data::mutateOnly(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
    // Mutation is applied later this effectively only passes things through.
    // I need to implement a mutation operator that replaces sub-trees with similar sized
    // new sub-trees.
    cr_data *cr1 = new cr_data(*op1);
    child1.reset(cr1);
    cr_data *cr2 = new cr_data(*op2);
    child2.reset(cr2);
}

*/

/*****************************************************************************
* This next section (with its supporting routines) is designed to explore 
* the usefulness or otherwise of Harvey's SAGA suggestions. I have been 
* putting this off for some time, I am not sure why but I have my doubts 
* now if it will work, still nothing ventured ...
* 21/2/2005 - I have decided to implement two version, the first more as 
* Harvey intended it on the one point codon operator, this may well have 
* some beneficial impact on that operator although the "polymorphism" 
* of GE may cause it to behave in strange ways.  And on the two point 
* replacement operator - this is more difficult to anticipate, I just
* don't know how it will work. Currently I intend to make a few tweaks, 
* namely there will be an expression length threshhold below which it will 
* not apply. I will go into more detail at the appropraite procedure (maybe) but 
* suffice it to say if you are repacing an atom of length one using Havey's SAGA
* - I think you will always replace it with the same atom (assuming an 
* identical one type atom exists in the other parent (likely)). This cannot be 
* beneficial to the search. Anyway the cut-off length will almost certainly 
* be a soft switch so I can imagine another set of comprehensive compartive runs, 
* joy of joys.
* 19/08/2005 I note on updating this file that my initial runs did not seem to provide
* any benefit either way. I haven't quite given up on this yet, I have just delayed
* further consideration until later.
* 24/7/06 - I am considering looking at homologus crossovers again
* and this might form the basis so I am leaving in here.
*********************************************************************************/
/****************************************************************************
* HISTORIC
* The twoPointReplacementSAGAOperator implements the SAGA (Harvey) crossover.  
* see under sub-dir operators for the code.      
* It works as per the twoPointReplacementOperator save that instead of the first.
* match it identifies all possible replacements and uses SAGA (or LCSS) to select
* the "best" match.
* A new static variable SAGAApplicationMinimum has been declared.
* before the SAGA matching is applied the length of expression on parent one
* must equal or exceed the SAGAApplicationMinimum.
***************************************************************************/

/********************************************************
* The first supporting subroutine is algb which takes two codon lists
* being the genes and startFirst and endFirst being the start and end of the first one
* and lengthSecond being the length of (you guessed it) the second.
* L is an output vector which is constructed as follows:
* Assume A is 1..m and B is 1..n, L is 1..(n+1)
* L[j] = LCSS (longest common substring) of string A[1..m] and substring B[1..j]
*************************************************************/
#define max(a,b) ((a)>(b)?a:b)

void cr_data::algb(long *first,int startFirst,int endFirst,long *second,int lengthSecond,int *L) {
	int current = 0;
	int last = 0;
	
	int K[2][lengthSecond+1];
	
	int fwd = (endFirst > startFirst) ? 1: -1;
	
	for (int j=0;j<lengthSecond+1;j++) 
		K[1][j]=0;
	for (int j=0;j<lengthSecond+1;j++) 
		K[0][j]=0;
	for (int i = startFirst; i * fwd < endFirst * fwd; i += fwd) {
		last = 1 - (current = last); // flip last and current
		for (int j=0;j<lengthSecond;j++) {
			int tomatch = (fwd == 1) ? j:lengthSecond - j - 1;
            K[current][j+1] =
                (((first[i] & CODONMASK) == (second[tomatch] & CODONMASK)) ?
                 K[last][j]+1 :
                 max(K[current][j],K[last][j+1]));
		}
	}
	for (int j=0;j<lengthSecond+1;j++) L[j]=K[current][j];
}

/********************************************************
* The second supporting subroutine is algd, which comes from Harvey
* We take the two two codon lists, their lengs and the proposed cross for
* the first parent. It returns the proposed selection point for the second parent.
*************************************************************/
int cr_data::algd(long *first,int lengthFirst, long *second,int lengthSecond,int cross) {
	int maxL = max(lengthFirst,lengthSecond)+1;
	int L1[maxL];
	int L2[maxL];
	int best=0;
	int temp=0;
	for (int i=0;i<maxL;i++) L1[i]=L2[i]=0;
	
	int numberOfEqualBest = 0;
    
	algb(first,0,cross,second,lengthSecond,L1);
	
	algb(first,lengthFirst-1,cross-1,second,lengthSecond,L2);
	
	for (int i =0;i<=lengthSecond;i++) {
		temp = L1[i]+L2[lengthSecond-i];
		if (temp > best) {
			numberOfEqualBest = 0;
			best = temp; 
		}
		if (temp==best) // note the above condition falls through to here as well i.e. no else
			L1[numberOfEqualBest++]=i;
	}
	return L1[randomint(numberOfEqualBest)];
}

// wrapper to algb for use in twoPointReplacementSAGAOperator.

int cr_data::algrp(long *first,int startFirst,int endFirst, long *second,int startSecond,int endSecond) {
	int lengthSecond = endSecond - startSecond;
	int results[lengthSecond+1];
	algb(first,startFirst,endFirst,&second[startSecond],lengthSecond,results);
    //	cout << "Checking\n";
    //	for (int i=startFirst;i<endFirst;i++) {
    //		if (SAGAUseGrammar) cout << ((first[i] >> CODON) & 0xFF) << ": ";
    //		else cout << (first[i] & CODONMASK) << ": ";
    //	}
    //	cout << "\n against \n";
    //	for (int i=startSecond;i<endSecond;i++) {
    //		if (SAGAUseGrammar) cout << ((second[i] >> CODON) & 0xFF) << ": ";
    //		else cout << (second[i] & CODONMASK) << ": ";
    //	}
    //	cout << "\nTo return: " << results[lengthSecond] << "\n";
	return results[lengthSecond];
}
