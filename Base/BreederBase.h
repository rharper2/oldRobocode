#ifndef BREEDERBASE
#define BREEDERBASE

#include "critter.h"
#include <vector>

using namespace std;

// The enums which have been commented out 
// were all part of the process of writing the
// paper I am hoping to publish at CEC2006
// I removed the code to which they related 
// on 14 March 2006, but there is a file called
// ProbabilityBreeder as at 14March2006.cpp
// which contains it (and the various archives).


enum {
            singlePointBit = 0,
            singlePointCodon = 1,
            singlePointMatched = 2,
            headlessBit = 3,
            taillessBit = 4,
            twoPointBit = 5,
            twoPointCodon =6,
            twoPointMatched = 7,
            twoPointReplacement = 8,
            twoPointRSaga = 9,
            twoPointGutless = 10,
            variableCrossover = 11,
            randomCrossover = 12,
            uniformCrossover = 13,
            mutateOnly = 14,
            variable3Crossover = 15,
//            variableCrossoverM = 16,
//            variableCrossoverMessed = 17,
            randomCrossoverG = 18,
//            variableCrossoverMG = 19,
//            variableCrossoverGRP = 20,
//            variableCrossoverGFP = 21,
            singlePointBitG = 22,
            twoPointReplacementG = 23,
            gp = 24
};


class BreederBase {
public:
        virtual int generate(vector<crPtr> &pop,int gens,int retain,int breeders,int mutate_chance, 
		int op = variableCrossover,
		bool showAlt = false, 
		int terminate_fitness =0,
		ostream &os=cout) = 0;
        virtual ~BreederBase() {}
};

#endif
