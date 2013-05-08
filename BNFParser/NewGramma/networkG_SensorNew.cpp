//*************************************************************************
//                                                                       
//    FILE : networkG_SensorNew.cpp                                    
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
#include "networkG_SensorNew.h" 
#include "myrandom.h"
/***************************************************************/
// File generated from grammar included in file:
// networkG_SensorNew.bnf
/***************************************************************/
// code:	DEFUN *(  )* FUNCBODY
// 		| defineFunc code
// 
// defineFunc:	NOP
// 			| DEFUN *( )*
// 			| DEFUN *( )* DEFUN *( )*
// 
// 
// FUNCBODY: if ( bool ) then { commands } else { FUNCBODY } |
// 		  commands
// 
// commands:	command | command
// 			| command commands
// 			| if ( cond ) then { commands } else { commands }
// 			
// command:	newNetwork
//         	| connection
// 			| setweight ( Value )
// 			| wait
// 			| end
// 			| FUNC			
// 
// loopCommands:	{ commands } ( Value )
// 
// Value:	Number
// 		| ( Value Op Value )
// 
// Number: CVAL
//         | PUSE
// 
// Op:		+ | - | % | *
// 
// PARAMS:  PARAM | PARAM PARAMS
// 
// FPARS: Value 
// 
// newNetwork: 	threeCommand *{ commands }* *{ commands }* commands
// 			    | twoCommand *{ commands }* commands
// 				| oneCommand commands
//       
// 
// threeCommand: 	add_split ( Value )
//                 | add_split_cont ( Value , Value )
// 				| attach
//     
// twoCommand: add_reverse
//             | connect
//             | loop
//             | destinationToNext
//             | sourceToNext
//             | sourceToParent
// 			| Seq
// 			| Par
// 			| SeqCopyIn
// 			| SeqCopyOut
// 			| LSPLITNo ( Value )
// 			| USPLITNo ( Value )
// 			| xSPLIT ( Value , Value )
// 			| tSplit ( Value , Value )
// 
// oneCommand: push ( Value )
// 	   | pushr ( Value , Value )
// 	   | setSigma
// 	   | setExpa
// 	   | setBias ( Value )
//     
// connection:	connectInput ( Value )  *{ commands }* commands
//             | connectOutput ( Value ) *{ commands }* commands
// 			| splitInputHorizontally ( Value ) *{ commands }* *{ commands }* commands
// 			| splitInputVertically ( Value ) *{ commands }* *{ commands }* commands
// 			| connectHeight *{ commands }* *{ commands }*
// 			| connectAOA *{ commands }* *{ commands }*
// 			
// bool: PUSE comp Value
// 
// cond: 	cond boolop cond 
// 	| NOT cond 
// 	| [ Value comp Value ]
// 
// boolop:	AND | OR
// 	
// DFVAL: command
// 
// DPVAL: Value
// 
// comp: 		= | < | >
// 
// NotUsed:    NOPP | 
//             AMOUNT | JMP | JMPIFTRUE |JMPIFFALSE | DUMMY | XOR 
//             | EXEC | CALL | DUP | POP | PPR | NORMALISELOOPCOUNT | AVALUE | PAR1 | PAR2 | PVAL
//             
// 
// %%
/***************************************************************/
using namespace networkG_SensorNew_namespace;

networkG_SensorNew *networkG_SensorNew::_instance = 0;


networkG_SensorNew::networkG_SensorNew() { 
          allowFunctions = true;
		  ExtractingGrammarInfo = true;
		  stack.clear();
		  dc = dataCollector::Instance();
}

networkG_SensorNew* networkG_SensorNew::Instance() {
          if (_instance == 0) {
                    _instance = new networkG_SensorNew();
          }
          return _instance;
}


