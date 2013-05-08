/****************************************************************************
* The idea here is that this is a version of the uniform crossover used
* primarily in GA. I (famour last words) have real doubts about its efficacy
* but have decided to at least test it in the various problem domains I have
* implemented ... you never now...
* some design decisions. Basically uniform crossover does not change the DNA length
* so given that GE is a variable length bit stream representation the way it will work
* is that child 1 will take the length of parent 1 and child 2 the length of parent 2
* uniformaly cross them to the shortest of the parents and then retain the same pattern from
* the longer parent from the excess. I know I could have explained that clearer!
***************************************************************************/

void cr_data::uniformCodonOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
   
          int thisInvalidCount;
          thisInvalidCount = 0;
          entity *pge0,*pge1;
          
          pge0 = op1->theEntity;
          pge1 = op2->theEntity; 
 
 
          // I originally used pointers as I returned only one child, and the "first" parent
          // was randomly determined. I changed to the two child method only because its the way its normally done
          // and I was wanting to improve the operator - see the implementation of the * and % ops at the same time
          // I should do some empirical tests on this.
          
          if (pge0->getUsefulDna() > pge0->getDnaLength()) cout << "Well here is a problem\n";
          long dnaL1 = pge0->getDnaLength();
          long len1 = dnaL1/CODON;
          long dnaL2 = pge1->getDnaLength();
          long len2 = dnaL2/CODON; 
 

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
          bool valid = false;
 
          // okay the + is only at the CODON boundaries.
 
          do {
  
                    long newDnaLength = dnaL1;
                    long newDnaLength2 = dnaL2;
                    if ((newDnaLength > INVALIDTHRESHOLD) || (newDnaLength2 > INVALIDTHRESHOLD)) {
                              valid =false;
                    } else valid =true; // valid so far anyway
                    if (valid) {
                        int newDna[newDnaLength];
                        int newDna2[newDnaLength2];
                        long firstCopy;
                        bool dnaL1Short;
                        if ((dnaL1Short = (dnaL1 < dnaL2))) firstCopy = dnaL1;
                        else firstCopy = dnaL2;
                        long lastMatchCodon = firstCopy/CODON;
                        long offset = 0;
                        for (int i=0;i<lastMatchCodon;i++) {
                            if (i % 2 == 0) {
                                for (int j=0;j<CODON;j++) {
                                    newDna[offset+j] = pdna1[offset+j];
                                    newDna2[offset+j] = pdna2[offset+j]; 
                                }
                            } else {
                                for (int j=0;j<CODON;j++) {
                                    newDna[offset+j] = pdna2[offset+j];
                                    newDna2[offset+j] = pdna1[offset+j]; 
                                }
                            }
                            offset += CODON; // i.e. offset = i*CODON
                        }
                        // okay we have uniformly swapped up to the length of the shortest.
                        
                        if (dnaL1Short) 
                            for (int i=dnaL1;i<dnaL2;i++)
                                newDna2[i]=pdna2[i];
                        else
                            for (int i=dnaL2;i<dnaL1;i++)
                                newDna[i]=pdna1[i];
                        
                        cr_data *newcr;
                        newcr = new cr_data(newDna,newDnaLength);
                        valid = newcr->isValid();
                        if (valid) {
                            child1.reset(newcr); // this is a bit of a work around - need to think about it.
                            child2.reset(new cr_data(newDna2,newDnaLength2));
                        } if (!valid) delete newcr;
                        
                    }
                    if (!valid) {
                              //                     invalid_count++;
                              thisInvalidCount++;
                              if (thisInvalidCount>5) {
                                        invalid_count++;
                              }
                    }
          } while( !valid && (thisInvalidCount <=5));
//		  if (newMethodMutation) copyMut(op1,op2,toreturn);
          return ;
}

