/*
 *  entity.cpp
 *  
 *
 *  Created by Robin Harper on 30/08/04.
 *  Copyright (c) 2004 robinh@cse.unsw.edu.au. All rights reserved.
 *
 *            Based on original file by            
 *              AUTHOR :  Michael O'Neill                                       
 *              DATE   :  NOV 97                                                
 *              SCHOOL :  Dept. Computer Science                              
 *                        University of Limerick                              
 */


#include <cmath>
#include <sstream>
#include "entity.h" 
#include "myrandom.h"
#include "grammarBase.h"
#include <stdlib.h>

using namespace std;

bool entity::displayCodonInfo = false;

//******************* Creators, and destructors ***********************************
entity::entity(long strlen, int str[])
{
          dna = NULL;
          codonIntegers = NULL;
          usefulDna = dnaLength = strlen;
          initialiseDynamic();
          copyBits(str,NULL);
          initialiseRest();
#ifdef DEBUGALLOCATION
          cout << "Normal entity called given number " << mynumb << "\n";
#endif
}
//*************************************************************************
void entity::initialiseDynamic() {
	if (dnaLength > 0) {
		dna = new int[dnaLength];
		numberOfCodons = dnaLength/CODON;
		codonIntegers = new long[numberOfCodons];
	}
    else {
		dnaLength = 0;
		numberOfCodons=0;
    }
}
//*************************************************************************
void entity::initialiseRest() {
	
	valid = true; //individual assumed to be valid until proven otherwise!
				  // Why? - need to work out I would prefer = false;
	decoded = false;
	changed = true; 
    usedLength = 0;
    usefulDna=0;
    
	functionVector.clear();
	
	//     changed is set to true to indicate that it is necessary to decode and map before fitness can be taken.
	//     this may not actually be the case where an individual is constructed through the copy constructor
	//     may wish to consider this point as it will impact where the testing procedure is non trivial
	
	
#ifdef DEBUGALLOCATION
	mynumb=entityNumber++;
#endif
}
//*************************************************************************
void entity::copyBits(int *dnaFrom,long *codonFrom) {
	if (dnaFrom != NULL) {
		for (long i = 0;i<dnaLength;i++)
			dna[i]=dnaFrom[i];
	}
	if (codonFrom != NULL) {
		for (int j= 0;j<numberOfCodons;j++) {
			codonIntegers[j]=codonFrom[j];
		}
	}
}
//*************************************************************************
entity::entity(const entity &tocopy) {
	dnaLength = tocopy.dnaLength;
	usefulDna = tocopy.usefulDna;
	numberOfCodons = tocopy.numberOfCodons;
	usedLength = tocopy.usedLength;
	if (tocopy.valid && tocopy.decoded && !tocopy.changed) {
		expressionLength = tocopy.expressionLength;
	}  
	else {
		// If the entity being copied has not been validly decoded 
		// (and hasn't subsequently changed) all this information
		// is  wrong in it, so reset to zero for this
		expressionLength=0;
		usefulDna = 0;
		usedLength = 0;
	}
	initialiseDynamic();
	copyBits(tocopy.dna,tocopy.codonIntegers);
	initialiseRest();
	if (tocopy.valid && tocopy.decoded && !tocopy.changed) {
		for (int i=0;i<expressionLength;i++)
			expression[i]=tocopy.expression[i];
		expression[expressionLength]=0; // null terminate
		
		// THESE MUST APPEAR AFTER INITIALISE REST
		functionVector = tocopy.functionVector;
		valid=tocopy.valid;
		changed = tocopy.changed;
		decoded = tocopy.decoded;
		usedLength = tocopy.usedLength;
		usefulDna = tocopy.usefulDna;
		
	}
#ifdef DEBUGALLOCATION
         cout << "\nGE COPY CONSTRUCTOR CALLED number: " << mynumb << "\n";
#endif
/* 
 I decided in the copy constructor to copy codons and the "decoded/valid" state
    of the entites (same with assignment).
 This is principally because in the GUI there are a number of lists created
    by the breeders as each generation is passed back to the GUI for display
    this causes a number of copy assignments, which, if only the dna
    is copied cause a number of "decodings" and "interpretations" which really
    are redundant.
 
    I will need to watch this to make sure there are no ill-effects from
    copying the decoding, but it should be the speediest method of doing this.
 */
}
//*************************************************************************
entity& entity::operator=(const entity &op2) {
	if (this == &op2) return *this;
	if (dna) delete [] dna; // delete the storage existing in this structure.
	if (codonIntegers) delete [] codonIntegers;
#ifdef DEBUGALLOCATION
	cout << "\nEntity Assignment  CALLED number: " << mynumb << "\n";
#endif
	dnaLength = op2.dnaLength;
	usefulDna=op2.usefulDna;
	numberOfCodons = op2.numberOfCodons;
	usedLength = op2.usedLength;
    if (op2.valid && op2.decoded && !op2.changed) {
		expressionLength = op2.expressionLength;
    }  
    else {
		// If the entity being copied has not been validly decoded 
		// (and hasn't subsequently changed) all this information
		// is  wrong in it, so reset to zero for this
		expressionLength=0;
		usefulDna = 0;
		usedLength = 0;
		  }
	initialiseDynamic();
	copyBits(op2.dna,op2.codonIntegers);
	initialiseRest();
    
    // THESE MUST APPEAR AFTER INITIALISE REST
	if (op2.valid && op2.decoded && !op2.changed)
	{
		      expressionLength = op2.expressionLength;
		      if (expressionLength > LENGTH_OF_EXPRESSION-1) {
				  cout << "********** ERROR ON EXPRESSION LENGTH IN ENTITY = ***";
				  exit(1);
		      }
		      for (int i=0;i<expressionLength;i++)
     			  expression[i]=op2.expression[i];
    		  expression[expressionLength] = 0; //its null terminated!
			  functionVector=op2.functionVector; 
			  usedLength = op2.usedLength;
			  usefulDna = op2.usefulDna;
			  // I hope its as simple as this! May/will have to implement specific copy constructors
			  // in function if functions use dynamic memory
    		  valid=op2.valid;
    		  changed = op2.changed;
    		  decoded = op2.decoded;
	} else expressionLength = 0;
#ifdef DEBUGALLOCATION
	cout << "\nEnityt Assignment CALLED and now assigned number: " << mynumb << "\n";
#endif
	return *this;
}
//*************************************************************************
entity::~entity(){ 
#ifdef DEBUGALLOCATION
          cout << "\nAbout to try and delete a entity with number" << mynumb << "\n";   
#endif
          if (dna) {
                    delete [] dna;
                    dna = NULL;
          } else {
                    cout << "DNA already null\n";
          }
          if (codonIntegers) {
                    delete [] codonIntegers;
                    codonIntegers = NULL;
          } else {
                    cout << "CodonIntegers already null\n";
          }
#ifdef DEBUGALLOCATION
          cout << "\nDeleting a entity with number" << mynumb << "\n";
#endif
}

