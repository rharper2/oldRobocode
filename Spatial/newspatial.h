/*
 *  spatial.h
 *  GrammarTest
 *
 *  Created by Robin Harper on 15/08/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SPATIALWORLD
#define SPATIALWORLD

#include "critter.h"
#include <iostream>
#include <iomanip>

#include <boost/shared_ptr.hpp>

int selectOne(int);
extern int locationCount;
extern vector<int> layerLives;
// ***** Foreword ******
// On reflection I am not sure why I did this using templates rather than just a base class
// But it works and is kind of neat.
// You need to write two template classes which provide the relevant methods for both the parasite 
// and the creature - they basically encapsulate the type of creature and parasite you need.
// lets assume you were writing a new creature type and a new parasite type of types
// yourCreatureType and yourParasiteType
// the classes would need to encapsulate the following methods:
//		(THIS IS OUT OF DATE)
//      A default constructor
//      void doCrossover(yourCreatureType&) = replaces the creature/parasite with the result of the crossover
//      double getScore(yourParasiteType &)  - or double getScore(yourCreatureType &) for the parasite
//      static bool isBetterThan(double,double) - give the two score returns true if the lhs "is better than" the rhs 
//
// I think thats it.
//
// They would also likely have some form of pointer to the underlying cr_data for a creature type and whatever
// data you need for the test cases in parasite (could be more cr_data for a competitive evolving environment).
// Then you create a spatial world by the following:

// ******************************************************************
// spatialWorld<yourCreatureType,yourParasiteType> newSpatialWorld(rows,columns);
//*******************************************************************
//
// See the examples for more detail!
        
// Extensive use made of the STL library and the boost::shared_ptr framework.

// Location is a location in the spatial world
// It contains ptrs to the creature, the parasite and scratchpad
// storage for scores and creatures being created.
// Finally it contains a vector of all the neighbourhood locations.


template <class creatureType, class parasiteType> class location {
public:
    int id;
	int locationNumber;
	boost::shared_ptr<creatureType> creature;
    boost::shared_ptr<parasiteType> parasite;
    double creatureScore,parasiteScore;
    boost::shared_ptr<creatureType> newCreature;
    boost::shared_ptr<parasiteType> newParasite;
    vector<boost::shared_ptr<location <creatureType,parasiteType> > > *neighbours;
    vector<boost::shared_ptr<location <creatureType,parasiteType> > > *previousLayer;
    vector<boost::shared_ptr<location <creatureType,parasiteType> > > *neighboursAndPrevious;
	void displayPtrs();
    void display()  { cout << setw(4) << id << " [";
					  cout << setprecision(4) << setw(5) << creatureScore << ":";// << setw(5) << creature->theNetwork->getNumberOfInputsConnected(); 
					  cout << " {" <<  setw(3) << creature->geneticAge << "} ";
					 // cout << " [" << setw(6) << (creature->getCreature()->mySerial) << "]"; 
					  cout << "  ( " << setw(5) << setprecision(4) << parasite->getParasite() << ": ";// << setw(5) << parasite->theNetwork->getNumberOfInputsConnected();
					  cout << " {" << setw(3) << parasite->geneticAge << "} ";
					 // cout << " [" << setw(6) << (parasite->getParasite()->mySerial) << "]"; 
					  cout  << " )] ";
					 // if (!(creatureScore == creature->theNetwork->getNumberOfInputsConnected())) cout << "\nProblem in location " << locationNumber << "creature score mismatch.\n";
					 // if (parasiteScore != parasite->theNetwork->getNumberOfInputsConnected()) cout << "\nProblem in location " << locationNumber << "parasite score mismatch.\n";
					  }
    void displayNew()  { cout << setw(4) << id << " [" << setw(6) << (newCreature?newCreature->getCreature()->mySerial:0) << "  ( " << setw(5) <<  (newParasite?newParasite->getParasite()->mySerial:0) << ")] ";}
    location() {
		neighbours = new vector<boost::shared_ptr< location<creatureType,parasiteType> > >;
		//previousLayer = new vector<boost::shared_ptr< location<creatureType,parasiteType> > >;
		neighboursAndPrevious = new vector<boost::shared_ptr< location<creatureType,parasiteType> > >;
		id = locationCount++;
    }
    ~location() {
//		cout << "Location " << locationNumber << "( " << id << ") delted.\n";
		if (neighbours) neighbours->clear();
		if (previousLayer) previousLayer->clear();
		if (neighboursAndPrevious) neighboursAndPrevious->clear();
        delete neighbours;
		delete previousLayer;
		delete neighboursAndPrevious;
    }

};


template< class creatureType, class parasiteType> void location<creatureType,parasiteType>::displayPtrs() {
	cout << "**** Location: " << locationNumber << " : *****\n";
	cout << "Neighbours: ";
	for (int i=0;i<neighbours->size();i++) {if (i%3 == 0) cout << endl; cout << "[" << setprecision(1) << setw(2)<< (*neighbours)[i]->id << "]" << setprecision(4) << setw(5)<< (*neighbours)[i]->locationNumber << ", "; }
	cout << endl;
	cout << "Previous Layer ";
	if (previousLayer) {
		for (int i=0;i<previousLayer->size();i++) { if (i%3 == 0) cout << endl; cout << "[" << setprecision(1) << setw(2)<< (*previousLayer)[i]->id << "]" << setprecision(4) << setw(5)<< (*previousLayer)[i]->locationNumber << ", "; }
		cout << endl;
	} else cout << "None.\n";
	cout << "Previous and Neighbours.\n";
	if (neighboursAndPrevious) {
		for (int i=0;i<neighboursAndPrevious->size();i++) { if (i%3 == 0) cout << endl;cout << "[" << setprecision(1) << setw(2)<< (*neighboursAndPrevious)[i]->id << "]" << setprecision(4) << setw(5)<< (*neighboursAndPrevious)[i]->locationNumber << ", "; }
		cout << endl;
	} else cout << "None.\n";
}
	

// spatial world is, in effect, a vector of locations, representing the world.
// There are some methods implementing the evolutionary 
// algorithms:
// void calcScore() - calculate the scores for creatures/parasites
// void selectBestCritter() - place a random critter/parasite from the neighbourhood
//                          - (chance weighted by score) into the newCreature/newParasite slot
// void doCrossover() - call the doCrossover for a percentage of each newCreature/newParasite
// void update() - replace the creature/parasite with the newCreature/newParasite.

/************************************************************************
	spatialWorld
*************************************************************************/
template <class creatureType,class parasiteType> class spatialWorld {
    int xSize,ySize;
    int totalPop;
	int layerNo;
	int generationLife;
	spatialWorld<creatureType,parasiteType> *previousLayer;
	boost::shared_ptr< spatialWorld<creatureType,parasiteType> > expansionPreviousLayer;
    void setUpNeighbours(int x,int y);
	bool terminated;
public:
    vector<boost::shared_ptr<location<creatureType,parasiteType> > > theWorld;
    spatialWorld(int x,int y,int genLife);
	spatialWorld(spatialWorld<creatureType,parasiteType> *prev,int genLife);
	spatialWorld(int x,int y);
    ~spatialWorld();
//static vector<int> layerLives;
	void doAGeneration();
	bool terminate() { return terminated; }
//	void checkit();
	bool loadPop(const char *fname);
	bool savePop(const char *fname);
    void display();
 
private:
	void addBottomLayer();
    void calcScore();
	void clearAllScores();
	void clearAllLayeredScores();
	void retrieveScores();
    void selectBestCritters();
    void doCrossover();
	void preliminaryUpdate(bool test = false);
    void update();
    void doMutation();
    void bestRealScore();
	bool isItAnExpandingSpatialWorld;
	int getLayerNo() { return layerNo; }


};

