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

#include "spatial.h"
#include "spatialRobocodeCreatureDelta.h"

// Some typedefs to save myself typing and debugging the inevitable spelling msitakes.
typedef spatialWorld<spatialRobocodeCreatureDelta,spatialRobocodeParasiteDelta> theWorld;
typedef boost::shared_ptr< theWorld > myWorldPtr; // a boost pointer to myWorld;
typedef theWorld *rawWorldPointer;

extern int currentGen;
void initialiseServerStuff(short); // defined in spatialRobocodeCreatureDelta.



// Each generation we change the working directory
// (where the robots are saved) into the GenXX directory
// off the robocode/robots/ directory (XX = generation number)
// This is where the robocode engine expects to find them


void changeDirectory() {
	// clean up so robocode doesnt take ages loading all the old robots.
	if (currentGen != 0) {
		system("tar -czf robotsThetaAdvanced.tgz *.java *.class");
		system("rm *.class *.java");
	}
    
	char dirname[200];
	sprintf(dirname,"/Development/robocode/robots/Gen%d",currentGen);
	// if (opendir(dirname)==NULL) {
	//		printf("Errno %d\n",errno);
	printf("Creating directory %s\n",dirname);
	int j;
	j=mkdir(dirname,0775);
	if (!j) {
		int en=errno;
		printf("Mkdir returned %d, with errno %d\n",j,en);
	}
	//	} else {
	//		cout << "Directory already existed odd \n";
	//	}
	chdir(dirname);
}

// goes through the world (and each layer recursively)
// saving the phenotype (as a .java) file of each critter and parasite
// into the current working directory (which has been set up by changeDirectory)

void saveEm(theWorld *thePtr) {
	char fname[200];
	char jname[200];
	char gname[200];
	sprintf(gname,"Gen%d",currentGen);
	if (thePtr->getPreviousLayer()) saveEm(thePtr->getPreviousLayer());
	vector<boost::shared_ptr<location<spatialRobocodeCreatureDelta,spatialRobocodeParasiteDelta> > >::iterator vat,vend;
	vat = thePtr->theWorld.begin();
	while (vat != thePtr->theWorld.end()) {
		//cout << "Thinking of saving " << (*vat)->creature->getName() << " and parasite " << (*vat)->parasite->getName() << endl;
		if ((*vat)->creature->getCreature()->isValid()) {
			sprintf(fname,"%s",(*vat)->creature->getName());
			sprintf(jname,"%s.java",fname);
			ofstream robotout;
			robotout.open(jname);
			robocodeDelta::Instance()->print_grammar_individual(robotout,(*vat)->creature->getCreature()->getExpression(),(*vat)->creature->getCreature()->getExpressionLength(),fname,gname);
			robotout.close();
		} else {
			cout << "invalid individual in saveEm\n";
			exit(1);
		}
		if ((*vat)->parasite->getParasite()->isValid()) {
			sprintf(fname,"%s",(*vat)->parasite->getName());
			sprintf(jname,"%s.java",fname);
			ofstream robotout;
			robotout.open(jname);
			robocodeDelta::Instance()->print_grammar_individual(robotout,(*vat)->parasite->getParasite()->getExpression(),(*vat)->parasite->getParasite()->getExpressionLength(),fname,gname);
			robotout.close();
		} else {
			cout << "invalid parasite in saveEm\n";
			exit(1);
		}
		
		
		vat++;
	}
}

// highly descriptive function name - err it does it.
// some user parameters here, bascially the file you want the population saved to
// (its saved periodically - every two generations)
// the size of the layers and the layer lives (oh and the server socket, you will need to alter the clients
// if you alter that.

// ReportY and ReportX are lazy globals, basically controls the height and width of the print out
// on the console - useful just to check if anything appears to be going wrong.
// I probably spent far more time on the formatting than I should have.

void reportSome(rawWorldPointer currentWorld);

int SizeofX = 20;
int SizeofY = 20;
int GensToDo = 800;
int ReportY = 10;
int ReportX = 5;
void doit() {
    
    // This is the file the population is saved at, (in each second generation folder)
	const char *fileToUse = "TestRoboCodeDelta";
    
	
	robocodeDelta *grammar = robocodeDelta::Instance();
	cr_data::setGrammar(grammar);
	
	
	
	// We also need to provided to iterators "at" and "end" to allow the
	// the locations in a world to be iterated.
	// at and end need to be iterators to the locations in myWorld
	
	vector<boost::shared_ptr<location<spatialRobocodeCreatureDelta,spatialRobocodeParasiteDelta> > >::iterator vat,vend;
	
	
	// Size and number of layers **********
	// currentGen is a global declared in spatialRobocodeCreature.cpp
	// it is used to make sure that each generation is put in its own directory
	// since we "save" robots when they are created (as well as before each battle)
	// we need to set up the CurrentWorkingDirectory to the right one before
	// we create the spacial world.
	int startGen = 117;
	currentGen = startGen;
	
	// As an aside: the reason we save robots in the creation of them (as well as before each calcScore
	// is that when a layer is added it is added mid-update and we need to calcScore
	// for the new layer as update trickles down (or we are left with a layer with all zero (or random)
	// scores (being the new one added at the bottom) (at the time of writing this spatial.h line 1063)
	// So by ensuring calcScore compiles the robots
	// If we add the robots when they are created then they are batch comiled at calcScore
	// Note that with layers I might need to look at this - as it might be we are recompiling ALL the robots
	// (in a generation) each time a layer's competitions are being run.
	// I am not worrying about this just now as the compilation takes a few seconds and I think
	// the battles will take hours for each generation.
	
	
	layerLives.push_back(40);
	layerLives.push_back(70);
	layerLives.push_back(130);
	layerLives.push_back(258);
	layerLives.push_back(-1);
	
	myWorldPtr aWorld;
	changeDirectory();
	
	//aWorld.reset(new spatialWorld<spatialRobocodeCreatureDelta,spatialRobocodeParasiteDelta>(SizeofX,SizeofY));
	aWorld.reset(spatialWorld<spatialRobocodeCreatureDelta,spatialRobocodeParasiteDelta>::makeWorldFromLoadFile(SizeofX,SizeofY,3,fileToUse));
	reportSome(aWorld.get());
	initialiseServerStuff(9000);
	
	for (currentGen=startGen;currentGen<GensToDo;currentGen++) {
		changeDirectory();
		saveEm(aWorld.get());
		cout << "Calc\n";
		aWorld->calcScore();
		cout << "Update\n";
		reportSome(aWorld.get());
		aWorld->update();
		if (currentGen%2) aWorld->savePop(fileToUse);
	}
	
	
    
	cout << "All done.\n";
    
}


