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
    string fileName="ResultsofTopNowWith3SetsR2.csv";
    ofstream combinedFile;
    ifstream currentFile;
    string myline;
    combinedFile.open("/Development/ControlGPF1.csv");
    for (int i=start;i<=stop;i+=skip) {
        changeToGen(i);
        currentFile.open(fileName.c_str());
        getline(currentFile,myline);
        combinedFile << myline << endl;
        cout << "Gen " << i << myline << endl;
        currentFile.close();
    }
    combinedFile.close();
    combinedFile.open("/Development/ControlGPF2.csv");
    for (int i=start;i<=stop;i+=skip) {
        changeToGen(i);
        currentFile.open(fileName.c_str());
        getline(currentFile,myline);
        getline(currentFile,myline);
        getline(currentFile,myline);
        combinedFile << myline << endl;
        
        cout << "Gen " << i << myline << endl;
        currentFile.close();
    }
    combinedFile.close();
    combinedFile.open("/Development/ControlGPF3.csv");
    for (int i=start;i<=stop;i+=skip) {
        changeToGen(i);
        currentFile.open(fileName.c_str());
        getline(currentFile,myline);
        getline(currentFile,myline);
        getline(currentFile,myline);
        getline(currentFile,myline);
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
    letsCombine(4,100,5);
    exit(1);
    
	// Some user parameters
	int RunsToDo = 25;
	
	// The GensToRunFor - is a more of a "give up if you reach this number".
    
	int GensToRunFor = 100;
	
    int MutationRate = 2000;
    
    
    const char *baseFile = "RoboSaver";
	
	// Calculate some other paramaters from the data given
	int PopulationSize = 1000;
	
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
    
    // ******* Temporary hijace of this main file!
    /*//for (int i=0;i<5;i++) {
    {   theHandler.setPopulationSize(200);
        theHandler.setGen(577);
        theHandler.changeDirectory();
        string theFile;
      //  if (i<5) theHandler.fileLoad("savedPop.pop");
        //else
        theHandler.spatialFileLoad("NewTest2.layer3.pop");
        theHandler.workOutTheBestFewAndReallyTestThem();
    }
    */
    
    //******* Temporary hijack of this main file!
    PopulationSize=1000;
    theHandler.setPopulationSize(PopulationSize);
    for (int i=99;i>2;i-=5) {
//        if (i>1001) theHandler.setPopulationSize(1000);
        theHandler.setGen(i);
        theHandler.changeDirectory();
        string theFile;
        theHandler.fileLoad("RoboSaverrun2.csv");
        theHandler.workOutTheBestFewAndReallyTestThem();
    }
    
    
    exit(1);
    
    
    
    
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
        if (count == 0) {
            gen = 65;
            theHandler.setGen(gen);
            theHandler.changeDirectory();
            theHandler.fileLoad(fname);
        }
        cout << "\nGenerated the population, lets see how we go...\n";
        sprintf(fname,"%sRun%dGen%d",baseFile,count,gen);
        for (;gen < GensToRunFor;gen++) {
            theHandler.setGen(gen);
            cout << "Generation: " << gen;
            theHandler.breed();
        }
        cout << "All for that gendone.\n";
    }
}