/************************************************************************
	generateLocationEntities
*************************************************************************/

template <class creatureType, class parasiteType> class generateLocationEntities:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
public:
    generateLocationEntities() {}
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
        // generate one new creatureType and one new ParasiteType
        // and place in the location pointed to by p.
		p->creatureScore = p->parasiteScore = 0;
		p->creature.reset(new creatureType());
        p->parasite.reset(new parasiteType(true));
        p->newCreature.reset(new creatureType(false)); // i.e. create the creatureType class but not the underlying creature.
        p->newParasite.reset(new parasiteType(false));
		cout<<".";
    }
};


/************************************************************************
	copyOverPreviousNeighbours
*************************************************************************/
template <class creatureType, class parasiteType> class copyOverPreviousNeighbours:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
	spatialWorld<creatureType,parasiteType> *worldToCopy;
public:
    
	 copyOverPreviousNeighbours(spatialWorld<creatureType,parasiteType> *wToC=NULL): worldToCopy(wToC) { }
   
	 void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
		// Assume neighbourhood is correctly initialised for this layer
		// If there is a prevous layer (stored in wToC), then "previousLayer" becomes a copy of the neighbourhoodAndPrevious of the
		// location (determined by p.location) of this "location" in the previous layer. And yep that sentence was probably more
		// complicated than it really should be.
		if (worldToCopy != NULL) {
			 vector<boost::shared_ptr<location <creatureType,parasiteType> > > *tocopy;
//			 tocopy = worldToCopy->theWorld[p->locationNumber]->neighboursAndPrevious;
			 tocopy = worldToCopy->theWorld[p->locationNumber]->neighbours;
			 // so tocopy now points at the vector we want to copy
			 p->previousLayer = new vector<boost::shared_ptr< location<creatureType,parasiteType> > >(*tocopy);
		} else {
			p->previousLayer = new vector<boost::shared_ptr< location<creatureType,parasiteType> > >;
		}
		// there is no magic about using a back_insert_iterator, the order of locations is, really, not important especially
		// given that they will be shuffled about when we are deciding which one to pick each generation.
		p->neighboursAndPrevious->clear();
		std::back_insert_iterator< vector<boost::shared_ptr<location <creatureType,parasiteType> > > > combinedInserter((*(p->neighboursAndPrevious)));

		copy( p->neighbours->begin( ), p->neighbours->end( ), combinedInserter );
		copy( p->previousLayer->begin( ), p->previousLayer->end( ), combinedInserter );
	}
};



/************************************************************************
	spatialWorld(int x, int y)
*************************************************************************/

template <class creatureType, class parasiteType> spatialWorld<creatureType,parasiteType>::spatialWorld(int x,int y,int genLife) {
    typedef boost::shared_ptr<location<creatureType,parasiteType> > locPtr;
    typedef vector<locPtr> locVector;
	isItAnExpandingSpatialWorld = false;
	// default is false this will be overwritten for the most senior layer (and importantly only the senior layer)
	// if it is an expandingSpatialWorld
	terminated = false;
    xSize = x;
    ySize = y;
	generationLife = genLife;
    totalPop = x*y;
	layerNo = 0;
	previousLayer = NULL;
    setUpNeighbours(x,y);
    cout << "Set up a world, the size is now " << theWorld.size() << endl;
	if (genLife > 0) cout << "The generational life of this layer is: " << genLife << endl;
	else cout << "This layer has no life limit.\n";
	// we still need to call copyOverPrevious - to set up the neighboursAndPrevious correctly.
	copyOverPreviousNeighbours<creatureType,parasiteType> doCopy(NULL);
	for_each(theWorld.begin(),theWorld.end(),doCopy);
    // Now we need to populate
    for_each(theWorld.begin(),theWorld.end(),generateLocationEntities<creatureType,parasiteType>());
    cout << "\nAfter its populated it has a size of " << theWorld.size() << endl;
  
    
}


template <class creatureType, class parasiteType> spatialWorld<creatureType,parasiteType>::spatialWorld(int x,int y) {
    typedef boost::shared_ptr<location<creatureType,parasiteType> > locPtr;
    typedef vector<locPtr> locVector;
	isItAnExpandingSpatialWorld = true;
	terminated = false;
	if (layerLives.empty()) {
		cout << "For an expanding worlds, you need to push some life limits into spatialWorld<creatureType,parasiteType>::layerLives.\n";
		cout << "So for example assuming your creatureType was a regressionSpatialCreature and your parasite type was a regressionSpatialParasite.\n";
		cout << "Before you create your first spatial world you might need some code like this.\n";
		cout << "spatialWorld<regressionSpatialCreature,regressionSpatialParasite>::layerLives.push_back(20);\n";
		cout << "spatialWorld<regressionSpatialCreature,regressionSpatialParasite>::layerLives.push_back(100);\n";
		cout << "spatialWorld<regressionSpatialCreature,regressionSpatialParasite>::layerLives.push_back(-1);\n";
		cout << "This would specify that your spatial worlds should add a layer after 20 lives, another after 100 and then stop\n";
		cout << "adding layers (at three layers).\n";
		exit(1);
	}
    xSize = x;
    ySize = y;
	generationLife = layerLives[0];
    totalPop = x*y;
	layerNo = 0;
	previousLayer = NULL;
    setUpNeighbours(x,y);
    cout << "Set up a bottom layer of an expanding layer world, the size is now " << theWorld.size() << endl;
	if (generationLife > 0) cout << "The generational life of this layer is: " << generationLife << endl;
	else cout << "This layer has no life limit.\n";
	// we still need to call copyOverPrevious - to set up the neighboursAndPrevious correctly.
	copyOverPreviousNeighbours<creatureType,parasiteType> doCopy(NULL);
	for_each(theWorld.begin(),theWorld.end(),doCopy);
    // Now we need to populate
    for_each(theWorld.begin(),theWorld.end(),generateLocationEntities<creatureType,parasiteType>());
    cout << "\nAfter its populated it has a size of " << theWorld.size() << endl;
	isItAnExpandingSpatialWorld = true;
}

