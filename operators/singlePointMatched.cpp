/****************************************************************************
* The Matched Operator is the last (currently) operator to implement crossover.   (10/11/04)
* The idea here is that the grammar now stores in the higher bits 
* (shift back by CODON) the non-terminal which that codon decides.
* so if we are deciding which type of LINE to expand, then the codon
* will have the code (-1000) for LINE in the higher order bits.
* we can therefore make sure we only swap a branch of LINE for 
* another branch of LINE.
* However, it must be noted that this is not the same as tree node swapping
* which I may have to do later (look for the - operator) the reason is this
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
* and turned into Z, the expanion of the second LINES will be according to the second
* parents DNA NOT the first (as in the tree node expansion)
* But in our example - what is that DNA? since it was not part of the valid
* expression of parent 2 - well if we chop it off at usefulDnaLength it will
* invalid - unless we have enabled one of our terminating strategies
* but if we allow the second dna to go up to dnaLength (not usefulDnaLength)
* then we will have - well a whole new ballgame.
*
* Not that constructive a crossover operator!
*
* to implement a more "node" like operator I may need to consider
* storing with the codon the "length" of that expansion
* to do this I will need a recursive grammar or implement (maybe)
* some sort of stack in the grammar containing the (non-terminal) codonPosition which is pushed
* with each non-terminal and popped with each terminal or something like that.
* the dna "sequence" linked to that codon length is used to replace the sequence 
* of the to-be-replaced node. - In fact see the - operator
***************************************************************************/


void cr_data::singlePointMatchedOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
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
    // okay the % only makes sense at the CODON boundaries.
    int c_slice1 = (randomint(len1-1)+1); // so generate a number from 1..len1-1 inclusive
    int c_slice2 = (randomint(len2-2)+1); // so generate a number from 1..len2-1 
    if (c_slice1 < 1) {
        cout << "ERROR slice1 is " << c_slice1 << ", must have non-sensible dna lengths (overflow) \n";
        c_slice1 = 5;
    }
    if (c_slice2 < 1) {
        cout << "ERROR slice2 is " << c_slice2 << ", must have non-sensible dna lengths (overflow) \n";
        c_slice2 = 5;
    }
    // we need to match up slice 1 and slice 2;
    int expr_to_match = ((codons0[c_slice1] >> CODON) & LHSMASK); // take the top bits
    if (expr_to_match ==0) {
        cout << "Problem\n";
        
    }
    
    int low = c_slice2;
    int high = c_slice2;
    bool done = false;
    bool high_limit_reached = false;
    bool low_limit_reached = false;
    int found = -1;
    while ( (found < 0) && !done) {
        if ( ((codons1[low] >> CODON) & LHSMASK) == expr_to_match) {
            found = low;
            done = true;
        } else if (((codons1[high] >> CODON) & LHSMASK) == expr_to_match) {
            found = high;
            done = true;
        } else {
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
    if (found > 0) {
        c_slice2 = found;
        long offset,offset2;
        long flen0,flen1;
        if (fullDnaToBeUsed) {
            flen0 = pge0->getDnaLength();
            flen1 = pge1->getDnaLength();
            if (c_slice1*CODON +flen1 -c_slice2*CODON > USEFULMANDATORY)
                flen1 = pge1->getUsefulDna();
            if (c_slice2*CODON + flen0 - c_slice1*CODON > USEFULMANDATORY)
                flen0 = pge0->getUsefulDna();
        } else {
            flen0 = pge0->getUsefulDna();
            flen1 = pge1->getUsefulDna();
        }
        long newDnaLength = c_slice1*CODON+(offset = (flen1 - c_slice2*CODON));
        long newDnaLength2 = (c_slice2)*CODON + (offset2 = (flen0 - c_slice1*CODON));
        if (newDnaLength < INVALIDTHRESHOLD) {
            int newDna[newDnaLength];
            
            for (int i=0;i<c_slice1*CODON;i++) 
                newDna[i]=pdna1[i];
            for (int i=0;i<offset;i++) 
                newDna[c_slice1*CODON+i]=pdna2[c_slice2*CODON+i];
            child1.reset(new cr_data(newDna,newDnaLength));
            
        }       
        if (newDnaLength2 < INVALIDTHRESHOLD) {
            int newDna2[newDnaLength2];
            // As usual to keep it clear:
            //    <------- len1 --------->
            //    <-slice1-><---offset2-->
            //    <------- len2 --------->
            //    <-slice2-><---offset--->
            //
            
            for (int i=0;i<c_slice2*CODON;i++)
                newDna2[i]=pdna2[i];
            for (int i=0;i<offset2;i++)
                newDna2[c_slice2*CODON+i]=pdna1[c_slice1*CODON+i];
            if (newDnaLength > 60000) {
                cout << "Dna length seems very high (" << newDnaLength << ") may want to look at this\n";
            }
            child2.reset(new cr_data(newDna2,newDnaLength2));
        }
    }
    return ;
}

    
