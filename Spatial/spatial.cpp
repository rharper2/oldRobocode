/*
 *  spatial.cpp
 *  GrammarTest
 *
 *  Created by Robin Harper on 15/08/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "spatial.h"
#include <iostream>
#include <iomanip>
#include "singlePopHandler.h"

using namespace std;

/************************************************************************
	selectOne (of 16)
*************************************************************************/
 
int locationCount = 0;
int serialNo = 0;
vector<int> layerLives;

int selectOne(int choices) {
        if (choices > 16) {
			  choices = 16;
//            cout << "Caution, the neighbour hood size is greater than 16, I am using bit shifting ";
//            cout << "to select amongst the neighbours, you will need to examine the code, ";
//            cout << "verify it has sufficient bits per int (or change int to long) to use ";
//            cout << "that many neighbours (oh and remove this warning and exit instruction)\n";
//            exit(1);
        }
        int max = 0x01 << (choices-2);
       
         // I am using an efficient way (I think) to implement the ranking system 
        // the idea is the best one has a .5 chance of being chosen
        // the next one .5^2, then .5^3 - an easy way of doing this would just be to 
        // the random number gen with a 50% chance each time until you get it or are at last
        // one - but this is a lot of calls which takes time
        // Better (probably) to call it once and get a number up to max
        // then check the first bit if its set select, shift right and repeat
        // each time a 50% chance of it being selected.
        
        int number = randomint(max) | max; 
        
        // OR (|) with max to set lhs bit of last one. So we can be sure
        // That (e.g. we roll a zero) the last one is selected.
        // cout << "Maximum is " << max << " Number was " << oct << number << dec << " ";
        
        int count = 0;
        while (! (number & 0x01)) {
            count++;
            number = number >> 1;
        }
        //        cout << "Selecting " << count << endl;
        return count;
}