template <class creatureType, class parasiteType>void  spatialWorld<creatureType,parasiteType>::addBottomLayer() {
    typedef boost::shared_ptr<location<creatureType,parasiteType> > locPtr;
    typedef vector<locPtr> locVector;
	terminated = false;
	if (layerLives.empty()) {
		cout << "Not sure how this can be the case, layerLives was empty when we wanted to add a new bottom layer.\n";
		cout << "For an expanding worlds, you need to push some life limits into spatialWorld<creatureType,parasiteType>::layerLives.\n";
		cout << "So for example assuming your creatureType was a regressionSpatialCreature and your parasite type was a regressionSpatialParasite.\n";
		cout << "Before you create your first spatial world you might need some code like this.\n";
		cout << "spatialWorld<regressionSpatialCreature,regressionSpatialParasite>::layerLives.push_back(20);\n";
		cout << "spatialWorld<regressionSpatialCreature,regressionSpatialParasite>::layerLives.push_back(100);\n";
		cout << "spatialWorld<regressionSpatialCreature,regressionSpatialParasite>::layerLives.push_back(-1);\n";
		cout << "This would specify that your spatial worlds should add a layer after 20 lives, another after 100 and then stop\n";
		cout << "adding layers (at three layers).\n";
		exit(1);
	}
	layerNo++;

	// the next section isn't strictly necessary but it keeps the location id's distinct in each of 
	// the layers, which is useful for debugging as otherwise a location doesn't really know which layer its on.

    typename vector<locPtr>::iterator at;
	at = theWorld.begin();
    while (at != theWorld.end()) {
		(*at)->id += totalPop;
		++at;
	}
	if (previousLayer == NULL) {
		// okay we are at the soon to be second bottom layer
		expansionPreviousLayer.reset(new spatialWorld<creatureType,parasiteType>(xSize,ySize,layerLives[0]));
		// so the shared_ptr points at it now and that means (provided no other shared_ptrs point to it)
		// it will be auto deleted when this world is deleted.
		previousLayer = &(*(expansionPreviousLayer)); // the address of the thing expansionPreviousLayer points to
		// we need to add the neghbourhood locations in the new layer into the approviate place now.
		copyOverPreviousNeighbours<creatureType,parasiteType> doCopy(previousLayer);
		for_each(theWorld.begin(),theWorld.end(),doCopy);
		previousLayer->calcScore(); // prime it as this will occur in the middle of an "update"
	} else previousLayer->addBottomLayer();
	if (layerNo < (int) layerLives.size()) {
		generationLife = layerLives[layerNo];
	} else {
		cout << "Old layerNo is " << layerNo << " but lifeLimits has a size of " << layerLives.size() << " so just giving a life of -1\n";
		generationLife = -1;
	}
	cout << "Layer " << layerNo -1 << " is now layer " << layerNo << " with a new life limit of " << generationLife << endl;
}

   
/************************************************************************
	spatialWorld(spatialWorld<creatureType,parasiteType> *prev,int genLife)
*************************************************************************/

template <class creatureType, class parasiteType> spatialWorld<creatureType,parasiteType>::spatialWorld(spatialWorld<creatureType,parasiteType> *prev,int genLife) {
    typedef boost::shared_ptr<location<creatureType,parasiteType> > locPtr;
    typedef vector<locPtr> locVector;
	isItAnExpandingSpatialWorld = false;
	if (prev == NULL) {
		cout << "SpatialWorld, layer constructor called with a NULL previous layer!\n";
		exit(1);
	}
	terminated = false;
	previousLayer = prev;
    xSize = prev->xSize;
    ySize = prev->ySize;
	generationLife = genLife;
	totalPop = xSize*ySize;
	
	cout << "Okay its a layer world\n";
	layerNo = prev->getLayerNo() + 1;
	cout << "We are layer.." << layerNo << endl;
		
    setUpNeighbours(xSize,ySize);
    
	cout << "Set up a world, the size is now " << theWorld.size() << endl;
    if (genLife > 0) cout << "The generational life limit of this layer is: " << genLife << endl;
	else cout << "This layer has no life limit.\n";
	
    // Now we update the "layer" neighbourhood vectors
	
	copyOverPreviousNeighbours<creatureType,parasiteType> doCopy(prev);
	for_each(theWorld.begin(),theWorld.end(),doCopy);
    // Now we need to populate
	for_each(theWorld.begin(),theWorld.end(),generateLocationEntities<creatureType,parasiteType>());

    cout << "\nAfter this layer is populated it has a size of " << theWorld.size() << endl;
  
    
}


/************************************************************************
	neighbourHoodCreatureCompetitions
*************************************************************************/

template <class creatureType, class parasiteType> class neighbourHoodCreatureCompetitions:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
private:
    boost::shared_ptr<creatureType> thisCreature;
	int locationID;
public:
    neighbourHoodCreatureCompetitions(boost::shared_ptr<creatureType> &cr,int id): thisCreature(cr),locationID(id) {} 
     neighbourHoodCreatureCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
		//cout << "NHCC thisCreature id:" << locationID << " Checked against parasite in " << p->id << endl;
        thisCreature->calcScore(*(p->parasite),locationID);
		//cout << "*** Parasite in location " << p->id << " tested***\n";
    }
};

/************************************************************************
	neighbourHoodParasiteCompetitions
*************************************************************************/

template <class creatureType, class parasiteType> class neighbourHoodParasiteCompetitions:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
private:
    boost::shared_ptr<parasiteType> thisParasite;
	int locationID;
public:
    neighbourHoodParasiteCompetitions(boost::shared_ptr<parasiteType> &pr,int id): thisParasite(pr),locationID(id) {} 
    neighbourHoodParasiteCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
		//cout << "NHPC thisParasite Checked against creature in " << p->id << endl;
        thisParasite->calcScore(*(p->creature),locationID);
		//cout << "*** Creature in location " << p->id << " tested***\n";
    }
};
/************************************************************************
	neighbourHoodPreviousCreatureCompetitions
*************************************************************************/

template <class creatureType, class parasiteType> class neighbourHoodPreviousCreatureCompetitions:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
private:
    boost::shared_ptr<creatureType> thisCreature;
public:
    neighbourHoodPreviousCreatureCompetitions(boost::shared_ptr<creatureType> &pr): thisCreature(pr) {} 
    neighbourHoodPreviousCreatureCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
		//cout << "NHPrevCC Checked parasite in " << p->id << " against that creature " << endl;
        p->parasite->calcScore(*thisCreature,-1);
		//cout << "*** Creature checked again parasite in location " << p->id << " tested***\n";
    }
};


/************************************************************************
	neighbourHoodPreviousParasiteCompetitions
*************************************************************************/

template <class creatureType, class parasiteType> class neighbourHoodPreviousParasiteCompetitions:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
private:
    boost::shared_ptr<parasiteType> thisParasite;
public:
    neighbourHoodPreviousParasiteCompetitions(boost::shared_ptr<parasiteType> &pr): thisParasite(pr) {} 
    neighbourHoodPreviousParasiteCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
		//cout << "Checked creature in " << p->id << " against that parasite " << endl;
        p->creature->calcScore(*thisParasite,-1);
		//cout << "*** Creature in location " << p->id << " tested***\n";
    }
};


/************************************************************************
	sumAndStoreCreatureAndParasiteScores
*************************************************************************/

template <class creatureType, class parasiteType> class sumAndStoreCreatureAndParasiteScores:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
    spatialWorld<creatureType,parasiteType> *previousLayer;

