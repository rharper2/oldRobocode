


/****************************************************************************
* The Replacement Operator  is the conceptually the last operator to implement crossover.   (18/11/04)
* In fact following some suggestions for the paper I am writing (31/1/05) there are numberous
* other similar two point operators and random operators - none of which are much good, but are
* there for comparison purposes.
*
* The Replacement Operator described below is the most productive so far. (1/2/05)
*
* There was grammar matching operator implemented first I keep the comments here
* to explain the problem and then discuss how its solved.
*
* The idea behind the "matching operator was that the grammar now has a long codon to play with
* - to recap the first 8 bits represent the dna
* - the next 8 bits represent the non terminal which that codon was used to expand
*   e.g. the rule line-> A|B|C where that codon was used to determine whether A, B or C was 
*   selected
* - the last 16 bits now store the codon position (absolute) where that expansion finished
*
* The point now is that like the matching operator we can match the change to similar non-terminals
* eg line, but we can now implement a more tree type insertion by deleteing the previous expansion
* by length and inserting the "to be substituted version.
*
* the reason for implementing this tree type crossover, was the enormous benefit
* (although with the resultant dna bloat problems) which were achived by
* matching the non-terminal which that codon decides in each of the parents.
* so if we are deciding which type of LINE to expand, then the codon
* will have the code (-1000) for LINE in the higher order bits.
* we can therefore make sure we only swap a branch of LINE for 
* another branch of LINE.
* However, it was  noted that this is not the same as tree node swapping
* the reason is this:
* assume you had a grammar which included LINES -> LINE | LINE LINES
* and something like LINE -> X|Y|Z, in a tree swap lets assume that the first
* LINES expanded to LINE LINES. Then if we chose to do the crossover on the first LINE
* the tree node  swap would leave the second expansion of LINES untouched.
*     Lines         LINES
*     /    \         \
*   LINE   LINES        LINE
*   /        \          \
*  X          LINE         Z
*                 \
*                   Y
* it would leave all the expansion of LINES in the second line untouched.
*
* HERE however, the whole dna is linear. Although the LINE would be replaced
* and turned from X into Z, the expanion of the second LINES will be according to the second
* parents DNA NOT the first (as in the tree node expansion)
* But in our example - what is that DNA? since it was not part of the valid
* expression of parent 2 - well if we chop it off at usefulDnaLength it will
* invalid - unless we have enabled one of our terminating strategies
* but if we allow the second dna to go up to dnaLength (not usefulDnaLength)
* then we will have - well a whole new ballgame.
*
* Not that constructive a crossover operator!
*
* to implement a more "parse tree" like operator I  needed to 
* store with the codon the "length" of that expansion
* the grammar contains details on how I did this.
* for this comment what is important to realise is that now codon has several values stored in it.
* the array of codons is a minimum of 32 bits of info now.
* the first CODON bits contain the codon use & CODONMASK to get them.
* the next LHSINFO bits contins the Non-terminal associated with that codon.
* thin the next EXPRLENGTH bits contain the poition in that codon array where
* the expansion of this non-terminal ends - its not a relative position its an 
* absolute position.
* The code shows how to bit shift and mask to get all the info out.
* Note CODON LHSINFO EXPLENGTH and the corresponding masks are all
* defined in params.h
***************************************************************************/


