//*************************************************************************
//                                                                       
//                   FILE : networkG_SensorNew.h                                    
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

#ifndef networkG_SensorNew_t
#define networkG_SensorNew_t

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <list>
#include "functions.h"
#include "grammarBase.h"
#include "DataCollector.h"
#include "pct2Grow.h"
#include <cmath>
#define MAXFUNCS 15
#define TOOBIGERRORMARGIN 30


using namespace std;

class networkG_SensorNew: public grammarBase {
public:
          static networkG_SensorNew* Instance();
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
          networkG_SensorNew(); //Constructor
private:
          static networkG_SensorNew* _instance;
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

namespace networkG_SensorNew_namespace {


	enum {	CODE = 1001,
		DEFINEFUNC = 1002,
		FUNCBODY = 1003,
		COMMANDS = 1004,
		COMMAND = 1005,
		LOOPCOMMANDS = 1006,
		VALUE = 1007,
		NUMBER = 1008,
		OP = 1009,
		PARAMS = 1010,
		FPARS = 1011,
		NEWNETWORK = 1012,
		THREECOMMAND = 1013,
		TWOCOMMAND = 1014,
		ONECOMMAND = 1015,
		CONNECTION = 1016,
		BOOL = 1017,
		COND = 1018,
		BOOLOP = 1019,
		DFVAL = 1020,
		DPVAL = 1021,
		COMP = 1022,
		NOTUSED = 1023,
		_DOFUNC = 1024,
		_DOPUSE = 1025,
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
		IF = 37,
		THEN = 38,
		ELSE = 39,
		SETWEIGHT = 40,
		WAIT = 41,
		END = 42,
		_DOFUNC = 43,
		CVAL = 44,
		_DOPUSE = 45,
		PARAM = 46,
		ADD_SPLIT = 47,
		ADD_SPLIT_CONT = 48,
		ATTACH = 49,
		ADD_REVERSE = 50,
		CONNECT = 51,
		LOOP = 52,
		DESTINATIONTONEXT = 53,
		SOURCETONEXT = 54,
		SOURCETOPARENT = 55,
		SEQ = 56,
		PAR = 57,
		SEQCOPYIN = 58,
		SEQCOPYOUT = 59,
		LSPLITNO = 60,
		USPLITNO = 61,
		XSPLIT = 62,
		TSPLIT = 63,
		PUSH = 64,
		PUSHR = 65,
		SETSIGMA = 66,
		SETEXPA = 67,
		SETBIAS = 68,
		CONNECTINPUT = 69,
		CONNECTOUTPUT = 70,
		SPLITINPUTHORIZONTALLY = 71,
		SPLITINPUTVERTICALLY = 72,
		CONNECTHEIGHT = 73,
		CONNECTAOA = 74,
		NOT = 75,
		AND = 76,
		OR = 77,
		NOPP = 78,
		AMOUNT = 79,
		JMP = 80,
		JMPIFTRUE = 81,
		JMPIFFALSE = 82,
		DUMMY = 83,
		XOR = 84,
		EXEC = 85,
		CALL = 86,
		DUP = 87,
		POP = 88,
		PPR = 89,
		NORMALISELOOPCOUNT = 90,
		AVALUE = 91,
		PAR1 = 92,
		PAR2 = 93,
		PVAL = 94,
		POPTREE = 95,
		POPTREED = 96,
		_FUNC = 97,
		_PUSE = 98,
		__DOFUNC = 5001,
		__DOPUSE = 5002};
}


#endif
