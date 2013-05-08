void cr_data::twoPointReplacementSAGAOperator(const crPtr &op1,const crPtr &op2,crPtr &child1,crPtr &child2) {
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
	bool valid = false;
	// okay the Matching only makes sense at the CODON boundaries.
	do {
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
		
#ifdef PROTECTIONENABLED
		if (USEPROTECTION) {
			// - first of all clear protection bits.
			for (int i=0;i< len1;i++) {
				codons0[i] = codons0[i] & EXPRPROTMASK;
			}
			// now go through setting the protection bits
			for (int i=1;i<len1;i++) { // start at 1, don't allow the whole set to be protected.
				if (codons0[i] & PROTECTEDBITMASK) { // bit eight of codon set
					int eOfexp = (codons0[i] >> EXPRLENGTH) & EXPRMASK;
					while (i< (eOfexp-1)) { // eOfexp-1 as we increment i before using it.
						i++; // this isnt a mistake we do not want to set it on the first one.
						codons0[i] = codons0[i] | INPROTECTIONMASK; //set last bit
					}
				}
			}
			// Okay the protection bits are now set so slide up and down until we find a c_slice we can use
			bool done = false;
			int low = c_slice1;
			int high = c_slice1;
			bool low_limit_reached = false;
			bool high_limit_reached = false;
			int found = -1;
			while (!done) {
				if (!(codons0[low] & INPROTECTIONMASK)) { // top bit not set
					found = low;
					done = true;
				} else if (!(codons0[high] & INPROTECTIONMASK)) {
					found = high;
					done = true;
				} else {
					if (low <=0) {
						if (high_limit_reached) done = true;
						else low_limit_reached = true;
						low = 0;
					} else low--;
					if (high >= len1-1) {
						if (low_limit_reached) done = true;
						else high_limit_reached = true;
					} else high++;
				}
			}
			c_slice1 = found;
			if (found <= 0) {
				cout << "The whole parent appears to have been protected!";
				// The question is what do we do here?
				// My first thought is to return the  parents untouched.
				// But this might "reward" survival of the genome by some
				// evolutionary stopping behaviour - but then if the others don't ... hmm
				// well thats what I will try initially. @@@@ Some @'s to make me 
				// think about it again later.
				child1 = op1;
				child2 = op2;
				//if (newMethodMutation) copyMut(op1,op2,toreturn);
				return ;
			}
		} // okay so we have now found a c_slice1 that is not within a protected part of the genome.
#endif	
		//  long i= 0;
		// we need to match up slice 1 and slice 2;
		int expr_to_match = ((codons0[c_slice1] >> CODON) & LHSMASK); // take the top bits
		if (expr_to_match ==0) {
			cout << "Problem\n";
			
		}
		
		// Search from the chosen point, higher and lower to find a match
		// with the new protection strategy we now need to prepare the second set of codons
		// to set the appropriate "protection bits"
#ifdef PROTECTIONENABLED
		if (USEPROTECTION) {
			// - first of all clear protection bits.
			for (int i=0;i< len2;i++)
				codons1[i] = codons1[i] & EXPRPROTMASK;
			// now go through setting the protection bits
			for (int i=1;i<len2;i++) {
				if (codons1[i] & PROTECTEDBITMASK) { // bit eight of codon set
					int eOfexp = (codons1[i] >> EXPRLENGTH) & EXPRMASK;
					while (i< (eOfexp-1)) { // eOfexp-1 as we increment i before using it.
						i++; // this isnt a mistake we do not want to set it on the first one.
						codons1[i] = codons1[i] | INPROTECTIONMASK; //set last bit
					}
				}
			}
		}

		// Okay so codons1 is now prepared
		// the only difference to the matching mechanism is that 
		// we cannot match where the protection bit is set.
#endif		
		// Search from the chosen point, higher and lower to find a match
		// Okay with SAGA we either continue as per the twopointreplacement
		// if the expression length is less than SAGAApplicationMinimum
		// otherwise we do this new saga thing.
		
		int endOfExp = ((codons0[c_slice1] >> EXPRLENGTH) & EXPRMASK);
		if (endOfExp < c_slice1) {
			cout << "We have an error, the end of an expression was lower than its start (Ow!) \n";
			exit(1);
		}
		int found = -1;
		if (endOfExp - c_slice1 < SAGAApplicationMinimum) { 
			// continue as before to determine c_slice2
//			cout << "To small to saga - site: " << c_slice1 << " with length: " << endOfExp << "\n";
			int matched;
			int low = c_slice2;
			int high = c_slice2;
			bool done = false;
			bool low_limit_reached = false;
			bool high_limit_reached = false;
			while ( (found < 0) && !done) {
#ifdef PROTECTIONENABLED
				if ( !(USEPROTECTION && (codons1[low] & INPROTECTIONMASK)) && (((codons1[low] >> CODON) & LHSMASK) == expr_to_match)) 
#else
				if ( ((codons1[low] >> CODON) & LHSMASK) == expr_to_match) 
#endif					
				{
						// okay that is: check that it is not true that ( we are using protection and its protected)
						// and then check that we have a match
						matched = codons1[low] >> CODON & LHSMASK;
						found = low;
						done = true;
				} //else 
#ifdef PROTECTIONENABLED
				if ( !(USEPROTECTION && (codons1[high] & INPROTECTIONMASK)) && (((codons1[high] >> CODON) & LHSMASK) == expr_to_match)) {
#else
				if ( ((codons1[high] >> CODON) & LHSMASK) == expr_to_match) {
#endif		
					matched = codons1[high] >> CODON & LHSMASK;
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
		} else { // we need to locate each possible match site on codons1
			int low = c_slice2;
			int high = c_slice2+1;
			while (high >= len2-1) {
				high--;
				low--;
			}
			bool done = false;
			bool low_limit_reached = false;
			bool high_limit_reached = false;
			list<int> siteStack;
			siteStack.clear();
			while (!done) {
#ifdef PROTECTIONENABLED
				if ( !(USEPROTECTION && (codons1[low] & INPROTECTIONMASK)) &&
					 (!low_limit_reached && ((codons1[low] >> CODON) & LHSMASK) == expr_to_match)) {
#else								 
				if ( !low_limit_reached && ((codons1[low] >> CODON) & LHSMASK) == expr_to_match) {
#endif
					siteStack.push_back(low);
				}
#ifdef PROTECTIONENABLED
					if ( !(USEPROTECTION && (codons1[high] & INPROTECTIONMASK)) &&
						 (!high_limit_reached && ((codons1[high] >> CODON) & LHSMASK) == expr_to_match)) {
#else								 
						if ( !high_limit_reached && ((codons1[high] >> CODON) & LHSMASK) == expr_to_match) {
#endif
						siteStack.push_back(high);
				} 
				if (low <=0) {
					if (high_limit_reached) done = true;
					else low_limit_reached = true;
					low = 0;
				} else low--;
				if (high >= len2-1) {
					if (low_limit_reached) done = true;
					else high_limit_reached = true;
					high = len2-1;
				} else high++;
			}
//			cout << "Okay we found " << siteStack.size() << " possible sites.\n";
			// okay we have a list of possible sites.
			
			list<int>::iterator at = siteStack.begin();
			int bestScore = -1;
			int bestSite = -1;
			int count = 0;
			int bestNo = -1;
//			cout << "Sites are: " ;
//			while (at != siteStack.end()) {
//				cout << *at << ": ";
//				at++;
//			}
			at = siteStack.begin();
			while (at != siteStack.end()) {
				// so lets recap, the first codon string begins at c_slice1 and ends at endOfExp
				// the next string starts at *at and ends at (codons1[*at] >> 16) & 0xFFFF
				// I am going to call a variation on algb which elminates the need to pass L
				// but wraps it and returns the highest number in L, which is always going to be at 
				// the end
				// rather than call with *at and *at >> etc Ill split them out for sanity checks.
				int siteToTry = *at;
			    count++;
				int endExp2 = (codons1[*at] >> EXPRLENGTH) & EXPRMASK;
				if (endExp2 < siteToTry) {
					cout << "We have an expression of negative length which is fascinating.\n";
					cout << op1->display();
					cout << "Second one: \n";
					cout << op2->display();
					cout << "At was " << *at << " site to try " << siteToTry << "\n";
					cout << "c_slice2 started at" << c_slice2 << "\n";
					at = siteStack.begin();
					while (at !=siteStack.end()) {
						cout << "Stack: " << *at << "\n";
						at++;
					}
					exit(1);
				}
				int temp = algrp(codons0,c_slice1,endOfExp,codons1,siteToTry,endExp2);
					if (temp > bestScore) {
						bestScore = temp;
						bestSite = siteToTry;
						bestNo = count;
					}
				at++;
			}
			found = bestSite; // and here we have it.
//			cout << "The best one was number " << bestNo << " at site " << bestSite << "\n";
		}
			
		/* I notice that Coza when matching is s-expressions prohibits the replacement of a
			non-terminal with a terminal - I may have to contemplate not allowing a match
			unless the length of the sub-expressions are greater than a certain length -
			subsequently I have determined to try SAGA to match possible crossover locations
			must remember to come back here if SAGA proves to be less productive.
			*/
		
		if (found > 0) {
			c_slice2 = found;
			int endOfExp1 = ((codons0[c_slice1] >> EXPRLENGTH) & EXPRMASK);
			int endOfExp2 = ((codons1[found] >> EXPRLENGTH) & EXPRMASK);
			long part1a = c_slice1 * CODON;
			long part1b = (endOfExp2 - c_slice2) * CODON; // the length of the expression to be inserted
			long part1c;
			if (fullDnaToBeUsed) { 
				
				// This is still necessary for the best results even with the replacement crossover.
				// Just now I have two theories:
				//   (A) It allows an individual to survive certain mutations which ripple out the grammar
				//   (B) It allows crossovers with functions (currently) which cause a ripple - NOTE my next
				//         bit of work is to implement functions in a more information preserving manner.
				
				part1c = pge0->getDnaLength() - endOfExp1 * CODON;
				if (part1c < 0) cout << "Error we have a negative part1c\n";
			} else { 
				part1c = pge0->getUsefulDna() - endOfExp1 * CODON;
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
			long newDnaLength = part1a + part1b + part1c;
			long part2a = c_slice2*CODON;
			long part2b = (endOfExp1 - c_slice1) * CODON;
			long part2c;
			if (fullDnaToBeUsed) { 
				part2c = pge1->getDnaLength() - endOfExp2 * CODON;
				if (part2c < 0) cout << "Error we have a negative part2c\n";
			} else { 
				part2c = pge1->getUsefulDna() - endOfExp2 * CODON;
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
			
			// the following will probably help
			// Parent 1: <--part1a--><--part2b---><---part1c--->
			// which is  < c_slice1-><- lofexpr----><--rest---------->
			// and note < end of exp 1---------------->
			
			// Parent 2: <--part2a--><---part1b---><--part2c--->
			// Child1:   <-part1a--><--part1b---><---part1c--->
			// Child2:   <--part2a--><---part2b----><part2c-->
			//
			
			if ((newDnaLength > INVALIDTHRESHOLD) || (newDnaLength2 > INVALIDTHRESHOLD)) {
				valid =false;
			} else {
				valid = true;
			}
			if (valid) {
				
				int newDna[newDnaLength];
				int newDna2[newDnaLength2];
				
				for (int i=0;i<part1a;i++) 
					newDna[i]=pdna1[i]; // first part of child1 from dna1
				for (int i=0;i<part1b;i++) 
					newDna[part1a+i]=pdna2[part2a+i]; // second part from dna2
				for (int i=0;i<part1c;i++)
					newDna[part1a+part1b+i]= pdna1[part1a+part2b+i]; // and third from dna1
				
				for (int i=0;i<part2a;i++) 
					newDna2[i]=pdna2[i]; // first part of child2 from dna2
				for (int i=0;i<part2b;i++) 
					newDna2[part2a+i]=pdna1[part1a+i]; // second part from dna1
				for (int i=0;i<part2c;i++)
					newDna2[part2a+part2b+i]= pdna2[part2a+part1b+i]; // and third from dna2
				
				
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
		}
	} while( !valid && (thisInvalidCount <=5));
	//if (newMethodMutation) copyMut(op1,op2,toreturn);
	return ;
}

	
