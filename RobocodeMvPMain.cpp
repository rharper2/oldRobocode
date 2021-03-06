#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
using namespace std;


#include "critter.h"
#include <iomanip>
#include "robocodeBeta.h"

#include "spatial.h"
#include "spatialRobocodeCreatureMvP.h"

/*
 Note This is an older file, I have kept it in as a way to show how to evolve the robots against human robots
 *I think the way I am about to propose doing it in the Delta series is better*
 but this kept it simple with the parasites just being a selection of human robots, tougher ones at higher levels
 It uses the Beta grammar (which is more fully fledged in some ways than the Delta grammar, but from memory has a few flaws
 I probably need to tidy up the grammars totally, but apart from this fix up I am intending to move completely to StackGP based
 robots (still evolved through GE SCALP) and so am unlikley to do much more work on this
 */

// Some typedefs to save myself typing and debugging the inevitable spelling msitakes.
typedef spatialWorld<spatialRobocodeCreatureMvP,spatialRobocodeMvPParasite> theWorld;
typedef boost::shared_ptr< theWorld > myWorldPtr; // a boost pointer to myWorld;
typedef theWorld *rawWorldPointer;

extern int currentGen;
void initialiseServerStuff(short);

extern scoreStore storedScores[NUMBEROFPARASITES][SCORESTOSTORE];

ofstream DataOutputFile;

// Each generation we change the working directory
// (where the robots are saved) into the GenXX directory
// off the robocode/robots/ directory (XX = generation number)
// This is where the robocode engine expects to find them

#define STARTING_GEN 290

void changeDirectory() {
	// clean up so robocode doesnt take ages loading all the old robots.
	if (currentGen != STARTING_GEN) {
		system("tar -czf robotsMvPAdvanced.tgz *.java *.class");
		system("rm *.class *.java");
	//	system("rm /Development/robocode/robots/davidalves/Phoenix.data/*"); // just gets too big and its not going to face them again.
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
	system("rm *.class *.java"); // just start them clean!
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
	vector<boost::shared_ptr<location<spatialRobocodeCreatureMvP,spatialRobocodeMvPParasite> > >::iterator vat,vend;
	vat = thePtr->theWorld.begin();
	while (vat != thePtr->theWorld.end()) {
		//cout << "Thinking of saving " << (*vat)->creature->getName() << " and parasite " << (*vat)->parasite->getName() << endl;
		if ((*vat)->creature->getCreature()->isValid()) {
			sprintf(fname,"%s",(*vat)->creature->getName());
			sprintf(jname,"%s.java",fname);
			ofstream robotout;
			robotout.open(jname);
			robocodeBeta::Instance()->print_grammar_individual(robotout,(*vat)->creature->getCreature()->getExpression(),(*vat)->creature->getCreature()->getExpressionLength(),fname,gname);
			robotout.close();
		} else {
			cout << "invalid individual in saveEm\n";
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

// our seed population was a 15x16 world but both 
// parasites and creatures were evolved
// load them together in a 30x16 world to fight
// the common enemies :)

int SizeofX = 12;
int SizeofY = 12;
int GensToDo = 2000;
int ReportY = 6;
int ReportX = 6;


//**********************************************************
// LETS DO IT!
//**********************************************************

void doit() {

 // This is the file the population is saved at, (in each second generation folder)
	const char *fileToUse = "TestPVMRoboCodeVariableParasites";

	// Still using the beta grammar.
	robocodeBeta *grammar = robocodeBeta::Instance();
	cr_data::setGrammar(grammar);
	
	// open it now before we change directories etc
	//ofstream myOutput("PVMSavefile.txt");
	//if (myOutput == NULL) {
	//	cout << "Error opening the output text file\n";
	//	exit(1);
	//}
	
	
	// We also need to provided to iterators "at" and "end" to allow the 
	// the locations in a world to be iterated.
	// at and end need to be iterators to the locations in myWorld
	
	vector<boost::shared_ptr<location<spatialRobocodeCreatureMvP,spatialRobocodeMvPParasite> > >::iterator vat,vend;
	
	
	// Size and number of layers **********
	// currentGen is a global declared in spatialRobocodeCreatureMvP.cpp
	// it is used to make sure that each generation is put in its own directory
	// since we "save" robots when they are created (as well as before each battle)
	// we need to set up the CurrentWorkingDirectory to the right one before
	// we create the spacial world.
	
	currentGen = STARTING_GEN;
	
	// As an aside: the reason we save robots in the creation of them (as well as before each calcScore
	// is that when a layer is added it is added mid-update and we need to calcScore
	// for the new layer as update trickles down (or we are left with a layer with all zero (or random)
	// scores (being the new one added at the bottom) (at the time of writing this spatial.h line 1063)
	// So by ensuring calcScore compiles the robots
	// If we add the robots when they are created then they are batch comiled at calcScore
	// Note that with layers I might need to look at this - as it might be we are recompiling ALL the robots
	// (in a generation) each time a layer's competitions are being run.
	// I am not worrying about this just now as the compilation takes a few seconds and  
	// the battles take hours for each generation, once we reach this stage.
	
	
	layerLives.push_back(40);
	layerLives.push_back(70);
	layerLives.push_back(130);
	layerLives.push_back(258);
	layerLives.push_back(-1);
	
	myWorldPtr aWorld;
	
	// Do we want to creat a new world or load an old one
	// If we are loading put in the number of layers to load and the base file name
	
	bool noload = false;
	if (noload) {
		aWorld.reset(new spatialWorld<spatialRobocodeCreatureMvP,spatialRobocodeMvPParasite>(SizeofX,SizeofY));
	} else {
		 aWorld.reset(spatialWorld<spatialRobocodeCreatureMvP,spatialRobocodeMvPParasite>::makeWorldFromLoadFile(SizeofX,SizeofY,3,fileToUse));
	}
	
	// okay world created/loaded, lets get started.
	
	changeDirectory();
	initialiseServerStuff(9000);
	
	for (currentGen=STARTING_GEN;currentGen<GensToDo;currentGen++) {
		changeDirectory();
		saveEm(aWorld.get());
		cout << "Calc\n";
		aWorld->calcScore();
		cout << "Update\n";
		reportSome(aWorld.get());
		aWorld->update();
		if (currentGen%2) {
			aWorld->savePop(fileToUse);
		}
		cout << "======================== BEST SO FAR ==================\n";
		for (int i=0;i<NUMBEROFPARASITES;i++) {
			cout << spatialRobocodeMvPParasite::parasiteName[i] << endl;
			for (int j=0;j<SCORESTOSTORE;j++) {
				cout << storedScores[i][j].name << ": " << storedScores[i][j].score <<
				" v " << storedScores[i][j].parasiteScore << "= " << storedScores[i][j].percentage << "% at gen " << 
				storedScores[i][j].generation << " age = " << storedScores[i][j].age << endl;
			}
		}
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
				cout << setw(14) << left <<  (*at)->theWorld[y*SizeofY+x]->parasite->getParasite() << " ";
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
	cr_data::setGrammar(robocodeBeta::Instance());
	crPtr currentCritter;
	for (int i = 0;i< 10;i++) {
		cout << "Making number " << i << endl;
		currentCritter.reset(new cr_data());
		currentCritter->makeptc2(i+1,i*2);
		cout << currentCritter->display();
		cout << "Number " << i << " made " << endl;
	}
}

int main (int argc, char * const argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
	myrandomize();
	DataOutputFile.open("ParasiteComprisonData.csv");
	doit();
	DataOutputFile.close();
	//testSome();
    return 0;
}
