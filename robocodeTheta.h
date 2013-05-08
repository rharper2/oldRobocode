//*************************************************************************
//                                                                       
//                   FILE : robocodeTheta.h                                    
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

#ifndef robocodeTheta_t
#define robocodeTheta_t

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

class robocodeTheta: public grammarBase {
public:
	static robocodeTheta* Instance();
	bool parse(int *exp,int *length,int maxExp,long codons_list[],int max_codons,int &used,vector<function> *funcVec);
	void print_grammar_individual(ostream& output_stream,int expr[],int length) { print_grammar_individual(output_stream,expr,length,"NAME","0"); }
	void print_grammar_individual(ostream& output_stream,int expr[],int length, char *name , char *gen);
	void AllowFunctions(bool);
	bool doACreate(int *exp,int *length,int maxExp,long codons_list[],int max_codons,int &used,vector<function> *funcVec,int maxD = 6,
				   int minD = 2, bool maxG = false);
	bool ptc2Grow(int *exp, int *length, int maxExp, long codon_list[],
				  int max_codons, int &cUsed, vector<function> *funcVec,int maxD,int req_expansions);
	bool haveGrammarInfo() { return ExtractingGrammarInfo; }
	const string getInvalidReason() { return invalidReason; }
protected:
	robocodeTheta(); //Constructor
private:
	static robocodeTheta* _instance;
	void changeCodonsForGrow(int);
	bool doParse(int &);
	int localVariableType;
	
	
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
	void shrinkExpression();
	int latestVariableNo;
	int topVariableNo;
};

