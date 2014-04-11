//
//  RoboClassMain.cpp
//  Robocode
//
//  Created by robin on 11/04/2014.
//
//
//
//  PopulationMain.cpp
//  Robocode
//
//  Created by robin on 20/03/2014.
//
//


#include <iostream>

#include <boost/shared_ptr.hpp>
#include <iostream>
#include "critter.h"
#include "DeltaWrappedCritter.h"
#include "RoboPopHandler.h"
#include "robocodeDelta.h"



using namespace std;

void changeToGen(int gen) {
    char dirname[200];
	sprintf(dirname,"/Development/robocode/robots/Gen%d",gen);
    chdir(dirname);
	
}

void letsCombine(int start,int stop, int skip) {
    string fileName="ResultsofTop.csv";
    ofstream combinedFile;
    ifstream currentFile;
    string myline;
    combinedFile.open("/Development/CombinedResultsCoEvolution.csv");
    for (int i=start;i<=stop;i+=skip) {
        changeToGen(i);
        currentFile.open(fileName.c_str());
        getline(currentFile,myline);
        combinedFile << myline << endl;
        cout << "Gen " << i << myline << endl;
        currentFile.close();
    }
    combinedFile.close();
}

int main (int argc, char * const argv[]) {
	
	//****** The following initialisation code is all GE run specific
    // if you are using a different system you will have your own stuff
    // to initialise.
    
    
    
    
    myrandomize();
    
    // we want to be able to add to the population, named critters, which will then be rated.
    
    const char *baseFile = "RoboSaver";
	
    
    cr_data::setGrammar(robocodeDelta::Instance());
    
    // not setting the interpreter, RoboPopHandler takes care of scoring.
	
    
    // This is just to save stuff for later analysis.
    //	ofstream tout;
    //	tout.open(fileToUse);
	
	cr_data::higherIsBetter = true; // again RoboPopHandler will ignore this.
	cr_data::COMPARISONIMPACT = 1000; // start to prefer identically scoring critters
	// when the useful dna is over this amount.
	
    char fname[120];
    char dirname[200];
	sprintf(dirname,"/Development/robocode/robots");
    chdir(dirname);
    
	LightHandler theHandler;
    theHandler.clearPop();
    theHandler.addRobot("Gen.Critterxxx");
    
    
    DWHandler::initialiseServerStuff(9000);
    
    theHandler.ReallyTestThem();
    
    }
    
    
    exit(1);
}



