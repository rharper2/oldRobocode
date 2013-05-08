


/****************************************************************************
* With gp crossover we assume (effectively) a single rule grammar
* There are however non-terminals and terminals
* we weight the probability of picking a non-terminal and a terminals.
* Once we know what we want we scan will we pick the first
* one and then scan the matching part of the codon until we find
* a crossover in the second parent
* - to simulate what Koza calls branch typing we need to assume the grammar
* has a different rule (or lhs) for each branch - 
* to use, say regression as an example the main (result producing branch) could
* expand a "value" whereas the function branch might expand an f-value
* - if we ensure that only matching non-terminals (and ditto for terminals) are
* swappeed we effectively have implemented branch typeing
* the comments in the body might make this clearer.
***************************************************************************/


void cr_data::gpReplacementOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
//    cout << "Into operator\n";
    if (!randomint(10)) {//i.e. a 10% chance of reproduction
        cr_data *c1 = new cr_data(*op1);
        cr_data *c2 = new cr_data(*op2);
        child1.reset(c1);
        child2.reset(c2);
        // Note you can't just use 
        // child1=op1;
        // child2=op2;
        // As this merely copies over the (shared) pointers pointing to a specific instance
        // of the parents - thus if, say, the children mutate so will the parents.
//        cout << "Out of operator (reproduction)\n";
        return;
    }
    if (!op1->isValid()) {
        cout << "WHOA op1 was not valid\n";
        exit(1);
    }
    if (!op2->isValid()) {
        cout << "WHOA op2 was not valid\n";
        exit(1);
    }
    int thisInvalidCount;
    thisInvalidCount = 0;
    entity *pge0,*pge1;
    long *codons0,*codons1;
    pge0 = op1->theEntity;
    pge1 = op2->theEntity;
    if (pge0->getUsefulDna() > pge0->getDnaLength()) cout << "Well here is a problem\n";
    int len1 = pge0->getUsefulDna()/CODON;
    int len2 = pge1->getUsefulDna()/CODON; 
    //Note here limited to Useful dna as this is the only part which has codons for the match up
    if (len1 <= 2 || len2 <=2) {
        cout << "Error with minimum lenghts, len1 " << len1 << " and lend2 " << len2<< endl;
        exit(1);
        return; // quick fix to stop crashing     
    } 
    // Some initial sanity checks, if I were to optimise aggressively then I could look at this stuff.
    if (len1 > 32000) {
        cout << "Length of dna divided by " << CODON << " is greater than 32000 ( " << len1 << ") this is going to cause an overflow on randomint, so chopping down\n";
        len1 = 32000;
    }
    if (len2 > 32000) {
        cout << "Length of dna divided by " << CODON << " is greater than 32000 ( " << len2 << ") this is going to cause an overflow on randomint, so chopping down\n";
        len2 = 32000;
    }
    
    const int *pdna1 = pge0->getDna();
    const int *pdna2 = pge1->getDna();
    codons0 = pge0->getCodons();
    codons1 = pge1->getCodons();
		  
    grammarBase *gb = op1->grammar;
    /* 
        // I dont think we need this as the generic code I have written to see if something is a terminal/non-terminal should work in most
        // circumstances.
     
     if (!gb->isGPEnabled()) {
         cout << "The grammar you are using does not appear to be gp crossover compatible, it needs to return true when isGPEnabled is invoked\n";
         exit(1);
     }
     */   
    // Originally I decided where to make the crossover by selecting a random place in the genome and scanning up or down 
    // until I found a non-terminal or terminal. However in the regression grammar (which is the one this was tested on)
    // a number takes four codons (or now with the encoded values two codons)  to express fully whereas a "use x" only takes one, this meant that a number
    // was four times more likely to be chosen when we were choosing non-terminals - this was a bias that
    // is, likely, unhelpful and certainly did not exist in the koza implementation.
    // Even where we are selecting a non-terminal it leads to a bias. An opeartor (say +) with two X operands is a lot less likely to be
    // selected than one with two number operands.
    
    // The final bias which needed to be eliminated was the fact that if you choose and scan, branches lower in the tree were disproportionately 
    // more likely to be selected than those higher up
    // Imagine you have a parse of ivalue-> (ivalue * ivalue) -> ( (ivalue + ivalue) op ivalue) -> ( (2+ 3) * 4)
    // the codons are thus ivalue (selecting i * i) ivalue (selecting i + i) ivalue (selecting 2) ivalue (selecting 3) ivalue (selecting 4)
    // If we chose at random and scanned till we got a non-terminal
    // we would only get the root if we started in the first position (the ivalue * ivalue) but if we start in any of the other 
    // 4 positions we get the lower down tree (the ivalue + ivalue one) 
    
    // So now the (probably inefficient, but this is just a testing operator) is to scan the genome counting and noting
    // the non-terminals and terminals and put their location in a vector. and then just chose one of the positions from the vector.
    
    
    // *** THOUGHT OF THE MOMENT - THIS METHOD SELECTS WHICH BRANCH DEPENDING ON THE NUMBER OF 
    // NONTERMINALS OR TERMINALS (AS APPROPRIATE) IN THE BRANCHES
    // KOZA GP MIGHT SELECT DEPENDING ON (TERMINALS+NON-TERMINALS) IN EACH OF THE BRANCHES
    // NO MATTER WHETHER A TERMINAL OR NON-TERMINAL WILL BE SELECTED
    // UNLIKELY TO MAKE A DIFFERENCE AS MOST FUNCTIONS = TWO ARGUMENTS BUT NOT
    // TRUE FOR ADF CALLS (TREATED AS NONTERMINAL) OR IN SOME, SAY, THREE BRANCH
    // FUNCTIONS LIKE IF THEN ELSE
    
    vector<int> terminalPos;
    vector<int> nonTerminalPos;
    terminalPos.clear();
    nonTerminalPos.clear();
    int c_slice1,c_slice2;
    // I used to delegate to the grammar to decide if it was a non-terminal or nt
    // But the problem is the grammar can decide from just looking at the LHS of the expansion the 
    // codon could be. Using regression as an example value could expand to (value + value) [nonTerminal]
    // or just straight X [terminal], looking at value in isolation doesn't pass muster.
    // So the current idea is to compare the size of the expansion with a user paramater (contained in grammar so we 
    // can change it with the grammar? - Good idea not done yet). If it expands then its a nonTerminal
    // What we then have to deal with is something like CVAL where CVAL uses the next codon (or codons) as absolute values
    // for say numerical constants. The way round this is not to count (as an expansion) anything which has a zero LHS (i.e. no
    // matching grammar expansion.) We still dont deal with expansion like number-> digit digit digit where number would 
    // be treated as a non-terminal, when it is really a terminal but its not an issue just now.
    
    // Actually I will push it back to the grammars, but what we pass them is the ptr in the codons to make the assessment from not
    // just the grammar expansion.
    // And I will write a general purpose check in grammarBase rather than just a stub.
    
    // We need to check both isNonTerminal and isTerminal separately as theoretically something might be neither (but not both?-added else).
    for (int i=0;i<len1;i++) {
        if (gb->isNonTerminal(&codons0[i])) nonTerminalPos.push_back(i);
        else if (gb->isTerminal(&codons0[i])) terminalPos.push_back(i);
    }
    // if only we had had the foresight to make codons a vector we could have used an iterator!
    int numberOfTerminals = terminalPos.size();
    int numberOfNonTerminals = nonTerminalPos.size();
    // I think this is better than keeping a count in the loop?
    
    //        int totalPositions = noTerminals+noNonTerminals;
    bool findNonTerminal;
    if (randomint(10)) { 
        // we want to find a non-terminal
        findNonTerminal = true;
        if (numberOfNonTerminals ==0) {
            return; // just abandon attempt we have a leaf parent.
            cout << "ERROR NO NON TERMINALS\n";
            cout << op1->display();
            exit(1);
        }
        c_slice1 = nonTerminalPos[randomint(numberOfNonTerminals)];
    } else {
        // 10% chance of wanting to find a terminal
        findNonTerminal = false;
        c_slice1 = terminalPos[randomint(numberOfTerminals)];
    }
    if ((c_slice1 < 0) || c_slice1> len1-1) {
        cout << "Error in gpcrossover finding first slice, got a " << c_slice1 << " where len1 is " << len1 << endl;
        exit(1);
    }
    int matched = (codons0[c_slice1] >> CODON) & LHSMASK; 
    long treeSize  = ((codons0[c_slice1] >> (CODON + LHSINFO)) & EXPRMASK);
    bool rootNode = gb->isRoot(codons0,c_slice1,len1);
   
    //cout << op1->display();
   
    //cout << "Story so far, is that we have selected " << c_slice1 << " as the crossover point. ";
    //if (rootNode) cout << ", it IS a root node "; else cout << ", it is NOT a root node. ";
    //if (findNonTerminal) cout << "It should be a non-terminal\n"; else cout << "It should be a terminal.\n";
    
    // Find NonTerminal is now set for the second crossover point. So we don't
    // Need to populate both vectors. (Side note subject to the blurb in 
    // block caps near c_slice1 this is probably true for the first 
    // crossover point.
    bool secondFindNonTerminal = true;
   // cout << op2->display();
    if (rootNode) secondFindNonTerminal = true;
    else secondFindNonTerminal = (randomint(10) != 0);
    vector<int> secondNTPos,secondTPos;
    secondNTPos.clear();
    secondTPos.clear();
    for (int i=0;i<len2;i++) {
        if (((codons1[i] >> CODON ) & LHSMASK) == matched) {
            if (secondFindNonTerminal) {
                if (!findNonTerminal) { // i.e. we have a terminal on the otherside
                                        // only push onto vector of possible matches if its not the root.
                                        // Query how this works with fair crossover, just now allowing a terminal to replace (and be replaced any size).
                                        // Only 1 in 10 or so fall down this route anyway.
                    if ( (gb->isNonTerminal(&codons1[i])) && !gb->isRoot(codons1,i,len2))  secondNTPos.push_back(i);
                } else { // i.e. not a terminal on the otherside
                    if (gb->isNonTerminal(&codons1[i])) {
                          long secondLength = ((codons1[i] >> (CODON + LHSINFO)) & EXPRMASK);
                          // before we push it on the vector as a possible contender in the crossover sites, ensure its length is
                          // within the relevant paramaters for a "fair replacement".
                          if ( (secondLength > firstLength * minMatchingTree) && (secondLength < firstLength * MaxMatchingTree) ) {
                            secondNTPos.push_back(i);
                          }
                    }
                }
            }
            else { 
                if (gb->isTerminal(&codons1[i])) secondTPos.push_back(i);
            }
        }
    } 
    // Still need to deal with first = Terminal and second = NT and only NT is root node (and therefore the secondNTPos is empty)
    // do this by deciding to select a terminal from the second one.
    if (!findNonTerminal && secondFindNonTerminal && secondNTPos.empty()) {
//        cout << "========================================================================\n";
//        cout << "Checking second for a terminal, having failed to find a non-root non-terminal, for type " << matched << "\n";
//        cout << op2->display();
//        cout << "========================================================================\n";
        secondFindNonTerminal = false;
        for (int i=0;i<len2;i++) {
            if ((((codons1[i] >> CODON) & LHSMASK) == matched) && gb->isTerminal(&codons1[i])) {
                secondTPos.push_back(i);
            }
        }
    }
    if (secondFindNonTerminal) {
        int size = secondNTPos.size();
        if (size ==0) {
            return; // just abandon attemp we have a leaf entity.
            cout << "Error in find non terminal for c_slice2, the size was zero\n";
            cout << "Matched was " << matched << endl;
            cout << pge0->display(gb);
            cout << pge1->display(gb);
            cout << "Cslice1 was " << c_slice1<< endl;
            exit(1);
        }
        int pos = randomint(size);
        c_slice2 = secondNTPos[pos];
    } else {
        int size = secondTPos.size();
        if (size ==0) {
            cout << "Matched was " << matched << endl;
            cout << pge0->display(gb);
            cout << pge1->display(gb);
            cout << "Cslice1 was " << c_slice1<< endl;
            cout << "Error in find terminal for c_slice2, the size was zero\n";
            exit(1);
        }
        int pos = randomint(size);
        c_slice2 = secondTPos[pos];
    }
    
    int secondMatched = codons1[c_slice2] >> CODON & LHSMASK; 
    if (matched != secondMatched) {
        cout << "Matched was " << matched << "secondMatched was " << secondMatched << " they need to be the same!\n";
        cout << pge0->display(gb);
        cout << pge1->display(gb);
        //     cout << "Cslice started as " << c_slice2 << "low is " << low << " high is " << high << " len2 is " << len2 << endl;
        exit(1);
    }
    
    // Well at the end of that we have c_slice1 and c_slice2 pointing to the to bits we want to swap.
    long p1,p2,p3;
  //  cout << "c_slice1 is " << c_slice1 << " c_slice2 is " <<c_slice2 << " matched is " << matched << endl;
    
    
    if (c_slice2 >= 0) {
        /*
        if (findNonTerminal) {
            cout << "The first one is a non Terminal. ";
            if (rootNode) cout << "It is a root node.\n"; else cout << "It is not a root node.\n";
        } else cout << "The first one is a terminal.\n";
        if (secondFindNonTerminal) {
            cout << "The second one is a non Terminal. ";
            if (gb->isRoot(codons1,c_slice2,len2)) cout << "It is a root node.\n"; else cout << "It is not a root node.\n";
        } else cout << "The second  one is a terminal.\n";
        */
        int endOfExp1 = ((codons0[c_slice1] >> (CODON + LHSINFO)) & EXPRMASK) + c_slice1;
        int endOfExp2 = ((codons1[c_slice2] >> (CODON + LHSINFO)) & EXPRMASK) + c_slice2;
        //        cout << "EndofExp1 = " << endOfExp1 << "End of Exp2 " << endOfExp2 << endl;
        long part1a = c_slice1 * CODON;
        long part1b = (endOfExp2 - c_slice2) * CODON; // the length of the expression to be inserted
        long part1c;
        long part1cAlt;
        if (fullDnaToBeUsed) { 
            
            // This is still necessary for the best results even with the replacement crossover.
            // Just now I have two theories:
            //   (A) It allows an individual to survive certain mutations which ripple out the grammar
            //   (B) It allows crossovers with functions (currently) which cause a ripple - NOTE my next
            //         bit of work is to implement functions in a more information preserving manner.
            
            part1c = pge0->getDnaLength() - endOfExp1 * CODON;
            part1cAlt = pge0->getUsefulDna() - endOfExp1 * CODON;
            if (part1c < 0) cout << "Error we have a negative part1c\n";
            if (part1cAlt < 0) cout << "Error we have a negative part1cAlt\n";
        } else { 
            part1c = pge0->getUsefulDna() - endOfExp1 * CODON;
            part1cAlt = pge0->getUsefulDna() - endOfExp1 * CODON;
            if (part1c < 0) cout << "Error we have a negative part1c\n";
        } 
        bool bad = false;
        if (part1a < 0) {
            cout << "Error with part1a it's less than 0\n";
            bad = true;
        }
        if (part1b < 0) {
            cout << "Error with par1b it's less than 0\n";
            bad = true;
        }
        if (part1c < 0) { 
            cout << "Error with part1c it's less than 0\n";
            bad = true;
        }
        if (endOfExp1 > pge0->getDnaLength()) {
            cout << "End of Expression malformed in the first parent\n";
            bad = true;
        }
        p1=part1a;
        p2=part1b;
        p3=part1c;
        long newDnaLength = part1a + part1b + part1c;
        if (newDnaLength > INVALIDTHRESHOLD) { // we can rebuild it (perhaps!)
                                               // the idea here is part1cAlt contains the usefulDNA length
                                               // if the unused tail is making it too long slice it down to size.
            long excessToTrim = newDnaLength - INVALIDTHRESHOLD;
            // I have a slight disquiet that this method might make the useful DNA creep up too 
            // high and also that all the dna will quickly become sized at invalid threshold
            // need to keep an eye on this.
            if ((part1c - part1cAlt) > excessToTrim) { // ok so we have the room to trim it.
                newDnaLength -= excessToTrim; // so we dont need to copy all of the tail over.
                part1c -= excessToTrim;
            }
        }
        long part2a = c_slice2*CODON;
        long part2b = (endOfExp1 - c_slice1) * CODON;
        long part2c;
        long part2cAlt;
        if (fullDnaToBeUsed) { 
            part2c = pge1->getDnaLength() - endOfExp2 * CODON;
            part2cAlt = pge1->getUsefulDna() - endOfExp2 * CODON;
            if (part2cAlt < 0) cout << "Error we have a negative part1cAlt\n";
            if (part2c < 0) cout << "Error we have a negative part2c\n";
        } else { 
            part2c = pge1->getUsefulDna() - endOfExp2 * CODON;
            part2cAlt = pge1->getUsefulDna() - endOfExp2 * CODON;
            if (part2cAlt < 0) cout << "Error we have a negative part1cAlt\n";
            if (part2c < 0) cout << "Error we have a negative part2c\n";
        } 
        if (part2a < 0) {
            cout << "Error with part2a it's less than 0\n";
            bad = true;
        }
        if (part2b < 0) {
            cout << "Error with par2b it's less than 0\n";
            bad = true;
        }
        if (part2c < 0) { 
            cout << "Error with part2c it's less than 0\n";
            bad = true;
        }
        if (endOfExp2 > pge1->getDnaLength()) {
            cout << "End of Expression malformed in the second parent\n";
            bad = true;
        }
        if (bad) {
            cout << op1->display();
            cout << op2->display();
            op2->reEvaluate();
            op2->get_fitness();
            bad = true;
            exit(2);
        }
        
        long newDnaLength2 = part2a + part2b + part2c;
        if (newDnaLength2 > INVALIDTHRESHOLD) { // we can rebuild it (perhaps!)
                                                // the idea here is part1cAlt contains the usefulDNA length
                                                // if the unused tail is making it too long slice it down to size.
            long excessToTrim = newDnaLength2 - INVALIDTHRESHOLD;
            // I have a slight disquiet that this method might make the useful DNA creep up too 
            // high and also that all the dna will quickly become sized at invalid threshold
            // need to keep an eye on this.
            if ((part2c - part2cAlt) > excessToTrim) { // ok so we have the room to trim it.
                newDnaLength2 -= excessToTrim; // so we dont need to copy all of the tail over.
                part2c -= excessToTrim;
            }
        }
        
        // the following will probably help
        // Parent 1: <--part1a--><--part2b---><---part1c--->
        // which is  < c_slice1-><- lofexpr----><--rest---------->
        // and note < end of exp 1---------------->
        
        // Parent 2: <--part2a--><---part1b---><--part2c--->
        // Child1:   <-part1a--><--part1b---><---part1c--->
        // Child2:   <--part2a--><---part2b----><part2c-->
        //
        
        if (newDnaLength <= INVALIDTHRESHOLD) 
        {
            
            int newDna[newDnaLength];
            
            for (int i=0;i<part1a;i++) 
                newDna[i]=pdna1[i]; // first part of child1 from dna1
            for (int i=0;i<part1b;i++) 
                newDna[part1a+i]=pdna2[part2a+i]; // second part from dna2
            for (int i=0;i<part1c;i++)
                newDna[part1a+part1b+i]= pdna1[part1a+part2b+i]; // and third from dna1
            child1.reset(new cr_data(newDna,newDnaLength));
        } else {
            cout << "Failed to build child1 within the validity threshhold - odd!\n";
            cout << "Len1 was " << len1 << "(" << len1*CODON << ") " << "First slice:" << c_slice1 << "( " << c_slice1*CODON << ") " << part2b << " was replaced with a segment " << part1b << " and a min tail of " << part1cAlt << " for a min total of " << (part1a+part1b+part1cAlt) << endl;
        }
        if  (newDnaLength2 <= INVALIDTHRESHOLD) {
            int newDna2[newDnaLength2];
            for (int i=0;i<part2a;i++) 
                newDna2[i]=pdna2[i]; // first part of child2 from dna2
            for (int i=0;i<part2b;i++) 
                newDna2[part2a+i]=pdna1[part1a+i]; // second part from dna1
            for (int i=0;i<part2c;i++)
                newDna2[part2a+part2b+i]= pdna2[part2a+part1b+i]; // and third from dna2
            
            child2.reset(new cr_data(newDna2,newDnaLength2));
        } else {
            cout << "Failed to build child2 within the validity threshhold - odd!\n";
            cout << "Len2 was " << len2 << "(" << len2*CODON << ") " << "First slice:" << c_slice2 << "( " << c_slice2*CODON << ") " << part1b << " was replaced with a segment " << part2b << " and a min tail of " << part2cAlt << " for a min total of " << (part2a+part2b+part2cAlt) << endl;

        }
    } else {
        cout << "c_slice2 was " << c_slice2 << " which is not valid.\n";
    }
    if (child1 && child2) {
        //       cout << "Following: " << endl;
        //       cout << pge0->display(gb);
        //       cout << pge1->display(gb);
        //       cout << "C-slice 1 " << c_slice1 << "c_slice2 " << c_slice2 << endl;
        if (!child1->isValid()) {
            cout << "Error not valid: " << gb->getInvalidReason() << "\n";
            cout << "Following: " << endl;
            cout << pge0->display(gb);
            cout << pge1->display(gb);
            cout << "C-slice 1 " << c_slice1 << "c_slice2 " << c_slice2 << endl;
            cout << "Part1a/codon " << p1/CODON << " Part1b/codon " << p2/CODON <<  " Part1c/codon " << p3/CODON << endl;
            cout << child1->theEntity->display(gb);
            exit(1);
            
        }
        if (!child2->isValid()) {
            cout << "Error child2 not valid:"<< gb->getInvalidReason() << "\n";
            exit(1);
        }
        //      cout << child1->theEntity->display(gb);
        //      cout << child2->theEntity->display(gb);
    } else cout << "Hmm didnt create the children\n";
    /*if (child1) cout << "Child one created\n";
    if (child1->isValid()) cout << "Child one valid.\n";
    if (child2) cout << "Child two created.\n";
    if (child2->isValid()) cout << "Child two is valid.\n";
  */ 
  //cout << "Out of operator.\n";
    
    return ;
}
