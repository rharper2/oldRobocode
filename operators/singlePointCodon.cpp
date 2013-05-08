/****************************************************************************
* The singlePointCodonOperator implements the CODON crossover.        
* Initially it just took it anywhere in the DNA but now it ensures   
* its at a codon boundary - as otherwise the information in the tail  
* is effectively destroyed. The entity donating the "head" of the new  
* DNA is chosen at random and randomly 1..total of its genomes are used 
* then 1..total genomes of the second parent are chosen. The size of the 
* resultant dna may be as small as 2 and as big as the combination of the 
* two parents            
*
* Following my implemetation of "codon values" e.g. a CVAL
* where the codon which follows the CVAL is returned to teh interpreter
* I am wondering if I should stop a "chop" between the CVAL and the value
* like they are joined individibly.
* it wont stop one CVAL replacing another, but I am a bit concerned about the ripple
* effect if it gets spliced inbetween the CVAL and the value.
***************************************************************************/

void cr_data::singlePointCodonOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
    
    int thisInvalidCount;
    thisInvalidCount = 0;
    entity *pge0,*pge1;
    
    pge0 = op1->theEntity;
    pge1 = op2->theEntity; 
    long *codons0,*codons1;
    codons0 = pge0->getCodons();
    codons1 = pge1->getCodons();
	
    // I originally used pointers as I returned only one child, and the "first" parent
    // was randomly determined. I changed to the two child method only because its the way its normally done
    // and I was wanting to improve the operator - see the implementation of the * and % ops at the same time
    // I should do some empirical tests on this.
    
    if (pge0->getUsefulDna() > pge0->getDnaLength()) cout << "Well here is a problem\n";
    long len1 = pge0->getUsefulDna()/CODON;
    long len2 = pge1->getUsefulDna()/CODON; 
	if (len1 <= 2 || len2 <=2) {
      // refuse to have anything to do with a parent who is just a leaf.
      // This can happen through, for example, mutation.
       return; 
    } 
    
    
    // at one point this was just the length of the DNA in order to keep "junk" dna
    // not a good idea I think better to keep the introns i.e. expressions which are not used.
    // however initial results (10/11/2004) with the % operator indicated that the solution
    // was converged on far quicker if the rest of the dna was "kept" - but that differed from
    // my original approach as the "slices" were constrained to the useful dna BUT the 
    // copy over on the second individual was performed up to dna length. I did however
    // notice (for the first time) the "explosive" dna growth towards the end of the run.
    // This would appear to be the normal for genetic programs (put in reference - in the book)
    // I have introduced a soft switch (14/11/2004) using the static fullDnaToBeUsed.
    // Ideas to  prevent the explosive growth would be early detection and flipping the softswitch
    // or introducing some selection pressure (via the "<" opeator) favouring smaller dnas - After they
    // have reached a certain point - previously such an operator prohibited growth at the beginning.
    // of course it may be that the % operator was what was giving the benefit. We shall see...
    
    // 18/1/04 I have just spent some time analysing the results the most surprising thing is that the two operators
    // being plus and minus give the best results - but plus only if the full dna length is used.
    // the mod operator (which is similar to the plus except it constrains joins at matching places
    // does not seem to perform well.
    // I am now going to analyse the following - allowing a random bit string at the end of the plus selection
    // and in the minus operator retaining some "additional" dna junk at the end of the dna. (creating it if necessary).
    // 1/2/2005 - Bit tardy in updating this section, the additional dna at the end of the minus was very beneficial.
    // Some theories for this is included in the comments in the implementation of what is now called the
    // twoPointReplacementOperator (Minus is of course far easier to say!).
    //
    // Whilst I am typing it occured to me that if I was to change the excess tail for a random bit string in some operators
    // I would find out if it contains any useful data (left over dara?) or if its just an entity saver when the grammar
    // ripples out. (1/2/2005).
    
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
    
    // okay the + is only at the CODON boundaries.
    //cout << "Len1 " << len1 << " and len2 " << len2 << endl;
    long c_slice1;
	do {
		c_slice1 = (randomint(len1-1)+1); // so generate a number from 1..len1-1 inclusive
	} while (!((codons0[c_slice1] >> CODON) & LHSMASK));
	// so keep choosing c_slice1 until the codon is used to interepret a grammar rule i.e. is non-zero.
	long slice1=c_slice1 * CODON;
    long c_slice2;
	do { 
	 c_slice2 = (randomint(len2-2)+1); // so generate a number from 1..len2-1 
    } while (!((codons1[c_slice2] >> CODON) & LHSMASK));
	long slice2= c_slice2 * CODON;
    
    if (slice1 < 1) {
        cout << "ERROR slice1 is " << slice1 << ", must have non-sensible dna lengths (overflow) \n";
        slice1 = 50;
    }
    if (slice2 < 1) {
        cout << "ERROR slice2 is " << slice2 << ", must have non-sensible dna lengths (overflow) \n";
        slice2 = 50;
    }
    
    long i= 0;
    
    long flen1,flen2;
    
    if (fullDnaToBeUsed) {
        flen1 = pge0->getDnaLength();
        flen2 = pge1->getDnaLength();
        if (flen2-slice2+slice1 > USEFULMANDATORY) {
            flen2 = pge1->getUsefulDna();
        }
        if (flen1-slice1+slice2 > USEFULMANDATORY) {
            flen1 = pge0->getUsefulDna();
        }
    } else {
        flen1 = pge0->getUsefulDna();
        flen2 = pge1->getUsefulDna();
    }
    
    
    long offset1 = flen1-slice1;
    long offset2 = flen2-slice2;
    
    //   Just to keep it clear we have the following defined
    //    <------- lenX ---------->
    //    <-sliceX-><---offsetX--->
    //   So the first child consists of 
    //   <-----slice1-----><----offset2----> and the second 
    //   <-----slice2-----><----offset1---->
    //
    
    long newDnaLength = slice1+offset2;
    long newDnaLength2 = slice2 + offset1;
    if (newDnaLength <  INVALIDTHRESHOLD) {
        int newDna[newDnaLength];
        for (i=0;i<slice1;i++) 
            newDna[i]=pdna1[i];
        for (i=0;i<offset2;i++) 
            newDna[slice1+i]=pdna2[slice2+i];
        child1.reset(new cr_data(newDna,newDnaLength));
    }
    
    if (newDnaLength > 60000) {
        cout << "Dna length seems very high (" << newDnaLength << ") may want to look at this\n";
    }
    if (newDnaLength2 < INVALIDTHRESHOLD) {
        int newDna2[newDnaLength2];
        
        for (i=0;i<slice2;i++)
            newDna2[i]=pdna2[i];
        for (i=0;i<offset1;i++)
            newDna2[slice2+i]=pdna1[slice1+i];
        child2.reset(new cr_data(newDna2,newDnaLength2));
    }
    return ;
    }