public:
    sumAndStoreCreatureAndParasiteScores(spatialWorld<creatureType,parasiteType> *pl): previousLayer(pl) {}
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> > (&p)) {
		//cout << "CHECKING CREATURE IN LOCATION " << p->id << "being checked against neighbouring parasites." << endl;
		neighbourHoodCreatureCompetitions<creatureType,parasiteType> doCreatureComp(p->creature,p->id);
		for_each(p->neighbours->begin(),p->neighbours->end(),doCreatureComp);
		if (previousLayer) {
			neighbourHoodCreatureCompetitions<creatureType,parasiteType> doCreatureComp(p->creature,-1);
			for_each(p->previousLayer->begin(),p->previousLayer->end(),doCreatureComp);
		}
		if (previousLayer) {
			//cout << "There is a previous layer - about to do a previous check for creature in " << p->id << endl;
			neighbourHoodPreviousCreatureCompetitions<creatureType,parasiteType> doPrevCreatureComps(p->creature);
			for_each(p->previousLayer->begin(),p->previousLayer->end(),doPrevCreatureComps);
		}

		//cout << "CHECKING PARASITE IN LOCATION " << p->id << "against neighbouring creatures." << endl;
		neighbourHoodParasiteCompetitions<creatureType,parasiteType> doParasiteComp(p->parasite,p->id);
		for_each(p->neighbours->begin(),p->neighbours->end(),doParasiteComp);
		if (previousLayer) {
			neighbourHoodParasiteCompetitions<creatureType,parasiteType> doParasiteComp(p->parasite,-1);
			for_each(p->previousLayer->begin(),p->previousLayer->end(),doParasiteComp);
		}
		
		if (previousLayer) {
			//cout << "There is a previous layer. About to do a previous check for parasite in " << p->id << endl;
			neighbourHoodPreviousParasiteCompetitions<creatureType,parasiteType> doPrevParasiteComps(p->parasite);
			for_each(p->previousLayer->begin(),p->previousLayer->end(),doPrevParasiteComps);
		}
		
    }
   
};


/************************************************************************
	clearScores
*************************************************************************/

template <class creatureType, class parasiteType> class clearScores:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
public:
    clearScores() {}
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> > (&p)) {
//		cout << "clearing the score for location " << p->id << endl;
        p->creature->clearScore();
		p->parasite->clearScore();
    }
   
};

/************************************************************************
	clearAllScores
*************************************************************************/

template <class creatureType,class parasiteType> void spatialWorld<creatureType,parasiteType>::clearAllScores() {
 	if (previousLayer != NULL) previousLayer->clearAllScores();
	for_each(theWorld.begin(),theWorld.end(),clearScores<creatureType,parasiteType>());
};


/************************************************************************
	clearLayeredScores
*************************************************************************/

template <class creatureType, class parasiteType> class clearLayeredScores:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
public:
    clearLayeredScores() {}
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> > (&p)) {
//		cout << "clearing the score for location " << p->id << endl;
        p->creature->clearLayeredScore();
		p->parasite->clearLayeredScore();
    }
   
};

/************************************************************************
	clearAllLayeredScores
*************************************************************************/

template <class creatureType,class parasiteType> void spatialWorld<creatureType,parasiteType>::clearAllLayeredScores() {
 	if (previousLayer != NULL) previousLayer->clearAllLayeredScores();
	for_each(theWorld.begin(),theWorld.end(),clearLayeredScores<creatureType,parasiteType>());
};

/************************************************************************
	clearLocations
*************************************************************************/

template <class creatureType, class parasiteType> class clearLocations:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
public:
    clearLocations() {}
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> > (&p)) {
	//	cout << "clearing the location neighbour vectors " << p->id << endl;
     p->neighbours->clear();
    p->previousLayer->clear();
    p->neighboursAndPrevious->clear();
     
	}
   
};

/************************************************************************
	~spatialWorld
*************************************************************************/

template <class creatureType,class parasiteType> spatialWorld<creatureType,parasiteType>::~spatialWorld() {	
    for_each(theWorld.begin(),theWorld.end(),clearLocations<creatureType,parasiteType>());
    theWorld.clear();
    cout << "spatialWorld Deleted\n";
}

/************************************************************************
	retrieveCreatureAndParasiteScores
*************************************************************************/

template <class creatureType, class parasiteType> class retrieveCreatureAndParasiteScores:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
public:
    retrieveCreatureAndParasiteScores() {}
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> > (&p)) {
	//	cout << "Retrieving the score for location " << p->id << endl;
        p->creatureScore = p->creature->getScore();
		p->parasiteScore = p->parasite->getScore();
    }
   
};


/************************************************************************
	retrieveScores
*************************************************************************/

template <class creatureType,class parasiteType> void spatialWorld<creatureType,parasiteType>::retrieveScores() {
 	if (previousLayer != NULL) {
		for_each(previousLayer->theWorld.begin(),previousLayer->theWorld.end(),retrieveCreatureAndParasiteScores<creatureType,parasiteType>());
		// was just previousLayer->retrieveScores(); but we dont want it to recurse down any more
	}
	for_each(theWorld.begin(),theWorld.end(),retrieveCreatureAndParasiteScores<creatureType,parasiteType>());
};


/************************************************************************
	calcScore
*************************************************************************/

template <class creatureType,class parasiteType> void spatialWorld<creatureType,parasiteType>::calcScore() {
	// first off if this is layerworld, get the previous layer to calculate its stuff
	
	if (previousLayer != NULL) {
		previousLayer->calcScore();
		// this will also select best critter and docrossover for previous layers.
		// At the end of this, the critters in the previous layer will have their
		// scores .v. their own layer of parasites stored in storedScore
		// and the scores of them .v. "their" previous layer of parasites in storedLayerScore
		// we need to wipe storedLayerScore as we are no longer interested in the score
		// of critter in Layer-1 .v. parasite in Layer-2
		previousLayer->clearAllLayeredScores();
	}
//	cout << "CALC SCORE FOR LAYER *********************** " << getLayerNo() << endl;
    // for each location call the sumAndStore funcitons
    // which sums the creature/parasite 
	// Clear the scores - as for the current layer, this is the first time we are scoring them
	for_each(theWorld.begin(),theWorld.end(),clearScores<creatureType,parasiteType>());
	// The sum and Store scores.
	// Set up the function so we have access to the previous layer
	sumAndStoreCreatureAndParasiteScores<creatureType,parasiteType> sumAndStore(previousLayer);
	// Iterate through the world calling the function sumAndStoreCreatureAndParasiteScores()
    for_each(theWorld.begin(),theWorld.end(),sumAndStore);
	
	// so at this point - we have each creature and each parasite in each location has their score 
	// .v. the creature and parasite on their own layer in animalspatialCreature/Parasite.storedScore
	// and the score .v. the creature and parasite on the other layer (the one below this one for the current layer)
	// stored in storedLayeredScore
	// we need to move it to the location score, creatureScore and parasiteScore
	// moved to a function so we can call it for previous layer (which will have had its score altered by this layer)
	
    // Side note I have been a bit sceptical about using the algorithms rather than writing my own loops
    // But I think the above lines convince me.
    // Indebted to Scott Meyers, Effective STL pp 158-160 (actually all his three books - all pages!)
	
	// Previously (before we layered) we nicely split up calc score
	// from selection and crossover but with layers we cant do this
	// Each previous layer needs to carry out its "stuff" before the higher layers influence their results.
	// Since this function is called recursively by each higher layer, for the previous layer, just calculate as 
	// if this were "the" layer.
	retrieveScores();
	selectBestCritters();
	doCrossover();
	doMutation();
	// so we have not "updated" yet - what this means is that the "old" critters are still in the creature/parasite slots
	// but the "new" selected and possibly crossovered critters are 
	//update(); // cant update here - wromg order
}


