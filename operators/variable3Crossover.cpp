


/****************************************************************************
* This crossover utilises a preference from the parents
* if they both have the same preference then use that one
* otherwise take it from op1
* if they both had the same one children inherit
* otherwise from one randomly
***************************************************************************/


void cr_data::variable3Crossover(const crPtr &op1,const crPtr &op2,crPtr &c1,crPtr &c2) {
	static int NumberOfChoices = 3; // so we choose between two operators
    int cross1,cross2;
    if (!op1) {
        cout << "variableCrossover returned a null parent1\n";
        exit(1);
    }
    if (!op2) {
        cout << "variableCrossover returned a null parent2\n";
        exit(1);
    }
    if (!op1->isValid()) {
        cout << "variable crossover invalid parent1\n";
        exit(1);
    }
    if (!op2->isValid()) {
        cout << "variable crossover invalid parent2\n";
        exit(1);
    }
    cross1 = op1->getCrossMethod()%NumberOfChoices;
    cross2 = op2->getCrossMethod()%NumberOfChoices;
    
    crPtr t1,t2;
    c1.reset();
    c2.reset();
    bool done = false;
    int FailCount = 0;
	int methodUsed;
	if (op1->get_fitness() > op2->get_fitness()) methodUsed = cross1; else methodUsed = cross2;
//	methodUsed = randomint(2)?cross1:cross2;
    do {
        FailCount++;
        switch(methodUsed) { // choose randomly between the parents methods
            case 0: // single point Bit
				methodUsed = 0;
                cr_data::singlePointBitOperator(op1,op2,t1,t2);
                break;
            case 1: // two Point Replacement
				methodUsed = 1;
                cr_data::gpReplacementOperator(op1,op2,t1,t2);
    //              cr_data::twoPointReplacementOperator(op1,op2,t1,t2);
                break;
			case 2: // single Point Codon
				methodUsed = 2;
				cr_data::singlePointCodonOperator(op1,op2,t1,t2);
				break;
        }
        if (!c1) {
            if (t1 && t1->isValid()) {
                c1 = t1;
				c1->setCrossMethod(methodUsed);
                if (t2 && t2->isValid()) {
                    c2 = t2;
					c2->setCrossMethod(methodUsed);
                    done = true;
                }
            } else if (t2 && t2->isValid()) {
                c1 = t2;
				c1->setCrossMethod(methodUsed);
            }
        } else if (!c2) {
            if (t1 && t1->isValid()) {
                c2 = t1;
				c2->setCrossMethod(methodUsed);
                done = true;
            } else if (t2 && t2->isValid()) {
                c2 = t2;
				c2->setCrossMethod(methodUsed);
                done = true;
            }
        }
        if (FailCount > 25) break; // Give up eventually don't want it to hang here.
    } while (!done);
    if (c1 != NULL) {
            if (randomint(10000) < variableCrossoverMutateChance) c1->setCrossMethod(randomint(NumberOfChoices));
    }
    if (c2 != NULL ) {
            if (randomint(10000) < variableCrossoverMutateChance) c1->setCrossMethod(randomint(NumberOfChoices));
    }
    
}

