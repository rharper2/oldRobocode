
/*******************************************************************
                    TWO POINT CODON OPERATOR
                    choose two codon boundaries within the useful dna
                    and exchange the segments delineated by them.
********************************************************************/

void cr_data::twoPointCodonOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
          int thisInvalidCount;
          thisInvalidCount = 0;
          entity *pge0,*pge1;
          pge0 = op1->theEntity;
          pge1 = op2->theEntity; 
 
          if (pge0->getUsefulDna() > pge0->getDnaLength()) cout << "Well here is a problem\n";
          long len1 = pge0->getUsefulDna()/CODON;
          long len2 = pge1->getUsefulDna()/CODON; 
 
 
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
                    long slice1 = (randomint(len1-1)+1)*CODON; // so generate a number from 1..len1-1 inclusive
                    long slice2 = (randomint(len2-2)+1)*CODON; // so generate a number from 1..len2-1 
        
                    long chunk1 = (randomint((len1 - slice1/CODON) -1)+1)*CODON; // chunk1 is the amount of the first critter we take
                    long chunk2 = (randomint((len2-slice2/CODON)-1)+1)*CODON; // chunk2 is the amount of the second critter we take
                
                    if (slice1 < 1) {
                              cout << "ERROR slice1 is " << slice1 << ", must have non-sensible dna lengths (overflow) \n";
                              slice1 = 50;
                    }
                    if (slice2 < 1) {
                              cout << "ERROR slice2 is " << slice2 << ", must have non-sensible dna lengths (overflow) \n";
                              slice2 = 50;
                    }
                    if (slice1 + chunk1 > len1*CODON) cout << "ERROR SLICE1 and CHUNK1 CANT EXCEED LEN!\n";
                    if (slice2 + chunk2 > len2*CODON) cout << "ERROR SLICE2 and CHUNK2 cant exceed LEN2\n";
  
                    long i= 0;
  
                    long flen1,flen2;
  
                    if (fullDnaToBeUsed) {
                              flen1 = pge0->getDnaLength();
                              flen2 = pge1->getDnaLength();
                              if (flen1-chunk1+chunk2 > USEFULMANDATORY) {
                                        flen1 = pge0->getUsefulDna();
                              }
                              if (flen2-chunk2+chunk1 > USEFULMANDATORY) {
                                        flen2 = pge1->getUsefulDna();
                              }
                    } else {
                              flen1 = pge0->getUsefulDna();
                              flen2 = pge1->getUsefulDna();
                    }
  
  
                    long offset1 = flen1-(slice1+chunk1); // this is the length of the remainder;
                    long offset2 = flen2-(slice2+chunk2); // this is where we start to recopy
  
                    //   Just to keep it clear we have the following defined
                    //    <------- lenX ------------------------>
                    //    <-sliceX-><chunkX><offsetX>
                    //   So the first child consists of 
                    //   <-----slice1-----><----chunk2----><offset1>
                    //   <-----slice2-----><----chunk1----><--offset2>
                    //
  
                    long newDnaLength = slice1+chunk2+offset1;
                    long newDnaLength2 = slice2 + chunk1+offset2;
                    if ((newDnaLength > INVALIDTHRESHOLD) || (newDnaLength2 > INVALIDTHRESHOLD)) {
                              valid =false;
                    } else valid =true; // valid so far anyway
                    if (valid) {
                              int newDna[newDnaLength];
                              int newDna2[newDnaLength2];
   
                              for (i=0;i<slice1;i++) 
                                        newDna[i]=pdna1[i];
                              for (i=0;i<chunk2;i++)
                                        newDna[slice1+i]=pdna2[slice2+i];
                              for (i=0;i<offset1;i++) 
                                        newDna[slice1+chunk2+i]=pdna1[slice1+chunk1+i];
                              for (i=0;i<slice2;i++)
                                        newDna2[i]=pdna2[i];
                              for (i=0;i<chunk1;i++)
                                        newDna2[slice2+i]=pdna1[slice1+i];
                              for (i=0;i<offset2;i++)
                                        newDna2[slice2+chunk1+i]=pdna2[slice2+chunk2+i];
  
                              if (newDnaLength > 60000) {
                                        cout << "Dna length seems very high (" << newDnaLength << ") may want to look at this\n";
                              }
#ifdef VERBOSE
                              cout << "DNA Splice took " << slice1 << "," << chunk1 << " from first and " << slice2  << "," << chunk2 << "  from the second \n";
#endif
                              cr_data *newcr;
                              newcr = new cr_data(newDna,newDnaLength);
                              valid = newcr->isValid();
                              if (valid) {
                                        child1.reset(newcr); // this is a bit of a work around - need to think about it.
                                        child2.reset(new cr_data(newDna2,newDnaLength2));
                              } else delete newcr;
   
                    }
                    if (!valid) {
                              thisInvalidCount++;
                              if (thisInvalidCount>5) {
                                        invalid_count++;
                              }
                    }
          } while( !valid && (thisInvalidCount <=5));
//		  if (newMethodMutation) copyMut(op1,op2,toreturn);
		  return ;
}

