/*
 *  critter.h
 *
 *
 *  Created by Robin Harper on 30/08/04.
 *  Copyright (c) 2004 robinh@cse.unsw.edu.au. All rights reserved.
 *
 */
 
#ifndef cr_data_t
#define cr_data_t

// ********************************* USE ************************************************

// Critter is the main class which ties together each of the individual programs
// used in GE. - It is a wrapper for the entity class which implements
// the core of an individual - e.g. the phenotype and displaying properties.
//
// It is normally accessed through a boost shared pointer 
// (this is a form of smart pointer)
// It has a defined type of: crPtr
// So for example, to define a pointer "thePointer"
// you could use:

// *********
// crPtr thePointer(new cr_data)
// *********
// When thePointer goes out of context (and provided no other smart pointer is pointing to
// the critter then it will be auto deleted.
// This makes it relatively easy to use with STL containers
//
// *********
// vector<crPtr> theVector;
// *********
//
// Now the STL vector copies and moves shared_ptrs rather than the critters (which can be rather large)
// and (provided the critter isn't referred to elsewhere) removing entities from
// vectors will automatically delete them.
//
// You need to set the grammar and the interpreter to use critters.
//
// *********
// cr_data::setGrammar(theGrammar);
// cr_data::setInterpreter(theInterpreter);
// *********
//
// where theGrammar is a class which inherits grammarBase
// and theInterpreter is a class which inherits interpreterBase

// After that a critter can have its validity queried
// *********
// bool isValid(); // returns true of the the critter parses ie. the phenotype can
// be fully decoded given the codons of the critter.
// *********
// int get_fitness(); // returns the fitness of the critter - calculating it if necessary.
// *********
// int getAltScore(); // returns the alternative fitness score (if any) 
// defined by the interpreter.
// *********
// void reEvaluate(); // causes any stored fitness to be zeroed, meaning that a subsequent get_fitness will
// cause the fitness of the critter to be reEvaluated.
// *********
//
// **************************** NOTES *****************************************************
// There are three levels of dna size control techniques
// I am using defines just now, I don't use const so that
// they can be set in the program body, and thus the impact can be
// tested (if I change then here for each body I don't have a record
// in the program of what paramater choices were being used).

// They are:
//
// COMPARISONIMPACT - which is the most gentle, this is the level when
//     - two entities have the same fitness, the one with 
//     - the lower dna length is preferred - note that
//     - this shouldn't be used in a fashion where it impacts
//	   - too early as otherwise sometimes 
//     - very simple low scoring entities swamp
//     - initial populations.
// USEFULMANDATORY  - this is the level at which the crossover will not use
//     - the fullDna even if the use of full dna is enabled
//     - they only use the useful dna after that level
//     - this is only relevant to some crossover operators
//     - eg bit and codon operators
//     - the structure preserving crossovers will always try and swap complete
//     - structures and will ignore this level.
// INVALIDTHESHOLD - Any entities with a DNA greater than this will be invalid

// 19/08/05 I have updated the whole of GE to use smartpointers
// Although my initial philosophy of not worrying about the efficiency of this part
// of the implementation since it is the fitness functions that will ultimately consume
// the majority of computing - given the large number of runs, large size of the genomes
// the memory management issues where critters are copied into and out of lists is
// (in my belief) getting ridiculous. Smart Pointers are the answer. Boost shared pointers.
// I don't like making the system dependant on a non-standard library, but its better than
// implementing my own smart pointers. I think.
// 
// [HISTORIC]
// One of the follow on changes from shared_ptrs is removing 
// the link between +,-,/,* and % from forms of crossover - it 
// was fine when it was one or two, but there are now over 12 crossover operators
// Also returning children by a structure just was always a bit of a hack.
// [CURRENT]
// now the static crossover functions will take smart pointers 
// (pointing to nothing) and change them to point at the new sprogs 
// if they could be created. I think that is a lot "smarter" an implementation.

