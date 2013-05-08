
/*******************************************************************
                    TAILLESS OPERATOR - fill the tails with random bits
********************************************************************/


void cr_data::taillessBitOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
          int thisInvalidCount;
          thisInvalidCount = 0;
          entity *pge0,*pge1;
          pge0 = op1->theEntity;
          pge1 = op2->theEntity;
          if (pge0->getUsefulDna() > pge0->getDnaLength()) cout << "Well here is a problem\n";
          long len1 = pge0->getUsefulDna();
          long len2 = pge1->getUsefulDna(); 
          if (len1 > 64000) {
                    cout << "Length of dna is greater than 64000 ( " << len1 << ") this is going to cause an overflow on randomint, so chopping down\n";
                    len1 = 64000;
          }
          if (len2 > 64000) {
                    cout << "Length of dna  is greater than 64000 ( " << len2 << ") this is going to cause an overflow on randomint, so chopping down\n";
                    len2 = 64000;
          }
          const int *pdna1 = pge0->getDna();
          const int *pdna2 = pge1->getDna();
          bool valid = false;
          do { 
                    long slice1 = (randomint(len1-1)+1); // so generate a number from 1..len1-1 inclusive
                    long slice2 = (randomint(len2-1)+1); // so generate a number from 1..len2-1 
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
                    
     
                    long newDnaLength = slice1+offset2;
                    long newDnaLength2 = slice2 + offset1;
     
                    if ((newDnaLength > INVALIDTHRESHOLD) || (newDnaLength2 > INVALIDTHRESHOLD)) {
                              valid =false;
                    } else valid =true; // valid so far anyway
                    if (valid) {
                              int newDna[newDnaLength];
                              int newDna2[newDnaLength2];
      
                              for (i=0;i<slice1;i++) 
                                        newDna[i]=pdna1[i];
                              for (i=0;i<offset2;i++) 
                                        newDna[slice1+i]=randomint(2); // fill with random bits
                              for (i=0;i<slice2;i++)
                                        newDna2[i]=pdna2[i];
                              for (i=0;i<offset1;i++)
                                        newDna2[slice2+i]=randomint(2);
                              if (newDnaLength > 60000) {
                                        cout << "Dna length seems very high (" << newDnaLength << ") may want to look at this\n";
                              }
      
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
                   
                              
/*******************************************************************
                    HEADLESS OPERATOR - fill the head with random bits 
                                    (what does that remind me of?)
********************************************************************/


void cr_data::headlessBitOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
          int thisInvalidCount;
          thisInvalidCount = 0;
          entity *pge0,*pge1;
          pge0 = op1->theEntity;
          pge1 = op2->theEntity;
          if (pge0->getUsefulDna() > pge0->getDnaLength()) cout << "Well here is a problem\n";
          long len1 = pge0->getUsefulDna();
          long len2 = pge1->getUsefulDna(); 
          if (len1 > 64000) {
                    cout << "Length of dna is greater than 64000 ( " << len1 << ") this is going to cause an overflow on randomint, so chopping down\n";
                    len1 = 64000;
          }
          if (len2 > 64000) {
                    cout << "Length of dna  is greater than 64000 ( " << len2 << ") this is going to cause an overflow on randomint, so chopping down\n";
                    len2 = 64000;
          }
          const int *pdna1 = pge0->getDna();
          const int *pdna2 = pge1->getDna();
          bool valid = false;
          do { 
                    long slice1 = (randomint(len1-1)+1); // so generate a number from 1..len1-1 inclusive
                    long slice2 = (randomint(len2-1)+1); // so generate a number from 1..len2-1 
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
                    
     
                    long newDnaLength = slice1+offset2;
                    long newDnaLength2 = slice2 + offset1;
     
                    if ((newDnaLength > INVALIDTHRESHOLD) || (newDnaLength2 > INVALIDTHRESHOLD)) {
                              valid =false;
                    } else valid =true; // valid so far anyway
                    if (valid) {
                              int newDna[newDnaLength];
                              int newDna2[newDnaLength2];
      
                              for (i=0;i<slice1;i++) 
                                        newDna[i]=randomint(2); // fill the head with random bits
                              for (i=0;i<offset2;i++) 
                                        newDna[slice1+i]=pdna2[slice2+i];
                              for (i=0;i<slice2;i++)
                                        newDna2[i]=randomint(2);
                              for (i=0;i<offset1;i++)
                                        newDna2[slice2+i]=pdna1[slice1+i];
                              if (newDnaLength > 60000) {
                                        cout << "Dna length seems very high (" << newDnaLength << ") may want to look at this\n";
                              }
      
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


