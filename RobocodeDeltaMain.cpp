//
//  RobocodeDeltaMain.cpp
//  Robocode
//
//  Created by robin on 8/05/13.
//
//

// Main class for the Delta Grammar.

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
using namespace std;


#include "critter.h"
#include <iomanip>
#include "robocodeDelta.h"



int main (int argc, char * const argv[]) {
    // insert code here...
    robocodeDelta *grammar = robocodeDelta::Instance();
	cr_data::setGrammar(grammar);
    // the initial test then is just to initialise one (or a number) and print them out to see.
    crPtr toMake;
    toMake.reset(new cr_data);
    toMake->makeptc2(10);
    cout << toMake->display();
    
    
    
}