bool networkG_SensorNew::doACreate(int *exp,int *length,int maxExp,long codon_list[],
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

bool networkG_SensorNew::ptc2Grow(int *exp, int *length, int maxExp, long codon_list[],
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

void networkG_SensorNew::ptc2GrowOne(bool allowExpand) {
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


			
			
			
			
bool networkG_SensorNew::traverseParseTree(parseTreePtr cNode) {
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


void networkG_SensorNew::AllowFunctions(bool al) {
          allowFunctions = al;
          if (al) cout << "Functions are allowed.\n";
          else cout << "Functions are prohibited\n";
}

bool networkG_SensorNew::parse(int *exp,int *length,
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
		  
		  stack.clear();
		  doingPTC2 = false;
		  balloonExpansion = 0;
		  expandPTC2 = false;
		  // dc->resetCounters();
		  return doParse(cUsed);
}

bool networkG_SensorNew::doParse(int &cUsed) {
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
                              } else if (expression[expressionPos] < 1000) { // I SEEM TO HAVE HARD CODED < 1000 MEANS TERMINAL.
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
                                        apply_one_grammar(expression,&expressionPos,expressionLength);
                              }
                    } 
          } while (!invalid && (expressionPos<*expressionLength)); 
          // when expressionPos == expressionLength - it means we have fully expanded the expression.
          expression[*expressionLength]=0;  // The interpreter now expects the expression to be null terminated.
          if (!terminating ) cUsed=codonPos;
          else cUsed = maxCodons;
          return !invalid;
}

bool networkG_SensorNew::dealtWithPop(int expr[],int *curr,int *length) {
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

void networkG_SensorNew::changeCodonsForGrow(int divisors) {
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


void networkG_SensorNew::read_func() {
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

void networkG_SensorNew::apply_one_grammar(int curr_expr[],int *curr,int *length)
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
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=DEFUN; //DEFUN
					curr_expr[*curr+1]=OFB; //*(
					curr_expr[*curr+2]=CFB; //)*
					curr_expr[*curr+3]=FUNCBODY; //FUNCBODY
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=DEFINEFUNC; //defineFunc
					curr_expr[*curr+1]=CODE; //code
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
			}//of case code
		case(DEFINEFUNC):{ /* defineFunc */
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
					curr_expr[*curr+0]=NOP; //NOP
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
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
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+6]=curr_expr[i];
					*length = *length + 6;
					curr_expr[*curr+0]=DEFUN; //DEFUN
					curr_expr[*curr+1]=OFB; //*(
					curr_expr[*curr+2]=CFB; //)*
					curr_expr[*curr+3]=DEFUN; //DEFUN
					curr_expr[*curr+4]=OFB; //*(
					curr_expr[*curr+5]=CFB; //)*
					curr_expr[*curr+6]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case defineFunc
		case(FUNCBODY):{ /* FUNCBODY */
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
						curr_expr[i+12]=curr_expr[i];
					*length = *length + 12;
					curr_expr[*curr+0]=IF; //if
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=BOOL; //bool
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=THEN; //then
					curr_expr[*curr+5]=OCB; //{
					curr_expr[*curr+6]=COMMANDS; //commands
					curr_expr[*curr+7]=CCB; //}
					curr_expr[*curr+8]=ELSE; //else
					curr_expr[*curr+9]=OCB; //{
					curr_expr[*curr+10]=FUNCBODY; //FUNCBODY
					curr_expr[*curr+11]=CCB; //}
					curr_expr[*curr+12]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=COMMANDS; //commands
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
			}//of case FUNCBODY
		case(COMMANDS):{ /* commands */
			divisor = 4;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				nonTerminals.push_back(2);
				nonTerminals.push_back(3);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=COMMAND; //command
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=COMMAND; //command
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=COMMAND; //command
					curr_expr[*curr+1]=COMMANDS; //commands
					curr_expr[*curr+2]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+12]=curr_expr[i];
					*length = *length + 12;
					curr_expr[*curr+0]=IF; //if
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=COND; //cond
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=THEN; //then
					curr_expr[*curr+5]=OCB; //{
					curr_expr[*curr+6]=COMMANDS; //commands
					curr_expr[*curr+7]=CCB; //}
					curr_expr[*curr+8]=ELSE; //else
					curr_expr[*curr+9]=OCB; //{
					curr_expr[*curr+10]=COMMANDS; //commands
					curr_expr[*curr+11]=CCB; //}
					curr_expr[*curr+12]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case commands
		case(COMMAND):{ /* command */
			divisor = 6;
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
				terminals.push_back(4);
				terminals.push_back(5);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=NEWNETWORK; //newNetwork
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=CONNECTION; //connection
					curr_expr[*curr+1]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=SETWEIGHT; //setweight
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=WAIT; //wait
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=END; //end
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=_DOFUNC; //FUNC
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case command
		case(LOOPCOMMANDS):{ /* loopCommands */
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
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=OCB; //{
					curr_expr[*curr+1]=COMMANDS; //commands
					curr_expr[*curr+2]=CCB; //}
					curr_expr[*curr+3]=ORB; //(
					curr_expr[*curr+4]=VALUE; //Value
					curr_expr[*curr+5]=CRB; //)
					break;
					}
			}//of switch
			break;
			}//of case loopCommands
		case(VALUE):{ /* Value */
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(1);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=NUMBER; //Number
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=ORB; //(
					curr_expr[*curr+1]=VALUE; //Value
					curr_expr[*curr+2]=OP; //Op
					curr_expr[*curr+3]=VALUE; //Value
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
			}//of case Value
		case(NUMBER):{ /* Number */
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
					curr_expr[*curr+0]=CVAL; //CVAL
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=_DOPUSE; //PUSE
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case Number
		case(OP):{ /* Op */
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
					curr_expr[*curr+0]=PLUS; //+
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=MINUS; //-
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=DIV; //%
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=MULT; //*
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case Op
		case(PARAMS):{ /* PARAMS */
			divisor = 2;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				terminals.push_back(1);
				nonTerminals.push_back(1);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PARAM; //PARAM
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=PARAM; //PARAM
					curr_expr[*curr+1]=PARAMS; //PARAMS
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
			}//of case PARAMS
		case(FPARS):{ /* FPARS */
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
					curr_expr[*curr+0]=VALUE; //Value
					break;
					}
			}//of switch
			break;
			}//of case FPARS
		case(NEWNETWORK):{ /* newNetwork */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				nonTerminals.push_back(1);
				nonTerminals.push_back(2);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+8]=curr_expr[i];
					*length = *length + 8;
					curr_expr[*curr+0]=THREECOMMAND; //threeCommand
					curr_expr[*curr+1]=BEGINEXEC; //*{
					curr_expr[*curr+2]=COMMANDS; //commands
					curr_expr[*curr+3]=ENDEXEC; //}*
					curr_expr[*curr+4]=BEGINEXEC; //*{
					curr_expr[*curr+5]=COMMANDS; //commands
					curr_expr[*curr+6]=ENDEXEC; //}*
					curr_expr[*curr+7]=COMMANDS; //commands
					curr_expr[*curr+8]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=TWOCOMMAND; //twoCommand
					curr_expr[*curr+1]=BEGINEXEC; //*{
					curr_expr[*curr+2]=COMMANDS; //commands
					curr_expr[*curr+3]=ENDEXEC; //}*
					curr_expr[*curr+4]=COMMANDS; //commands
					curr_expr[*curr+5]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=ONECOMMAND; //oneCommand
					curr_expr[*curr+1]=COMMANDS; //commands
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
			}//of case newNetwork
		case(THREECOMMAND):{ /* threeCommand */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				nonTerminals.push_back(1);
				terminals.push_back(2);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=ADD_SPLIT; //add_split
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+6]=curr_expr[i];
					*length = *length + 6;
					curr_expr[*curr+0]=ADD_SPLIT_CONT; //add_split_cont
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=COMMA; //,
					curr_expr[*curr+4]=VALUE; //Value
					curr_expr[*curr+5]=CRB; //)
					curr_expr[*curr+6]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=ATTACH; //attach
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case threeCommand
		case(TWOCOMMAND):{ /* twoCommand */
			divisor = 14;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				terminals.push_back(1);
				terminals.push_back(2);
				terminals.push_back(3);
				terminals.push_back(4);
				terminals.push_back(5);
				terminals.push_back(6);
				terminals.push_back(7);
				terminals.push_back(8);
				terminals.push_back(9);
				terminals.push_back(10);
				nonTerminals.push_back(10);
				terminals.push_back(11);
				nonTerminals.push_back(11);
				nonTerminals.push_back(12);
				nonTerminals.push_back(13);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=ADD_REVERSE; //add_reverse
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=CONNECT; //connect
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=LOOP; //loop
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=DESTINATIONTONEXT; //destinationToNext
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SOURCETONEXT; //sourceToNext
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SOURCETOPARENT; //sourceToParent
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(6):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SEQ; //Seq
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(7):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PAR; //Par
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(8):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SEQCOPYIN; //SeqCopyIn
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(9):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SEQCOPYOUT; //SeqCopyOut
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(10):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=LSPLITNO; //LSPLITNo
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(11):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=USPLITNO; //USPLITNo
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(12):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+6]=curr_expr[i];
					*length = *length + 6;
					curr_expr[*curr+0]=XSPLIT; //xSPLIT
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=COMMA; //,
					curr_expr[*curr+4]=VALUE; //Value
					curr_expr[*curr+5]=CRB; //)
					curr_expr[*curr+6]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(13):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+6]=curr_expr[i];
					*length = *length + 6;
					curr_expr[*curr+0]=TSPLIT; //tSplit
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=COMMA; //,
					curr_expr[*curr+4]=VALUE; //Value
					curr_expr[*curr+5]=CRB; //)
					curr_expr[*curr+6]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case twoCommand
		case(ONECOMMAND):{ /* oneCommand */
			divisor = 5;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				terminals.push_back(0);
				nonTerminals.push_back(0);
				nonTerminals.push_back(1);
				terminals.push_back(2);
				terminals.push_back(3);
				terminals.push_back(4);
				nonTerminals.push_back(4);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=PUSH; //push
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+6]=curr_expr[i];
					*length = *length + 6;
					curr_expr[*curr+0]=PUSHR; //pushr
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=COMMA; //,
					curr_expr[*curr+4]=VALUE; //Value
					curr_expr[*curr+5]=CRB; //)
					curr_expr[*curr+6]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SETSIGMA; //setSigma
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=SETEXPA; //setExpa
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+4]=curr_expr[i];
					*length = *length + 4;
					curr_expr[*curr+0]=SETBIAS; //setBias
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case oneCommand
		case(CONNECTION):{ /* connection */
			divisor = 6;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				nonTerminals.push_back(1);
				nonTerminals.push_back(2);
				nonTerminals.push_back(3);
				nonTerminals.push_back(4);
				nonTerminals.push_back(5);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+8]=curr_expr[i];
					*length = *length + 8;
					curr_expr[*curr+0]=CONNECTINPUT; //connectInput
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=BEGINEXEC; //*{
					curr_expr[*curr+5]=COMMANDS; //commands
					curr_expr[*curr+6]=ENDEXEC; //}*
					curr_expr[*curr+7]=COMMANDS; //commands
					curr_expr[*curr+8]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+8]=curr_expr[i];
					*length = *length + 8;
					curr_expr[*curr+0]=CONNECTOUTPUT; //connectOutput
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=BEGINEXEC; //*{
					curr_expr[*curr+5]=COMMANDS; //commands
					curr_expr[*curr+6]=ENDEXEC; //}*
					curr_expr[*curr+7]=COMMANDS; //commands
					curr_expr[*curr+8]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+11]=curr_expr[i];
					*length = *length + 11;
					curr_expr[*curr+0]=SPLITINPUTHORIZONTALLY; //splitInputHorizontally
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=BEGINEXEC; //*{
					curr_expr[*curr+5]=COMMANDS; //commands
					curr_expr[*curr+6]=ENDEXEC; //}*
					curr_expr[*curr+7]=BEGINEXEC; //*{
					curr_expr[*curr+8]=COMMANDS; //commands
					curr_expr[*curr+9]=ENDEXEC; //}*
					curr_expr[*curr+10]=COMMANDS; //commands
					curr_expr[*curr+11]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+11]=curr_expr[i];
					*length = *length + 11;
					curr_expr[*curr+0]=SPLITINPUTVERTICALLY; //splitInputVertically
					curr_expr[*curr+1]=ORB; //(
					curr_expr[*curr+2]=VALUE; //Value
					curr_expr[*curr+3]=CRB; //)
					curr_expr[*curr+4]=BEGINEXEC; //*{
					curr_expr[*curr+5]=COMMANDS; //commands
					curr_expr[*curr+6]=ENDEXEC; //}*
					curr_expr[*curr+7]=BEGINEXEC; //*{
					curr_expr[*curr+8]=COMMANDS; //commands
					curr_expr[*curr+9]=ENDEXEC; //}*
					curr_expr[*curr+10]=COMMANDS; //commands
					curr_expr[*curr+11]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+7]=curr_expr[i];
					*length = *length + 7;
					curr_expr[*curr+0]=CONNECTHEIGHT; //connectHeight
					curr_expr[*curr+1]=BEGINEXEC; //*{
					curr_expr[*curr+2]=COMMANDS; //commands
					curr_expr[*curr+3]=ENDEXEC; //}*
					curr_expr[*curr+4]=BEGINEXEC; //*{
					curr_expr[*curr+5]=COMMANDS; //commands
					curr_expr[*curr+6]=ENDEXEC; //}*
					curr_expr[*curr+7]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+7]=curr_expr[i];
					*length = *length + 7;
					curr_expr[*curr+0]=CONNECTAOA; //connectAOA
					curr_expr[*curr+1]=BEGINEXEC; //*{
					curr_expr[*curr+2]=COMMANDS; //commands
					curr_expr[*curr+3]=ENDEXEC; //}*
					curr_expr[*curr+4]=BEGINEXEC; //*{
					curr_expr[*curr+5]=COMMANDS; //commands
					curr_expr[*curr+6]=ENDEXEC; //}*
					curr_expr[*curr+7]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case connection
		case(BOOL):{ /* bool */
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
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=_DOPUSE; //PUSE
					curr_expr[*curr+1]=COMP; //comp
					curr_expr[*curr+2]=VALUE; //Value
					break;
					}
			}//of switch
			break;
			}//of case bool
		case(COND):{ /* cond */
			divisor = 3;
			if (growing) {
				nonTerminals.clear();
				terminals.clear();
				nonTerminals.push_back(0);
				nonTerminals.push_back(1);
				nonTerminals.push_back(2);
				changeCodonsForGrow(divisor);
				currentCodon = (terminating?0:codons[codonPos]) & CODONMASK;
			}
			switch(currentCodon%divisor){
				case(0):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+3]=curr_expr[i];
					*length = *length + 3;
					curr_expr[*curr+0]=COND; //cond
					curr_expr[*curr+1]=BOOLOP; //boolop
					curr_expr[*curr+2]=COND; //cond
					curr_expr[*curr+3]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+2]=curr_expr[i];
					*length = *length + 2;
					curr_expr[*curr+0]=NOT; //NOT
					curr_expr[*curr+1]=COND; //cond
					curr_expr[*curr+2]=POPTREED;
					if (growing) depth++;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+5]=curr_expr[i];
					*length = *length + 5;
					curr_expr[*curr+0]=OSB; //[
					curr_expr[*curr+1]=VALUE; //Value
					curr_expr[*curr+2]=COMP; //comp
					curr_expr[*curr+3]=VALUE; //Value
					curr_expr[*curr+4]=CSB; //]
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
			}//of case cond
		case(BOOLOP):{ /* boolop */
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
					curr_expr[*curr+0]=AND; //AND
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
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
		case(DFVAL):{ /* DFVAL */
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
					curr_expr[*curr+0]=COMMAND; //command
					break;
					}
			}//of switch
			break;
			}//of case DFVAL
		case(DPVAL):{ /* DPVAL */
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
					curr_expr[*curr+0]=VALUE; //Value
					break;
					}
			}//of switch
			break;
			}//of case DPVAL
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
					curr_expr[*curr+0]=EQUAL; //=
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
					curr_expr[*curr+0]=GREATER; //>
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
		case(NOTUSED):{ /* NotUsed */
			divisor = 17;
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
					curr_expr[*curr+0]=NOPP; //NOPP
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(1):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=AMOUNT; //AMOUNT
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(2):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=JMP; //JMP
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(3):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=JMPIFTRUE; //JMPIFTRUE
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(4):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=JMPIFFALSE; //JMPIFFALSE
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(5):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=DUMMY; //DUMMY
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(6):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=XOR; //XOR
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(7):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=EXEC; //EXEC
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(8):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=CALL; //CALL
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(9):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=DUP; //DUP
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(10):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=POP; //POP
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(11):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PPR; //PPR
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(12):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=NORMALISELOOPCOUNT; //NORMALISELOOPCOUNT
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(13):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=AVALUE; //AVALUE
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(14):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PAR1; //PAR1
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(15):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PAR2; //PAR2
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
				case(16):{
					for(int i=*length-1;i>*curr;i--)
						curr_expr[i+1]=curr_expr[i];
					*length = *length + 1;
					curr_expr[*curr+0]=PVAL; //PVAL
					curr_expr[*curr+1]=POPTREE;
					if (ExtractingGrammarInfo) stack.push_back(codonPos);
					break;
					}
			}//of switch
			if (!terminating && ExtractingGrammarInfo)
				codons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));
			codonPos = codonPos + 1; // query if this should be incremented if terminating?
			break;
			}//of case NotUsed
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
					curr_expr[*curr+1]=OFB; //  *( = 7
					if (fPars > 0) {
						curr_expr[*curr+2]=FPARS; // FPARS = 1011
						for (int j=0;j<fPars-1;j++) {
							curr_expr[*curr+3+2*j]=COMMA; // , = 1
							curr_expr[*curr+4+2*j]=FPARS; // FPARS = 1011
						}
					}
					curr_expr[*curr+shift-1]=CFB; // )* = 8
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
			*curr = *curr + 1;
			}
		}//Of switch
	}