//*************************************************************************
void entity::decode() {
    decodeBinaryString(dna);
    changed = true;
    decoded = false;
}

//*************************************************************************
unsigned int entity::grayencode(unsigned int g) {
    return g ^ (g >> 1);
}

//*************************************************************************
unsigned int entity::graydecode(unsigned int gray) {
    unsigned int bin;
    for (bin = 0;gray; gray = gray >>1) {
        bin ^=gray;
    }
    return bin;
}
//*************************************************************************


bool entity::validate(grammarBase *g) {
	//   the boolean flag changed is used to decide if it is necessary to reconstruct
	//   and re-test the individual - since this is (potentially) quite time consuming.
	//   It is (therefore) important to ensure changed is set to true if the individual needs
	//   to be re-assessed.
	if (!changed && decoded) return valid;
    // we need to revalue it - initialise data structures 
    // - consider if we should rely on grammar to do this
	expressionLength = 0;
	functionVector.clear();
	for (int i=1;i<MAX_LENGTH_OF_EXPRESSION;i++) expression[i]=0;
	decodeBinaryString(dna);
    valid = g->parse(expression,&expressionLength,MAX_LENGTH_OF_EXPRESSION,
					 codonIntegers,numberOfCodons,usedLength,&functionVector);
    decoded = true;
	changed = false;
//	if (!valid) cout << "It wasnt valid for: " << g->getInvalidReason() << endl;
	
	if (isValid()) {
		long usedDna = usedLength * CODON; // this is the amount of dna useful used
		if (usedDna == 0) {
			cout << "Just set usedDna to zero, which can't be right for a valid individual.\n";
            exit(1); // I am now exiting the programs if any unusual things occur otherwise
            // I will miss the messages and if something shouldn't happen I need to find out why it does.
		}
		if (usedDna < dnaLength) {
			usefulDna = usedDna;
		} else usefulDna = dnaLength;
#ifdef VERBOSE
		cout << "Cut down dnaLength to (" << dnaLength << ")\n";
#endif
	}
    return valid;
 /* ********
 [CURRENT]	 
  There is a variable - usefulDna - this is set to the amount of dna actually used.
  When a crossover occurs this allows me the choice of splicing either 1..usefulDna (so the 
  change WILL be noticed) or in, for instance, the second entity, you might use 1..dnaLength - so if 
  it has an "unused" portion in its tail will be carried with the splice - <hmm> it does mean unused tails 
  will be retained and could "open" up later. 19/8/05 - What is important is that without the tail mutation can
  cause individuals to fail - at least the empirical tests show that the evolution proceeds far better if the 
  unused tails are retained.
  
  Finally I must try runs where I randomise the tails with every child to see if any information is retained.
		  
  After having implemented the two point replacement crossover (and the currently no more effective SAGA operators) it 
  appears the other bloat control  mechanisms are fine - whilst I still use the usefulDna to limit crossover 
  selection points, and there are various other downward pressures, there is no need to "trim" the DNA 
  and thus the option to do so has been removed - 25/2/2005. In fact as I now note above (6 months later) the trim is
  detrimental to evolution.
  
  *** */                 
 
}