// ***************** WARNING ******************************************************
// Warning: if using the new operators always check the children to ensure 
// they are not null successful breeding is not guaranteed!

// cr_data is copy create safe, it will copy all the approrpriate structures
            
#include <iostream>

/*************************************************************************
*                                                                       
*             FILE   :  critter.h                                       
*             AUTHOR :  Robin Harper                                    
*             DATE   :  August 2004                                    
*                                                                       
*  -------------------------------------------------------------------  
*    FUNCTIONALITY :                                                    
*      Encapusaltion of an entity                                     
*  -------------------------------------------------------------------  
*                                      
*                                                                       
*************************************************************************/
#include <cmath>
#include <string>
#include <bitset>
#include "entity.h"
#include "myrandom.h"
#include "params.h"
#include <list>
#include <boost/shared_ptr.hpp>
#include <set>

#include "interpreterBase.h"

class cr_data; // forward declaration

typedef boost::shared_ptr<cr_data> crPtr;
// These two comparisons are needed to ensure we can use the 
// shared ptrs in a STL setting.
bool crPtrGreaterThan(const crPtr &lhs,const crPtr &rhs); 
bool crPtrLessThan(const crPtr &lhs,const crPtr &rhs); 

class cr_data {
public:
          cr_data();
          ~cr_data();
		  static int serialNo;
		  int mySerial;
          cr_data(int*,long);
          cr_data(const cr_data &tocopy);               // copy constructor - needed as we have dynamic members
          cr_data& operator=(const cr_data &op2);		// assignment operator - again we have dynamic members
    
          void randomise(long,int *); // randomise the bit string
 
          const double get_fitness() const; // const so that it works with STL containers
          const int getAltScore() const; // ditto - the alternative score needs to be implemented in the 
                                         // Interpreter.
		  
		  bool fitness_tested() { return tested && !changed; } // lets you know if this critter has a fitness calculated.
          void reEvaluate(); // wipes any stored fitness, forcing a reCalculation (by a call to the interpreter) when
                             // get_fitness (or getAltScore) is called


          void set_fitness(double score); // Sometimes (normally for testing) you might want to set the fitness of the critter
                                       // You would not normally (ever) call this, instead you would call get_fitness which 
									   // submits the critter to the Interpreter and using the score provided by the Interpreter
                                       // assigns a fitness value to the critter
 
          void mutate(int chance);     // Mutate with the scecified chance in 10,000 of each bit flipping.
          void qmutate(int chance);
          void mutate(); // Mutate with an average of one bit flip per entity.
		  void smartMutate(int chance); // Specified chance in 10,000 of causing a mutation, somewhere in the dna, incrementing or decrementing a CODON
		  void smartSureMutate(int chance); // Specified chance in 10,000 of causing a muationt somewhere in the USED dna, inc or dec a CODON
 
          const string display() const;  // Wrapper for the display in entity - causes the phenotype to be printed out.
          const string displayMap(int map) const; // Calls the displayMap in the Interpreter (normally gives puzzle specific info)
                                                  // Map is just a problem specific parameter that can be passed to the interpreter.
 
          bool isEqual(cr_data *comp); // Straight bit comparison between two entities - not a phenotype comparison - not very useful.
          