void cr_data::twoPointReplacementOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
    
    if (!op1->isValid()) {
            cout << "Parent 1 not valid!\n";
            exit(1);
    }
    if (!op2->isValid()) {
            cout << "Parent 2 not valid\n";
            exit(1);
    }
    entity *pge0,*pge1;
    long *codons0,*codons1;
    pge0 = op1->theEntity;
    pge1 = op2->theEntity;
    child1.reset();
    child2.reset();
    if (pge0->getUsefulDna() > pge0->getDnaLength()) cout << "Well here is a problem\n";
    
    int len1 = pge0->getUsefulDna()/CODON;
    int len2 = pge1->getUsefulDna()/CODON; 
    
    //Note here limited to Useful dna as this is the only part which has codons for the match up
    
    if (len1 <= 2 || len2 <=2) return; // quick fix to stop crashing     
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
    
    
    int c_slice1;
    int expr_to_match;
    //cout << "two point in.\n";
    do {
        c_slice1 = (randomint(len1-1)+1); // so generate a number from 1..len1-1 inclusive
        if (c_slice1 < 1) {
            cout << "ERROR slice1 is " << c_slice1 << ", must have non-sensible dna lengths (overflow) \n";
            exit(1);
        }
        expr_to_match = ((codons0[c_slice1] >> CODON) & LHSMASK); // take the top bits
        if (expr_to_match == 0 && len1 <= 2) {
			cout << "Refusing to process a leaft\n";
			return;
	} 
    } while (expr_to_match ==0);
    //cout << "two point out.\n";
    bool leafcrossover = true; 
    if ((randomint(100) < chanceOfTreeCrossover)) {
	 leafcrossover = false;
 //       cout << "Not allowing parent 1 to choose a leaf\n";
        if ( (((codons0[c_slice1] >> (CODON + LHSINFO)) & EXPRMASK) - c_slice1) < minSizeofNonLeaf) {
            
            // if we are to find a non-leaf node and the one we have chosen doesnt use as many codons to expand
            // as the minSizeOfNonLeaf (i.e. is a leaf) then scan up and down until we find one that is.
            // remember sizes are absolut so the length of the node is endCodon(extracted from grammar) - position of this codon
//            cout << "Rejected codon " << c_slice1 << " in parent1 - too small\n";
            int low = c_slice1;
            int high = c_slice1;
            bool done = false;
            bool low_limit_reached = false;
            bool high_limit_reached = false;
            
            int found = -1;
            
            while ( (found < 0) && !done) {
                if ( ((codons0[low] >> (CODON+LHSINFO)) & EXPRMASK) - low >= minSizeofNonLeaf) 
                {
                    // okay that is: check that it is not true that ( we are using protection and its protected)
                    // and then check that we have a match
                    found = low;
                    done = true;
                } else if ( ((codons0[high] >>  (CODON+LHSINFO)) & EXPRMASK) - high >= minSizeofNonLeaf) 
                {
                    found = high;
                    done = true;
                } else {
 //                   cout << "rejected both " << low << " and " << high << " as being too small\n";
                    if (low <=0) {
                        if (high_limit_reached) done = true;
                        else low_limit_reached = true;
                        low = 0;
                    } else low--;
                    if (high >=len1-1) {
                        if (low_limit_reached) done = true;
                        else high_limit_reached = true;
                        high = len1 -1;
                    } else high++;
                }
            } 
//            if (found > 0) cout << "Parent 1 accepted " << found << " as being suitable\n";
 //           else cout << "No suitable candidate found\n";
            c_slice1 = found;
        }
    }
    
    
    if (c_slice1 < 1) {
//        cout << "Failed to find a non-leaf node\n";
        return;
    }
    expr_to_match = ((codons0[c_slice1] >> CODON) & LHSMASK); // update expr_to_match in case c_slice slid.
    int sizeOfP1 = (codons0[c_slice1] >> (CODON + LHSINFO)) & EXPRMASK - c_slice1;
    
    // Okay a zero in the grammar indicates that its a place we can't crossover so we keep picking cuts 
    // until we get something thats ok.
    // so far (26/7/06) the only zero value is where we have a codon value i.e. a raw value that the codon want
    // CVAL in the current implementation.
    // We don't scan up or down as that might bias the search if we have chunks of zero value we would be more likely
    // to pick the grammar rule that leads to zero value.
    
    
    int c_slice2 = (randomint(len2-2)+1); // so generate a number from 1..len2-1 
    if (c_slice2 < 1) {
        cout << "ERROR slice2 is " << c_slice2 << ", must have non-sensible dna lengths (overflow) \n";
        exit(1);
    }
    
    // Note There needs to be a better way to do this - thinking about just now allowing the leaf/non-leaf
    // Distinction - problem I have is the ways numbers are done.
    
    // HISTORIC
    // Just now a number takes up 3 Codons so the chance of hitting a DIGIT in a x-over point is 
    // dispropotionately high.
    // SOLUTION - I have implemnted the idea of storing codon values - similar (broadly) in concept
    // to a paper [QUOTE] at WCCI2006 -Introducing Grammar Based Extension for GE Nicolau/Dempsey
    
    // With respect to leaf/non-leaf now (with CVALS) we could look at just the size of the expansion
    // And implement a selection >= 4 ? And then a fair crossover +/- 50% of size?
    
    // Search from the chosen point, higher and lower to find a match
    
    int matched;
    int low = c_slice2;
    int high = c_slice2;
    bool done = false;
    bool low_limit_reached = false;
    bool high_limit_reached = false;
    
    int found = -1;
    int maxTree = (int) (((double) sizeOfP1)*maxMatchingTree);
    int minTree = (int) (((double) sizeOfP1)*minMatchingTree);
    if (!leafcrossover && minTree<=3) minTree = 3;
    while ( (found < 0) && !done) {
        if ( ((codons1[low] >> CODON) & LHSMASK) == expr_to_match) 
        {
            if (maxMatchingTree == 0 || 
                ( ( ( (codons1[low] >> (CODON + LHSINFO)) & EXPRMASK) - low) <= maxTree)) {
                
                if (minMatchingTree == 0 || 
                    ( ( ( (codons1[low] >> (CODON + LHSINFO)) & EXPRMASK) - low) >= minTree)) {
                    
                    matched = codons1[low] >> CODON & LHSMASK;
                    found = low;
                    done = true;
//                    cout << found << " Accepted as being suitable in parent2\n";
                } else {
//                    cout << low << " rejected as being too small\n";
                }
            } else {
//                cout << low << " rejected as being too big\n";
            }
        }
        
        if (!done &&  ((codons1[high] >> CODON) & LHSMASK) == expr_to_match) 
        {
            if (maxMatchingTree == 0 || 
                ( ( ( (codons1[high] >> (CODON + LHSINFO)) & EXPRMASK) - high) <= maxTree)) {
                
                if (minMatchingTree == 0 || 
                    ( ( ( (codons1[high] >> (CODON + LHSINFO)) & EXPRMASK) - high) >= minTree)) {
                    
                    matched = codons1[high] >> CODON & LHSMASK;
                    found = high;
                    done = true;
 //                   cout << found << " Accepted as being suitable in parent2\n";
                } else {
//                    cout << high << " rejected as bing too small\n";
                }
            } else {
//                cout << high << " rejected as being too big\n";
            }
        }
        
        if (!done)  {
            if (low <=0) {
                if (high_limit_reached) done = true;
                else low_limit_reached = true;
                low = 0;
            } else low--;
            if (high >=len2-1) {
                if (low_limit_reached) done = true;
                else high_limit_reached = true;
                high = len2 -1;
            } else high++;
        }
    } 
    
    // To Recap:
    // c_slice1 is where we are making our current slice of the DNA in the first parent
    // found is where we have found a matching slice in the second parent (assuming its zero or greater).
    
    
    if (found > 0) {
        c_slice2 = found;
        int endOfExp1 = ((codons0[c_slice1] >> (CODON + LHSINFO)) & EXPRMASK);
        int endOfExp2 = ((codons1[found] >> (CODON + LHSINFO)) & EXPRMASK);
        
        // So far we have been mainly dealing with codons
        // But we need to cut and dice the bits
        // So we use part1a,part1b,part1c to represent the positions of the bits we need to swap.
        
        long part1a = c_slice1 * CODON;
        long part1b = (endOfExp2 - c_slice2) * CODON; // the length of the expression to be inserted
        long part1c;
        // it might be that part1c e.g. the tail of the parent might cause the critter to be invalidly long.
        // however some of the tail might be non-expressed dna i.e. not used in the parse of the critter
        // so we might just allow the critter but chop the dna off to fit.
        long part1cAlt;
        
        if (fullDnaToBeUsed) { 
            
            // This is still necessary for the best results even with the replacement crossover.
            // Just now I have two theories:
            //   (A) It allows an individual to survive certain mutations which ripple out the grammar
            //   (B) It allows crossovers with functions (currently) which cause a ripple - NOTE my next
            //         bit of work is to implement functions in a more information preserving manner.
            //         (brave words above as at 24/7/06 I still haven't done this.
            
            part1c = pge0->getDnaLength() - endOfExp1 * CODON;
            part1cAlt = pge0->getUsefulDna() - endOfExp1 * CODON;
            
            // some sanity checking when I ran into problems at one point.
            
            if (part1c < 0) cout << "Error we have a negative part1c\n";
            if (part1cAlt < 0) cout << "Error we have a negative part1cAlt\n";

        } else { 
            // Left for historical interest I cant see not allowing fullDnaToBeUsed.
            part1c = pge0->getUsefulDna() - endOfExp1 * CODON;
            part1cAlt = pge0->getUsefulDna() - endOfExp1 * CODON;
            if (part1c < 0) cout << "Error we have a negative part1c\n";
        } 

        bool bad = false;
        // Again a few sanity checks - I don't use asserts as later on I want
        // to cach a bad and print out some more info about the critters.
        
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
        
        long newDnaLength = part1a + part1b + part1c;
        if (newDnaLength > INVALIDTHRESHOLD) { // we can rebuild it (perhaps!)
                                               // the idea here is part1cAlt contains the usefulDNA length
                                               // if the unused tail is making it too long slice it down to size.
//            cout << "*************New DNA is over INVALIDTHRESHHOLD*****************\n";
//            cout << "Cslice1 is " << c_slice1 << " and c_slice2 " << c_slice2 << endl;
//            cout << "Part1a is " << part1a << " Part1b is " << part1b << " Part1c " << part1c << endl;
//            cout << "DNA length" << newDnaLength << " and invalid threshold " << INVALIDTHRESHOLD << endl;
            long excessToTrim = newDnaLength - INVALIDTHRESHOLD;
//            cout << "Excess to trim was " << excessToTrim << endl;
//            cout << "Part1c before changing " << part1c << endl;
            // I have a slight disquiet that this method might make the useful DNA creep up too 
            // high and also that all the dna will quickly become sized at invalid threshold
            // need to keep an eye on this.
            if ((part1c - part1cAlt) > excessToTrim) { // ok so we have the room to trim it.
                newDnaLength -= excessToTrim; // so we dont need to copy all of the tail over.
                part1c -= excessToTrim;
            }
//            cout << "And after... " << part1c << endl;
        }
        
        // Now repeat all that for the second parent.
        
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
        
        
        //     cout << "Cslice1 is " << c_slice1 << " and c_slice2 " << c_slice2 << endl;
	//    cout << "Len1 is " << len1 << " and len2 " << len2 << endl;
	//    if (leafcrossover) cout << "Leaf crossover\n";
	//    else cout << "Non leaf crossover\n";
	//    cout << "min tree" << minTree << " max Tree " << maxTree << endl;
        //    cout << "Part1a is " << part1a << " Part1b is " << part1b << " Part1c " << part1c << endl;
        //    cout << "Part2a is " << part2a << " Part2b is " << part2b << " Part2c " << part2c << endl;
	//    entity::displayCodonInfo = true;



       // Okay if something "bad" happened, print out some info and exit
       // Exit as sometimes the runs are so long I might just miss a message.
       // And really none of the "bads" should happen so I should fix it. 
        
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
                                                // the idea here is part2cAlt contains the usefulDNA length
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
        // which is              ^            ^             ^
        //              < c_slice1>      <lofexpr>     <rest>
        
        
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
   /*         if (!child1->isValid()){
                cout << "Expression Matched was " << expr_to_match << " this matched with " << ((codons1[c_slice2] >> CODON) & LHSMASK) << endl;
                cout << "Newly calculated expresion to match is: " << ((codons0[c_slice1] >> CODON) & LHSMASK)<< endl;
                cout << "Cslice1 is " << c_slice1 << " and c_slice2 " << c_slice2 << endl;
                cout << "Len1 is " << len1 << " and len2 " << len2 << endl;
                if (leafcrossover) cout << "Leaf crossover\n";
                else cout << "Non leaf crossover\n";
                cout << "min tree" << minTree << " max Tree " << maxTree << endl;
                cout << "Part1a is " << part1a << " Part1b is " << part1b << " Part1c " << part1c << endl;
                cout << "Part2a is " << part2a << " Part2b is " << part2b << " Part2c " << part2c << endl;
                entity::displayCodonInfo = true;
                
            	cout << op1->display();
            	cout << op2->display();
                cout << child1->display();
                
            }*/
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
        }
    }
    
    return ;
}