// See header file for an explanaton of this
// only use with apporpriate grammars.
bool entity::makeValid(grammarBase *g,int maxDepth,int minDepth,bool growToMax) {
	expressionLength = 0;
	functionVector.clear();

	for (int i=1;i<MAX_LENGTH_OF_EXPRESSION;i++) expression[i]=0;
	
	decodeBinaryString(dna);
	valid = g->doACreate(expression,&expressionLength,MAX_LENGTH_OF_EXPRESSION,
					 codonIntegers,numberOfCodons,usedLength,&functionVector,maxDepth,minDepth,growToMax);
	  decoded = true;
	changed = false;
    recode_binary();
    
    // Now we need to translate the codons back into the dna as doACreate can alter the codons.
    // I dont know why I have the dna I should just use the codons.
	
    if (valid) {
		long usedDna = usedLength * CODON; // this is the amount of dna useful used
		if (usedDna == 0) {
			cout << "Just set usedDna to zero, which can't be right for a valid individual.\n";
		}
		if (usedDna < dnaLength) {
			usefulDna = usedDna;
		} else usefulDna = dnaLength;
#ifdef VERBOSE
		cout << "Cut down dnaLength to (" << dnaLength << ")\n";
#endif
	}
    return valid;
 
}
//*************************************************************************
bool entity::makeptc2(grammarBase *g,int expectedExpansions,int maxDepth) {
	expressionLength = 0;
	functionVector.clear();

	for (int i=1;i<MAX_LENGTH_OF_EXPRESSION;i++) expression[i]=0;
	
	decodeBinaryString(dna);
	valid = g->ptc2Grow(expression,&expressionLength,MAX_LENGTH_OF_EXPRESSION,
					 codonIntegers,numberOfCodons,usedLength,&functionVector,maxDepth, expectedExpansions);
	decoded = true;
	changed = false;
    recode_binary();
    
    // Now we need to translate the codons back into the dna as doACreate can alter the codons.
    // I dont know why I have the dna I should just use the codons.
	
    if (valid) {
		long usedDna = usedLength * CODON; // this is the amount of dna useful used
		if (usedDna == 0) {
			cout << "Just set usedDna to zero, which can't be right for a valid individual.\n";
		}
		if (usedDna < dnaLength) {
			usefulDna = usedDna;
		} else usefulDna = dnaLength;
#ifdef VERBOSE
		cout << "Cut down dnaLength to (" << dnaLength << ")\n";
#endif
	}
    return valid;
 
}