          int age; // experimental to allow them to age and die. 
          // or more recently Age Layering - see Gregory Hornby ALPS paper.
          

//***************
// I need to think hard about exposing these, but it certainly simplifies a lot of things!
// Well more specifically it simplifies things in PopHandlers as I want to save this information.
// Perhaps what I should be doing is declaring them as friends.
// Do I need any of this in interpreters?
          void decode() { theEntity->decode(); } // Wrapper causing the bit string to be decoded into codons.
          long getDnaLength() const { 
                    if (theEntity) return theEntity->getDnaLength(); else return 0; 
          }
          long getUsefulDna() const { // Useful Dna is the Dna that is actually used to decode the phenotype.
                    if (theEntity) return theEntity->getUsefulDna(); else return 0; 
          }
          const int* getDna() { if (theEntity) return theEntity->getDna(); else return NULL; }
          const long* getCodons() { if (theEntity) return theEntity->getCodons(); else return NULL; }
          long getNumberOfCodons() const {
            if (theEntity) return theEntity->getNumberOfCodons(); else return 0; }
          int getUsefulCodonLength() { if (theEntity) return theEntity->getUsefulCodonLength(); else return 0; }
          int getFuncNos() const { // Assuming we are using Dynamically Defined functions, returns the number in the phenotype.
                    if (theEntity) return theEntity->getFuncNos(); else return -3; }
          
          vector<function> *getFunctions() { return theEntity->getFunctions();}
          int *getExpression() { return theEntity->getExpression(); }
		  int getExpressionLength() { return theEntity->getExpressionLength(); }
          int getCrossMethod() { return crossMethod; } // With self-selecting crossover we store a crossover preference here
          void setCrossMethod(int method) { crossMethod = method; }  // And set it here.
          const string getInvalidReason() { return grammar->getInvalidReason(); } // Bit of a hack - if the grammar fails to decode an 
          // an individual it fills in a static string with the reason it faile (e.g. ran out of codons).
          // It can be accessed through this method. But (like c Errno) it will change if the grammar is used in the meantime. So you 
          // you need to call this method as soon as the critter failed to decode. Again mainly used to debug - (e.g. why are all my critters
          // failing to decode?)
//*****************          

          bool isValid() const; // Tells you if the genotype decodes to a valid genotype using the grammar set with setGrammar.
		  bool validate(); // This checks to see if the interpreter needs to validate an individual, useful if you want to 
						   // keep some time of individuals out of the population e.g. in a network you might want
						   // to insist they connect at least one input and one output.
          bool makeValid(int maxDepth = 6,int minDepth = 2,bool maxGrow=false); 
		  // This is hook for a koza style ramped half and half. Its a bit experimental just now
          // You will need to implement it in the gramamr. It is saying conform the dna/codons of this critter so that it implements
          // a phenotype which has a depth of depth - if maxGrow is true, it makes sure each branch is a maxGrow depth.
          // calls a matching method in entity (needs to be at that level as needs to encode any changes needed into the dna).
          bool makeptc2(int expectedExpansions,int maxDepth=16);
		  // This is a hook for the ptc2 style of tree growth.

          int calcMutateChance(int); 
		  // This is experimental and currently not enabled. Its designed to allow the dna to control the mutation rate
          // given a base mutation rate (passed as a parameter) it gives the dna modified mutation rate. 
		  // I had some initial success with this but it got tricky and then I got distracted.
          
		  int getMutationRate(int); // Seems to be a wrapper for calcMutateChance - I forget why i had this as well.
          


// These are handy as static functions as its easy to set up runs
// But it will limit it to one interpreter and one grammar per "run"
// Will I need multiple runs of different critters when I move to a competitive environment?
// Only a problem if they are in the same process - no change at moment.

          static void setInterpreter(interpreterBase *ib) { interpreter = ib; } 
          static interpreterBase *getInterpreter() { return interpreter; }
          static void setGrammar(grammarBase *gb) { grammar = gb; }
#ifdef DEBUGCRITTERALLOC
		  static set<int> extantCritters;
#endif
// invalid_count can be used to gather statistics on, say, the number
// of invalid individuals created when certain crossover operators are used.
// its static so cr_data::invalid_count++; will increment it.
// invalid_count itself is protected so it can only be altered by methods of cr_data
// like the crossover operators.

          static int getInvalidCount() { return invalid_count; }
          static void  resetInvalidCount() { invalid_count = 0; }
          
		  // Some switches - they sure do seem to build up.

