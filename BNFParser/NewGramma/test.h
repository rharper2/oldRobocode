//*************************************************************************
//                                                                       
//                   FILE : test.h                                    
//     Automatically generated from a DynamicFunctionTemplate grammar.
//     The author of the template grammar was Robin Harper - robinh@cse.unsw.edu.au, 
//     rharper2@mac.com
//     Date : 28 October 2004
//	   Updated to version 0.3: 8 August 2010
//     
//     Based on grammar.h generated from BNFParser, part of a package written by
//                 AUTHOR :  Michael O'Neill                               
//                 DATE   :  MAY 98 ->                                     
//                 SCHOOL :  Dept. Computer Science                        
//                          University of Limerick                        
//*************************************************************************

#ifndef test_t
#define test_t

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <list>
#include "functions.h"
#include "grammarBase.h"
#include "DataCollector.h"
#include "ptc2Grow.h"
#include <cmath>
#define MAXFUNCS 15
#define TOOBIGERRORMARGIN 30


using namespace std;

class test: public grammarBase {
public:
          static test* Instance();
		  bool parse(int *exp,int *length,int maxExp,long codons_list[],int max_codons,int &used,vector<function> *funcVec);
          void print_grammar_individual(ostream& output_stream,int expr[],int length);
          void AllowFunctions(bool);
          bool doACreate(int *exp,int *length,int maxExp,long codons_list[],int max_codons,int &used,vector<function> *funcVec,int maxD = 6,
                          int minD = 2, bool maxG = false);
		  bool ptc2Grow(int *exp, int *length, int maxExp, long codon_list[],
							int max_codons, int &cUsed, vector<function> *funcVec,int maxD,int req_expansions);
          bool haveGrammarInfo() { return ExtractingGrammarInfo; }
          const string getInvalidReason() { return invalidReason; }
protected:
          test(); //Constructor
private:
          static test* _instance;
          void changeCodonsForGrow(int);
          bool doParse(int &);
           
          void read_func();
          int *expression,*expressionLength,functionNos;
		  long *codons;
          int maxCodons,maxExpression,expressionPos,codonPos,noOfParams;
          vector<function> *funcVector;
          void apply_one_grammar(int expr[], int *curr, int *length); 
		  bool dealtWithPop(int expr[],int *curr,int *length);
          bool invalid,AllowDefun,AllowParams;
          bool terminating;
          bool allowFunctions;
          string invalidReason;
		  list<int> stack;
		  dataCollector *dc;
		  bool ExtractingGrammarInfo;
          vector<int> terminals,nonTerminals;
          int depth;
          int maxDepth,minDepth;
          bool maxGrow,growing;
    	  void ptc2GrowOne(bool allowExpand);
		  bool traverseParseTree(parseTreePtr cNode);
		  vector<parseTreePtr> currentNT;
		  int expansions;
		  int toExpand[100],expandAt,expandLength;
		  bool doingPTC2;
		  int balloonExpansion;
		  bool expandPTC2;
		  int lastCodonPos;
};

namespace test_namespace {


	enum {	CODE = 1001,
		VALUE = 1002,
		COMMA = 1,
		ORB = 2,
		CRB = 3,
		OSB = 4,
		CSB = 5,
		DEFUN = 6,
		OFB = 7,
		CFB = 8,
		OCB = 9,
		CCB = 10,
		BEGINEXEC = 11,
		ENDEXEC = 12,
		PLUS = 13,
		MINUS = 14,
		MULT = 15,
		DIV = 16,
		SEMICOLON = 17,
		COLON = 18,
		GREATER = 19,
		LESS = 20,
		EEQUAL = 21,
		EQUAL = 22,
		ZERO = 23,
		ONE = 24,
		TWO = 25,
		THREE = 26,
		FOUR = 27,
		FIVE = 28,
		SIX = 29,
		SEVEN = 30,
		EIGHT = 31,
		NINE = 32,
		NOP = 33,
		BEPOPTREE = 34,
		FUNCPLACEHOLDER = 35,
		PUSEPLACEHOLDER = 36,
		XVAL = 37,
		CVAL = 38,
		POPTREE = 39,
		POPTREED = 40};
}


#endif