//*************************************************************************
void entity::decodeBinaryString(int str[])
{
	long index = 0;
	for (int i=0;i<numberOfCodons;i++) codonIntegers[i] = 0;
	// decode binary string 
	for(int i=0;i<numberOfCodons;i++) {
		index = i*CODON;
		codonIntegers[i] = decode_binary(str,index,CODON);
	}
    
}
//*************************************************************************
int entity::decode_binary(int string[], long start, int len)
{
 //********************************************* 
 // Decode a binary string to numeric, from    
 // string position "start" up to start + len 
 //********************************************* 
	int accum = 0;
	for (long j=start; j < (start+len); j++){
		accum = accum*2 + (string[j]);
	}
	return accum;
    // Now if we assume that accum is a gray encoded string
    // we would now return
    // return graydecode(accum);
    
}


void entity::recode_binary() {
    for (int i=0;i<numberOfCodons;i++) {
        recode_binary(&dna[i*CODON],(codonIntegers[i] & CODONMASK));
    }
}

void entity::recode_binary(int *str,long cod) {
    int cods = cod;
    // and if we want to gray encode
    // cods = grayencode(cods);
    for  (int i=(CODON-1);i>=0;i--) {
        str[i] = cods & 0x1;
        cods = cods >> 1;
    }
}


int entity::getFuncNos() {
          if (!valid) return -2;
          return functionVector.size();
}          

//*************************************************************************
string entity::display(grammarBase *g)
{ 
	ostringstream outs;
	outs << "---------------------------------------" << endl;
	outs << "No. Codons(" << numberOfCodons << ")" << endl;
	outs << "No. Used Codons(" << usedLength << ")" << endl;
	outs << "Map Successfull? (" << valid << ")" << endl;
	if (valid) {
		int i;
		outs << "Number of functions: " << (i=functionVector.size()) << ".\n";
		for (int j=0;j<i;j++) {
			outs << "Function " << j << " name: " << functionVector[j].fno << " has " << functionVector[j].params << " parameters.\n";
			g->print_grammar_individual(outs,functionVector[j].expression,functionVector[j].expLength);
			outs <<  "\n--------- End of function " << j << "  -------------\n";
		}
	}
	if (int wr = g->getWraps() != -1) cout << "Number of Wraps: " << wr << "\n";
	outs << "The expression length is : " << expressionLength << "\n";
	outs << "Output Code:" << endl;
	if (valid) {
        g->print_grammar_individual(outs,expression,expressionLength);
		outs << endl;
    } else {
        outs << "INVALID INDIVIDUAL:" << g->getInvalidReason() << endl;
        g->print_grammar_individual(outs,expression,expressionLength);
		outs << endl;
    }
    // should put this on a switch
    if (displayCodonInfo) {
        outs << "\n---------------------------------------" << endl;
        outs << endl;
        outs << (codonIntegers[0] & CODONMASK);
        for (int i=1;i<usedLength;i++) outs << ": (" << i << ") " << (codonIntegers[i] & CODONMASK);
        outs << endl;
        outs << "************* grammar node ****************\n";
        outs << "(0) "<< ((codonIntegers[0] >> CODON) & LHSMASK) << "[" << ((codonIntegers[0] >> (CODON + LHSINFO)) & EXPRMASK) <<"]";
        for (int i=1;i<usedLength;i++) {
            outs << ": (" << i << ") " << ((codonIntegers[i] >> CODON) & LHSMASK) << "[" << ((codonIntegers[i] >> (CODON + LHSINFO)) & EXPRMASK) <<"]";
            if (g->isNonTerminal(&codonIntegers[i])) outs << "NT";
            if (g->isTerminal(&codonIntegers[i])) outs << "T";
        }
        outs << endl;
/*        outs << "************* node length ******************\n";
        outs << "(0) " << ((codonIntegers[0] >> (CODON + LHSINFO)) & EXPRMASK);
        for (int i=1;i<usedLength;i++) outs << ": " << "(" << i<< ") "<< ((codonIntegers[i] >> (CODON + LHSINFO)) & EXPRMASK);
        outs << endl;
        outs << "************* node length (relative) ******************\n";
        outs << "(0) " << ((codonIntegers[0] >> (CODON + LHSINFO)) & EXPRMASK);
        for (int i=1;i<usedLength;i++) outs << ": " << "(" << i<< ") "<< ((codonIntegers[i] >> (CODON + LHSINFO)) & EXPRMASK)-i;
        outs << endl;
*/
        
         outs << "************* raw codon ******************\n";
         outs << "(0) " <<  ((codonIntegers[0] ));
         for (int i=1;i<usedLength;i++) outs << ": " << "(" << i<< ") "<< ((codonIntegers[i] ));
         outs << endl;
        
    }
	outs << '\0';
    return (outs.str());
}

