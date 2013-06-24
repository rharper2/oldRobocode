/*
 *  entity.h
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

#ifndef entity_t
#define entity_t

//#define DEBUGALLOCATION  // Turns on constructor and destructor messages

#include <fstream>
#include <iostream>
#include <string>
#include "grammarBase.h"
#include "params.h"

// Note this class is now wrapped by cr_data which is the class 
// to be used in a GE system using this software.


class entity{

public:

          // Normal constructor as well as copy and assignment constructors. No Default constructor 
          entity(long strlen, int str[]); //Constructor
          entity(const entity &tocopy); // copy constructor - needed as we have dynamic members
          entity& operator=(const entity &op2); // assignment operator - again we have dynamic members
          ~entity();
 
          // Operational functions
          bool validate(grammarBase *g); 
          
          // makeValid is designed as a hook to allow koza
          // style creature initialisation i.e. to allow a bit
          // stream to be randomly generated which when parsed leads to 
          // a tree of a certain minimum depth
          // it needs to be implemented in the specific grammar and is thus
          // problem dependent.
          
          bool makeValid(grammarBase *g,int maxDepth,int minDepth,bool maxCreate); 
          bool makeptc2(grammarBase *g,int expectedExpansions,int maxDepth);

          bool mutate(int chance);
          bool qmutate(double chance);
          bool mutate(); // sets the chance to give on average one relevant mutation.
		  bool smartMutate(double chance);
		  bool smartSureMutate(double chance);
          void decode(); // decodes the binary string into codons
          // Now with gray encoding!
          unsigned int grayencode(unsigned int g);
          unsigned int graydecode(unsigned int gray);
          
          // Access functions 
          long getDnaLength() { return dnaLength; } // consider changing this to a bitstring
          const int *getDna() { return dna; }
          int getFuncNos();

          //consider if we can make these return const
          int getExpressionLength() { return expressionLength; }
          int *getExpression() { return expression; }
    
          long *getCodons() { return codonIntegers; }
          int getUsefulCodonLength() { return usedLength; }
		  int getNumberOfCodons() { return numberOfCodons; }
          vector<GEfunction> *getFunctions() { return &functionVector; }
          
          string display(grammarBase *g);
          
          // Test functions 
 //         int getScore(func_interp &testbox);
 //         string scoreDisplay(int,func_interp &testbox);
          bool isValid() { return valid; }
		  bool beenDecoded() { return decoded; }
          bool isEqual(entity *);

          long getUsefulDna() { return usefulDna; }
          void resetChanged() { changed = true; }
          
          static bool displayCodonInfo;
          
          
private:

// The following three are used to help initialisation
          void initialiseDynamic();
          void initialiseRest();
          void copyBits(int *,long *);

// Misc internal functions
          void decodeBinaryString(int str[]);
          int decode_binary(int string[], long start, int len);
          void recode_binary();
          void recode_binary(int*,long);

// Private data
          int *dna;
          long dnaLength;
          long usefulDna;

          long *codonIntegers;
          int numberOfCodons;
          int usedLength;

          vector<GEfunction> functionVector;
          int expression[MAX_LENGTH_OF_EXPRESSION]; // parsed grammatical expression
          int expressionLength;

          bool valid;
          bool changed;
          bool decoded;

#ifdef DEBUGALLOCATION
          static int entityNumber;
          int mynumb;
#endif

};


#endif

