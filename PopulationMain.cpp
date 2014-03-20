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

int main (int argc, char * const argv[]) {
	
	//****** The following initialisation code is all GE run specific
    // if you are using a different system you will have your own stuff
    // to initialise.
    myrandomize();
    
	// Some user parameters
	int RunsToDo = 25;
	
	// The GensToRunFor - is a more of a "give up if you reach this number".
    
	int GensToRunFor = 100;
	
    int MutationRate = 2000;
    
    
    const char *baseFile = "RoboSaver";
	
	// Calculate some other paramaters from the data given
	int PopulationSize = 500;
	
    cr_data::setGrammar(robocodeDelta::Instance());

    // not setting the interpreter, RoboPopHandler takes care of scoring.
	
    
    // This is just to save stuff for later analysis.
    //	ofstream tout;
    //	tout.open(fileToUse);
	
	int StartRun = 0;
	int EndRun = StartRun + RunsToDo;
	
	
    cr_data::higherIsBetter = true; // again RoboPopHandler will ignore this.
	cr_data::COMPARISONIMPACT = 1000; // start to prefer identically scoring critters
	// when the useful dna is over this amount.
	
    char fname[120];

	RoboPopHandler theHandler;
    theHandler.setPopulationSize(PopulationSize);
    theHandler.setGen(0);
    DWHandler::initialiseServerStuff(9000);
	// ************** Tell them the system we are running, just in case its not what they want.
	
	cout << "****************************\n";
	cout << "Starting Run at run number: " << StartRun << " and ending at: " << EndRun-1 << " for a total of " << (EndRun - StartRun) << " runs. " << endl;
    std::cout << "PopulationSize "<< PopulationSize <<  " will run for a maximum (per run) of " << GensToRunFor << " generations, " << (EndRun - StartRun) << " runs.\n";
	std::cout << endl;
	std::cout << "Data saved to variations of " << baseFile << endl;
	
	int count = 0;
    for (count=StartRun;count<EndRun;count++) {
        
		sprintf(fname,"%srun%d.csv",baseFile,count);
        cout << "Generating population for run " << count << " please be patient..\n";
        theHandler.rampedptc2();
        theHandler.setFileName(fname);
        int gen =0;
        cout << "\nGenerated the population, lets see how we go...\n";
        sprintf(fname,"%sRun%dGen%d",baseFile,count,gen);
        for (gen = 0;gen < GensToRunFor;gen++) {
            theHandler.setGen(gen);
            cout << "Generation: " << gen;
            theHandler.breed();
        }
        cout << "All for that gendone.\n";
    }
}