//*************************************************************************
/* DEPRECATED
    Well really what has happened is that I just worked out why some of my problems
    were running so slowly - if you look at this code how it works with a 10k+ size
    DNA - really its rolling a random number per bit (10k times) per individual
    per mating per generation!
    
    Better to do it just a few times.
*/
bool entity::mutate(int chance) {
   if (chance == 0) return false;
          int nobits=0;
          int rnd;
          for (int i= 0;i<dnaLength; i++) {
                    if (chance > (rnd = randomint(10000))) {
                              //   cout << "Chance: " << chance << "Randomint " << rnd << "\n";
                              changed = true;
                              nobits++;
                              dna[i] = (dna[i]==0)?1:0;
                    }
          }
#ifdef VERBOSE
          if (nobits >0) cout << "DNA of length " << dnaLength << " mutated, " << nobits << " flipped.\n";
#endif
          return (nobits>0);
}
//*************************************************************************

bool entity::qmutate(double chance) { 
    // The idea here is to determine how many mutations are to occur and then apply them
    // them randomly to the dna - chance is the number of mutations per dna
    // so 0.7 means 0.7 chance of one or more, if one occurs a 0.7 chance of another occuring
    // and if two occur a 0.7 chance of a third etc.
    // if its >1, say 2.7 then two will occur and we repeat as if it were a 0.7 so 
    // two occur, 0.7 of three or more, if three, 0.7 of four or more etc.
    
    double mutations;
    double perChance = modf(chance,&mutations);
    bool stillChecking = true;
//    cout << "Chance of " << chance << " splits to " << mutations << " and " << perChance << " and I threw a " << randreal() << endl;
    while (stillChecking) {
        if (randreal() > perChance) {
            mutations++;
        } else {
            stillChecking = false;
        }
    }
    long place;
//    cout << "Applying " << mutations << " mutations " << endl;
    for (int i=0;i< mutations;i++) {
        place = randomint(dnaLength); // query if this should be UsefulDna
//        cout << "Flipping the bit at " << place << endl;
        dna[place] = (dna[place]==0)?1:0;
    }
    if (mutations > 0) changed = true;
    return (mutations > 0);
}

//*************************************************************************
bool entity::mutate() { // determine the chance to give an average of one per entity
    int nobits=0;
    int chance;
    if (usefulDna > 30000) chance = 30000;
    else chance = usefulDna; // gives us the correct chance on average to mutate
          // one bit of the useful part of the Dna.
          for (int i= 0;i<dnaLength; i++) {
                    if (randomint(chance)==0) {
                              //   cout << "Chance: " << chance << "Randomint " << rnd << "\n";
                              changed = true;
                              nobits++;
                              dna[i] = (dna[i]==0)?1:0;
                    }
          }
#ifdef VERBOSE
          if (nobits >0) cout << "DNA of length " << dnaLength << " mutated, " << nobits << " flipped.\n";
#endif
          return (nobits>0);
}