/************************************************************************
	creatureBetterThan ?
*************************************************************************/
// point of this is it allows flexibility as to what is better than the other
// removes it to the creatureType class - whatever that defines it to be.
template <class creatureType, class parasiteType> 
        const bool creatureBetterThan(const boost::shared_ptr<location <creatureType,parasiteType> > &lhs,
                                 const boost::shared_ptr<location <creatureType,parasiteType> > &rhs) {
            return lhs->creature->isBetterThan(lhs->creatureScore,rhs->creatureScore);
}

/************************************************************************
	parasiteBetterThan ?
*************************************************************************/

template <class creatureType, class parasiteType> 
        const bool parasiteBetterThan(const boost::shared_ptr<location <creatureType,parasiteType> > &lhs,
                                 const boost::shared_ptr<location <creatureType,parasiteType> > &rhs) {
            return lhs->parasite->isBetterThan(lhs->parasiteScore,rhs->parasiteScore);
}

/************************************************************************
	displayLocation
*************************************************************************/

template <class creatureType,class parasiteType> class displayLocation:
    public unary_function<boost::shared_ptr<location <creatureType, parasiteType> >, void> {
    public:
        int x;
        displayLocation(int row) : x(row) { }
        void operator()(const boost::shared_ptr<location<creatureType,parasiteType> > &p) {
            if (!(p->id%(x))) cout << endl;
            p->display();
        }
};

/************************************************************************
	displayLocationPointers
*************************************************************************/

template <class creatureType,class parasiteType> class displayLocationPointers:
    public unary_function<boost::shared_ptr<location <creatureType, parasiteType> >, void> {
    public:
        displayLocationPointers() { }
        void operator()(const boost::shared_ptr<location<creatureType,parasiteType> > &p) {
            p->displayPtrs();
        }
};

template <class creatureType,class parasiteType> class displayNewLocation:
    public unary_function<boost::shared_ptr<location <creatureType, parasiteType> >, void> {
    public:
        int x;
        displayNewLocation(int row) : x(row) { }
        void operator()(const boost::shared_ptr<location<creatureType,parasiteType> > &p) {
            if (!(p->id%(x))) cout << endl;
            p->displayNew();
        }
};

/************************************************************************
	selectFromNeighbourhood - called from selectBestCritters
*************************************************************************/

template <class creatureType, class parasiteType> class selectFromNeighbourhood:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
private:
    public:
    selectFromNeighbourhood() { }
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
                
                // Calculate size of neighbourhood and determine the rank you want
                // to select using the probability distribution contained in selectOne.
                
		int nSize = p->neighboursAndPrevious->size();
		int select = selectOne(nSize);
                
                // The reason we select which one we want to "select" is that we don't need
                // to sort the neighbours - we just need to get the nth element if we know
                // the n we want.
                
		nth_element(p->neighboursAndPrevious->begin(),p->neighboursAndPrevious->begin()+select,p->neighboursAndPrevious->end(),creatureBetterThan<creatureType,parasiteType>);

				//  cout << "\nSorted for nth element, " << select << endl;
				//  for_each(p->neighbours->begin(),p->neighbours->end(),displayLocation<creatureType,parasiteType>(200));
               
                // we use makeCopyOfCreature and getCreature to change the underlying creature
                // if we just did p->newCreature = {blah}->creature we would just be swapping
                // pointers to Creature types, which is not what we want.
				// as then when we, say, mutated the creature it would mutate the copies in all
				// locations.
				// The copy is quite an expensive operation (being a new cr_data) but its what we need to do!
				// Note when we update creature to point to newCreature - we can just change pointers
				// (the update so p takes the value of p' - using the nomenclature in the papers.
		p->newCreature->makeCopyOfCreature((*(p->neighboursAndPrevious))[select]->creature);

				// cout << "Copied creature from location id " << (*(p->neighboursAndPrevious))[select]->id << "(Serial: " << (*(p->neighboursAndPrevious))[select]->creature->getCreature()->mySerial << ")  to location " << p->id << " with serial " << p->newCreature->getCreature()->mySerial << endl;
                // and repeat for parasites
                
		select = selectOne(nSize);
		nth_element(p->neighboursAndPrevious->begin(),p->neighboursAndPrevious->begin()+select,p->neighboursAndPrevious->end(),parasiteBetterThan<creatureType,parasiteType>);
		p->newParasite->makeCopyOfParasite((*(p->neighboursAndPrevious))[select]->parasite);

		
				// cout << "Copied parasite from location id " << (*(p->neighbours))[select]->id << " to location " << p->id << endl;    
				// cout << "Copied parasite from location id " << (*(p->neighboursAndPrevious))[select]->id << "(Serial: " << (*(p->neighboursAndPrevious))[select]->parasite->getParasite()->mySerial << ")  to location " << p->id << " with serial " << p->newParasite->getParasite()->mySerial << endl;
                
	}
};


/************************************************************************
	selectBestCritters
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::selectBestCritters() {
	//cout << "***BEGIN CREATURE AND PARASITE SELECTION***\n";
//	cout << "Check the location entites\n";
//	for_each(theWorld.begin(),theWorld.end(),checkLocationEntities<creatureType,parasiteType>());
//	cout << "then select the ones to be copied over\n";
    for_each(theWorld.begin(),theWorld.end(),selectFromNeighbourhood<creatureType,parasiteType>());
//	cout << "check the validity of those copied\n";
//	for_each(theWorld.begin(),theWorld.end(),checkNewLocationEntities<creatureType,parasiteType>());
	//cout << "***END CREATURE AND PARASITE SELECTION***\n";
}

/************************************************************************
	crossoverAtLoc
*************************************************************************/

template <class creatureType,class parasiteType> class crossoverAtLoc:
    public unary_function<boost::shared_ptr<location <creatureType, parasiteType> >, void> {
    public:
        crossoverAtLoc() { }
        void operator()(const boost::shared_ptr<location<creatureType,parasiteType> > &p) {

				int mate = randomint(p->neighbours->size());
				// doCrossover returns a pointer to a new child formed by crossing over the creatures
				// It can return NULL if no crossover was perfomed.
				p->newCreature->doCrossover(*(p->newCreature),*((*p->neighbours)[mate]->newCreature));
				mate = randomint(p->neighbours->size());
				p->newParasite->doCrossover(*(p->newParasite),*((*p->neighbours)[mate]->newParasite));
        }
};
/************************************************************************
	doCrossover
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::doCrossover() {
    //cout << "***BEGIN CREATURE AND PARASITE CROSSOVER***\n";
	for_each(theWorld.begin(),theWorld.end(),crossoverAtLoc<creatureType,parasiteType>());
	//cout << "***END CREATURE AND PARASITE CROSSOVER\n";
}    


/************************************************************************
	checkForMutation - called by doMutation
*************************************************************************/

template <class creatureType, class parasiteType> class checkForMutation:
    public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
