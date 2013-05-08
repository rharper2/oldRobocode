//*************************************************************************
//                                                                       
//                   FILE : robocodeAlpha.h                                    
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

#ifndef robocodeAlpha_t
#define robocodeAlpha_t

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

class robocodeAlpha: public grammarBase {
public:
	static robocodeAlpha* Instance();
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
	robocodeAlpha(); //Constructor
private:
	static robocodeAlpha* _instance;
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

namespace robocodeAlpha_namespace {


	enum {	CODE = 1001,
		MAINSENTENCES = 1002,
		MAINSENTENCE = 1003,
		MAINACTION = 1004,
		MTURNACTION = 1005,
		MMOVEACTION = 1006,
		MSETACTION = 1007,
		FIREACTION = 1008,
		MVARIABLES = 1009,
		MVARIABLE = 1010,
		TWOMVARIABLES = 1011,
		MBOOL = 1012,
		COMP = 1013,
		BOOLOP = 1014,
		OHBBSENTENCES = 1015,
		OHBBSENTENCE = 1016,
		OHBBACTION = 1017,
		OHBBTURNACTION = 1018,
		OHBBMOVEACTION = 1019,
		OHBBSETACTION = 1020,
		TWOOHBBVARIABLES = 1021,
		OHBBBOOL = 1022,
		OHBBVARIABLES = 1023,
		OHBBVARIABLE = 1024,
		OSRSENTENCES = 1025,
		OSRSENTENCE = 1026,
		OSRACTION = 1027,
		OSRTURNACTION = 1028,
		OSRMOVEACTION = 1029,
		OSRSETACTION = 1030,
		TWOOSRVARIABLES = 1031,
		OSRBOOL = 1032,
		OSRVARIABLES = 1033,
		OSRVARIABLE = 1034,
		OHRSENTENCES = 1035,
		OHRSENTENCE = 1036,
		OHRACTION = 1037,
		OHRTURNACTION = 1038,
		OHRMOVEACTION = 1039,
		OHRSETACTION = 1040,
		TWOOHRVARIABLES = 1041,
		OHRBOOL = 1042,
		OHRVARIABLES = 1043,
		OHRVARIABLE = 1044,
		OHWSENTENCES = 1045,
		OHWSENTENCE = 1046,
		OHWACTION = 1047,
		OHWTURNACTION = 1048,
		OHWMOVEACTION = 1049,
		OHWSETACTION = 1050,
		TWOOHWVARIABLES = 1051,
		OHWBOOL = 1052,
		OHWVARIABLES = 1053,
		OHWVARIABLE = 1054,
		GLOBALVARIABLE = 1055,
		MAINLOOPVARIABLE = 1056,
		ONHITBYBULLETVARIABLE = 1057,
		ONSCANNEDROBOTVARIABLE = 1058,
		ONHITROBOTVARIABLE = 1059,
		ONHITWALLVARIABLE = 1060,
		PDIVCODE = 1061,
		NOTUSED = 1062,
		_DOFUNC = 1063,
		_DOPUSE = 1064,
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
		ROBOT = 44,
		VOID = 45,
		RUN__ = 46,
		WHILE = 47,
		TRUE_ = 48,
		ONSCANNEDROBOT_SCANNEDROBOTEVENT = 49,
		E_CB = 50,
		ONHITBYBULLET_HITBYBULLETEVENT = 51,
		ONHITROBOT_HITROBOTEVENT = 52,
		ONHITWALL_HITWALLEVENT = 53,
		IF = 54,
		ELSE = 55,
		TURNRIGHT_OB = 56,
		TURNLEFT_OB = 57,
		TURNGUNRIGHT_OB = 58,
		TURNGUNLEFT_OB = 59,
		TURNRADARRIGHT_OB = 60,
		TURNRADARLEFT_OB = 61,
		AHEAD_OB = 62,
		BACK_OB = 63,
		RESUME__ = 64,
		STOP__ = 65,
		SETADJUSTGUNFORROBOTTURN_OB = 66,
		SETADJUSTRADARFORROBOTTURN_OB = 67,
		SETADJUSTRADARFORGUNTURN_OB = 68,
		FIRE_1 = 69,
		FIRE_2 = 70,
		FIRE_3 = 71,
		FIRE_4 = 72,
		PDIV_OB = 73,
		NOT_OB = 74,
		AND = 75,
		XOR = 76,
		OR = 77,
		AHEAD_ = 78,
		BACK_ = 79,
		PDIV_ = 80,
		NOT_ = 81,
		GETX__ = 82,
		GETY__ = 83,
		GETHEADING__ = 84,
		GETGUNHEADING__ = 85,
		GETGUNHEAT__ = 86,
		GETRADARHEADING__ = 87,
		GETBATTLEFIELDWIDTH__ = 88,
		GETENERGY__ = 89,
		E_DOTGETBEARING__ = 90,
		E_DOTGETHEADIGN__ = 91,
		E_DOTGETHEADING__ = 92,
		E_DOTGETVELOCITY__ = 93,
		E_DOTGETENERGY__ = 94,
		E_DOTGETDISTANCE__ = 95,
		E_DOTISMYFAULT__ = 96,
		DOUBLE = 97,
		TOP = 98,
		BOTTOM = 99,
		RETURN = 100,
		DIVIDED = 101,
	//	_DOFUNC = 102,
	//	_DOPUSE = 103,
		DPVAL = 104,
		DFVAL = 105,
		PARAMS = 106,
		PARAM = 107,
		FPARS = 108,
		FUNCBODY = 109,
		CVAL = 110,
		POPTREE = 111,
		POPTREED = 112,
		_FUNC = 113,
		_PUSE = 114,
		__DOFUNC = 5001,
		__DOPUSE = 5002};
}


#endif