/***************************************************************/
void networkG_SensorNew::print_grammar_individual(ostream& output_stream,int curr_expr[],int length){
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
		case OCB: {output_stream << " { ";break;}
		case CCB: {output_stream << " } ";break;}
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
		case IF: {output_stream << " if ";break;}
		case THEN: {output_stream << " then ";break;}
		case ELSE: {output_stream << " else ";break;}
		case SETWEIGHT: {output_stream << " setweight ";break;}
		case WAIT: {output_stream << " wait ";break;}
		case END: {output_stream << " end ";break;}
		case _DOFUNC: {output_stream << " FUNC ";break;}
		case CVAL: { output_stream << " CVAL :"; i++ ; output_stream << curr_expr[i] << ": "; break;}
		case _DOPUSE: {output_stream << " PUSE ";break;}
		case PARAM: {output_stream << " PARAM ";break;}
		case ADD_SPLIT: {output_stream << " add_split ";break;}
		case ADD_SPLIT_CONT: {output_stream << " add_split_cont ";break;}
		case ATTACH: {output_stream << " attach ";break;}
		case ADD_REVERSE: {output_stream << " add_reverse ";break;}
		case CONNECT: {output_stream << " connect ";break;}
		case LOOP: {output_stream << " loop ";break;}
		case DESTINATIONTONEXT: {output_stream << " destinationToNext ";break;}
		case SOURCETONEXT: {output_stream << " sourceToNext ";break;}
		case SOURCETOPARENT: {output_stream << " sourceToParent ";break;}
		case SEQ: {output_stream << " Seq ";break;}
		case PAR: {output_stream << " Par ";break;}
		case SEQCOPYIN: {output_stream << " SeqCopyIn ";break;}
		case SEQCOPYOUT: {output_stream << " SeqCopyOut ";break;}
		case LSPLITNO: {output_stream << " LSPLITNo ";break;}
		case USPLITNO: {output_stream << " USPLITNo ";break;}
		case XSPLIT: {output_stream << " xSPLIT ";break;}
		case TSPLIT: {output_stream << " tSplit ";break;}
		case PUSH: {output_stream << " push ";break;}
		case PUSHR: {output_stream << " pushr ";break;}
		case SETSIGMA: {output_stream << " setSigma ";break;}
		case SETEXPA: {output_stream << " setExpa ";break;}
		case SETBIAS: {output_stream << " setBias ";break;}
		case CONNECTINPUT: {output_stream << " connectInput ";break;}
		case CONNECTOUTPUT: {output_stream << " connectOutput ";break;}
		case SPLITINPUTHORIZONTALLY: {output_stream << " splitInputHorizontally ";break;}
		case SPLITINPUTVERTICALLY: {output_stream << " splitInputVertically ";break;}
		case CONNECTHEIGHT: {output_stream << " connectHeight ";break;}
		case CONNECTAOA: {output_stream << " connectAOA ";break;}
		case NOT: {output_stream << " NOT ";break;}
		case AND: {output_stream << " AND ";break;}
		case OR: {output_stream << " OR ";break;}
		case NOPP: {output_stream << " NOPP ";break;}
		case AMOUNT: {output_stream << " AMOUNT ";break;}
		case JMP: {output_stream << " JMP ";break;}
		case JMPIFTRUE: {output_stream << " JMPIFTRUE ";break;}
		case JMPIFFALSE: {output_stream << " JMPIFFALSE ";break;}
		case DUMMY: {output_stream << " DUMMY ";break;}
		case XOR: {output_stream << " XOR ";break;}
		case EXEC: {output_stream << " EXEC ";break;}
		case CALL: {output_stream << " CALL ";break;}
		case DUP: {output_stream << " DUP ";break;}
		case POP: {output_stream << " POP ";break;}
		case PPR: {output_stream << " PPR ";break;}
		case NORMALISELOOPCOUNT: {output_stream << " NORMALISELOOPCOUNT ";break;}
		case AVALUE: {output_stream << " AVALUE ";break;}
		case PAR1: {output_stream << " PAR1 ";break;}
		case PAR2: {output_stream << " PAR2 ";break;}
		case PVAL: {output_stream << " PVAL ";break;}
		case POPTREE: {output_stream << " POPTREE ";break;}
		case POPTREED: {output_stream << " POPTREED ";break;}
		case _FUNC: {output_stream << " _FUNC ";break;}
		case _PUSE: {output_stream << " _PUSE ";break;}
		case __DOFUNC: {output_stream << " __DOFUNC ";break;}
		case __DOPUSE: {output_stream << " __DOPUSE ";break;}
		default: {
			if (curr_expr[i]>=400 && curr_expr[i]<500) {
				output_stream << "P" << curr_expr[i]-400;
			} else if (curr_expr[i]>=500 && curr_expr[i]<600) {
				output_stream << "F" << curr_expr[i]-500;
			} else {
				output_stream << " UNKNOWN(" << curr_expr[i] << ") ";
			}
			break;
			} // of default branch
		}
	output_stream << endl;
}