namespace robocodeTheta_namespace {
	
	
	enum {	CODE = 1001,
		MAINCODE = 1002,
		SCANROBOTCODE = 1003,
		HITBYBULLETCODE = 1004,
		HITROBOTCODE = 1005,
		HITWALLCODE = 1006,
		SENTENCES = 1007,
		SENTENCE = 1008,
		ACTION = 1009,
		TURNACTION = 1010,
		MOVEACTION = 1011,
		SETACTION = 1012,
		FIREACTION = 1013,
		VARIABLES = 1014,
		VARIABLE = 1015,
		CONTEXTVARIABLE = 1016,
		TWOVARIABLES = 1017,
		BOOL = 1018,
		COMP = 1019,
		BOOLOP = 1020,
		GLOBALVARIABLE = 1021,
		STACKVARIABLE = 1022,
		STACKTYPE = 1023,
		MAINLOOPVARIABLE = 1024,
		ONHITBYBULLETVARIABLE = 1025,
		ONSCANNEDROBOTVARIABLE = 1026,
		ONHITROBOTVARIABLE = 1027,
		ONHITWALLVARIABLE = 1028,
		SOMEFUNCTIONS = 1029,
		PDIVCODE = 1030,
		PUSHCODE = 1031,
		POPCODE = 1032,
		PEEKCODE = 1033,
		ONSTART = 1034,
		ONEXIT = 1035,
		NOTUSED = 1036,
		_DOFUNC = 1037,
		_DOPUSE = 1038,
		LOOPVARIABLE = 1039,
		MODIFIEDVARIABLE = 1040,
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
		IMPORT = 37,
		ROBOCODEDOTSTAR = 38,
		N = 39,
		PUBLIC = 40,
		CLASS = 41,
		NAME = 42,
		EXTENDS = 43,
		ADVANCEDROBOT = 44,
		STATIC = 45,
		STACK = 46,
		GLOBALSTACK1 = 47,
		GLOBALSTACK2 = 48,
		VOID = 49,
		RUN__ = 50,
		INMAINLOOP = 51,
		WHILE = 52,
		TRUE_ = 53,
		ONSCANNEDROBOT_SCANNEDROBOTEVENT = 54,
		E_CB = 55,
		SCANNEDROBOTEVENT = 56,
		PE = 57,
		INSCANNEDLOOP = 58,
		ONHITBYBULLET_HITBYBULLETEVENT = 59,
		HITBYBULLETEVENT = 60,
		INHITBYBULLETLOOP = 61,
		ONHITROBOT_HITROBOTEVENT = 62,
		HITROBOTEVENT = 63,
		INHITROBOTLOOP = 64,
		ONHITWALL_HITWALLEVENT = 65,
		HITWALLEVENT = 66,
		INHITWALLLOOP = 67,
		IF = 68,
		ELSE = 69,
		SETTURNRIGHT_OB = 70,
		SETTURNLEFT_OB = 71,
		SETTURNGUNRIGHT_OB = 72,
		SETTURNGUNLEFT_OB = 73,
		SETTURNRADARRIGHT_OB = 74,
		SETTURNRADARLEFT_OB = 75,
		SETAHEAD_OB = 76,
		SETBACK_OB = 77,
		RESUME__ = 78,
		STOP__ = 79,
		SETADJUSTGUNFORROBOTTURN_OB = 80,
		SETADJUSTRADARFORROBOTTURN_OB = 81,
		SETADJUSTRADARFORGUNTURN_OB = 82,
		SAFEPUSH_OB = 83,
		SAFESWAP_OB = 84,
		FIRE_OB = 85,
		SETFIRE_OB = 86,
		PDIV_OB = 87,
		NOT_OB = 88,
		AND = 89,
		XOR = 90,
		OR = 91,
		GETX__ = 92,
		GETY__ = 93,
		GETHEADING__ = 94,
		GETGUNHEADING__ = 95,
		GETGUNHEAT__ = 96,
		GETRADARHEADING__ = 97,
		GETBATTLEFIELDWIDTH__ = 98,
		GETENERGY__ = 99,
		SAFEPOP_OB = 100,
		SAFEPEEK_OB = 101,
		//GLOBALSTACK1 = 102,
		//LOCALSTACK1 = 103,
		//LOCALSTACK2 = 104,
		E_DOTGETBEARING__ = 105,
		E_DOTGETHEADING__ = 106,
		PE_DOTGETBEARING__ = 107,
		PE_DOTGETHEADING__ = 108,
		E_DOTGETVELOCITY__ = 109,
		E_DOTGETENERGY__ = 110,
		E_DOTGETDISTANCE__ = 111,
		PE_DOTGETVELOCITY__ = 112,
		PE_DOTGETENERGY__ = 113,
		PE_DOTGETDISTANCE__ = 114,
		DOUBLE = 115,
		TOP = 116,
		BOTTOM = 117,
		RETURN = 118,
		DIVIDED = 119,
		THESTACK = 120,
		ITEM = 121,
		THESTACKDOTPUSH_OB = 122,
		NEW = 123,
		DOUBLE_OB = 124,
		THESTACKDOTEMPTY__ = 125,
		THESTACKDOTPOP__ = 126,
		THESTACKDOTEMTPY__ = 127,
		THESTACKDOTPEEK__ = 128,
		//LOCALSTACK1, = 129,
		BOOLW = 130,
		HISTORY = 131,
		FALSE = 132,
		NOTHISTORY = 133,
		E = 134,
		TRUE = 135,
		//		_DOFUNC = 136,
		//		_DOPUSE = 137,
		DPVAL = 138,
		DFVAL = 139,
		PARAMS = 140,
		PARAM = 141,
		FPARS = 142,
		FUNCBODY = 143,
		CVAL = 144,
		POPTREE = 145,
		POPTREED = 146,
		_FUNC = 147,
		_PUSE = 148,
		EXECUTE = 149,
		MLLOCALSTACK1 = 150,
		MLLOCALSTACK2 = 151,
		OHBBLOCALSTACK1 = 152,
		OHBBLOCALSTACK2 = 153,
		OHRLOCALSTACK1 = 154,
		OHRLOCALSTACK2 = 155,
		OHWLOCALSTACK1 = 156,
		OHWLOCALSTACK2 = 157,
		OSRLOCALSTACK1 = 158,
		OSRLOCALSTACK2 = 159,
		EXCLAMATION = 161,
		HBBHISTORY = 162,
		OSRHISTORY = 163,
		HRHISTORY = 164,
		HITWALLHISTORY = 165,
		PHBBEVENT = 166,
		PSREVENT = 167,
		PHREVENT = 168,
		PHWEVENT = 169,
		DOVARIABLEDEFS = 170,
	    SOMECOMMENTS = 171,
		FOR = 172,
		OUTOFSCOPE = 173,
		NEWINSCOPE = 174,
		ABS = 175,
		ACOS = 176,
		ASIN = 177,
		ATAN = 178,
		ATAN2 = 179,
		IN = 180,
		COS = 181,
		SIN = 182,
		MAX = 183,
		MIN = 184,
		ROUND = 185,
		SQRT = 186,
		POW = 187,
		ISNEAR = 188,
		NORMALABSOLUTEANGLE = 189,
		NORMALNEARABSOLUTEANGLE = 190,
		NORMALRELATIVEANGLE = 191,
		GETBATTLEFIELDHEIGHT__ = 192,
		COMMATWO = 193,
		__DOFUNC = 5001,
		__DOPUSE = 5002};
}


#endif
