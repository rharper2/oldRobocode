//*************************************************************************
//                                                                       
//    FILE : robocodeTheta.cpp                                    
//    Author : Generated automatically by the DynamicFunctionGrammar
//    DynamicFunctionGrammar was written by Robin Harper currently v0.3
//    Based on Gramma v0.63 programmed by Miguel Nicolau
//*************************************************************************
//
//
// Please note that the grammar re-writes some of the rules for function calls and paramater use
// The branches where DEFUN,FUNC and PUSE appear will be re-written in this code.
// In particular calls to FUNC and PUSE are replaced by calls to _DOFUNC and _DOPUSE respectively.
// Then additional rules are inserted into the grammar to deal with these new non-terminals 
// which respectively use the codons to call functions or use paramaters (if they have been defined)
// or to use the default values if they have not.
// The main reason for this is it simplifies the PTC2 algorithm vastly (see later comments for details).
//
//
/***************************************************************/
#include <iostream>
#include "robocodeTheta.h" 
#include "myrandom.h"
/***************************************************************/
// File generated from grammar included in file:
// robocodeTheta.bnf
/***************************************************************/
// code:	import robocodeDOTSTAR ; N public class NAME extends AdvancedRobot { N static stack globalStack1, globalStack2 ; // N SOMEFUNCTIONS N MAINCODE N SCANROBOTCODE N HITBYBULLETCODE N HITROBOTCODE N HITWALLCODE }
// 
// MAINCODE: public void run() { N INMAINLOOP Sentences N while ( true_ ) { Sentences } N } 
// 
// SCANROBOTCODE: public void onScannedRobot_ScannedRobotEvent e_CB { N static ScannedRobotEvent pe ; N ONSTART INSCANNEDL// OOP Sentences ONEXIT } N 
// 
// HITBYBULLETCODE: public void onHitByBullet_HitByBulletEvent e_CB { N static HitByBulletEvent pe ; N ONSTART INHITBYBULL// ETLOOP Sentences ONEXIT } N 
// 
// HITROBOTCODE: public void onHitRobot_HitRobotEvent e_CB { N static HitRobotEvent pe ; N ONSTART INHITROBOTLOOP Sentence// s ONEXIT } N 
// 
// HITWALLCODE: public void onHitWall_HitWallEvent e_CB { N static HitWallEvent pe ; N ONSTART INHITWALLLOOP Sentences ONE// XIT } N 
// 
// 
// Sentences:  Sentence  Sentences
// 		| Sentence 
// 
// 
// Sentence:	if ( Bool ) { Sentences } else { Sentences }
// 		| Action ;
// 
// 
// Action:	turnAction 
// 		| moveAction
// 		| setAction
// 		| fireAction
// 
// turnAction:	setTurnRight( Variable )
// 	 	| setTurnLeft( Variable )
// 		| setTurnGunRight( Variable )
// 		| setTurnGunLeft( Variable )
// 		| setTurnRadarRight( Variable )
// 		| setTurnRadarLeft( Variable )
// 
// moveAction:	setAhead( Variables )
// 		| setBack( Variables )
// 		| resume__
// 		| stop__
// 
// setAction:	setAdjustGunForRobotTurn( Bool )
// 		| setAdjustRadarForRobotTurn( Bool )
// 		| setAdjustRadarForGunTurn( Bool )
// 		| safePush( stackType , Variables )
// 		| safeSwap( stackType )
// 
// fireAction:	fire( Variable % 5 )
// 		| setfire( Variable % 5 )
// 		
// Variables: 	twoVariables
// 		|  Variable
// 
// Variable:	globalVariable
// 		|  contextVariable
// 	
// contextVariable: globalVariable
// 
// twoVariables:	( Variables + Variables )
// 		| ( Variables - Variables )
// 		| ( Variables * Variables )
// 		| PDIV( Variables , Variables )
// 
// Bool: 		( Bool boolop Bool )
// 		| ( Variable comp Variable )
// 		| NOT( Bool )
// 
// comp:		> | < | ==
// 
// boolop:		AND | XOR | OR
// 
// globalVariable:	getX() 
// 		| getY__
// 		| getHeading__
// 		| getGunHeading__
// 		| getGunHeat__
// 		| getRadarHeading__
// 		| getBattleFieldWidth__	
// 		| getBattleFieldWidth__
// 		| getEnergy__
// 		| 1
// 		| 0
// 		| stackVariable
// 
// stackVariable: safePop( stackType )
// 				| safePeek( stackType )
// 				
// stackType: globalStack1 | globalStack2 | localStack1 | localStack2
// 		
// 
// mainLoopVariable: 
// 		getHeading__
// 
// onHitByBulletVariable:	e.getBearing__
// 			| e.getHeading__
// 			| pe.getBearing__
// 			| pe.getHeading__
// 
// onScannedRobotVariable:  e.getBearing__
// 			| e.getHeading__
// 			| e.getVelocity__
// 			| e.getEnergy__
// 			| e.getDistance__
// 			| pe.getBearing__
// 			| pe.getHeading__
// 			| pe.getVelocity__
// 			| pe.getEnergy__
// 			| pe.getDistance__
// 
// onHitRobotVariable:	e.getBearing__		
// 			| e.getEnergy__
// 			| pe.getBearing__		
// 			| pe.getEnergy__
// 			
// 
// onHitWallVariable:	e.getBearing__
// 					| pe.getBearing__
// 
// SOMEFUNCTIONS: PDIVCODE N PUSHCODE N POPCODE N PEEKCODE 
// 
// PDIVCODE:	public double PDIV( double top , double bottom ) { if ( bottom == 0 )  return 1 ; else return top DIVIDED b// ottom ;  }
// PUSHCODE:	public void safePush( stack thestack , double item ) { thestack.push( new Double( item ) ; }
// POPCODE:	public double safePop( stack thestack ) { if ( thestack.empty() ) return 0 else return thestack.pop() ; // }
// PEEKCODE:	public double safePeek( stack thestack ) { if ( thestack.emtpy() ) return 0 else return thestack.peek__//  ; }
// 		
// ONSTART:	static stack localStack1, localStack2 ; N static bool history = false ; N if ( NOThistory ) pe = e ; N
// ONEXIT:		if ( history ) pe = e ;  else history = true ; N
// NOTUSED:	DEFUN *( )* | FUNC | PUSE | DPVAL | DFVAL | PARAMS | PARAM | FPARS | FUNCBODY | CVAL
// 
// %%
/***************************************************************/
using namespace robocodeTheta_namespace;

robocodeTheta *robocodeTheta::_instance = 0;


robocodeTheta::robocodeTheta() { 
	allowFunctions = true;
	ExtractingGrammarInfo = true;
	stack.clear();
	dc = dataCollector::Instance();
}

robocodeTheta* robocodeTheta::Instance() {
	if (_instance == 0) {
		_instance = new robocodeTheta();
	}
	return _instance;
}
// #define DEBUGPTC2

bool robocodeTheta::doACreate(int *exp,int *length,int maxExp,long codon_list[],
							 int max_codons,int &cUsed,vector<function> *funcVec,int maxD,int minD,bool mG) {
	maxGrow = mG;
	maxDepth = maxD;
	minDepth = minD;
	depth = 0;
	growing = true;
	bool back = parse(exp,length,maxExp,codon_list,max_codons,cUsed,funcVec);
	growing = false; //tidy up
	return back;
}

bool robocodeTheta::ptc2Grow(int *exp, int *length, int maxExp, long codon_list[],
							int max_codons, int &cUsed, vector<function> *funcVec,int maxD,int req_expansions) {
	expression = exp;
	expressionLength = length;
	
	maxDepth = maxD;
	expansions = req_expansions;
	int expansionsDone = 0;
	currentNT.clear();
	parseTreePtr start;
	start.reset(new parseTreeNode());
	start->value = 1001;
	start->depth = 0;
	maxGrow = true;
	terminating = false;
	AllowDefun = allowFunctions;
	AllowParams = false;
	funcVector = funcVec;
	funcVector->clear();
	functionNos = 0;
	latestVariableNo = topVariableNo = 0;
	currentNT.push_back(start);
	
	// So expand the tree one nt at a time, until we have
	// the requisite number of expansions (or the tree has no place
	// for more expansions.
	codons = codon_list;
	codonPos = 0;
	maxCodons = max_codons;
	maxExpression = maxExp;
	
	// PTC2 randomly expands non-terminals into more non-terminals
	// until we have hit the requisite number of expansions. It then 
	// expands any remaining non-terminals into terminal expansions.
	
	// We maintain a parse tree and each time a non-terminal is expanded the parse tree
	// is grown and the value of the codon required to expand it is stored.
	// Once we have fully expanded the parse tree, the codons required are 
	// read from the parse tree and stored in the codon_list.
	
	// The relevant function is ptc2GrowOne, which chooses a non-terminal in
	// the parse tree (a vector of these is kept at currentNT)
	// and if its called ptc2GrowOne(true), it expands it if possible - choosing 
	// a grammar branch with at least one more non-terminal in it
	// if its called with ptc2GrowOne(false), then it attempts to terminate the 
	// expansion of the non-terminal (i.e. expand it with a branch which contains only
	// terminals)
	
	doingPTC2 = true; // we are going to construct it
	expandPTC2= false; // but we are not yet creating it
	
	// Effectively this flags that when we hit a potential function call
	// just to place a marker that a function call will be made.
	
	// The reason we need to do this is that the way PTC2 works it expands randomly
	// chosen potential expansions in the expression (upto the designated number)
	// This is fine and dandy apart from when we have dynamically defined functions.
	// Since we are not expanding the genome sequentially we might not yet know
	// how many functions we have and even more importantly how many parameters there are.
	// for any funciton we may or may not call.
	
	// So we deal with this as follows:
	//   Place a marker in the parse tree we are creating saying a funciton will be called here
	//   Carry on expanding, when we are done, terminate the expansions as normal
	//   Now put all the codons we needed for the expansions (and terminations) in the codon list
	//   Set the flags that we will be expanding a PTC2 tree
	//   Then when parsing (sequentially) and we hit a potential function call
	//   We set balloonexpansions to true and add codons until after the function call is dealt
	//   with then carry on as before (we can do this, because this parse we are doint it sequentially
	//   and will know how many functions we have).
	
	localVariableType = 2;
	
	
	do {
		ptc2GrowOne(true); // true = expand
		expansionsDone++;
		//				cout << "Expansions Done " << expansionsDone << endl;
	} while (!currentNT.empty() && (expansionsDone < expansions));
	if (expansionsDone < expansions) {
		cout << "Check this, your tree couldn't be expanded enough - strange\n";
		exit(1); // not really a terminating error, but whilst testing I want to be sure of seeing it
	}
	while (!currentNT.empty() && !terminating) {
		ptc2GrowOne(false); // false = put in terminals
	}
	// so now we have a parse tree pointed to by start
	// which contains the requisite number of expansions
	// We need to traverse the tree, copying the codons
	// stored in it into the codon_list
	if (terminating) {
		// not sure how!
		cout << "Terminating was set to true, track it down!";
		exit(1);
	}
	codons = codon_list;
	codonPos = 0;
	maxCodons = max_codons;
	traverseParseTree(start);
	// ok, codons is now all set up, lets parse them (again) which
	// a) checks we have done them properly, 
	// b) sets up everything else i.e. all the grammar info tied back to the codons.
	// c) deals with functions (remember we just have a marker that a function should be called)
	
	growing = false;
	
	// Now we need to put it through the parser
	doingPTC2 = false; // we have finished constructing it
	expandPTC2= true; // but we are going to create it
	// What this allows us to do is it flags that when
	// we hit a branch with a potential function call
	// we set balloonExpansion and carry on
	// When balloonExpansion is set this means dont use up the 
	// codons, but create new ones as necessary until we have dealt with the
	// function call (we know we have dealt with it because
	// we will hit a BEPOPTREE
	// balloonExpansion is an integer to allow function calls
	// to "stack" so we only stop expanding when we hit zero.
	
	balloonExpansion = 0;
	localVariableType = 0;
	latestVariableNo = topVariableNo = 0;
	
	expression = exp;
	expressionLength = length;
	
	maxExpression = maxExp;
	maxCodons = max_codons;
	
	funcVector = funcVec;
	funcVector->clear();
	functionNos = 0;
	
	invalid = false;
	terminating = false;
	*length = 1;
	
	AllowDefun = allowFunctions;
	AllowParams = false;
	expressionPos = 0;
	expression[0]=1001; 
	codonPos = 0;
	stack.clear();
	// dc->resetCounters();
	//cout << "PTC2 all set up to parse, about to do it\n";
#ifdef DEBUGPTC2
	cout << "First 50 codons are:\n";
	for (int i=0;i<50;i++) {
		cout << (codons[i] & CODONMASK) << ", ";
	}
	cout << endl;
#endif
	bool toreturn = doParse(cUsed);
	
	// For debugging/testing purposes lets print it out
	// print_grammar_individual(cout,expression,*expressionLength);
	return toreturn;
}

