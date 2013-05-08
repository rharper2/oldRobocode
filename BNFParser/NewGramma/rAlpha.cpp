//*************************************************************************
//                                                                       
//    FILE : rAlpha.cpp                                    
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
#include "rAlpha.h" 
#include "myrandom.h"
/***************************************************************/
// File generated from grammar included in file:
// rAlpha.bnf
/***************************************************************/
// code:	mainSentence
// 
// mainSentence:	if ( mBool ) { mainSentences } else { mainSentences }
// 		| mainAction
// %%
/***************************************************************/
