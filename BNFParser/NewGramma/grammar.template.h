//*************************************************************************
//                                                                       
//                   FILE : GRAMMARNAME.h                                    
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

#ifndef GRAMMARNAME_t
#define GRAMMARNAME_t

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

class GRAMMARNAME: public grammarBase {
public:
          static GRAMMARNAME* Instance();
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
          GRAMMARNAME(); //Constructor
private:
          static GRAMMARNAME* _instance;
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

namespace GRAMMARNAME_namespace {