private:
    public:
    checkForMutation() { }
    
    void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
            p->newCreature->doMutation(*(p->creature));
			p->newParasite->doMutation(*(p->parasite));
    }
};

/************************************************************************
	doMutation
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::doMutation() {
	//cout << "***BEGIN CREATURE AND PARASITE MUTATION***\n";
    for_each(theWorld.begin(),theWorld.end(),checkForMutation<creatureType,parasiteType>());
	//cout << "***END CREATURE AND PARASITE MUTATION***\n";
}

/************************************************************************
preliminaryUpdateLocation - called from update - just moves h'->h and p'->p
Other stuff is done after crossover and mutation
*************************************************************************/

template <class creatureType,class parasiteType> class preliminaryUpdateLocation:
public unary_function<boost::shared_ptr<location <creatureType, parasiteType> >, void> {
	bool dotest;
public:
	preliminaryUpdateLocation(bool test = false) { dotest = test; }

	void operator()(const boost::shared_ptr<location<creatureType,parasiteType> > &p) {
		// Basically we are moving the selected creatures/parasite (in newCreature and newParasite)
		// into the creature/parasite slot, so that we can then do more interesting things with them 
		// (like crossover/mutation etc).
		if (p->newCreature) {
			if (p->newCreature->isReallyValid()) {
				// only going to move it if its valid.
				// Set the underlying pointers in a creature to point to those in newCreature - we dont need to copy the creature 
				double scoreOne,scoreTwo;
				int hits1,hits2;
				/*if (dotest) {
					scoreOne = p->creature->getRealScore(hits1);
				}
				*/
				p->creature->setCreature(p->newCreature); 
				/*
				if (dotest) {
					scoreTwo = p->creature->getRealScore(hits2);
					if (scoreOne == scoreTwo) {
						cout << "Scores match " << scoreOne << "hits are " << hits1 << ", " << hits2 <<endl;
					} else 
						cout << "Scores DONOT match " << scoreOne << "hits are " << hits1 << ", " << hits2 <<endl;
				}
				*/
				p->newCreature->clearCreature(); // we do this so we don't re-examine it every time prelinaryUpdateLocation is called.
				if (p->newCreature->isReallyValid()) {
					cout << "Very odd\n";
				}
				//				cout << "Location " << p->id << " Creature has id: " << p->creature->getCreature()->mySerial << " newCreature: " << p->newCreature->getCreature()->mySerial << endl;
			} else {
				p->newCreature->clearCreature();
			}
		}
		if (p->newParasite) {
			if (p->newParasite->isReallyValid()) {
			    //if (p->parasite->mySerial%2) cout << "new parasite ";
				//else cout << "old parasite ";
				//cout << p->parasite->mySerial/2 << " (" << p->parasite->getParasite() << ") ";
				//cout << "replaced by ";
				//if (p->newParasite->mySerial%2) cout << "new parasite ";
				//else cout << "old parasite ";
				//cout << p->newParasite->mySerial/2 << " (" << p->newParasite->getParasite() << ") \n";
				if (dotest) {
					if (p->parasite == p->newParasite) {
						cout << "Parasites are the same\n";
					}
				}
				p->parasite->setParasite(p->newParasite);
				p->newParasite->clearParasite();
			} else {
				p->newParasite->clearParasite();
			}
		}
	}
};


/************************************************************************
updateLocation - called from update
*************************************************************************/

template <class creatureType,class parasiteType> class updateLocation:
public unary_function<boost::shared_ptr<location <creatureType, parasiteType> >, void> {
	int maxGenerationAge;
	spatialWorld<creatureType,parasiteType> *previousLayer;
public:
		updateLocation() { }
    updateLocation(int maxAge,spatialWorld<creatureType,parasiteType> *pl): maxGenerationAge(maxAge), previousLayer(pl)  {}
	
	void operator()(const boost::shared_ptr<location<creatureType,parasiteType> > &p) {
	
		// Update already has the "new" creatures and parasites in creature/parasite
		// And we know they will be valid.

		if (p->newCreature->isReallyValid()) {
			// Set the underlying pointers in a creature to point to those in newCreature - we dont need to copy the creature 
			//				cout << "Location " << p->id << " Creature has id: " << p->creature->getCreature()->mySerial << " newCreature: " << p->newCreature->getCreature()->mySerial << endl;
			p->creature->setCreature(p->newCreature); 
			//				cout << "Location " << p->id << " Creature has id: " << p->creature->getCreature()->mySerial << " newCreature: " << p->newCreature->getCreature()->mySerial << endl;
		} else {
			//				cout << "New Creature was invalid, not updating " << p->id << endl;
		}

		if (maxGenerationAge < 0) p->creature->geneticAge++;
		else if (p->creature->geneticAge++ > maxGenerationAge) {
			// creature in too old to live!
			if (previousLayer == NULL) {
				p->creature.reset(new creatureType());
				if (!p->creature->isReallyValid()) {
					cout << "UpdateLocation created a new creature - which is not really valid!\n";
					exit(1);
				}
				// automatically sets the genetic Age to zero.
			} else {
				if ((previousLayer->theWorld[p->locationNumber])->newCreature->isReallyValid())
					p->creature->makeCopyOfCreature((previousLayer->theWorld[p->locationNumber])->newCreature);
				else p->creature->makeCopyOfCreature((previousLayer->theWorld[p->locationNumber])->creature);
				if (!p->creature->isReallyValid()) {
					p->creature.reset(new creatureType());
					if (!p->creature->isReallyValid()) {
						cout << "UpdateLocation created a new creature as the once it copied was not really valid, but the new creature is not really valid!\n";
						exit(1);
					}
				}	
			}
		} 

		if (p->newParasite->isReallyValid()) {
			p->parasite->setParasite(p->newParasite);
			//				cout << "Location " << p->id << " Parasite has id: " << p->creature->getCreature()->mySerial << " newParsite: " << p->newCreature->getCreature()->mySerial << endl;
		} else {
			//				cout << "New Parasite was invalid, not updating " << p->id << endl;
		}
		
		if (maxGenerationAge < 0) p->parasite->geneticAge++;
		else if ( p->parasite->geneticAge++ > maxGenerationAge) {
			if (previousLayer == NULL) {
				p->parasite.reset(new parasiteType());
				if (!p->parasite->isReallyValid()) {
					cout << "Update created a new parasite which is not really valid!\n";
					exit(1);
				}
			} else {
				if ((previousLayer->theWorld[p->locationNumber])->newParasite->isReallyValid())
					p->parasite->makeCopyOfParasite((previousLayer->theWorld[p->locationNumber])->newParasite);
				else p->parasite->makeCopyOfParasite((previousLayer->theWorld[p->locationNumber])->parasite);
				if (!p->parasite->isReallyValid()) {
					p->parasite.reset(new parasiteType());
					if (!p->parasite->isReallyValid()) {
						cout << "Update created a new parasite (since the previous one was not valid, but the new one was not really valid!\n";
						exit(1);
					}
				}
			}
		}	
	}
};

/************************************************************************
	checkAgedLayer
*************************************************************************/