void robocodeTheta::ptc2GrowOne(bool allowExpand) {
	// choose a random member of currentNT, get temp pointing at it
	// swap temp and the last one in currentNT
	// delete the last one. (i.e. remove temp from currentNT).
	
	int size = currentNT.size();
	int choice = randomint(size);
	parseTreePtr toUse = currentNT[choice];
	if (choice != size -1) { // not the last one
		currentNT[choice]=currentNT[size-1];
	}
	currentNT.pop_back();
	
	// ok toUse points at the expansion we want to expand
	
	toExpand[0] = toUse->value;
	if (toExpand[0] == CONTEXTVARIABLE) toExpand[0]=ONHITBYBULLETVARIABLE; // doesnt matter which it is.
	if (toExpand[0]==-1) {
		cout << "Error in ptc2GrowOne, we are expanding a default -1 value, so something went wrong.\n";
		exit(1);
	}
	if (allowExpand) depth = toUse->depth;
	else (depth = maxDepth+1);
	expandAt=0;
	expandLength=1;
	growing = true;
	codons[0] = randomint(4096);
	codonPos = 0;
	// we are going to use apply_one_grammar to expand this
	// apply_one_grammar, basically takes a non-terminal and replaces
	// it by the expansion determined by codons[0]
	// if growing is flagged (previously used for ramped half-and-half
	// apply_one_grammar attempts to ensure the expansion is only to an 
	// expansion which contains other non-terminals (i.e. it grows the tree).
	// the codon in codon[0] is modified to ensure that it would pick such a 
	// branch.
	
	apply_one_grammar(toExpand,&expandAt,&expandLength);
	
	if (codonPos != 0) { // i.e. we needed to use a codon for that expantion
		toUse->codonValue = codons[0]; // a codon to create that expansion has been provided for us
		toUse->codonUsed = true;
	} else {
		toUse->codonValue = 0;
		toUse->codonUsed = false;
	}
	
	depth = toUse->depth;
	parseTreePtr tomake;
	codonPos = 0;
	bool first = true;
	for (int i=0;i<expandLength;i++) {
		if (toExpand[i] == POPTREE || toExpand[i] == POPTREED) continue;
		
		// slight "hack" is about to occur:
		// the problem here is that in the body of the grammar
		// DEFUNS are automatically expanded to the PARAMS and FUNCBODY
		// keywords, so we have to do it here as we by pass that part
		// of the grammar (we dont scan the "parse-so-far"
		if (toExpand[i]==DEFUN) {
			i++;
			toExpand[i]=PARAMS; // overwrite the token *( in DEFUN *( )*
			toExpand[i+1]=FUNCBODY; // overwrite the token )* in DEFUN *( )*
		}
		tomake.reset(new parseTreeNode());
		if (first) { 
			toUse->expansion = tomake;
			first = false;
		}
		else toUse->next = tomake;
		tomake->value = toExpand[i];
		tomake->depth = depth+1;
		
		// I am using my knowledge that non-terminals have a value > 1000
		// should be careful here, using a hard coded number.
		
		if (tomake->value > 1000) 
		{ 
			currentNT.push_back(tomake);
		}
		toUse = tomake;
	}
}






bool robocodeTheta::traverseParseTree(parseTreePtr cNode) {
	// we have a parse tree, with each "node" in the parse tree
	// indicating if a codon was used to expand that node and if
	// so what the codon value was.
	// We just need to traverse the parse tree and store the codons in the 
	// codon array.
	
	// Again I am using hard-coded numbers > 1000 is a non-terminal.
	
	if (codonPos > maxCodons) {
		cout << "Traverse Parse Tree ran out of codons, failed to build individual.\n";
		return false;
	}
	if (cNode->expansion) {
		if (cNode->value < 1000) {
			cout << "Error in printOutParse, cNode appears to have an expansion, but a value less than 1000:- " << cNode->value << endl;
			exit(1);
		}
		if (cNode->codonUsed) {
			codons[codonPos++] = cNode->codonValue;
		}
#ifdef DEBUGPTC2
		expression[0] = cNode->value;
		*expressionLength = 1;
		cout << "Codon " << (codons[codonPos-1] & CODONMASK) << " used :";
		print_grammar_individual(cout,expression,*expressionLength);
		cout << endl;
#endif	
		// Dont need to test for CVAL here as it must be >= 1000 
		traverseParseTree(cNode->expansion);
	}
	else {
		if (cNode->value < 1000) {
			//codons[codonPos++] = cNode->codonValue; // because its not an expansionary node, there is no codon value
			if (cNode->value == CVAL)  codons[codonPos++] = randomint(4000);
			// *** for debugging/testing purposes the next section prints out the expression as we traverse the tree
#ifdef DEBUGPTC2
			expression[0] = cNode->value;
			*expressionLength = 1;
			if (cNode->value == CVAL) {
				expression[1] = codons[codonPos-1];
				*expressionLength = 2;
			}
			cout << "Codon " << (codons[codonPos-1] & CODONMASK) << " used :";
			print_grammar_individual(cout,expression,*expressionLength);
			cout << endl;
#endif	
			// **** end of printing code
		} else {
			cout << "Error in printOutParse, cNode appears to have no expansion, but a value greater than 1000:-" << cNode->value << endl;
			exit(1);
		}
	}
	if (cNode->next) return traverseParseTree(cNode->next);
	else return true;
}


void robocodeTheta::AllowFunctions(bool al) {
	allowFunctions = al;
	if (al) cout << "Functions are allowed.\n";
	else cout << "Functions are prohibited\n";
}

bool robocodeTheta::parse(int *exp,int *length,
						 int maxExp,long codon_list[],int max_codons,int &cUsed,vector<function> *funcVec)
{
	// This is realy the raison d'être of the gramar class
	// it takes a list of codons and returns the parsed expression
	// With the dynamic grammar it also returns a vector of any functions that come out of the parse 
	// (stored as a vector of type "function")
	// Finally additional information about the grammar is packed into each codon.
	
	// Each Codon is assumed to be 32 bits long (params.h contains most of the relevant defines).
	// The value of the codon used to decode the grammar (or is it the grammar which decodes the codon?)
	// is stored in the first 12 bits (defined as CODON - again check params.h the value might change).
	// The next 6 bits (defined as LHSINFO again in params.h) is used to store the non-terminal
	// decoded by that codon -1000 (all non-terminals are > 1000, so 1000 is subtracted from the code)
	// To find out what the non-terminal is add 1000 to these 6 bits and check the enums used by the 
	// system - typcially contained in the grammar header file.
	// Finally the last 14 bits contain the "expression length" that is the number of codons required
	// to fully expand the non-terminal (i.e. all the way to terminals) which was decoded by the system.
	// NOTE - that this methodology effectively limits the size of genomes the system can handle.
	// The largest EXPRLENGTH is 2^16 (which is about 65,000) - therefore we would need to re-jig all 
	// of this if you are playing around with "critters" which have a potential codon size in excess of
	// 65,536ish codons.
	// CODONMASK, LHSMASK and EXPRMASK are 12, 6 and 14 bit masks respectively, used to help extract 
	// (or insert) the relevant information.
	
	expression = exp;
	expressionLength = length;
	
	codons = codon_list;
	
	maxExpression = maxExp;
	maxCodons = max_codons;
	
	funcVector = funcVec;
	funcVector->clear();
	functionNos = 0;
	
	invalid = false;
	terminating = false;
	*length = 1;
	
	AllowDefun = allowFunctions;
	AllowParams = false;
	
	codonPos = 0; // start at the beginning.
	expressionPos = 0;
	expression[0]=1001; 
	
	latestVariableNo = topVariableNo = 0;
	stack.clear();
	doingPTC2 = false;
	balloonExpansion = 0;
	localVariableType = 0;		
	expandPTC2 = false;
	// dc->resetCounters();
	return doParse(cUsed);
}


void robocodeTheta::shrinkExpression() {
	for (int i= expressionPos+1;i< *expressionLength;i++)
		expression[i-1]=expression[i];
	*expressionLength = *expressionLength-1;
}

bool robocodeTheta::doParse(int &cUsed) {
	// Assumes that all rethe relevant variables such as expression, codons etc have been set
	// typcially by parse(...)
	
	
	// NB - as implemented codonPos will be greater than maxCodons if terminating is true - make sure
	// that no reference to codon[codonPos] is made without an initial check on the terminating flag.
	// or we could easily get a memory access fault.
	
	// To facilitate "stat" gathering, there is a "dc" to a dataCollector singleton class.
	// Where problems occur, the problem is logged to dc and can be read later if needed.
	
	// dc->resetCounters(); // typically we want to collect data over many parses, so dont reset the counters here.
	
	// Just some reminders as to the class data members we use
	// codonPos is the current reading position in the array of codons.
	// expression, is the array in which we are building up our parsed expression
	// expressionLength is the number of items we have (so far) in our parsed expression.
	// expressionPos, is the item we are currently examining - if its a non-terminal we will expand it
	// if its a terminal we skip past it.
	// When expressionPos equals expressionLength (and the item is a terminal)
	// that means we the "expression" array contains only terminals - and the parse is finished.
	// We need to deal with two "special cases" - one is if we define a function (DEFUN)
	// the other is the keyword "CVAL" which means store the next codon value for the interpreter.
	// The use/meaning of the codon is up to the interpreter - it can for instance be used to create
	// numeric constants.
	
	// If the value of the expression is > 1000, its a non-terminal, if < 1000 then its a terminal.
	// The translation from grammar name (like "VALUE") to expression code (like 1002) are contained in the enum, typcially
	// found in the header file of the grammar.
	
	// In order to be able to insert the expression length data into the codon array the grammar inserts a POPTREE
	// or POPTREED into the expression array after the expansion caused by that codon
	// (and pushes the position of the codon which caused that expansion into a LIFO stack).
	// So we also need to call "dealtWithPop" to check if the expression is a POPTREE. If it is it pops the codon position
	// from the stack and the difference between the position of the codonPos in the stack and the codoPos when the POPTREE is encountered
	// is the number of codons required to fully expand that non-terminal.
	
	// balloonExpansion is to deal with PTC2 initialisation and is more fully described in those functions.
	// if you are looking through this for the first time, just ignore it. (i.e. assume its false).
	
	do {
		if (codonPos >= maxCodons ) {
			terminating = true; 
			dc->invalidAsNoCodons++;
			invalid = true;
			invalidReason = "We ran out of codons before we could finish the expression.";
		}
		if (!invalid) {  
			if (expression[expressionPos] == DEFUN) { 
				// So we are going to define a new dynamic funciton
				// This is passed off to read_func (which is broadly identical to here, except the function
				// is stored in the expression data of a "function" class, which is then inserted into the
				// function vetor.
				
				if (!terminating) read_func(); // if we are terminating don't define new functions
				if (*expressionLength > expressionPos) {
					for (int i = expressionPos ;i< *expressionLength-3;i++) {
						expression[i]=expression[i+3];
					}
					*expressionLength = *expressionLength - 3;
				} else {
					invalid = true;
					invalidReason = "Function Defined as last token in an expression\n";
					cout << "Error with function being the last token in an expression. Check the grammar definition.\n";
				}
			} else {
				
				{
					if (expression[expressionPos] == INMAINLOOP) {
//						cout << "Hit INMAINLOOP\n";
						localVariableType=1;
						shrinkExpression();
					}
					if (expression[expressionPos] == INHITBYBULLETLOOP) {
//						cout << "Hit INHITBYBULLETLOOP\n";
						localVariableType=2;
						shrinkExpression();
					}
					if (expression[expressionPos] == INSCANNEDLOOP) {
//						cout << "Hit INSCANNEDLOOP\n";
						localVariableType=3;
						shrinkExpression();
					}
					if (expression[expressionPos] == INHITROBOTLOOP) {
//						cout << "Hit INHITROBOTLOOP\n";
						localVariableType=4;
						shrinkExpression();
					}
					if (expression[expressionPos] == INHITWALLLOOP) {
//						cout << "Hit INHITWALLLOOP\n";
						localVariableType=5;
						shrinkExpression();
					}
					if (expression[expressionPos] == NEWINSCOPE) {
						topVariableNo++;
						shrinkExpression();
					}
					if (expression[expressionPos] == OUTOFSCOPE) {
						// do it by retracting topVariable (rather than incrementing lastVariable) 
						// as otherwise, nested for loops are not handled properly.
						// LIFO stack equivalent this way.
						topVariableNo--;
						shrinkExpression();
					}
				}
				
				if (expression[expressionPos] < 1000) { // I SEEM TO HAVE HARD CODED < 1000 MEANS TERMINAL.
					if (!dealtWithPop(expression,&expressionPos,expressionLength)) {
						if (expression[expressionPos]== CVAL) {
							if (balloonExpansion) {
								if (lastCodonPos != codonPos) {
									// shift the codons one to the right unless we haven't already used the "inserted" codon
									for(int i=maxCodons-1;i>=codonPos;i--)
										codons[i+1]=codons[i];
									codons[codonPos]=randomint(4096); // NOTE HERE I AM IMPLICITLY ASSUMING 12 BIT CODONS
								}
							}
							
							// Make room to put in the codon value
							for (int i=*expressionLength-1; i > expressionPos;i--)
								expression[i+1]=expression[i];
							*expressionLength = *expressionLength + 1;
							expressionPos++;
							expression[expressionPos]= codons[codonPos] & FCODONMASK;
							codons[codonPos] =  ( (codons[codonPos] & FCODONMASK) | ((0) << CODON)); // so we store it as 1000 (or 0) 
							codonPos = codonPos + 1;
						}
						expressionPos++;  // skip over any terminals
					}
					
					
					
				}
				else {
					if (balloonExpansion) {
						if (lastCodonPos != codonPos) {
							// shift the codons one to the right
							for(int i=maxCodons-1;i>=codonPos;i--)
								codons[i+1]=codons[i];
							codons[codonPos]=randomint(4096);
						} 
						lastCodonPos = codonPos;
					}
					if (expression[expressionPos] == CONTEXTVARIABLE) {
						switch (localVariableType) {
							case 1:	expression[expressionPos] = MAINLOOPVARIABLE;
								break;
							case 2:	expression[expressionPos] = ONHITBYBULLETVARIABLE;
								break;
							case 3:	expression[expressionPos] = ONSCANNEDROBOTVARIABLE;
								break;
							case 4:	expression[expressionPos] = ONHITROBOTVARIABLE;
								break;
							case 5:	expression[expressionPos] = ONHITWALLVARIABLE;
								break;
							default:	cout << "LocalVariable type was " << localVariableType << " when we hit a CONTEXTVARIABLE\n";
								exit(1);
								break;
						}
					}
					apply_one_grammar(expression,&expressionPos,expressionLength);
				}
			}
		} 
	} while (!invalid && (expressionPos<*expressionLength)); 
	// when expressionPos == expressionLength - it means we have fully expanded the expression.
	expression[*expressionLength]=0;  // The interpreter now expects the expression to be null terminated.
	if (!terminating ) cUsed=codonPos;
	else cUsed = maxCodons;
	return !invalid;
}

