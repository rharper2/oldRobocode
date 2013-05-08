


/****************************************************************************
* This crossover chooses from
* amongs a pre-deterimined selection
* of different crossover types 
* randomly.
***************************************************************************/


void cr_data::randomCrossover(const crPtr &p1,const crPtr &p2,crPtr &c1,crPtr &c2) {
    
    int cross = -1;
    if (!p1) {
        cout << "Findit returned a null parent in Probability Breeder\n";
        exit(1);
    }
    if (!p2) {
        cout << "Findit returned a null p2 parent in Probability Breeder\n";
        exit(1);
    }
    if (!p1->isValid()) {
        cout << "Findit it returned and invalid parent in Probability Breeder\n";
        exit(1);
    }
    if (!p2->isValid()) {
        cout << "Findit it returned and invalid p2 parent in Probability Breeder\n";
        exit(1);
    }
    switch(randomint(2)) {
        case 0: // single point Bit
            cr_data::singlePointBitOperator(p1,p2,c1,c2);
            cross = 0;
            break;
        case 1: // two Point Replacement
            cr_data::twoPointReplacementOperator(p1,p2,c1,c2);
            cross =1;
            break;
    }
    if (c1 != NULL) {
        c1->setCrossMethod(cross); // just noting that this was created with cross type.
    }
    if (c2 != NULL ) {
        c2->setCrossMethod(cross); // just noting that this was created with cross type.
    }
}