void reportSome(rawWorldPointer currentWorld) {
	cout << endl << "**************** GENERATION " << currentGen << " ********************************\n";
	list<rawWorldPointer> layerList;
	layerList.clear();
	while (currentWorld) {
		layerList.push_front(currentWorld);
		currentWorld = currentWorld->getPreviousLayer();
	}
	list<rawWorldPointer>::iterator at;
	for (int y=0;y<ReportY;y++) {
		at = layerList.begin();
		while (at!=layerList.end()) {
			for (int x=0;x<ReportX;x++) {
				cout << setw(14) << left <<  (*at)->theWorld[y*SizeofY+x]->parasite->getName() << " ";
			}
			cout << " ** ";
			at++;
		}
		cout << endl;
		at = layerList.begin();
		while (at!=layerList.end()) {
			for (int x=0;x<ReportX;x++) {
				cout << setw(14) << left <<  (*at)->theWorld[y*SizeofY+x]->creature->getName() << " ";
			}
			cout << " ** ";
			at++;
		}
		cout << endl;
		at = layerList.begin();
		while (at!=layerList.end()) {
			for (int x=0;x<ReportX;x++) {
				cout << "ScoreCR: " << setw(5) << (*at)->theWorld[y*SizeofY+x]->creature->getScore() << " ";
			}
			cout << " ** ";
			at++;
		}
		cout << endl;
		at = layerList.begin();
		while (at!=layerList.end()) {
			for (int x=0;x<ReportX;x++) {
				cout << "ScorePA: " << setw(5) << (*at)->theWorld[y*SizeofY+x]->parasite->getScore() << " ";
			}
			cout << " ** ";
			at++;
		}
		cout << endl;
		for (int i=0;i<layerList.size();i++) {
			for (int i=0;i<ReportX;i++) cout << "               ";
			cout << " ** ";
		}
		cout << endl;
		for (int i=0;i<layerList.size();i++) {
			for (int i=0;i<ReportX;i++) cout << "               ";
			cout << " ** ";
		}
		cout << endl;
	}
	for (int i=0;i<layerList.size();i++) {
		for (int i=0;i<ReportX;i++) cout << "***************";
		cout << " ** ";
	}
	cout << endl;
	at = layerList.begin();
	for (int i=0;i<layerList.size();i++) {
		for (int i=0;i<ReportX;i++) cout << "               ";
		cout << " ** ";
	}
	cout << endl;
	at = layerList.begin();
	while (at != layerList.end()) {
		cout << "Layer:     " << setw(4) << (*at)->getLayerNo();
		for (int i=0;i<ReportX-1;i++) cout << "               ";
		cout << " ** ";
		at++;
	}
	cout << endl;
	at = layerList.begin();
	while (at != layerList.end()) {
		cout << "Age: (CR): " << setw(4) << (*at)->theWorld[SizeofY+1]->creature->geneticAge;
		for (int i=0;i<ReportX-1;i++) cout << "               ";
		cout << " ** ";
		at++;
	}
	cout << endl;
	at = layerList.begin();
	while (at != layerList.end()) {
		cout << "Age: (PA): " << setw(4) << (*at)->theWorld[SizeofY+1]->creature->geneticAge;
		for (int i=0;i<ReportX-1;i++) cout << "               ";
		cout << " ** ";
		at++;
	}
	cout << endl;
	
	
}


void testSome() {
    robocodeDelta *grammar = robocodeDelta::Instance();
	cr_data::setGrammar(grammar);
    // the initial test then is just to initialise one (or a number) and print them out to see.
    
    crPtr critter;
    int count = 0;
    for (int i=0;i<1000;i++) {
        cout << ".";
        //cout << "Looping\n";
        if (i%50 == 0) cout << endl;
        int maxTree = randomint(8)+240;
        critter.reset(new cr_data);
        //    cout << "Reset\n";
        critter->makeptc2(maxTree);
        //    cout << "Made\n";
        bool done = critter->isValid();
        //    cout << "Checking\n";
        if (!done ) {
            cout << "Failed to make one";
            count++;
            cout << critter->display();
            return;
        }
    }
    cout << "finished, with a count of " << count;
    
}

int main (int argc, char * const argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
	doit();
	//testSome();
    return 0;
}