template <class creatureType,class parasiteType> class checkAgeLayer:
public unary_function<boost::shared_ptr<location <creatureType, parasiteType> >, void> {
	int maxGenerationAge;
public:
	checkAgeLayer() { }
    checkAgeLayer(int maxAge): maxGenerationAge(maxAge)  { }
	bool operator()(const boost::shared_ptr<location<creatureType,parasiteType> > &p) {
		if ((maxGenerationAge > 0) ) {
			return ((p->creature->geneticAge >= maxGenerationAge) || (p->parasite->geneticAge >= maxGenerationAge));
		} else return false;
	}
};

/************************************************************************
	preliminaryUpdate
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::preliminaryUpdate(bool test) {
	// go through and move the selected creatures/parasites into their location
	// uses preliminaryUpdateLocation to do this
	
	// then update this one.
	preliminaryUpdateLocation<creatureType,parasiteType> doPreliminaryUpdate(test);
	for_each(theWorld.begin(),theWorld.end(),doPreliminaryUpdate);
	if (previousLayer) previousLayer->preliminaryUpdate(test); // update the worlds below this
	//if (previousLayer) previousLayer->update();
}


/************************************************************************
	update
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::update() {
	// hmm expandingSpatialWorld stuff.
	if (isItAnExpandingSpatialWorld) { // only true for most senior layer
	//	so what we really want looks like this
	// we age it even if we have a '-1' (infinite) aged world, just to age the creatures so we can see how old they got.
	// if you were not interested in that you could short circuit a few bit by uncommenting the if statement.
	
		if (generationLife!=-1) {
			checkAgeLayer<creatureType,parasiteType> doAgeLayer(generationLife);
			if (find_if(theWorld.begin(),theWorld.end(),doAgeLayer) != theWorld.end()) addBottomLayer();
		}
	 }
	updateLocation<creatureType,parasiteType> doUpdate(generationLife,previousLayer);
	for_each(theWorld.begin(),theWorld.end(),doUpdate);
		// unlike docalc etc, we need to update this layer before we update the prevous layers, in case we wipe an earlier layer.
	if (previousLayer) previousLayer->update();
}

/************************************************************************
	doAGeneration
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::doAGeneration() {

	cout << "Time to do a generation.\n";


	// Step 1, get all the layers to calculate the score of the relavant critter
	
	calcScore();

	// Step 2, select the best creatures/parasites and store in newCreature/newParasite
	//         We do this by calling selectBestCritters
	//		   it ranks them with a 50% chance of selecting the "best" one, failing which 50% the next best one etc

	
	
	//selectBestCritters();
	
	// Step 3, move all the newCreatures and newParasites to replace the old ones
	//         they are only moved if they are valid
	
	//preliminaryUpdate();
	
	// Step 4, check if any of them are to be "crossed" over and if so cross them over store children in h' and p'
	
	//doCrossover();
	
	// Step 5, those that were crossed over are moved back to h and p

	//preliminaryUpdate();
	
	// Step 6, check for mutaton
	
	//doMutation();
	
	// Step 7, move the successful mutations back to h and p
	
	//preliminaryUpdate(true);
	
	// And finally step 7, deal with age increases etc
	
	update();
}

/************************************************************************
	display
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::display() {
	if (previousLayer) previousLayer->display();
    //cout << "********************** DISPLAY ************************\n";
	cout << "Displaying layer " << getLayerNo() << endl;
	for_each(theWorld.begin(),theWorld.end(),displayLocation<creatureType,parasiteType>(xSize));
	
	// you can enable this to print out the location pointer, just to check that its working
	//for_each(theWorld.begin(),theWorld.end(),displayLocationPointers<creatureType,parasiteType>());
	// end of location pointer printout
	
	//cout << "********************** DISPLAYNew ************************\n";
	//	for_each(theWorld.begin(),theWorld.end(),displayNewLocation<creatureType,parasiteType>(xSize));
	cout << "\n********************END OF DISPLAY**********************\n";
    cout << endl;
}

/************************************************************************
	bestRealScore
*************************************************************************/

template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::bestRealScore() {
    typedef boost::shared_ptr<location<creatureType,parasiteType> > locPtr;
    typedef vector<locPtr> locVector;
    double best = 100.0;
    typename vector<locPtr>::iterator at;
    at = theWorld.begin();
    while (at != theWorld.end()) {
        double temp= (*at)->creature->realScore();
        if (temp < best) best = temp;
        at++;
    }
    cout << "\nBest Score so far " << best << endl;
}

/************************************************************************
	savePop
*************************************************************************/

template <class creatureType,class parasiteType> bool spatialWorld<creatureType,parasiteType>::savePop(const char *fname) {
 // I would have prefered to use the for_each format here, but 
 // it was causing instantiation errors with ostreams for some reason, so reverted to the old fashioned stepping through the vector
	ofstream outFile;
	outFile.open(fname,ios::out);
	if (!outFile) { 
			cout << "Couldn't open file " << fname << endl;
			return false;
	}      
	int size = theWorld.size();
	for (int i=0;i<size;i++) {
		theWorld[i]->creature->saveCreature(outFile);
		theWorld[i]->parasite->saveParasite(outFile);
	}
	outFile.close();
	return true;
}


/************************************************************************
	loadPop
*************************************************************************/
   
template<class creatureType,class parasiteType> bool spatialWorld<creatureType,parasiteType>::loadPop(const char *currFileName) {
	ifstream fin(currFileName);
	if (fin) cout << "Opened file " << currFileName << "\n";
	else {
		cout << "Failed to open file " << currFileName << endl;
		return false;
	}
   // the following isnt very robust if we dont have a well formed file
	int size = theWorld.size();
	for (int i=0;i<size;i++) {
		theWorld[i]->creature->loadCreature(fin);
		theWorld[i]->parasite->loadParasite(fin);
	}
	fin.close();
    return true;
}
	

/************************************************************************
	setUpNeighbours
*************************************************************************/