          // A simple switch where its true if the higher the fitness the better the individual
          // Set to false if the fitness is e.g. error and you want to minimise this.
          // The effect is to change the ">" and "<" operators 
          // so if higherIsBetter is false and critter1 has fitness 0.5 and critter2 has fitness 0.3
          // critter2 > critter1 is true.
          
		  static bool higherIsBetter;
          
          
		  static void AllowFullDnaToBeUsed(bool fl) { fullDnaToBeUsed = fl; }

          // Cant see why I should ever switch off AllowFullDnaToBeUSed. hmm
          // This is to do with the way that some crossover operators operate (see the code for
          // twoPointReplacementOperator as an example of what this means.
          
          // The next three are described at the top of this file.

          static int COMPARISONIMPACT;
          static int USEFULMANDATORY;
          static int INVALIDTHRESHOLD;
          
          // Again Two experimental switches. SAGA is to do with homologous type crossovers
          // using the LongestCommonSubString algorithm.
          // Mixed results have been obtained and I will probably explore his more later.
          
          static int SAGAApplicationMinimum;
		  static int MaxMut;
          
          // The next set of switches are to do with parse-tree crossovers 
          // like twoPointReplacementCrossover - using some of Koza's limits on leaves
		  // and more recently gpReplacement
          // and some of the ideas of a size fair crossover - W.B Langdon
          // Google search on "Size Fair and Homologous Tree Genetic Programming Crossovers"
          // I need to think about the Homologous elements.
          
          
          // Note: Do I want to implement a Depth-fair crossover? - see:
          // Kessler, M. and Haynes, T. 1999. Depth-fair crossover in genetic programming. 
          // In Proceedings of the 1999 ACM Symposium on Applied Computing 
          // (San Antonio, Texas, United States, February 28 - March 02, 1999). 
          // SAC '99. ACM Press, New York, NY, 319-323. DOI= http://doi.acm.org/10.1145/298151.298365
          
          // chance in 100 to constrain a tree-style crossover to crossing over
          // a non-leaf node. E.G set to 90 to mean that 90% of the time
          // any crossover is forced to choose non-leaf nodes
          
		  static int chanceOfTreeCrossover; 
          
		  // this is the size that a parse-tree will consider something not to be a leaf
          // based on the number of codons used to fully decode this expansion.
          // Normally this will be 2, allowing for single leafs (eg XVAL)
		  // or for CVALs (codon values) where you have the choice for CVAL 
		  // and then the codon value itself. It normally won't need to be
		  // changed, but there might be some grammars you need to fiddle with this.
		  
		  static int minSizeofNonLeaf; 
          
		  // If we are doing something like fair crossover set to a non-zero number (normaly above 1)
          // specifies the maximum eligible mathching tree in parent 2
          // so if we a choose, say a 8 codon tree and this is 1.5 then we can match it with a tree
          // of up to 12 codons.
          
		  static double maxMatchingTree;
          
		  // complimentary min paramater .e.g 0.5 would mean that we don't consider trees of less than 4 codons
          // if the first parents tree was 8 codons long- again 0 disables.

          static double minMatchingTree;
          
          // The next int is the chance in 10000 of the preferred crossover mutating if using variable crossover
		  // Probably still in the experimental category.
		  
		  static int variableCrossoverMutateChance;
          
//		  static bool SAGAUseGrammar;

// There is some residual of this left in the code, its harmless
// The idea behind it was to use a bit in each codon (the most sig one)
// to specify that crossover couldn't occur here!
// ie. to protect blocks, but it just caused the whole creature to become protected and
// premature convergence.
// However, with spatial co-evolution maybe this is not a problem
// Also I could borrow some ideas from Angeline and allow not an aboslute no crossover here!
// but have a mechanism whereby crossover is less or more likely to occur in areas along the DNA.
// But this is more in the speculative future than anything else.

#ifdef PROTECTIONENABLED
		  static bool USEPROTECTION;
#endif

// This was another thing I experimented with, whereby I allowed the mutation rate to be influenced by the 
// value of the codons - using the fact that multiple codon values give the same phenoytpe
// I raised (or was it lowered) the mutation rate if the codons values were below a certain number
// and did the reverse if they were above a certain number
// so without changing the phenotype it would be possible to change the mutation rate
// I had some initial success (I did this very early on) and then some confusing results
// so I discontinued meaning to come back to it, but so far never have.