bool robocodeTheta::dealtWithPop(int expr[],int *curr,int *length) {
	// This is the helper function for extracting grammar information 
	// from the parse and storing it with the codons.
	
	// BEPOPTREE is only to do with PTC2 and can be ignored on a first
	// read through.
	
	// POPTREE and POPTREED are essentially identical, but one contains
	// information about the depth of a tree (i.e. changed the depth up)
	// and is used in the ramped half and half initialisation.
	// On a first read through, just assume they are the same thing.
	
	// Whenever a poptree (or POPTREED) is encountered it means that 
	// an "expansion" of a non-terminal has been completed.
	// The position of the codon that started that expansion will be at the stop of "stack"
	// The number of codons used to fully expand that expansion is the differnece in the 
	// codonPos now and then.
	// This information is then inserted into the most significant 14 bits of the codon.
	
	if (expr[*curr]==BEPOPTREE) {
		if (!balloonExpansion) {
			cout << "Hit a BEPOPTREE, but balloonExpansion was " << balloonExpansion << endl;
			exit(1);
		}
		balloonExpansion--;
		for (int i=*curr+1;i<*length;i++)
			expr[i-1]=expr[i];
		*length = *length -1;
	    if (balloonExpansion == 0) {
			growing = false;
			if (lastCodonPos == codonPos) { 
				// So every time a codon was used and we were "balloon expanding"
				// doParse would insert a new codon, but we are turning off balloon expansion
				// so we want to get rid of the "new codon" that was inserted.
				// i.e. if we hit here with expansion zero we have an extra codon
				for (int i=codonPos;i<maxCodons-1;i++)
					codons[i]=codons[i+1]; // shuffle down one
			}
		}
	    return true;
	}
	if (!(expr[*curr]==POPTREE || expr[*curr]== POPTREED)) {
		return false;
	}
	if (growing) if (expr[*curr]==POPTREED) depth--;
    if (!ExtractingGrammarInfo) { // Get rid of the pop no further action.
		for (int i=*curr+1;i<*length;i++)
			expr[i-1]=expr[i];
		*length = *length -1;
		return true; // no need to pop stack as all pushes are conditional
		// on ExtractingGrammarInfo being true.
	}
	if (stack.empty()) {
		cout << "Error in the grammar, popping an empty stack.\n";
		invalid = true;
		return false;
	}
	for (int i=*curr+1;i<*length;i++)
		expr[i-1]=expr[i];
	*length = *length -1;
	int value = stack.back();
	stack.pop_back();
	// Okay this is putting in the "Jump" as the length - i.e. relative values
    // So the value is where the codon we are altering is - i.e. the expression which has just finished expanding
    // and codonPos = the place where the next codon (after the expansion) will be.
    // So we want codonPos-value, make it a long to make sure we have enough bits
    // EXPRMask it so that we are only looking at the correct bits (in case its too big - should maybe check this.
    // SHIFT it right by (CODON+LHSINFO)
    // and then "or" it with the value in the codon we are altering.
    // Kind of assumes that the codon we are altering doesnt already have length info in it (i.e. is zero)
    // Ill just note it here rather than clearing it @@
	// Actually although I have not had problems, I should probably just clear it first, an abundance of caution.
	codons[value] = codons[value] & CLEAREXPRMASK; 
	codons[value] = codons[value] | ((((long) (codonPos-value)) & EXPRMASK) << (CODON + LHSINFO));
	
	return true;
}

void robocodeTheta::changeCodonsForGrow(int divisors) {
	static int maxCodonSize = (int) pow(2.0,CODON);
	int newCodon;
	if (!growing) return;
	
	// This function was originally used for ramped-half and half initialisation
	// but is also used by PTC2
	
	// If growing is true, it does the following:
	// If depth is greater than MaxDepth, it chooses an expansion from the terminals vector.
	// If maxgrow is true and depth is not greater than maxdepth it chooses an expansion from the nontTerminal vector
	// otherwise it chooses one randomly from the union of the two vectors.
	// The codon at codonPos is altered (if required) to reflect the expansion chosen.
	
	// The passed paramater (divisors) is the number of choices the codon has
	// as to which expansion to choose.
	
	
	// Just because its simpler we might need to do some culling here
	// The problem is that in certain circumstance, divisor might have been
	// decreased (for instance params are not allowed), but (in this example)
	// the param choice will still have been pushed into either the nonTerminals
	// or the Terminals - now I could have fiddled with Gramma.y to ensure that this
	// would not happen, but its proving tricky to catch every possible case - the 
	// simpler and safer (but more time consuming) method is to cull here.
	
	vector<int>::iterator at = nonTerminals.begin();
	while (at != nonTerminals.end()) {
		if (*at >= divisors) {
			at = nonTerminals.erase(at);
		} else at++;
	}
	at = terminals.begin();
	while (at != terminals.end()) {
		if (*at >= divisors) {
			at = terminals.erase(at);
		} else at++;
	}
	
	unsigned int nonT = nonTerminals.size();
	unsigned int term = terminals.size();
	
	if (divisors == 0) {
		cout << "changeCodons for growth called when there were no choices to grow to!\n";
		exit(1);
	}
	if (nonT + term == 0) {
		//cout << "In change Codons for grow, and both the nonTerminals and terminals are empty.\n";
		//exit(1);
		// Just now this can happen, not sure why, but easier just to let it through
		// doesnt cause any problems (I think)
		// The reason it happens is that they way the system deals with default calls where
		// a funciton is to be called but none of them have been defined or where a paramater
		// is to be used but there are none. Letting it jsut return is the correct behaviour in such
		// circumstances.
		
		return;
	}
	
	if (depth>maxDepth) { // terminate if we can.
		if (term > 0) { // so we have termination choices
			int choice = terminals[randomint(term)];
			int base = (maxCodonSize / divisors );
			int start = randomint(base) * divisors;
			// so start is a a number between 1 to maxCodonSize such that start%divisors == 0
			newCodon = start+choice;
			codons[codonPos] = newCodon; // I dont think I need to retain any other info as we will fill it in.
		} else { 
			// we want to terminate but we dont have termination choices, 
			// so we will just have to choose a non-termination choice and 
			// hope to terminate later.
			if (nonT > 0) {
				int choice = nonTerminals[randomint(nonT)];
				int base = (maxCodonSize / divisors );
				int start = randomint(base) * divisors;
				// so start is a a number between 1 to maxCodonSize such that start%divisors == 0
				newCodon = start+choice;
				codons[codonPos]= newCodon;
			} else {
				// no termination or non-termination choices!
				cout << "In change Codons for Grow (and trying not to grow) and somehow we have no non-termination or termination choices.\n";
				// just a bit of a safety catch should have been caught above
				exit(1);
			}
		}
	} else if (maxGrow || depth<minDepth) {  // grow if we can
		// we have to grow until we reach min depth
		// or if max grow until we reach maxDepth (above)
		if (nonT>0) {
			int choice = nonTerminals[randomint(nonT)];
			int base = (maxCodonSize / divisors );
			int start = randomint(base) * divisors;
			// so start is a a number between 1 to maxCodonSize such that start%divisors == 0
			newCodon = start+choice;
			codons[codonPos]= newCodon;
		} else {
			if (term > 0) {
				int choice = terminals[randomint(term)];
				int base = (maxCodonSize / divisors );
				int start = randomint(base) * divisors;
				// so start is a a number between 1 to maxCodonSize such that start%divisors == 0
				newCodon = start+choice;
				codons[codonPos] = newCodon; // I dont think I need to retain any other info as we will fill it in.
			} else { 
				cout << "In change Codons for Grow (and trying to grow) but somehow we have non non-termination or termination choices.\n";
			}
		}
	} 
	// else we don't care (if we grow or terminate) so let it fall as it falls
}