template <class creatureType,class parasiteType> void spatialWorld<creatureType,parasiteType>::setUpNeighbours(int x,int y) {
    // The idea here is to set up in each location a list of the 
    // topographical neighbours for each location
    // There were a couple of reasons for doing it this way, rather than calculate on the fly
    // The first is that it seemed a good idea to remove the topography from the other procedures
    // So now rather than work out where the neighbours are (or even how many) you can just iterate
    // through the neighbours vector
    // The second is that the pain of setting them up is only gone through once (and really the storage is nothing)
    // compared to the storage of the GE critters - there are a lot of calculations and tests in working out the
    // neighbours if its done on the fly.
        int total = x*y;
        theWorld.clear();
        theWorld.reserve(total);
        boost::shared_ptr< location < creatureType, parasiteType> >  locP;
        for (int i=0;i<total;i++) {
          locP.reset(new location<creatureType,parasiteType>());
          theWorld.push_back(locP);
        }
        for (int i=1;i<x-1;i++) { // step along the "top" xrow
            int at = i;
			theWorld[at]->locationNumber = at;
            theWorld[at]->id = at+layerNo*total;
            theWorld[at]->neighbours->push_back(theWorld[total-x+i-1]);
            theWorld[at]->neighbours->push_back(theWorld[total-x+i]);
            theWorld[at]->neighbours->push_back(theWorld[total-x+i+1]);
            theWorld[at]->neighbours->push_back(theWorld[at-1]);
            theWorld[at]->neighbours->push_back(theWorld[at]);
            theWorld[at]->neighbours->push_back(theWorld[at+1]);
            theWorld[at]->neighbours->push_back(theWorld[at+x-1]);
            theWorld[at]->neighbours->push_back(theWorld[at+x]);
            theWorld[at]->neighbours->push_back(theWorld[at+x+1]);
        }
        for (int i=1;i<x-1;i++) { // step along the middle
            for (int j=1;j<y-1;j++) {
                int at =i+x*j;
				theWorld[at]->locationNumber = at;
                theWorld[at]->id = at+layerNo*total;
                theWorld[at]->neighbours->push_back(theWorld[at-x-1]);
                theWorld[at]->neighbours->push_back(theWorld[at-x]);
                theWorld[at]->neighbours->push_back(theWorld[at-x+1]);
                theWorld[at]->neighbours->push_back(theWorld[at-1]);
                theWorld[at]->neighbours->push_back(theWorld[at]);
                theWorld[at]->neighbours->push_back(theWorld[at+1]);
                theWorld[at]->neighbours->push_back(theWorld[at+x-1]);
                theWorld[at]->neighbours->push_back(theWorld[at+x]);
                theWorld[at]->neighbours->push_back(theWorld[at+x+1]);
            }
        }
        for (int j=1;j<y-1;j++) { // LHS
                int at = x*j; 
				theWorld[at]->locationNumber = at;
                theWorld[at]->id = at+layerNo*total;
                theWorld[at]->neighbours->push_back(theWorld[at-1]);
                theWorld[at]->neighbours->push_back(theWorld[at-x]);
                theWorld[at]->neighbours->push_back(theWorld[at-x+1]);
                theWorld[at]->neighbours->push_back(theWorld[at+x-1]);
                theWorld[at]->neighbours->push_back(theWorld[at]);
                theWorld[at]->neighbours->push_back(theWorld[at+1]);
                theWorld[at]->neighbours->push_back(theWorld[at+2*x-1]);
                theWorld[at]->neighbours->push_back(theWorld[at+x]);
                theWorld[at]->neighbours->push_back(theWorld[at+x+1]);
        }
        for (int j=1;j<y-1;j++) { // RHS
                int at = x*j+x-1; 
				theWorld[at]->locationNumber = at;
                theWorld[at]->id = at+layerNo*total;
                theWorld[at]->neighbours->push_back(theWorld[at-x-1]);
                theWorld[at]->neighbours->push_back(theWorld[at-x]);
                theWorld[at]->neighbours->push_back(theWorld[at-2*x+1]);
                theWorld[at]->neighbours->push_back(theWorld[at-1]);
                theWorld[at]->neighbours->push_back(theWorld[at]);
                theWorld[at]->neighbours->push_back(theWorld[at-x+1]);
                theWorld[at]->neighbours->push_back(theWorld[at+x-1]);
                theWorld[at]->neighbours->push_back(theWorld[at+x]);
                theWorld[at]->neighbours->push_back(theWorld[at+1]);
        }
        for (int j=1;j<x-1;j++) { // Bottom row
                int at = (y-1)*x+j; 
				theWorld[at]->locationNumber = at;
                theWorld[at]->id = at+layerNo*total;
                theWorld[at]->neighbours->push_back(theWorld[at-x-1]);
                theWorld[at]->neighbours->push_back(theWorld[at-x]);
                theWorld[at]->neighbours->push_back(theWorld[at-x+1]);
                theWorld[at]->neighbours->push_back(theWorld[at-1]);
                theWorld[at]->neighbours->push_back(theWorld[at]);
                theWorld[at]->neighbours->push_back(theWorld[at+1]);
                theWorld[at]->neighbours->push_back(theWorld[j-1]);
                theWorld[at]->neighbours->push_back(theWorld[j]);
                theWorld[at]->neighbours->push_back(theWorld[j+1]);
        }
        // Now corner 0,0
        theWorld[0]->id = 0+layerNo*total;
		theWorld[0]->locationNumber = 0;
        theWorld[0]->neighbours->push_back(theWorld[total-1]);
        theWorld[0]->neighbours->push_back(theWorld[total-x]);
        theWorld[0]->neighbours->push_back(theWorld[total-x+1]);
        theWorld[0]->neighbours->push_back(theWorld[x-1]);
        theWorld[0]->neighbours->push_back(theWorld[0]);
        theWorld[0]->neighbours->push_back(theWorld[1]);
        theWorld[0]->neighbours->push_back(theWorld[2*x-1]);
        theWorld[0]->neighbours->push_back(theWorld[x]);
        theWorld[0]->neighbours->push_back(theWorld[x+1]);
        // corner X,0 (which has value x-1 as we start at 0.
        theWorld[x-1]->id = x-1+layerNo*total;
        theWorld[x-1]->locationNumber = x-1;
        theWorld[x-1]->neighbours->push_back(theWorld[total-2]);
        theWorld[x-1]->neighbours->push_back(theWorld[total-1]);
        theWorld[x-1]->neighbours->push_back(theWorld[total-x]);
        theWorld[x-1]->neighbours->push_back(theWorld[x-2]);
        theWorld[x-1]->neighbours->push_back(theWorld[x-1]);
        theWorld[x-1]->neighbours->push_back(theWorld[0]);
        theWorld[x-1]->neighbours->push_back(theWorld[2*x-2]);
        theWorld[x-1]->neighbours->push_back(theWorld[2*x-1]);
        theWorld[x-1]->neighbours->push_back(theWorld[x]);
        // corner 0,Y
        theWorld[total-x]->id = total-x+layerNo*total;
        theWorld[total-x]->locationNumber = total-x;
        theWorld[total-x]->neighbours->push_back(theWorld[total-x-1]);
        theWorld[total-x]->neighbours->push_back(theWorld[total-2*x]);
        theWorld[total-x]->neighbours->push_back(theWorld[total-2*x+1]);
        theWorld[total-x]->neighbours->push_back(theWorld[total-1]);
        theWorld[total-x]->neighbours->push_back(theWorld[total-x]);
        theWorld[total-x]->neighbours->push_back(theWorld[total-x+1]);
        theWorld[total-x]->neighbours->push_back(theWorld[x-1]);
        theWorld[total-x]->neighbours->push_back(theWorld[0]);
        theWorld[total-x]->neighbours->push_back(theWorld[1]);
        // and last but not least corner x,y
        theWorld[total-1]->locationNumber = total-1;
        theWorld[total-1]->id = total-1+layerNo*total;
        theWorld[total-1]->neighbours->push_back(theWorld[total-x-2]);
        theWorld[total-1]->neighbours->push_back(theWorld[total-x-1]);
        theWorld[total-1]->neighbours->push_back(theWorld[total-2*x]);
        theWorld[total-1]->neighbours->push_back(theWorld[total-2]);
        theWorld[total-1]->neighbours->push_back(theWorld[total-1]);
        theWorld[total-1]->neighbours->push_back(theWorld[total-x]);
        theWorld[total-1]->neighbours->push_back(theWorld[x-2]);
        theWorld[total-1]->neighbours->push_back(theWorld[x-1]);
        theWorld[total-1]->neighbours->push_back(theWorld[0]);

}

#endif