//*************************************************************************
bool entity::smartMutate(double chance) { 
	if (!valid || !decoded) {
		return qmutate(chance);
	}
		
// mutates the codon in such a way that 1 in 4 its neutral, otherwise has an impact
    if (randreal() < chance) {
		 long place = randomint(getNumberOfCodons()); // query if this should be UsefulDna
		 long currentValue = codonIntegers[place];
		 switch (randomint(4)) {
		 case 0: currentValue++;
				 break;
		 case 1: currentValue +=72; // neutral for 2,3,4, 6, 9 and 8
				 break;
		 case 2: currentValue +=5; // neutral for 5
				 break;
		 case 3: currentValue +=7; //netural for 7.
				  break;
		 }
		 currentValue = currentValue & CODONMASK;
		 codonIntegers[place] = currentValue;
		 recode_binary(&dna[place*CODON],currentValue);
		 return true;
	} else {
		return false;
	}
}
//*************************************************************************
bool entity::smartSureMutate(double chance) { 
	if (!valid || !decoded) {
		return qmutate(chance);
	}
	// mutates the codon in such a way that 1 in 4 its neutral, otherwise has an impact
    if (randreal() < chance) {
		// Jury is still out on this, the thinking about it being useful codon length
		// (i.e. guaranteed to happen on the coding part of the genome - 
		// so it will alter the phenotype but not necessarily
		// impacting the part of the phenotype that alters the fitness
		// is that if we allow it happen on the non-coding region then
		// it might encourage long genotypes with large non-coding areas
		// to protect themselves against mutation. - hmm.
		long place = randomint(getUsefulCodonLength()); 
		long currentValue = codonIntegers[place];
		int grammarType = (currentValue >> CODON) & LHSMASK; 
		currentValue = currentValue & CODONMASK;
		// If the grammar Type is zero, we use the codon value of that codon
		// so it doesnt make sense to mutate it in a way which is designed to
		// change the rule the codon is decoding - rather we increment or
		// decrement its value by a random amount.
		if (grammarType == 0) {
			currentValue = currentValue + 255 - randomint(510);
			//cout << currentValue << endl;
		} else {
		// on the other hand if the codon is encoding a grammar rule
		// we want to ensure that there is an equal probability
		// that the codon will change the rule of grammar selected
		// irrespective of how many choices there are
		// as a practical matter we only do this for 
		// rules of 2 through to 9 choices
			switch (randomint(4)) {
				case 0: currentValue++;
					break;
				case 1: currentValue +=72; // neutral for 2,3,4, 6, 9 and 8
					break;
				case 2: currentValue +=5; // neutral for 5
					break;
				case 3: currentValue +=7; //netural for 7.
					break;
			}
			//currentValue = currentValue & CODONMASK;
			codonIntegers[place] = currentValue;
			recode_binary(&dna[place*CODON],currentValue);
			changed = true;
			//		 to test we should do this
			//		 
			//	     decode();
			//		 long newValue = codonIntegers[place];
			//		 cout << currentValue << " did change to " << newValue << endl;
		}
		return true;
	} else {
	    return false;
	}
}


//*************************************************************************
bool entity::isEqual(entity *comp) {
         if (getExpressionLength() != comp->getExpressionLength()) {
                    //  cout << "Lengths differ " << entitytExprLength() << "versus " << comp->entitytExprLength() << "\n";
          } return false;
          bool same = true;
          for (int i=0;i< getExpressionLength();i++)
                    if (expression[i] != comp->expression[i]) {
                    same = false;
                    //   cout << "Grammars don't match\n";
                    break;
          }
          // cout << (same?"Same":"Not the Same") << "\n";
          return same;
}
//*************************************************************************




#ifdef DEBUGALLOCATION
int entity::entityNumber;
#endif