void robocodeTheta::read_func() {
	// this function is almost embarassingly similar to doParse (embarassing as I suspect
	// there is a far neater way to program it)
	// It does the same job, except the expression is read into the appropriate data class of 
	// the most recent function class, which makes the access of the pointers to these arrays
	// more ...err... interesting.
	
	// We also need to expand first the keyword PARAMS, to determine how many parameters this 
	// funciton has.
	
	int funcPos=0;
	funcVector->push_back(function());
	(*funcVector)[functionNos].expression[0]=PARAMS;  
	AllowDefun = false; // we don't allow function definitions when working out how many params
	AllowParams = false; // whilst we working out how many paramaters.
	(*funcVector)[functionNos].expLength = 1;
	do {
		if (codonPos >= maxCodons) {
			terminating = true; 
			// start applying defaults to gracefully terminate the expression.
			dc->invalidAsNoCodons++;
			invalid = true;
			invalidReason = "We ran out of codons before we could finish the expression.";
			
		}
		if (!invalid) {
			if ((*funcVector)[functionNos].expression[funcPos] < 1000) {
				if (!dealtWithPop((*funcVector)[functionNos].expression,&funcPos,(&(*funcVector)[functionNos].expLength)))
					funcPos++;  // skip over any terminals
			} else {  
				if (balloonExpansion) {
					if (lastCodonPos != codonPos) {
						// shift the codons one to the right
						for(int i=maxCodons-1;i>=codonPos;i--)
							codons[i+1]=codons[i];
						codons[codonPos]=randomint(4096);
					} 
					//											lastCodonPos = codonPos;
				}
				apply_one_grammar((*funcVector)[functionNos].expression,&funcPos,(&(*funcVector)[functionNos].expLength));
			}
		} 
	} while (!invalid && (funcPos<(*funcVector)[functionNos].expLength)); //okay we have fully expanded param
	
	if (invalid) { // if the whole thing is invalid get rid of the partly parsed function and return.
		funcVector->pop_back();
		return;
	}
	
	// This is slightly awkward - I wanted to be able to define zero paramater functions
	// But couldn't in the five minutes I was scratching my head trying to work out how to write
	// in a BNF grammar "NO CHOICE", so the plan is NOP = no paramaters,
	// otherwise for each PARAM that the PARAMS expands to, you get a paramater
	// So typically the grammar might look like this:
	// PARAMS: NOP | someParams
	// someParams: PARAM | someParams
	// Note one side effect of this is that there is a 50% chance of no paramaters
	// A 25% chance of one, 12.5% of two, 6.25% of three etc.
	// This could, of course be influenced by chaning the grammar
	// e.g. PARAMS: NOP | PARAM | PARAM PARAM | PARAM PARAM PARAM
	// is now 25% chance of each of the options.
	
	
	// noOfParams = (funcPos+1)/2;
	// (*funcVector)[functionNos].params = noOfParams; // How it used to be, before NOP
	
	noOfParams = 0;
	if ((*funcVector)[functionNos].expression[0] == NOP) {
		noOfParams = 0;
	} else {
		for (int i=0;i<(*funcVector)[functionNos].expLength;i++)
			if ((*funcVector)[functionNos].expression[i] == PARAM) noOfParams++;
	} 
	
	if (noOfParams < 0 || noOfParams > 20) { // > 20 is just an arbitrary "oh you are just getting silly and I will make you invalid" limit.
		cout << "Error params not a sensible number, it is: " << noOfParams << ", making it invalid.\n";
		invalid = true;
		funcVector->pop_back();
		return;
	}
	(*funcVector)[functionNos].params = noOfParams;
	//          cout << "In function definition, this function has " << noOfParams << " parameters.\n";
	
	// This is identical to the functionality of doParse except
	// expressionLength is (*funcVector)[functionNos].expLength
	// expression is (*funcVector)[functionNos].expression
	// expressionPos is funPos
	// Oh and we don't allow the grammar to define functions inside functions.
	
	funcPos=0;
	(*funcVector)[functionNos].expression[0]=FUNCBODY; 
	AllowParams = true;
	(*funcVector)[functionNos].expLength = 1;
	do {
		if (codonPos >= maxCodons) {
			terminating = true; 
			invalid = true;
			dc->invalidAsNoCodons++;
			invalidReason = "We ran out of codons before we could finish the expression.";
		}
		if (!invalid) {
			if ((*funcVector)[functionNos].expression[funcPos] < 1000) {
				if (!dealtWithPop((*funcVector)[functionNos].expression,&funcPos,(&(*funcVector)[functionNos].expLength))) {
					if ((*funcVector)[functionNos].expression[funcPos] == CVAL) {
						if (balloonExpansion) {
							// shift the codons one to the right
							if (codonPos != lastCodonPos) {
								// shift the codons one to the right
								for(int i=maxCodons-1;i>=codonPos;i--)
									codons[i+1]=codons[i];
								codons[codonPos]=randomint(4096);
							}
						}	
						// Make room to put in the codon value
						for(int i=(*funcVector)[functionNos].expLength-1;i>funcPos;i--)
							(*funcVector)[functionNos].expression[i+1]=(*funcVector)[functionNos].expression[i];
						(*funcVector)[functionNos].expLength++;
						funcPos++;
						(*funcVector)[functionNos].expression[funcPos]= codons[codonPos] & FCODONMASK;
						codons[codonPos] =  ( (codons[codonPos] & FCODONMASK) | ((0) << CODON)); // so we store it as 1000 (or 0) 
						codonPos = codonPos + 1;
					}
					funcPos++;  // skip over any terminals
				}
			} else {
				if (balloonExpansion) {
					if (lastCodonPos != codonPos) {
						// shift the codons one to the right
						for(int i=maxCodons-1;i>=codonPos;i--)
							codons[i+1]=codons[i];
						codons[codonPos]=randomint(4096);
					}
					lastCodonPos = codonPos;
				}
				apply_one_grammar((*funcVector)[functionNos].expression,&funcPos,&((*funcVector)[functionNos].expLength));
			}
		} 
	} while (!invalid && (funcPos<(*funcVector)[functionNos].expLength)); //okay we have fully expanded param
	if (invalid) {
		funcVector->pop_back();
		return;
	}
	functionNos = functionNos+1; // we have a valid function
	(*funcVector)[functionNos-1].expression[(*funcVector)[functionNos-1].expLength]=0; // null terminate it
	//          cout << "Function " << functionNos << " done, parses as ..\n";
	//          fprint_grammar_individual(cout,(*funcVector)[functionNos-1].expression,(*funcVector)[functionNos-1].expLength);
	AllowParams = false;
	if (functionNos < MAXFUNCS) AllowDefun = allowFunctions;
	else AllowDefun = false;
	return;
}

