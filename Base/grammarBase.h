/*
 *  grammarBase.h
 *  Serves as a base grammar for passing into entities.
 *
 *  Created by Robin Harper on 16/10/04.
 *  Copyright (c) 2004 robinh@cse.unsw.edu.au. All rights reserved.
 *
 */

#ifndef grammarBase_t
#define grammarBase_t

#include <iostream>
#include <vector>
#include "functions.h"

using namespace std;

class grammarBase{
public:
//        virtual static grammarBase* Instance() =0;// Can't have "virtual" statics so comment left for template purposes.
          virtual bool parse(int *exp,int *length,int maxExp,long codons_list[],int max_codons,int &used,vector<GEfunction> *funcVec)=0;
		  // if doACreate is not implemented, just parses normally
          virtual bool doACreate(int *exp,int *length,int maxExp,long codons_list[],int max_codons,int &used,vector<GEfunction> *funcVec,int,int,bool) 
            { return parse(exp,length,maxExp,codons_list,max_codons,used,funcVec); }
		  // if ptc2Grow is not implemented, just parses normally
          virtual bool ptc2Grow(int *exp, int *length, int maxExp, long codon_list[],
							int max_codons, int &cUsed, vector<GEfunction> *funcVec,int maxD,int req_expansions)
            { return parse(exp,length,maxExp,codon_list,max_codons,cUsed,funcVec); }
          virtual void print_grammar_individual(ostream& output_stream,int expr[],int length)=0;
          virtual void print_a_grammar(ostream& output_stream,int g) { };
          virtual ~grammarBase() {};
          virtual  void AllowGEfunctions(bool) {}; //empty as opposed to pure virtual so they dont have to be overriden
          virtual void AllowWrap(bool,int) {};
          virtual void AllowTooBigGracefulTermination(bool) {};
          virtual void AllowRunOutGracefulTermination(bool) {};
          virtual bool haveGrammarInfo() { return false; }
          virtual const string getInvalidReason() { return "No reason"; }
          virtual int getWraps() { return -1; }
          
          // stub GEfunctions if the crossover operator needs to query about
          // the type of "thing" it is looking at
          // note its not necessarily the case isTerminal = not (isNonTerminal).
          virtual bool isTerminal(long *codons) {
                // A zero in the grammar section (LHS) means that it is a value holding codon not a grammar codon
                if (! ((codons[0] >> CODON) & LHSMASK)) return false;
                int length = (codons[0] >> (CODON + LHSINFO)) & EXPRMASK; // Get the length of the expansion
                int actualLength = length;
                for (int i=0;i<length;i++) {
                    if (! ((codons[i] >> CODON) & LHSMASK)) 
                            actualLength--;
                }
                if (actualLength < 1) {
                    int type = ((codons[0] >> CODON) & LHSMASK);
                    cout << endl << "Problem with isTerminal, actualLength is " << actualLength << " from a codon with expansion type " << type << endl;
                    cout << codons[0] << ", " << codons[1] << endl;
 //                   exit(1);
                }
                if (actualLength > 1) return false;
                else return true;
          }
          virtual bool isNonTerminal(long *codons) { 
                // A zero in the grammar section (LHS) means that it is a value holding codon not a grammar codon
                if (! ((codons[0] >> CODON) & LHSMASK)) return false;
                int length = (codons[0] >> (CODON + LHSINFO)) & EXPRMASK; // Get the length of the expansion
                int actualLength = length;
//                cout << "Length: " << length;
                for (int i=0;i<length;i++) {
                    if (! ((codons[i] >> CODON) & LHSMASK)) actualLength--;
                }
                if (actualLength < 1) {
                    int type = ((codons[0] >> CODON) & LHSMASK);
                    cout << endl << "Problem with isNonTerminal, actualLength is " << actualLength << " from a codon with expansion type " << type << endl;
                   cout << codons[0] << ", " << codons[1] << endl;
//                     exit(1);
                }
  //              cout << "Actual length " << actualLength << endl;
                if (actualLength > 1) return true;
                else return false;
          }
          virtual bool isRoot(long *codons,int pos,int maxPos) {
            // Default methodology here is to start at the beginning of the codon chain
            // it is assumed that the root of each branch spans from position 0 to the start of the next branch
            // therefore check if the pos appears in the links from position 0, to the one after its fully expanded
            // to the one after thats fully expanded until we hit the end.
            int currPos = 0;
//            cout << "Checking out root currPos is " << currPos << endl;
            while (currPos < maxPos) {
                if (pos == currPos) {
//                    cout << "Matched pos of " << pos << " and current Post of " << currPos << " it is a root\n";
                    return true;
                } else {
                    int length = (codons[currPos] >> (CODON +LHSINFO)) & EXPRMASK;
//                    cout << "Codon at that position has a length of " << length;
                    currPos += length;
 //                   cout << ". Advancing currPos to " << currPos << endl;
                }
            }
            return false;
        }
protected:
          grammarBase() {}; //Constructor
private:
};

#endif