		  static bool DnaInfluencedMutation;
		  
// Some SAGA functions to implement the LCSS algorithm. See Inman Harvey/SAGA

		  static void algb(long *first,int startFirst,int endFirst,long *second,int lengthSecond,int *L);
		  static int algd(long *first,int lengthFirst, long *second,int lengthSecond,int cross);
		  static int algrp(long *first,int startFirst,int endFirst,long *second,int startSecond,int endSecond);
		  
protected:

// The following few things are mutable so that the get_fitness functions  
// can alter them
// The get_fitness has to be const so cr_data STL containers can be sorted.)
		 // mutable bool valid;

          mutable bool changed;
          mutable double fitness; // fitness and valid need to be mutable as they can be changed (initialised) by
          mutable int altFitness;
          mutable bool tested;
          
// A pointer to the underlying entity which stores the bits and codons etc.    

          entity *theEntity;
          int crossMethod;
          
          static grammarBase *grammar; 
          static interpreterBase *interpreter;
          static int invalid_count;
          static bool fullDnaToBeUsed;

// [HISTORIC] when I was playing around with DNA influenced mutation rates.
// What I did was decide to see what happened if I kept a shadow set of codons
// which influenced the mutation only and could mutate/crossover like the other
// codons but which could not affect the phenotype (this probably doesnt make much 
// sense). It should have produced identical results to where I used the codon values
// themselves to calculate the the mutation rate, but it didn't and this was
// interesting, but proved too time consuming to track down - so like dnainfluenced 
// mutation was put on hold.
//		  int MutationCodons[200];
//		  static void copyMut(cr_data &,cr_data &,cr_children&);
//		  void mutateMutationCodons(int chance);

public:
// And the crossover operators - should they be part of this class or just friends?

          static void singlePointCodonOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          static void singlePointBitOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
		  static void gpReplacementOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
    
		  //static void singlePointMatchedOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          //static void taillessBitOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          //static void headlessBitOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          
          //static void twoPointBitOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          //static void twoPointCodonOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          //static void twoPointMatchedOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          //static void twoPointGutlessOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          static void twoPointReplacementOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          
		  //static void singlePointSAGAOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          //static void twoPointReplacementSAGAOperator(const crPtr&,const crPtr&,crPtr&,crPtr&);
          
          static void variableCrossover(const crPtr&,const crPtr&,crPtr &,crPtr &);
          static void variable3Crossover(const crPtr&,const crPtr&,crPtr &,crPtr &);
          //static void randomCrossover(const crPtr&,const crPtr&,crPtr &,crPtr &);
          //static void uniformCodonOperator(const crPtr&,const crPtr&,crPtr &,crPtr &);
          //static void mutateOnly(const crPtr&,const crPtr&,crPtr &,crPtr &);
	
// And comparison operators.
// Note that this compares critters based on their fitness
// If the fitness has not been calculated it will be (using the interpreter)
// Makes it easy to sort a vector of critters (or more accurate critter pointers)
// but could be time consuming if you have an expensive fitness calculation.
// so I thought I should make a note about it.
          friend const bool operator<(const  cr_data &o1,const  cr_data &o2);
          friend const bool operator>(const  cr_data &o1,const   cr_data &o2);
          friend const bool operator==(const cr_data &o1, const cr_data &o2);
          friend const bool operator!=(const cr_data &o1, const cr_data &o2);
 
};


#endif