void robocodeTheta::apply_one_grammar(int curr_expr[],int *curr,int *length)
{
	
	// This is the part of the grammar class that is unique to each grammar
	// It basically takes the non-terminal currently pointed to 
	// and uses the codon at codonPos to expand it in accordance with the grammar.
	// There is a giant switch statement which uses the non-terminal as the basis for
	// the switch. Each different "non-terminal" being a case of that switch statement.
	// The case then uses the codon value to determine which expansion
	// of that non-terminal to choose.
	
	// The whole thing uses the enums put in the header files so they appear in "English" 
	// to the reader of the file, and is robust if the enums are changed provided certain
	// rules are followed (basically non-terminals must be > 1000, terminals < 400
	// paramaters take the values 400-499, funciton calls 500-599 and 600-1000
	// is reserved for various things. This limits the number of distinct non-terminals in a
	// grammar to 399, but just now that is not an issue.
	
	// Of course its all made a lot simpler by the fact that the "giant switch statement" 
	// is created automatically by the Gramma program.
	
	// It is also responsible for calling changeCodonsForGrow(...) if we are 
	// initialising the individual and wish to use ramped half and half or 
	// ptc2 and it stores in the codon the information as to which 
	// non-terminal that codon expanded and it pushes onto the end
	// of each expansion the "POPTREE" value, so that we can store the 
	// expression length information (see dealtWithPop function for more info)
	
	int divisor;
	if (*length > maxExpression - TOOBIGERRORMARGIN) { // ditto with the -30
		invalid = true;
		dc->invalidAsExpressionLength++;
		//cout << "Expession too long, length " << *length << "\n";
		invalidReason = "The expression was too long";
		return;
	}
	
	int currentCodon = (terminating?0:codons[codonPos]) & CODONMASK; // only interested in the first CODON bits
	
	
	int expr_t = curr_expr[*curr];
	switch(curr_expr[*curr])
	{
		case(CODE):{ /* code */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+24]=curr_expr[i];
					*length = *length + 24;
					curr_expr[*curr+0]=IMPORT; //import
					curr_expr[*curr+1]=ROBOCODEDOTSTAR; //robocodeDOTSTAR
					curr_expr[*curr+2]=SEMICOLON; //;
					curr_expr[*curr+3]=N; //N
					curr_expr[*curr+4]=PUBLIC; //public
					curr_expr[*curr+5]=CLASS; //class
					curr_expr[*curr+6]=NAME; //NAME
					curr_expr[*curr+7]=EXTENDS; //extends
					curr_expr[*curr+8]=ADVANCEDROBOT; //AdvancedRobot
					curr_expr[*curr+9]=OCB; //{
					curr_expr[*curr+10]=N; //N
					curr_expr[*curr+11]=DOVARIABLEDEFS; //static
					curr_expr[*curr+12]=N; //N
					curr_expr[*curr+13]=SOMEFUNCTIONS; //SOMEFUNCTIONS
					curr_expr[*curr+14]=SOMECOMMENTS; //N
					curr_expr[*curr+15]=MAINCODE; //MAINCODE
					curr_expr[*curr+16]=N; //N
					curr_expr[*curr+17]=SCANROBOTCODE; //SCANROBOTCODE
					curr_expr[*curr+18]=N; //N
					curr_expr[*curr+19]=HITBYBULLETCODE; //HITBYBULLETCODE
					curr_expr[*curr+20]=N; //N
					curr_expr[*curr+21]=HITROBOTCODE; //HITROBOTCODE
					curr_expr[*curr+22]=N; //N
					curr_expr[*curr+23]=HITWALLCODE; //HITWALLCODE
					curr_expr[*curr+24]=CCB; //}
					break;
				}
			}//of switch
			break;
		}//of case code
		case(MAINCODE):{ /* MAINCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+20]=curr_expr[i];
					*length = *length + 20;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=VOID; //void
					curr_expr[*curr+2]=RUN__; //run__
					curr_expr[*curr+3]=OCB; //{
					curr_expr[*curr+4]=N; //N
					curr_expr[*curr+5]=INMAINLOOP; //INMAINLOOP
					curr_expr[*curr+6]=N; //INMAINLOOP
					curr_expr[*curr+7]=SENTENCES; //Sentences
					curr_expr[*curr+8]=N; //N
					curr_expr[*curr+9]=WHILE; //while
					curr_expr[*curr+10]=ORB; //(
					curr_expr[*curr+11]=TRUE_; //true_
					curr_expr[*curr+12]=CRB; //)
					curr_expr[*curr+13]=OCB; //{
					curr_expr[*curr+14]=SENTENCES; //Sentences
					curr_expr[*curr+15]=EXECUTE; //Sentences
					curr_expr[*curr+16]=SEMICOLON; //Sentences
					curr_expr[*curr+17]=N; //Sentences
					curr_expr[*curr+18]=CCB; //}
					curr_expr[*curr+19]=N; //N
					curr_expr[*curr+20]=CCB; //}
					break;
				}
			}//of switch
			break;
		}//of case MAINCODE
		case(SCANROBOTCODE):{ /* SCANROBOTCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+11]=curr_expr[i];
					*length = *length + 11;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=VOID; //void
					curr_expr[*curr+2]=ONSCANNEDROBOT_SCANNEDROBOTEVENT; //onScannedRobot_ScannedRobotEvent
					curr_expr[*curr+3]=E_CB; //e_CB
					curr_expr[*curr+4]=OCB; //{
					curr_expr[*curr+5]=N; //N
					curr_expr[*curr+6]=INSCANNEDLOOP; //INSCANNEDLOOP
					curr_expr[*curr+7]=ONSTART; //ONSTART
					curr_expr[*curr+8]=SENTENCES; //Sentences
					curr_expr[*curr+9]=ONEXIT; //ONEXIT
					curr_expr[*curr+10]=CCB; //}
					curr_expr[*curr+11]=N; //N
					break;
				}
			}//of switch
			break;
		}//of case SCANROBOTCODE
		case(HITBYBULLETCODE):{ /* HITBYBULLETCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+11]=curr_expr[i];
					*length = *length + 11;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=VOID; //void
					curr_expr[*curr+2]=ONHITBYBULLET_HITBYBULLETEVENT; //onHitByBullet_HitByBulletEvent
					curr_expr[*curr+3]=E_CB; //e_CB
					curr_expr[*curr+4]=OCB; //{
					curr_expr[*curr+5]=N; //N
					curr_expr[*curr+6]=INHITBYBULLETLOOP; //INHITBYBULLETLOOP
					curr_expr[*curr+7]=ONSTART; //ONSTART
					curr_expr[*curr+8]=SENTENCES; //Sentences
					curr_expr[*curr+9]=ONEXIT; //ONEXIT
					curr_expr[*curr+10]=CCB; //}
					curr_expr[*curr+11]=N; //N
					break;
				}
			}//of switch
			break;
		}//of case HITBYBULLETCODE
		case(HITROBOTCODE):{ /* HITROBOTCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+11]=curr_expr[i];
					*length = *length + 11;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=VOID; //void
					curr_expr[*curr+2]=ONHITROBOT_HITROBOTEVENT; //onHitRobot_HitRobotEvent
					curr_expr[*curr+3]=E_CB; //e_CB
					curr_expr[*curr+4]=OCB; //{
					curr_expr[*curr+5]=N; //N
					curr_expr[*curr+6]=INHITROBOTLOOP; //INHITROBOTLOOP
					curr_expr[*curr+7]=ONSTART; //ONSTART
					curr_expr[*curr+8]=SENTENCES; //Sentences
					curr_expr[*curr+9]=ONEXIT; //ONEXIT
					curr_expr[*curr+10]=CCB; //}
					curr_expr[*curr+11]=N; //N
					break;
				}
			}//of switch
			break;
		}//of case HITROBOTCODE
		case(HITWALLCODE):{ /* HITWALLCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+11]=curr_expr[i];
					*length = *length + 11;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=VOID; //void
					curr_expr[*curr+2]=ONHITWALL_HITWALLEVENT; //onHitWall_HitWallEvent
					curr_expr[*curr+3]=E_CB; //e_CB
					curr_expr[*curr+4]=OCB; //{
					curr_expr[*curr+5]=N; //N
					curr_expr[*curr+6]=INHITWALLLOOP; //INHITWALLLOOP
					curr_expr[*curr+7]=ONSTART; //ONSTART
					curr_expr[*curr+8]=SENTENCES; //Sentences
					curr_expr[*curr+9]=ONEXIT; //ONEXIT
					curr_expr[*curr+10]=CCB; //}
					curr_expr[*curr+11]=N; //N
					break;
				}
			}//of switch
			break;
		}//of case HITWALLCODE
		case(SENTENCES):{ /* Sentences */
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=SENTENCE; //Sentence
					curr_expr[*curr+1]=SENTENCES; //Sentences
					curr_expr[*curr+2]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SENTENCE; //Sentence
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case Sentences
		case(SENTENCE):{ /* Sentence */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				terminals.push_back(2);
				nonTerminals.push_back(1);
				nonTerminals.push_back(2);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+11]=curr_expr[i];
					*length = *length + 11;
					curr_expr[*curr+0]=IF; //if
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=BOOL; //Bool
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=OCB; //{
					curr_expr[*curr+5]=SENTENCES; //Sentences
					curr_expr[*curr+6]=CCB; //}
					curr_expr[*curr+7]=ELSE; //else
					curr_expr[*curr+8]=OCB; //{
					curr_expr[*curr+9]=SENTENCES; //Sentences
					curr_expr[*curr+10]=CCB; //}
					curr_expr[*curr+11]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+12]=curr_expr[i];
					*length = *length + 12;
					curr_expr[*curr+0]=FOR; //
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=DOUBLE; //
					curr_expr[*curr+3]= 300+topVariableNo; // so var0..var whatever.
					curr_expr[*curr+4]=IN; //{
					curr_expr[*curr+5]=STACKTYPE; //Sentences
					curr_expr[*curr+6]=CRB; //{
					curr_expr[*curr+7]=NEWINSCOPE; //else
					curr_expr[*curr+8]=OCB; //{
					curr_expr[*curr+9]=SENTENCES; //Sentences
					curr_expr[*curr+10]=CCB; //}
					curr_expr[*curr+11]=OUTOFSCOPE;
					curr_expr[*curr+12]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=ACTION; //Action
					curr_expr[*curr+1]=SEMICOLON; //;
					curr_expr[*curr+2]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case Sentence
		case(ACTION):{ /* Action */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				terminals.push_back(2);
				nonTerminals.push_back(2);
				terminals.push_back(3);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=TURNACTION; //turnAction
					curr_expr[*curr+1]=POPTREED;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=MOVEACTION; //moveAction
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SETACTION; //setAction
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=FIREACTION; //fireAction
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case Action
		case(TURNACTION):{ /* turnAction */
			divisor = 6;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETTURNRIGHT_OB; //setTurnRight_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETTURNLEFT_OB; //setTurnLeft_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETTURNGUNRIGHT_OB; //setTurnGunRight_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETTURNGUNLEFT_OB; //setTurnGunLeft_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETTURNRADARRIGHT_OB; //setTurnRadarRight_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETTURNRADARLEFT_OB; //setTurnRadarLeft_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case turnAction
		case(MOVEACTION):{ /* moveAction */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				terminals.push_back(2);
				terminals.push_back(3);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETAHEAD_OB; //setAhead_OB
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETBACK_OB; //setBack_OB
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=RESUME__; //resume__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=STOP__; //stop__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case moveAction
		case(SETACTION):{ /* setAction */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				terminals.push_back(2);
				nonTerminals.push_back(2);
				terminals.push_back(3);
				nonTerminals.push_back(3);
				//terminals.push_back(4);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETADJUSTGUNFORROBOTTURN_OB; //setAdjustGunForRobotTurn_OB
					curr_expr[*curr+1]=BOOL; //Bool
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETADJUSTRADARFORROBOTTURN_OB; //setAdjustRadarForRobotTurn_OB
					curr_expr[*curr+1]=BOOL; //Bool
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SETADJUSTRADARFORGUNTURN_OB; //setAdjustRadarForGunTurn_OB
					curr_expr[*curr+1]=BOOL; //Bool
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=SAFEPUSH_OB; //safePush_OB
					curr_expr[*curr+1]=STACKTYPE; //stackType
					curr_expr[*curr+2]=COMMA; //,
					curr_expr[*curr+3]=VARIABLES; //Variables
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(4):{ // not yet implemented.
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SAFESWAP_OB; //safeSwap_OB
					curr_expr[*curr+1]=STACKTYPE; //stackType
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case setAction
		case(FIREACTION):{ /* fireAction */
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=FIRE_OB; //fire_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=DIV; //%
					curr_expr[*curr+3]=FIVE; //5
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=SETFIRE_OB; //setfire_OB
					curr_expr[*curr+1]=VARIABLES; //Variable
					curr_expr[*curr+2]=DIV; //%
					curr_expr[*curr+3]=FIVE; //5
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case fireAction
		case(VARIABLES):{ /* Variables */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				//terminals.push_back(0);
				nonTerminals.push_back(0);
				nonTerminals.push_back(1);
				terminals.push_back(2);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=TWOVARIABLES; //twoVariables
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case (1): {
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=MODIFIEDVARIABLE; //twoVariables
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=VARIABLE; //Variable
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case Variables
		case(VARIABLE):{ /* Variable */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GLOBALVARIABLE; //globalVariable
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=CONTEXTVARIABLE; //contextVariable
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case (2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=LOOPVARIABLE; //contextVariable
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case Variable
		case(CONTEXTVARIABLE):{ /* contextVariable */
			// note that context variable actually is "Changed" in the parse loop (so we really shouldn't be here)
			// it is changed to the correct type of local variable, depending on where
			// we are in the parse.
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					curr_expr[*curr+0]=GLOBALVARIABLE; //globalVariable
					break;
				}
			}//of switch
			break;
		}//of case contextVariable
		case(TWOVARIABLES):{ /* twoVariables */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				nonTerminals.push_back(1);
				nonTerminals.push_back(2);
				nonTerminals.push_back(3);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=ORB; //(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=PLUS; //+
					curr_expr[*curr+3]=VARIABLES; //Variables
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=ORB; //(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=MINUS; //-
					curr_expr[*curr+3]=VARIABLES; //Variables
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=ORB; //(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=MULT; //*
					curr_expr[*curr+3]=VARIABLES; //Variables
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=PDIV_OB; //PDIV_OB
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=COMMA; //,
					curr_expr[*curr+3]=VARIABLES; //Variables
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case twoVariables
		case(BOOL):{ /* Bool */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				nonTerminals.push_back(2);
				nonTerminals.push_back(3);
				terminals.push_back(3);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=ORB; //(
					curr_expr[*curr+1]=BOOL; //Bool
					curr_expr[*curr+2]=BOOLOP; //boolop
					curr_expr[*curr+3]=BOOL; //Bool
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=ORB; //(
					curr_expr[*curr+1]=VARIABLE; //Variable
					curr_expr[*curr+2]=COMP; //comp
					curr_expr[*curr+3]=VARIABLE; //Variable
					curr_expr[*curr+4]=CRB; //)
					curr_expr[*curr+5]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=NOT_OB; //NOT_OB
					curr_expr[*curr+1]=BOOL; //Bool
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					 for(int i=*length-1;i>*curr;i--)
					 curr_expr[i+5]=curr_expr[i];
					 *length = *length + 5;
					 curr_expr[*curr+0]=ISNEAR;//Maths.XXX(
					 curr_expr[*curr+1]=VARIABLES; //Variables
					 curr_expr[*curr+2]=COMMA;
					 curr_expr[*curr+3]=VARIABLES;
					 curr_expr[*curr+4]=CRB; //+
					 curr_expr[*curr+5]=POPTREED;
					 if (growing) depth++;
					 if (ExtractingGrammarInfo) stack.push_back(codonPos);
					 break;
				}	
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case Bool
		case(COMP):{ /* comp */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GREATER; //>
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=LESS; //<
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=EEQUAL; //==
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case comp
		case(BOOLOP):{ /* boolop */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=AND; //AND
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=XOR; //XOR
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=OR; //OR
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case boolop
		case (MODIFIEDVARIABLE): {
			
			divisor = 15;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+12]=curr_expr[i];
					*length = *length + 12;
					curr_expr[*curr+0]=ACOS; //Maths.Acos(
					curr_expr[*curr+1]=MAX; //Variables
					curr_expr[*curr+2]=MINUS; //Variables
					curr_expr[*curr+3]=ONE; //Variables
					curr_expr[*curr+4]=COMMA; //Variables
					curr_expr[*curr+5]=MIN; //Variables
					curr_expr[*curr+6]=ONE; //Variables
					curr_expr[*curr+7]=COMMA; //Variables
					
					curr_expr[*curr+8]=VARIABLES; //Variables
					curr_expr[*curr+9]=CRB; // MIN
					curr_expr[*curr+10]=CRB; // MAX
					curr_expr[*curr+11]=CRB; // ACOS
					curr_expr[*curr+12]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=ABS; //Maths.abs(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+12]=curr_expr[i];
					*length = *length + 12;
					curr_expr[*curr+0]=ASIN; //Maths.asin(
					curr_expr[*curr+1]=MAX; //Variables
					curr_expr[*curr+2]=MINUS; //Variables
					curr_expr[*curr+3]=ONE; //Variables
					curr_expr[*curr+4]=COMMA; //Variables
					curr_expr[*curr+5]=MIN; //Variables
					curr_expr[*curr+6]=ONE; //Variables
					curr_expr[*curr+7]=COMMA; //Variables
					
					curr_expr[*curr+8]=VARIABLES; //Variables
					curr_expr[*curr+9]=CRB; // MIN
					curr_expr[*curr+10]=CRB; // MAX
					curr_expr[*curr+11]=CRB; // ACOS
					curr_expr[*curr+12]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=ATAN; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}	
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=ATAN2; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=COMMA;
					curr_expr[*curr+3]=VARIABLES;
					curr_expr[*curr+4]=CRB; //+
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=COS; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(6):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SIN; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(7):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=MAX;//Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=COMMA;
					curr_expr[*curr+3]=VARIABLES;
					curr_expr[*curr+4]=CRB; //+
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(8):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=MIN;//Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=COMMA;
					curr_expr[*curr+3]=VARIABLES;
					curr_expr[*curr+4]=CRB; //+
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}	
				case(9):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=ROUND; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(10):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SQRT; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(11):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=POW;//Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=COMMATWO;
					curr_expr[*curr+3]=CRB; //+
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}	
				case(12):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=NORMALABSOLUTEANGLE; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(13):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=NORMALNEARABSOLUTEANGLE; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(14):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=NORMALRELATIVEANGLE; //Maths.XXX(
					curr_expr[*curr+1]=VARIABLES; //Variables
					curr_expr[*curr+2]=CRB; //+
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
					
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case twoVariables
			
		
		case(LOOPVARIABLE): {
			if (topVariableNo > latestVariableNo) {
				for(int i=*length-1;i>*curr;i--)
					curr_expr[i+1]=curr_expr[i];
				*length = *length + 1;
				int divisor = topVariableNo - latestVariableNo;
				curr_expr[*curr+0] = 300 + currentCodon%divisor;
				curr_expr[*curr+1]=POPTREE;
				if (ExtractingGrammarInfo) stack.push_back(codonPos);
				if (!terminating && ExtractingGrammarInfo)
					codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
				codonPos = codonPos + 1; // query if this should be incremented if terminating?
				break;
			} else {
				curr_expr[*curr]=GLOBALVARIABLE;
			}
			break;
		}
																  
		case(GLOBALVARIABLE):{ /* globalVariable */
			divisor = 12;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETX__; //getX__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETY__; //getY__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETHEADING__; //getHeading__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETGUNHEADING__; //getGunHeading__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETGUNHEAT__; //getGunHeat__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETRADARHEADING__; //getRadarHeading__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(6):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETBATTLEFIELDWIDTH__; //getBattleFieldWidth__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(7):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETBATTLEFIELDHEIGHT__; //getBattleFieldHEIGHT__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(8):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETENERGY__; //getEnergy__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(9):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=ONE; //1
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(10):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=ZERO; //0
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(11):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=STACKVARIABLE; //stackVariable
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case globalVariable
		case(STACKVARIABLE):{ /* stackVariable */
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SAFEPOP_OB; //safePop_OB
					curr_expr[*curr+1]=STACKTYPE; //stackType
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=SAFEPEEK_OB; //safePeek_OB
					curr_expr[*curr+1]=STACKTYPE; //stackType
					curr_expr[*curr+2]=CRB; //)
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case stackVariable
		case(STACKTYPE):{ /* stackType */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GLOBALSTACK1; //globalStack1
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GLOBALSTACK2; //globalStack2
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					switch (localVariableType) {
						case 1: // MAINLOOPVARIABLE;
							curr_expr[*curr+0]=MLLOCALSTACK1; //localStack1
							break;
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+0]=OHBBLOCALSTACK1; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+0]=OSRLOCALSTACK1; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+0]=OHRLOCALSTACK1; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+0]=OHWLOCALSTACK1; //localStack1
							break;
						default:	cout << "In stacktype LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					switch (localVariableType) {
						case 1: // MAINLOOPVARIABLE;
							curr_expr[*curr+0]=MLLOCALSTACK2; //localStack1
							break;
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+0]=OHBBLOCALSTACK2; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+0]=OSRLOCALSTACK2; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+0]=OHRLOCALSTACK2; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+0]=OHWLOCALSTACK2; //localStack1
							break;
						default:	cout << "In stacktype LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case stackType
		case(MAINLOOPVARIABLE):{ /* mainLoopVariable */
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETHEADING__; //e.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=GETENERGY__; //pe.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case mainLoopVariable
		case(ONHITBYBULLETVARIABLE):{ /* onHitByBulletVariable */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETBEARING__; //e.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETHEADING__; //e.getHeading__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETBEARING__; //pe.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETHEADING__; //pe.getHeading__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case onHitByBulletVariable
		case(ONSCANNEDROBOTVARIABLE):{ /* onScannedRobotVariable */
			divisor = 10;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETBEARING__; //e.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETHEADING__; //e.getHeading__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETVELOCITY__; //e.getVelocity__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETENERGY__; //e.getEnergy__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETDISTANCE__; //e.getDistance__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETBEARING__; //pe.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(6):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETHEADING__; //pe.getHeading__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(7):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETVELOCITY__; //pe.getVelocity__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(8):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETENERGY__; //pe.getEnergy__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(9):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETDISTANCE__; //pe.getDistance__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case onScannedRobotVariable
		case(ONHITROBOTVARIABLE):{ /* onHitRobotVariable */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETBEARING__; //e.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETENERGY__; //e.getEnergy__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETBEARING__; //pe.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETENERGY__; //pe.getEnergy__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case onHitRobotVariable
		case(ONHITWALLVARIABLE):{ /* onHitWallVariable */
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=E_DOTGETBEARING__; //e.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PE_DOTGETBEARING__; //pe.getBearing__
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case onHitWallVariable
		case(SOMEFUNCTIONS):{ /* SOMEFUNCTIONS */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+6]=curr_expr[i];
					*length = *length + 6;
					curr_expr[*curr+0]=PDIVCODE; //PDIVCODE
					curr_expr[*curr+1]=N; //N
					curr_expr[*curr+2]=PUSHCODE; //PUSHCODE
					curr_expr[*curr+3]=N; //N
					curr_expr[*curr+4]=POPCODE; //POPCODE
					curr_expr[*curr+5]=N; //N
					curr_expr[*curr+6]=PEEKCODE; //PEEKCODE
					break;
				}
			}//of switch
			break;
		}//of case SOMEFUNCTIONS
		case(PDIVCODE):{ /* PDIVCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+25]=curr_expr[i];
					*length = *length + 25;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=DOUBLE; //double
					curr_expr[*curr+2]=PDIV_OB; //PDIV_OB
					curr_expr[*curr+3]=DOUBLE; //double
					curr_expr[*curr+4]=TOP; //top
					curr_expr[*curr+5]=COMMA; //,
					curr_expr[*curr+6]=DOUBLE; //double
					curr_expr[*curr+7]=BOTTOM; //bottom
					curr_expr[*curr+8]=CRB; //)
					curr_expr[*curr+9]=OCB; //{
					curr_expr[*curr+10]=IF; //if
					curr_expr[*curr+11]=ORB; //(
					curr_expr[*curr+12]=BOTTOM; //bottom
					curr_expr[*curr+13]=EEQUAL; //==
					curr_expr[*curr+14]=ZERO; //0
					curr_expr[*curr+15]=CRB; //)
					curr_expr[*curr+16]=RETURN; //return
					curr_expr[*curr+17]=ONE; //1
					curr_expr[*curr+18]=SEMICOLON; //;
					curr_expr[*curr+19]=ELSE; //else
					curr_expr[*curr+20]=RETURN; //return
					curr_expr[*curr+21]=TOP; //top
					curr_expr[*curr+22]=DIVIDED; //DIVIDED
					curr_expr[*curr+23]=BOTTOM; //bottom
					curr_expr[*curr+24]=SEMICOLON; //;
					curr_expr[*curr+25]=CCB; //}
					break;
				}
			}//of switch
			break;
		}//of case PDIVCODE
		case(PUSHCODE):{ /* PUSHCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+17]=curr_expr[i];
					*length = *length + 17;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=VOID; //void
					curr_expr[*curr+2]=SAFEPUSH_OB; //safePush_OB
					curr_expr[*curr+3]=STACK; //stack
					curr_expr[*curr+4]=THESTACK; //thestack
					curr_expr[*curr+5]=COMMA; //,
					curr_expr[*curr+6]=DOUBLE; //double
					curr_expr[*curr+7]=ITEM; //item
					curr_expr[*curr+8]=CRB; //)
					curr_expr[*curr+9]=OCB; //{
					curr_expr[*curr+10]=THESTACKDOTPUSH_OB; //thestack.push_OB
					curr_expr[*curr+11]=NEW; //new
					curr_expr[*curr+12]=DOUBLE_OB; //Double_OB
					curr_expr[*curr+13]=ITEM; //item
					curr_expr[*curr+14]=CRB; //)
					curr_expr[*curr+15]=CRB; //)
					curr_expr[*curr+16]=SEMICOLON; //;
					curr_expr[*curr+17]=CCB; //}
					break;
				}
			}//of switch
			break;
		}//of case PUSHCODE
		case(POPCODE):{ /* POPCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+18]=curr_expr[i];
					*length = *length + 18;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=DOUBLE; //double
					curr_expr[*curr+2]=SAFEPOP_OB; //safePop_OB
					curr_expr[*curr+3]=STACK; //stack
					curr_expr[*curr+4]=THESTACK; //thestack
					curr_expr[*curr+5]=CRB; //)
					curr_expr[*curr+6]=OCB; //{
					curr_expr[*curr+7]=IF; //if
					curr_expr[*curr+8]=ORB; //(
					curr_expr[*curr+9]=THESTACKDOTEMPTY__; //thestack.empty__
					curr_expr[*curr+10]=CRB; //)
					curr_expr[*curr+11]=RETURN; //return
					curr_expr[*curr+12]=ZERO; //0
					curr_expr[*curr+13]=SEMICOLON;
					curr_expr[*curr+14]=ELSE; //else
					curr_expr[*curr+15]=RETURN; //return
					curr_expr[*curr+16]=THESTACKDOTPOP__; //thestack.pop__
					curr_expr[*curr+17]=SEMICOLON; //;
					curr_expr[*curr+18]=CCB; //}
					break;
				}
			}//of switch
			break;
		}//of case POPCODE
		case(PEEKCODE):{ /* PEEKCODE */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+18]=curr_expr[i];
					*length = *length + 18;
					curr_expr[*curr+0]=PUBLIC; //public
					curr_expr[*curr+1]=DOUBLE; //double
					curr_expr[*curr+2]=SAFEPEEK_OB; //safePeek_OB
					curr_expr[*curr+3]=STACK; //stack
					curr_expr[*curr+4]=THESTACK; //thestack
					curr_expr[*curr+5]=CRB; //)
					curr_expr[*curr+6]=OCB; //{
					curr_expr[*curr+7]=IF; //if
					curr_expr[*curr+8]=ORB; //(
					curr_expr[*curr+9]=THESTACKDOTEMTPY__; //thestack.emtpy__
					curr_expr[*curr+10]=CRB; //)
					curr_expr[*curr+11]=RETURN; //return
					curr_expr[*curr+12]=ZERO; //0
					curr_expr[*curr+13]=SEMICOLON;
					curr_expr[*curr+14]=ELSE; //else
					curr_expr[*curr+15]=RETURN; //return
					curr_expr[*curr+16]=THESTACKDOTPEEK__; //thestack.peek__
					curr_expr[*curr+17]=SEMICOLON; //;
					curr_expr[*curr+18]=CCB; //}
					break;
				}
			}//of switch
			break;
		}//of case PEEKCODE
		case(ONSTART):{ /* ONSTART */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+18]=curr_expr[i];
					*length = *length + 18;
					//curr_expr[*curr+0]=STATIC; //static
					//curr_expr[*curr+1]=STACK; //stack
					//curr_expr[*curr+2]=LOCALSTACK1; //localStack1,
					//curr_expr[*curr+3]=COMMA;
					//curr_expr[*curr+4]=LOCALSTACK2; //localStack2
					//curr_expr[*curr+5]=SEMICOLON; //;
					//curr_expr[*curr+6]=N; //N
					//curr_expr[*curr+0]=STATIC; //static
					//curr_expr[*curr+1]=BOOLW; //bool
					//curr_expr[*curr+2]=HISTORY; //history
					//curr_expr[*curr+3]=EQUAL; //=
					//curr_expr[*curr+4]=FALSE; //false
					//curr_expr[*curr+5]=SEMICOLON; //;
					//curr_expr[*curr+6]=N; //N
					switch (localVariableType) {
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+0]=HITBYBULLETEVENT; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+0]=SCANNEDROBOTEVENT; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+0]=HITROBOTEVENT; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+0]=HITWALLEVENT; //localStack1
							break;
						default:	cout << "In onstart LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+1]=PE;
					curr_expr[*curr+2]=SEMICOLON;
					curr_expr[*curr+3]=N;
					
					
					curr_expr[*curr+4]=IF; //if
					curr_expr[*curr+5]=ORB; //(
					curr_expr[*curr+6]=EXCLAMATION;
					switch (localVariableType) {
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+7]=HBBHISTORY; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+7]=OSRHISTORY; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+7]=HRHISTORY; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+7]=HITWALLHISTORY; //localStack1
							break;
						default:	cout << "In onstart LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+8]=CRB; //)
					curr_expr[*curr+9]=PE;
					curr_expr[*curr+10]=EQUAL;
					
					curr_expr[*curr+11]=E; //pe
					curr_expr[*curr+12]=SEMICOLON;
					curr_expr[*curr+13]=ELSE;
					curr_expr[*curr+14]=PE;
					curr_expr[*curr+15]=EQUAL;
					switch (localVariableType) {
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+16]=PHBBEVENT; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+16]=PSREVENT; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+16]=PHREVENT; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+16]=PHWEVENT; //localStack1
							break;
						default:	cout << "In onstart LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+17]=SEMICOLON; //;
					curr_expr[*curr+18]=N; //N
					break;
				}
			}//of switch
			break;
		}//of case ONSTART
		case(ONEXIT):{ /* ONEXIT */
			divisor = 1;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+18]=curr_expr[i];
					*length = *length + 18;
					curr_expr[*curr+0]=N; // newline
					switch (localVariableType) {
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+1]=PHBBEVENT; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+1]=PSREVENT; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+1]=PHREVENT; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+1]=PHWEVENT; //localStack1
							break;
						default:	cout << "In onstart LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+2]=EQUAL; //=
					curr_expr[*curr+3]=E; //e
					curr_expr[*curr+4]=SEMICOLON; //;
					curr_expr[*curr+5]=N;
					curr_expr[*curr+6]=IF; //if
					curr_expr[*curr+7]=ORB; //(
					curr_expr[*curr+8]=EXCLAMATION;
					switch (localVariableType) {
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+9]=HBBHISTORY; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+9]=OSRHISTORY; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+9]=HRHISTORY; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+9]=HITWALLHISTORY; //localStack1
							break;
						default:	cout << "In onstart LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+10]=CRB; //)
					switch (localVariableType) {
						case 2:	// ONHITBYBULLETVARIABLE;
							curr_expr[*curr+11]=HBBHISTORY; //localStack1
							break;
						case 3:	// ONSCANNEDROBOTVARIABLE;
							curr_expr[*curr+11]=OSRHISTORY; //localStack1
							break;
						case 4:	// ONHITROBOTVARIABLE;
							curr_expr[*curr+11]=HRHISTORY; //localStack1
							break;
						case 5:	// ONHITWALLVARIABLE;
							curr_expr[*curr+11]=HITWALLHISTORY; //localStack1
							break;
						default:	cout << "In onstart LocalVariable type was " << localVariableType << " \n";
							exit(1);
							break;
					}
					curr_expr[*curr+12]=EQUAL; //=
					curr_expr[*curr+13]=TRUE; //true
					curr_expr[*curr+14]=SEMICOLON; //;
					curr_expr[*curr+15]=N; //N
					curr_expr[*curr+16]=EXECUTE;
					curr_expr[*curr+17]=SEMICOLON;
					curr_expr[*curr+18]=N;
					
					break;
				}
			}//of switch
			break;
		}//of case ONEXIT
		case(NOTUSED):{ /* NOTUSED */
			divisor = 10;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=DEFUN; //DEFUN
					curr_expr[*curr+1]=OFB; //*(
					curr_expr[*curr+2]=CFB; //)*
					curr_expr[*curr+3]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=_DOFUNC; //FUNC
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=_DOPUSE; //PUSE
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=DPVAL; //DPVAL
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=DFVAL; //DFVAL
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PARAMS; //PARAMS
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(6):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PARAM; //PARAM
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(7):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=FPARS; //FPARS
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(8):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=FUNCBODY; //FUNCBODY
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
				case(9):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=CVAL; //CVAL
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
				}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case NOTUSED
		case(_DOFUNC):{ /* _DOFUNC */
			if (doingPTC2) {
				curr_expr[0]=FUNCPLACEHOLDER;
				break;
			}
			if (expandPTC2) {
				//cout << "Hit a dofun with expandPTC2 true";
				balloonExpansion++;
				growing=true;
				maxDepth=2;
				maxGrow = false;
				if (balloonExpansion == 1) {
					depth = 1; // so we allow them to expand a bit but not too much
					lastCodonPos = codonPos-1; // prime the pump so to speak
				}
				// need to think what happens here with recursion
				//print_grammar_individual(cout,curr_expr,*length);
				//cout << endl;
				for(int i=*length-1;i>*curr;i--)
					curr_expr[i+1]=curr_expr[i];
				*length = *length + 1;
				curr_expr[*curr]=__DOFUNC;	// And no functions happen to have been defined. 
				curr_expr[*curr+1]=BEPOPTREE;
				//print_grammar_individual(cout,curr_expr,*length);
				//cout << endl;
				break;
			}
			curr_expr[*curr]=__DOFUNC;
			break;
		}
		case(__DOFUNC):{ /* __DOFUNC */
			expr_t=_DOFUNC;
			divisor = 0;
			if (functionNos>0) divisor = divisor + functionNos;
			if (divisor == 0) {		// This deals with the situation where no other members of the branch 
				for(int i=*length-1;i>*curr;i--)
					curr_expr[i+1]=curr_expr[i];
				*length = *length + 1;
				curr_expr[*curr]=DFVAL;	// And no functions happen to have been defined. 
				curr_expr[*curr+1]=POPTREE;
				if (ExtractingGrammarInfo) stack.push_back(codonPos);
			} else { 
				if (growing) {
					nonTerminals.clear();
					terminals.clear();
					for (int i=0;i<functionNos;i++) {
						terminals.push_back(0+i);
					}
					changeCodonsForGrow(divisor);
					currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
				}
				switch(currentCodon%divisor){
					default:{ // We are going to call a function
						if (functionNos == 0) {
							cout << "Error trying to call a non-extant function\n";
							break;
						}
						int function = (currentCodon%divisor) - (divisor - functionNos); // work out which function to call
						int fPars = (*funcVector)[function].params;
						int shift = (fPars == 0?3:fPars*2 +2); // need to shift by a number dependent on the paramaters of the function
						for (int i=*length-1;i>*curr;i--)
							curr_expr[i+shift]=curr_expr[i];
						*length = *length+shift;
						curr_expr[*curr+0]=500+function;
						curr_expr[*curr+1]=OFB; //  *( = 0
						if (fPars > 0) {
							curr_expr[*curr+2]=FPARS; // FPARS = 142
							for (int j=0;j<fPars-1;j++) {
								curr_expr[*curr+3+2*j]=COMMA; // , = 0
								curr_expr[*curr+4+2*j]=FPARS; // FPARS = 142
							}
						}
						curr_expr[*curr+shift-1]=CFB; // )* = 0
						curr_expr[*curr+shift]=POPTREE;
						if (ExtractingGrammarInfo) stack.push_back(codonPos);
						break;
					} // of default branch
				}//of switch
			}
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case _DOFUNC
		case(_DOPUSE):{ /* _DOPUSE */
			if (doingPTC2) {
				curr_expr[0]=PUSEPLACEHOLDER;
				break;
			}
			if (expandPTC2) {
				//cout << "Hit a dopuse with expandPTC2 true";
				balloonExpansion++;
				growing=true;
				maxDepth=2;
				maxGrow = false;
				if (balloonExpansion == 1) {
					depth = 1; // so we allow them to expand a bit but not too much
					lastCodonPos = codonPos-1; // prime the pump so to speak
				}
				// need to think what happens here with recursion
				//print_grammar_individual(cout,curr_expr,*length);
				//cout << endl;
				for(int i=*length-1;i>*curr;i--)
					curr_expr[i+1]=curr_expr[i];
				*length = *length + 1;
				curr_expr[*curr]=__DOPUSE;	// And no functions happen to have been defined. 
				curr_expr[*curr+1]=BEPOPTREE;
				//print_grammar_individual(cout,curr_expr,*length);
				//cout << endl;
				break;
			}
			curr_expr[*curr]=__DOPUSE;
			break;
		}
		case(__DOPUSE):{ /* __DOPUSE */
			expr_t=_DOPUSE;
			divisor = 0;
			if (AllowParams) divisor = divisor + noOfParams;
			if (divisor == 0) {		// This deals with the situation where no other members of the branch 
				for(int i=*length-1;i>*curr;i--)
					curr_expr[i+1]=curr_expr[i];
				*length = *length + 1;
				curr_expr[*curr]=DPVAL;	// And no functions/params happen to have been defined. 
				curr_expr[*curr+1]=POPTREE;
				if (ExtractingGrammarInfo) stack.push_back(codonPos);
			} else {
				if (growing) {
					nonTerminals.clear();
					terminals.clear();
					if (AllowParams) {
						for (int i=0;i<noOfParams;i++) {
							terminals.push_back(0+ i);
						}
					}
					changeCodonsForGrow(divisor);
					currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
				}
				switch(currentCodon%divisor){
					default: { // We are using a paramater
						for(int i=*length-1;i>*curr;i--)
							curr_expr[i+1]=curr_expr[i];
						*length = *length + 1;
						curr_expr[*curr+0] = 400 + currentCodon%divisor - (divisor-noOfParams);
						curr_expr[*curr+1]=POPTREE;
						if (ExtractingGrammarInfo) stack.push_back(codonPos);
						break;
					} // of default 
				}//of switch
			}
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
		}//of case _DOPUSE
		default:{ 
			cout << "Odd hit default in case\n";
			*curr = *curr + 1;
		}
	}//Of switch
}
/***************************************************************/
void robocodeTheta::print_grammar_individual(ostream& output_stream,int curr_expr[],int length, char *name, char *gen){
	int indent = 0;
	for(int i=0;i<length;i++)
		switch(curr_expr[i]){
			case COMMA: {output_stream << " , ";break;}
			case ORB: {output_stream << " ( ";break;}
			case CRB: {output_stream << " ) ";break;}
			case OSB: {output_stream << " [ ";break;}
			case CSB: {output_stream << " ] ";break;}
			case DEFUN: {output_stream << " DEFUN ";break;}
			case OFB: {output_stream << " *( ";break;}
			case CFB: {output_stream << " )* ";break;} 
			case OCB: {output_stream << "\n";indent+=2;for (int i=0;i<indent;i++) output_stream << " ";output_stream << "{ ";break;}
			case CCB: {output_stream << "\n";for (int i=0;i<indent;i++) output_stream << " ";output_stream << "}\n";indent -=2;for (int i=0;i<indent;i++) output_stream << " ";break;}
			case N: {output_stream << "\n";for (int i=0;i<indent;i++) output_stream << " ";break;}
			case BEGINEXEC: {output_stream << " *{ ";break;}
			case ENDEXEC: {output_stream << " }* ";break;}
			case PLUS: {output_stream << " + ";break;}
			case MINUS: {output_stream << " - ";break;}
			case MULT: {output_stream << " * ";break;}
			case DIV: {output_stream << " % ";break;}
			case SEMICOLON: {output_stream << " ; ";break;}
			case COLON: {output_stream << " : ";break;}
			case GREATER: {output_stream << " > ";break;}
			case LESS: {output_stream << " < ";break;}
			case EEQUAL: {output_stream << " == ";break;}
			case EQUAL: {output_stream << " = ";break;}
			case ZERO: {output_stream << " 0 ";break;}
			case ONE: {output_stream << " 1 ";break;}
			case TWO: {output_stream << " 2 ";break;}
			case THREE: {output_stream << " 3 ";break;}
			case FOUR: {output_stream << " 4 ";break;}
			case FIVE: {output_stream << " 5 ";break;}
			case SIX: {output_stream << " 6 ";break;}
			case SEVEN: {output_stream << " 7 ";break;}
			case EIGHT: {output_stream << " 8 ";break;}
			case NINE: {output_stream << " 9 ";break;}
			case NOP: {output_stream << " NOP ";break;}
			case BEPOPTREE: {output_stream << " BEPOPTREE ";break;}
			case FUNCPLACEHOLDER: {output_stream << " FUNCPLACEHOLDER ";break;}
			case PUSEPLACEHOLDER: {output_stream << " PUSEPLACEHOLDER ";break;}
			case IMPORT: {output_stream << " package " << gen << ";" << endl << "import ";break;}
			case ROBOCODEDOTSTAR: {
				output_stream << " robocode.*;\nimport java.util.*;\nimport robocode.util.Utils";
				break; }
			case PUBLIC: {output_stream << " public ";break;}
			case CLASS: {output_stream << " class ";break;}
			case NAME: {output_stream << name; break;}
			case EXTENDS: {output_stream << " extends ";break;}
			case ADVANCEDROBOT: {output_stream << " AdvancedRobot ";break;}
			case STATIC: {output_stream << " static ";break;}
			case STACK: {output_stream << " Stack ";break;}
				//		case GLOBALSTACK1,: {output_stream << " globalStack1, ";break;}
			case DOVARIABLEDEFS: {output_stream << "  Stack<Double> globalStack1, globalStack2;\n " <<	
				   "   Stack<Double> mlLocalStack1,mlLocalStack2;\n    Stack<Double> ohrLocalStack1,ohrLocalStack2;\n    Stack<Double> ohbbLocalStack1,ohbbLocalStack2;\n    Stack<Double> osrLocalStack1,osrLocalStack2;\n    Stack<Double> ohwLocalStack1,ohwLocalStack2;\n"
				<< "    HitByBulletEvent phbEvent;\n    HitRobotEvent phrEvent;\n    HitWallEvent phwEvent;\n    ScannedRobotEvent psrEvent;\n"
				<< "    boolean hbbHistory = false;\n    boolean hrHistory=false;\n    boolean srHistory = false;\n    boolean hwHistory = false;\n\n\n"
				<< "//********************************\n//Some useful helper functions\n//********************************\n"; break;}
			case SOMECOMMENTS: { output_stream 
				<< "\n//********************************\n//Start of the evolved code\n//********************************\n\n"; break;}
			case VOID: {output_stream << " void ";break;}
			case RUN__: {output_stream << " run() ";break;}
			case INMAINLOOP: {output_stream << " INMAINLOOP ";break;}
			case WHILE: {output_stream << " while ";break;}
			case TRUE_: {output_stream << " true ";break;}
			case ONSCANNEDROBOT_SCANNEDROBOTEVENT: {output_stream << " onScannedRobot(ScannedRobotEvent ";break;}
			case E_CB: {output_stream << " e) ";break;}
			case SCANNEDROBOTEVENT: {output_stream << " ScannedRobotEvent ";break;}
			case PE: {output_stream << " pe ";break;}
			case INSCANNEDLOOP: {output_stream << " INSCANNEDLOOP ";break;}
			case ONHITBYBULLET_HITBYBULLETEVENT: {output_stream << " onHitByBullet(HitByBulletEvent ";break;}
			case HITBYBULLETEVENT: {output_stream << " HitByBulletEvent ";break;}
			case INHITBYBULLETLOOP: {output_stream << " INHITBYBULLETLOOP ";break;}
			case ONHITROBOT_HITROBOTEVENT: {output_stream << " onHitRobot(HitRobotEvent ";break;}
			case HITROBOTEVENT: {output_stream << " HitRobotEvent ";break;}
			case INHITROBOTLOOP: {output_stream << " INHITROBOTLOOP ";break;}
			case ONHITWALL_HITWALLEVENT: {output_stream << " onHitWall(HitWallEvent ";break;}
			case HITWALLEVENT: {output_stream << " HitWallEvent ";break;}
			case INHITWALLLOOP: {output_stream << " INHITWALLLOOP ";break;}
			case IF: {output_stream << " if ";break;}
			case ELSE: {output_stream << " else ";break;}
			case SETTURNRIGHT_OB: {output_stream << " setTurnRight( ";break;}
			case SETTURNLEFT_OB: {output_stream << " setTurnLeft( ";break;}
			case SETTURNGUNRIGHT_OB: {output_stream << " setTurnGunRight( ";break;}
			case SETTURNGUNLEFT_OB: {output_stream << " setTurnGunLeft( ";break;}
			case SETTURNRADARRIGHT_OB: {output_stream << " setTurnRadarRight( ";break;}
			case SETTURNRADARLEFT_OB: {output_stream << " setTurnRadarLeft( ";break;}
			case SETAHEAD_OB: {output_stream << " setAhead( ";break;}
			case SETBACK_OB: {output_stream << " setBack( ";break;}
			case RESUME__: {output_stream << " resume() ";break;}
			case STOP__: {output_stream << " stop() ";break;}
			case SETADJUSTGUNFORROBOTTURN_OB: {output_stream << " setAdjustGunForRobotTurn( ";break;}
			case SETADJUSTRADARFORROBOTTURN_OB: {output_stream << " setAdjustRadarForRobotTurn( ";break;}
			case SETADJUSTRADARFORGUNTURN_OB: {output_stream << " setAdjustRadarForGunTurn( ";break;}
			case SAFEPUSH_OB: {output_stream << " safePush( ";break;}
			case SAFESWAP_OB: {output_stream << " safeSwap( ";break;}
			case FIRE_OB: {output_stream << " fire( ";break;}
			case SETFIRE_OB: {output_stream << " setFire( ";break;}
			case PDIV_OB: {output_stream << " PDIV( ";break;}
			case NOT_OB: {output_stream << " !( ";break;}
			case AND: {output_stream << " & ";break;}
			case XOR: {output_stream << " ^ ";break;}
			case OR: {output_stream << " | ";break;}
			case GETX__: {output_stream << " getX() ";break;}
			case GETY__: {output_stream << " getY() ";break;}
			case GETHEADING__: {output_stream << " getHeading() ";break;}
			case GETGUNHEADING__: {output_stream << " getGunHeading() ";break;}
			case GETGUNHEAT__: {output_stream << " getGunHeat() ";break;}
			case GETRADARHEADING__: {output_stream << " getRadarHeading() ";break;}
			case GETBATTLEFIELDWIDTH__: {output_stream << " getBattleFieldWidth() ";break;}
			case GETENERGY__: {output_stream << " getEnergy() ";break;}
			case SAFEPOP_OB: {output_stream << " safePop( ";break;}
			case SAFEPEEK_OB: {output_stream << " safePeek( ";break;}
			case GLOBALSTACK1: {output_stream << " globalStack1 ";break;}
			case GLOBALSTACK2: {output_stream << " globalStack2 ";break;}
				//case LOCALSTACK1: {output_stream << " localStack1 ";break;}
			//case LOCALSTACK2: {output_stream << " localStack2 ";break;}
			case E_DOTGETBEARING__: {output_stream << " e.getBearing() ";break;}
			case E_DOTGETHEADING__: {output_stream << " e.getHeading() ";break;}
			case PE_DOTGETBEARING__: {output_stream << " pe.getBearing() ";break;}
			case PE_DOTGETHEADING__: {output_stream << " pe.getHeading() ";break;}
			case E_DOTGETVELOCITY__: {output_stream << " e.getVelocity() ";break;}
			case E_DOTGETENERGY__: {output_stream << " e.getEnergy() ";break;}
			case E_DOTGETDISTANCE__: {output_stream << " e.getDistance() ";break;}
			case PE_DOTGETVELOCITY__: {output_stream << " pe.getVelocity() ";break;}
			case PE_DOTGETENERGY__: {output_stream << " pe.getEnergy() ";break;}
			case PE_DOTGETDISTANCE__: {output_stream << " pe.getDistance() ";break;}
			case DOUBLE: {output_stream << " double ";break;}
			case TOP: {output_stream << " top ";break;}
			case BOTTOM: {output_stream << " bottom ";break;}
			case RETURN: {output_stream << " return ";break;}
			case DIVIDED: {output_stream << " / ";break;}
			case THESTACK: {output_stream << " thestack ";break;}
			case ITEM: {output_stream << " item ";break;}
			case THESTACKDOTPUSH_OB: {output_stream << " if (thestack.size() > 10) thestack.removeElementAt(0); thestack.push( ";break;}
			case NEW: {output_stream << " new ";break;}
			case DOUBLE_OB: {output_stream << " Double( ";break;}
			case THESTACKDOTEMPTY__: {output_stream << " thestack.empty() ";break;}
			case THESTACKDOTPOP__: {output_stream << " ( (Double) thestack.pop()).doubleValue() ";break;}
			case THESTACKDOTEMTPY__: {output_stream << " thestack.empty() ";break;}
			case THESTACKDOTPEEK__: {output_stream << " ( (Double) thestack.peek()).doubleValue() ";break;}
				//		case LOCALSTACK1,: {output_stream << " localStack1, ";break;}
			case BOOLW: {output_stream << " bool ";break;}
			case HISTORY: {output_stream << " history ";break;}
			case FALSE: {output_stream << " false ";break;}
			case NOTHISTORY: {output_stream << " !history ";break;}
			case E: {output_stream << " e ";break;}
			case TRUE: {output_stream << " true ";break;}
			case _DOFUNC: {output_stream << " FUNC ";break;}
			case _DOPUSE: {output_stream << " PUSE ";break;}
			case DPVAL: {output_stream << " DPVAL ";break;}
			case DFVAL: {output_stream << " DFVAL ";break;}
			case PARAMS: {output_stream << " PARAMS ";break;}
			case PARAM: {output_stream << " PARAM ";break;}
			case FPARS: {output_stream << " FPARS ";break;}
			case FUNCBODY: {output_stream << " FUNCBODY ";break;}
			case CVAL: { output_stream << " CVAL :"; i++ ; output_stream << curr_expr[i] << ": "; break;}
			case POPTREE: {output_stream << " POPTREE ";break;}
			case POPTREED: {output_stream << " POPTREED ";break;}
			case _FUNC: {output_stream << " _FUNC ";break;}
			case _PUSE: {output_stream << " _PUSE ";break;}
			case __DOFUNC: {output_stream << " __DOFUNC ";break;}
			case __DOPUSE: {output_stream << " __DOPUSE ";break;}
			case EXECUTE: { output_stream << " execute() ";break;}
			case MLLOCALSTACK1: { output_stream << " mlLocalStack1 ";break;}
			case MLLOCALSTACK2: { output_stream << " mlLocalStack2 ";break;}
			case OHBBLOCALSTACK1: { output_stream << " ohbbLocalStack1 ";break;}
			case OHBBLOCALSTACK2: { output_stream << " ohbbLocalStack2 ";break;}
			case OHRLOCALSTACK1: { output_stream << " ohrLocalStack1 ";break;}
			case OHRLOCALSTACK2: { output_stream << " ohrLocalStack2 ";break;}
			case OHWLOCALSTACK1: { output_stream << " ohwLocalStack1 ";break;}
			case OHWLOCALSTACK2: { output_stream << " ohwLocalStack2 ";break;}
			case OSRLOCALSTACK1: { output_stream << " osrLocalStack1 ";break;}
			case OSRLOCALSTACK2: { output_stream << " osrLocalStack2 ";break;}
			
			case EXCLAMATION: { output_stream << " !";break;}
			case HBBHISTORY: { output_stream << " hbbHistory ";break;}
			case OSRHISTORY: { output_stream << " srHistory ";break;}
			case HRHISTORY: { output_stream << " hrHistory ";break;}
			case HITWALLHISTORY: { output_stream << " hwHistory ";break;}
			case PHBBEVENT: { output_stream << " phbEvent ";break;}
			case PSREVENT: { output_stream << " psrEvent ";break;}
			case PHREVENT: { output_stream << " phrEvent ";break;}
			case PHWEVENT: { output_stream << " phwEvent ";break;}
				
			case ABS: { output_stream << " Math.abs( ";break;}
			case ACOS: { output_stream << " Math.acos( ";break;}
			case ASIN: { output_stream << " Math.asin( ";break;}
			case ATAN: { output_stream << " Math.atan( ";break;}
			case ATAN2: { output_stream << " Math.atan2( ";break;}
			case COS: { output_stream << " Math.cos( ";break;}
			case SIN: { output_stream << " Math.sin( ";break;}
			case MAX: { output_stream << " Math.max( ";break;}
			case MIN: { output_stream << " Math.min( ";break;}
			case ROUND: { output_stream << " Math.round( ";break;}
			case SQRT: { output_stream << " Math.sqrt( ";break;}
			case POW: { output_stream << " Math.pow( ";break;}
			case ISNEAR: { output_stream << " Utils.isNear( ";break;}
			case NORMALABSOLUTEANGLE: { output_stream << " Utils.normalAbsoluteAngle( ";break;}
			case NORMALNEARABSOLUTEANGLE: { output_stream << " Utils.normalNearAbsoluteAngle( ";break;}
			case NORMALRELATIVEANGLE: { output_stream << " Utils.normalRelativeAngle( ";break;}
			case COMMATWO: { output_stream << ", 2";break;}
			case IN: { output_stream << " : ";break;}
			case GETBATTLEFIELDHEIGHT__: { output_stream << " getBattleFieldHeight() "; break;}
			case FOR: { output_stream << " for ";break;}
			
			default: {
				if (curr_expr[i]>=400 && curr_expr[i]<500) {
					output_stream << "P" << curr_expr[i]-400;
				} else if (curr_expr[i]>=500 && curr_expr[i]<600) {
					output_stream << "F" << curr_expr[i]-500;
				} else if (curr_expr[i]>=300 && curr_expr[i]<350) {
					output_stream << "VAR" << curr_expr[i]-300;
				} else {
					output_stream << " UNKNOWN(" << curr_expr[i] << ") ";
				}
				break;
			} // of default branch
		}
	output_stream << endl;
}
