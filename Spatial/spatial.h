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

#include "../Base/critter.h"
#include <iostream>
#include <iomanip>

#include <boost/shared_ptr.hpp>

int selectOne(int);
extern int locationCount;
extern int serialNo;
extern vector<int> layerLives;
// ***** Foreword ******
// On reflection I am not sure why I did this using templates rather than just a base class
// But it works and is kind of neat, and it keeps all the specialisation in the creatureType and parasiteType.

// You need to write two template classes which provide the relevant methods for both the parasite
// and the creature - they basically encapsulate the type of creature and parasite you need.
// lets assume you were writing a new creature type and a new parasite type of types
// yourCreatureType and yourParasiteType.

// the classes would need to encapsulate the methods contained in spatialRobocodeCreatureDelta
// easiest to follow that as template - its reasonably well documents.

// Then you create a spatial world by the following:

// ******************************************************************
// spatialWorld<yourCreatureType,yourParasiteType> newSpatialWorld(rows,columns);
//*******************************************************************
//
// See the examples for more detail!

// Extensive use made of the STL library and the boost::shared_ptr framework.

// now uses C++11 syntax (sometimes) - Ill slowoly update to use it all over time, it will simplify a lot of the unary classes
// that are contained here.

// Location is a location in the spatial world
// It contains ptrs to the creature, the parasite and scratchpad
// storage for scores and creatures being created.
// Finally it contains vectors of simple pointers to all the neighbourhood locations.



template <class creatureType, class parasiteType> class location {
public:
    int id; 
	int SerialNo;
	int locationNumber;
	boost::shared_ptr<creatureType> creature;
    boost::shared_ptr<parasiteType> parasite;
    double creatureScore,parasiteScore;
    typedef location<creatureType, parasiteType> *simpleLocPtr;
    
    boost::shared_ptr<creatureType> newCreature;
    boost::shared_ptr<parasiteType> newParasite;
    vector<simpleLocPtr> neighbours;
    vector<simpleLocPtr> previousLayer;
    vector<simpleLocPtr> neighboursAndPrevious;
	void displayPtrs();
    // should really delegate this to the crature and the parasite.
    void display()  { cout << setw(4) << id << " [";
        cout << setprecision(4) << setw(5) << creatureScore << ":";
        cout << " {" <<  setw(3) << creature->geneticAge << "} ";
        cout << "  ( " << setw(5) << setprecision(4) << parasite->getParasite() << ": ";
        cout << " {" << setw(3) << parasite->geneticAge << "} ";
        cout  << " )] ";
    }
    void displayNew()  { cout << setw(4) << id << " [" << setw(6) << (newCreature?newCreature->getCreature()->mySerial:0) << "  ( " << setw(5) <<  (newParasite?newParasite->getParasite()->mySerial:0) << ")] ";}
    location() {
		id = locationCount++;
		SerialNo = serialNo++;
	}
    ~location() {
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
	static spatialWorld<creatureType,parasiteType> *makeWorldFromLoadFile(int sizeX,int sizeY, int layers, const char *baseFileName);
    spatialWorld(int x,int y);
	
    vector<boost::shared_ptr<location<creatureType,parasiteType> > > theWorld;
	bool loadPop(const char *fname,int ll=0);
	bool savePop(const char *fname);
    void doGeneration() {
        calcScore();
		update();
    }
    // cant recall why but I seem to be calling calcScore and update from the main program rather than just doing doGeneration()
    void calcScore();
    void update();
    void display();
    void bestRealScore();
	bool terminate() { return terminated; }
	int getLayerNo() { return layerNo; }
    spatialWorld<creatureType,parasiteType> *getPreviousLayer() { return previousLayer; }
    ~spatialWorld();
    
protected:
    // currently these are only used by the makeWorldFromLoadFile and the spatialWorld constructors (including addBottomLayer).
    spatialWorld(int x,int y,int genLife,bool genEntities = true);
	spatialWorld(spatialWorld<creatureType,parasiteType> *prev,int genLife,bool genEntities = true);
	//static vector<int> layerLives;
    
	void addBottomLayer();
	void clearAllScores();
	void clearAllLayeredScores();
	void retrieveScores();
    void selectBestCritters();
    void doCrossover();
    void doMutation();
	bool isItAnExpandingSpatialWorld;
};

/************************************************************************
 generateLocationEntities - called from the SpatialWorld creators.
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
         generateSkeletalLocationEntities - called from the "from file" creators so we dont generate random critters first.
         *************************************************************************/
        
        template <class creatureType, class parasiteType> class generateSkeletalLocationEntities:
        public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
        public:
            generateSkeletalLocationEntities() {}
            
            void operator()(const boost::shared_ptr<location <creatureType,parasiteType> >& p) {
                // generate one new creatureType and one new ParasiteType
                // and place in the location pointed to by p.
                p->creatureScore = p->parasiteScore = 0;
                p->creature.reset(new creatureType(false)); // dont create the underlying creature (we will load one from file.)
                p->parasite.reset(new parasiteType(false));  // dont create the underlying parasite (we will load one from file or whatever.)
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
                typedef location<creatureType,parasiteType> *simpleLocPtr;

                // Assume neighbourhood is correctly initialised for this layer
                // If there is a prevous layer (stored in wToC), then the vector "previousLayer" becomes a copy of the vector neighbours of the
                // location (determined by p.location) of this "location" in the previous layer.
                // And yep that sentence was probably more
                // complicated than it really should be.
                if (worldToCopy != NULL) {
                    vector<simpleLocPtr> tocopy;
                    tocopy = worldToCopy->theWorld[p->locationNumber]->neighbours;
                    p->previousLayer = worldToCopy->theWorld[p->locationNumber]->neighbours;
                } else {
                    p->previousLayer.clear();
                }
                p->neighboursAndPrevious = p->neighbours;
                // then insert into that vector all the "previousLayer" neighbours.
                p->neighboursAndPrevious.insert(p->neighboursAndPrevious.end(),p->previousLayer.begin(),p->previousLayer.end());
                
            }
        };
        
        
        
        /************************************************************************
         spatialWorld(int x, int y, int genLife) - creates a layer with a specific life
         *************************************************************************/
        
        template <class creatureType, class parasiteType> spatialWorld<creatureType,parasiteType>::spatialWorld(int x,int y,int genLife,bool genEntities) {
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
            if (genEntities) for_each(theWorld.begin(),theWorld.end(),generateLocationEntities<creatureType,parasiteType>());
            else for_each(theWorld.begin(),theWorld.end(),generateSkeletalLocationEntities<creatureType,parasiteType>());
            cout << "\nAfter its populated it has a size of " << theWorld.size() << endl;
            
           
        }
        
        
        /************************************************************************
         spatialWorld(int x, int y) - creates a layer with a life taken from layerLives[0]
         *************************************************************************/
        
        
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
            
            
            if (previousLayer == NULL) {
                // okay we are at the soon to be second bottom layer
                expansionPreviousLayer.reset(new spatialWorld<creatureType,parasiteType>(xSize,ySize,layerLives[0]));
                // so the shared_ptr points at it now and that means (provided no other shared_ptrs point to it)
                // it will be auto deleted when this world is deleted.
                previousLayer = expansionPreviousLayer.get(); // the address of the thing expansionPreviousLayer points to
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
            int passedInLayer = layerNo;
            cout << "Layer " << layerNo -1 << " is now layer " << layerNo << " with a new life limit of " << generationLife << endl;
            // For each creature and parasite if its the top layer tell it is, passing in their location
            // The idea here is that if you want you can do things to (some) of the creatures - so in robocode, I turn some of the creatures
            // into human bots, so that the evolving bots can practice battling against them.
            // The isParticipating call for creatures and parasites meshes with this indicating that they are(or are not) participating
            // in the breeding cycle - so for instance those that have turned into human bots will not participate in the cycle.
                for_each(theWorld.begin(),theWorld.end(),[passedInLayer] (boost::shared_ptr<location<creatureType,parasiteType> >  &locationAt) {
                    locationAt->creature->inLayerWithLocation(passedInLayer,locationAt->locationNumber);
                    locationAt->parasite->inLayerWithLocation(passedInLayer,locationAt->locationNumber);
                });
           
        }
        
        
        /************************************************************************
         spatialWorld(spatialWorld<creatureType,parasiteType> *prev,int genLife)
         *************************************************************************/
        
        template <class creatureType, class parasiteType> spatialWorld<creatureType,parasiteType>::spatialWorld(spatialWorld<creatureType,parasiteType> *prev,int genLife,bool genEntities) {
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
            if (genEntities) for_each(theWorld.begin(),theWorld.end(),generateLocationEntities<creatureType,parasiteType>());
            else for_each(theWorld.begin(),theWorld.end(),generateSkeletalLocationEntities<creatureType,parasiteType>());
            cout << "\nAfter this layer is populated it has a size of " << theWorld.size() << endl;
            
            // For each creature and parasite if its the top layer tell it is, passing in their location
            // The idea here is that if you want you can do things to (some) of the creatures - so in robocode, I turn some of the creatures
            // into human bots, so that the evolving bots can practice battling against them.
            // The isParticipating call for creatures and parasites meshes with this indicating that they are(or are not) participating
            // in the breeding cycle - so for instance those that have turned into human bots will not participate in the cycle.
            cout << "Setting the participating thingy\n";
            int passedInLayer = layerNo;
            for_each(theWorld.begin(),theWorld.end(),[passedInLayer] (boost::shared_ptr<location<creatureType,parasiteType> >  &locationAt) {
                locationAt->creature->inLayerWithLocation(passedInLayer,locationAt->locationNumber);
                locationAt->parasite->inLayerWithLocation(passedInLayer,locationAt->locationNumber);
            });
            
        }
        
        
        /************************************************************************
         makeWorldFromLoadFile - makes a world of the specified dimensions by reading
         the creatures (and parasites) in from a load file. Specifically used to instantiate
         multi layer worlds from previous save/checkpointed worlds.
         *************************************************************************/
        
        template <class creatureType,class parasiteType> spatialWorld<creatureType,parasiteType> *spatialWorld<creatureType,parasiteType>::makeWorldFromLoadFile(int sizeX,int sizeY, int layers, const char *baseFileName) {
            char fileName[200];
            spatialWorld<creatureType,parasiteType> *aLayer,*lastLayer;
            if (layerLives.size() < layers) {
                cout << "layerLives needs to hold the lives of all " << layers << " layers you intend to load, but just now it is only of size " << layerLives.size() << " aborting load.\n";
                return NULL;
            }
            lastLayer = NULL;
            for (int i=0;i<layers;i++) {
                if (i==0) {
                    aLayer = new spatialWorld<creatureType,parasiteType>(sizeX,sizeY,layerLives[0],false); // false = create skeletal locations
                } else {
                    lastLayer = aLayer;
                    aLayer = new spatialWorld<creatureType,parasiteType>(lastLayer,layerLives[i],false);
                }
                sprintf(fileName,"%s.layer%d.pop",baseFileName,i);
                aLayer->loadPop(fileName,i); // loads in the actual creatures
            }
            aLayer->isItAnExpandingSpatialWorld = true;
            return aLayer;
        }
		
		
		
		
        /************************************************************************
         neighbourHoodCreatureCompetitions
         
         Note: unary_function to test the creature against the parasites in the passed in locations.
         *************************************************************************/
        
        template <class creatureType, class parasiteType> class neighbourHoodCreatureCompetitions:
        public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
        private:
            boost::shared_ptr<creatureType> thisCreature;
            int locationID;
        public:
            neighbourHoodCreatureCompetitions(boost::shared_ptr<creatureType> &cr,int id): thisCreature(cr),locationID(id) {}
            neighbourHoodCreatureCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
            void operator()(location<creatureType,parasiteType> *p) {
                thisCreature->calcScore(*(p->parasite),locationID);
            }
        };
        
        /************************************************************************
         neighbourHoodParasiteCompetitions
         
         Note: unary_function created with a parasite, that then gets tested against the creatures in the locations passed in to ().
         *************************************************************************/
        
        template <class creatureType, class parasiteType> class neighbourHoodParasiteCompetitions:
        public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
        private:
            boost::shared_ptr<parasiteType> thisParasite;
            int locationID;
        public:
            typedef location<creatureType,parasiteType> *simpleLocationPtr;
            neighbourHoodParasiteCompetitions(boost::shared_ptr<parasiteType> &pr,int id): thisParasite(pr),locationID(id) {}
            neighbourHoodParasiteCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
            void operator()(const simpleLocationPtr p) {
                thisParasite->calcScore(*(p->creature),locationID);
            }
        };
        /************************************************************************
         neighbourHoodPreviousCreatureCompetitions
         
         Note: this allows the parasites in the previous layer to be tested against the current critter
         Using the parasite->calcScore method.
         *************************************************************************/
        
        template <class creatureType, class parasiteType> class neighbourHoodPreviousCreatureCompetitions:
        public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
        private:
            boost::shared_ptr<creatureType> thisCreature;
        public:
            neighbourHoodPreviousCreatureCompetitions(boost::shared_ptr<creatureType> &pr): thisCreature(pr) {}
            neighbourHoodPreviousCreatureCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
            void operator()(location <creatureType,parasiteType> *p) {
                p->parasite->calcScore(*thisCreature,-1);
            }
        };
        
        
        /************************************************************************
         neighbourHoodPreviousParasiteCompetitions
         
         Note: tests the creatures in the previous layer (passed in as a location) against the parasite
         this unary_function is created with.
         *************************************************************************/
        
        template <class creatureType, class parasiteType> class neighbourHoodPreviousParasiteCompetitions:
        public unary_function<boost::shared_ptr<location <creatureType,parasiteType> >,void> {
        private:
            boost::shared_ptr<parasiteType> thisParasite;
        public:
            neighbourHoodPreviousParasiteCompetitions(boost::shared_ptr<parasiteType> &pr): thisParasite(pr) {}
            neighbourHoodPreviousParasiteCompetitions() { cout << "SUMNEIGHBOURHOODSCORE: THIS SHOULDNT BE CALLED\n"; exit(1); }
            void operator()(location <creatureType,parasiteType> *p) {
                //cout << "Checked creature in " << p->id << " against that parasite " << endl;
                p->creature->calcScore(*thisParasite,-1);
                //cout << "*** Creature in location " << p->id << " tested***\n";
            }
        };
        
        
        /************************************************************************
         sumAndStoreCreatureAndParasiteScores
         
         Note: that each location has it creature given a chance to compete against the
         parasites in the neighbourhood (and the previous layer neighbourhood if it exists)
         AND gives the parasites a chance to compete against the creatures. In some models
         (e.g. regression) the parasite competing against the creatures is just an empty method
         but you might have something like a chess game where each side is given a chance to play
         white.
         
         The model is to instantiate one of the unary-functions defined above with the creature (or parasite in question)
         and then use a for_each to cycle through the appropriate location vector, calling the unary funciton to cause
         the competitions to happen.
         
         This unary-function instantiation, for-each invocation is used extensively in this framework.
         *************************************************************************/
        
        template <class creatureType, class parasiteType> class sumAndStoreCreatureAndParasiteScores:
        public unary_function<location <creatureType,parasiteType> ,void> {
            spatialWorld<creatureType,parasiteType> *previousLayer;
            
        public:
            sumAndStoreCreatureAndParasiteScores(spatialWorld<creatureType,parasiteType> *pl): previousLayer(pl) {}
            
            void operator()(const boost::shared_ptr<location <creatureType,parasiteType> > (&p)) {
                // Do the creatures first.
                neighbourHoodCreatureCompetitions<creatureType,parasiteType> doCreatureComp(p->creature,p->id);
                for_each(p->neighbours.begin(),p->neighbours.end(),doCreatureComp);
                if (previousLayer) {
                    // Test the creature against the parasites in the previous layer (the -1 indicates they are not on the same layer).
                    neighbourHoodCreatureCompetitions<creatureType,parasiteType> doCreatureComp(p->creature,-1);
                    for_each(p->previousLayer.begin(),p->previousLayer.end(),doCreatureComp);
                    // Then give each parasite in the previous layers the chance to be tested against this creature.
                    neighbourHoodPreviousCreatureCompetitions<creatureType,parasiteType> doPrevCreatureComps(p->creature);
                    for_each(p->previousLayer.begin(),p->previousLayer.end(),doPrevCreatureComps);
                }
                
                // And then the parasites.
                neighbourHoodParasiteCompetitions<creatureType,parasiteType> doParasiteComp(p->parasite,p->id);
                for_each(p->neighbours.begin(),p->neighbours.end(),doParasiteComp);
                if (previousLayer) {
                    neighbourHoodParasiteCompetitions<creatureType,parasiteType> doParasiteComp(p->parasite,-1);
                    for_each(p->previousLayer.begin(),p->previousLayer.end(),doParasiteComp);
                    // Give each creature on the previous layer a chance to compete against this parasite.
                    neighbourHoodPreviousParasiteCompetitions<creatureType,parasiteType> doPrevParasiteComps(p->parasite);
                    for_each(p->previousLayer.begin(),p->previousLayer.end(),doPrevParasiteComps);
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
                p->neighbours->clear();
                p->previousLayer->clear();
                p->neighboursAndPrevious->clear();
            }
        };
        
        /************************************************************************
         ~spatialWorld
         *************************************************************************/
        
        template <class creatureType,class parasiteType> spatialWorld<creatureType,parasiteType>::~spatialWorld() {
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
                // of critter in the layer below us .v. parasite in the layer two below us
                previousLayer->clearAllLayeredScores();
            }
            
            // for each location call the sumAndStore funcitons
            // which sums the creature/parasite
            // Clear the scores - as for the current layer, this is the first time we are scoring them
            for_each(theWorld.begin(),theWorld.end(),clearScores<creatureType,parasiteType>());
            // Then sum and Store scores.
            // Set up the function so we have access to the previous layer
            sumAndStoreCreatureAndParasiteScores<creatureType,parasiteType> sumAndStore(previousLayer);
            // Iterate through the world calling the function sumAndStoreCreatureAndParasiteScores()
            for_each(theWorld.begin(),theWorld.end(),sumAndStore);
            
            // so at this point - we have each creature and each parasite in each location has their score
            // .v. the creature and parasite on their own layer stored in their respective spatialCreature/Parasite.storedScore
            // and the score .v. the creature and parasite on the other layer (the one below this one for the current layer)
            // stored in storedLayeredScore (this is just the way I do it in these classes, of course it could be done differently).
            
            // we need to move it to the location score, creatureScore and parasiteScore
            
            // Side note I have been a bit sceptical about using the algorithms rather than writing my own loops
            // But I think the above lines convince me.
            // Indebted to Scott Meyers, Effective STL pp 158-160 (actually all his three books - all pages!)
            
            retrieveScores();
            selectBestCritters();
            doCrossover();
            doMutation();
            
            // so we have not "updated" yet - what this means is that the "old" critters are still in the creature/parasite slots
            // but the "new" selected and possibly crossovered critters are in the new creature in the location slot.
            
            // NOTE WE CANT update() here as this may be a lower layer and all the layers need to complete first.
            // Thats why do generation calls calcScore() and then update();
        }
        
        
        /************************************************************************
         creatureBetterThan ?
         *************************************************************************/
        // point of this is it allows flexibility as to what is better than the other
        // removes it to the creatureType class - whatever that defines it to be.
        template <class creatureType, class parasiteType>
        const bool creatureBetterThan(const location <creatureType,parasiteType> *lhs,
                                      const location <creatureType,parasiteType> *rhs) {
            
			// this used to read "return (lhs->creature->isBetterThan(lhs->creatureScore,rhs->creatureScore);"
			// but this was not flexible enough (e.g if I wanted to introduce lexigraphic parsimony control.
			// The solution is to pass into isBetterThan, not only the scores but also a pointer to the creature
            // we need to compare with - then if needed the method in creature can use some other method of
            // comparing the two creatures.
            
			// Note (shared_ptr<blah>).get() is the better and neater version
			// of &(*(shared_ptr<blah>)) i.e. returns a ptr to the underlying object.
            
			return lhs->creature->isBetterThan(lhs->creatureScore,(rhs->creature).get(),rhs->creatureScore);
        }
        
        /************************************************************************
         parasiteBetterThan ?
         *************************************************************************/
        
        template <class creatureType, class parasiteType>
        const bool parasiteBetterThan(const location <creatureType,parasiteType> *lhs,
                                      const location <creatureType,parasiteType> *rhs) {
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
                typedef location<creatureType, parasiteType> *simpleLocPtr;
                
                // Calculate size of neighbourhood and determine the rank you want
                // to select using the probability distribution contained in selectOne.
                
                
                // we want to remove from neighboursAndPrevious (temporarily) any critter that
                // does not have the same isParticipating as the owner of this location.
                // now with c++11 syntax!
                vector<simpleLocPtr> candidates;
                if (p->creature->isParticipating()) {
                    // this didn't do what I hoped as it moved null versions of the non-matching one into the vector, i.e. it wasn't a shrunk copy
                    //std::remove_copy_if(p->neighboursAndPrevious.begin(),p->neighboursAndPrevious.end(),candidates.begin(),[](simpleLocPtr p){return !(p->creature->isParticipating());});

                    // this kept crashing xcode - so Ill just do it the long way.
                    //auto forward = std::remove_if(candidates.begin(),(),[](simpleLocPtr p){return !(p->creature->isParticipating());});
                    //candidates.resize(std::distance(candidates.begin(),forward));

                    candidates.clear();
                    for (auto ctr=p->neighboursAndPrevious.begin();ctr < p->neighboursAndPrevious.end(); ctr++) {
                        if ((*ctr)->creature->isParticipating()) candidates.push_back(*ctr);
                    }
                } else {
                    candidates.clear();
                    for (auto ctr=p->neighboursAndPrevious.begin();ctr < p->neighboursAndPrevious.end(); ctr++) {
                        if (!((*ctr)->creature->isParticipating())) candidates.push_back(*ctr);
                    }
                }
                int nSize = candidates.size();
                if (nSize > 0) {
                    // SelectOne randomly selects a number eg. 1..nSize based on a (0.5)^n distribution (i.e. 50% its 1, 25% its 2, 12.5% its 3 etc.)
                    int select = selectOne(nSize);
                    
                    // The reason we select which one we want to "select" is that we don't need
                    // to sort the neighbours - we just need to get the nth element if we know
                    // the n we want.
                    
                    nth_element(candidates.begin(),candidates.begin()+select,candidates.end(),creatureBetterThan<creatureType,parasiteType>);
                    
                    // we use makeCopyOfCreature and getCreature to change the underlying creature
                    // if we just did p->newCreature = {blah}->creature we would just be swapping
                    // pointers to Creature types, which is not what we want.
                    // as then when we, say, mutated the creature it would mutate the copies in all
                    // locations.
                    
                    // The copy is quite an expensive operation (being a new cr_data) but its what we need to do!
                    // Note when we update creature to point to newCreature - we can just change pointers
                    // (the update so p takes the value of p' - using the nomenclature in the papers.
                    p->newCreature->makeCopyOfCreature(candidates[select]->creature);
				}
                else {
                    p->newCreature->makeCopyOfCreature(p->creature); // if none to select from copy the current occupier.
                    
                }
                
                
                // do the same with the parasites.
                
                candidates.clear();
                if (p->parasite->isParticipating()) {
                    candidates.clear();
                    for (auto ctr=p->neighboursAndPrevious.begin();ctr < p->neighboursAndPrevious.end(); ctr++) {
                        if ((*ctr)->parasite->isParticipating()) candidates.push_back(*ctr);
                    }
                } else {
                    candidates.clear();
                    for (auto ctr=p->neighboursAndPrevious.begin();ctr < p->neighboursAndPrevious.end(); ctr++) {
                        if (!((*ctr)->parasite->isParticipating())) candidates.push_back(*ctr);
                    }
                }
                nSize = candidates.size();
                if (nSize > 0) {
                    int select = selectOne(nSize);
                    nth_element(candidates.begin(),candidates.begin()+select,candidates.end(),parasiteBetterThan<creatureType,parasiteType>);
                    p->newParasite->makeCopyOfParasite(candidates[select]->parasite);
                } else {
                    p->newParasite->makeCopyOfParasite(p->parasite);
                }
            }
        };
        
        
        /************************************************************************
         selectBestCritters
         *************************************************************************/
        
        template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::selectBestCritters() {
            cout << "Layer " << layerNo << endl;
            for_each(theWorld.begin(),theWorld.end(),selectFromNeighbourhood<creatureType,parasiteType>());
        }
        
        
        
        
        
        /************************************************************************
         doCrossover
         *************************************************************************/
        
        template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::doCrossover() {
            // kind of assuming here that the active critter population is roughly the same as the active parasite population.
            //int activePop = count_if(theWorld.begin(),theWorld.end(),[] (boost::shared_ptr<location<creatureType,parasiteType> > &loc) { return loc->creature->isParticipating(); });
            int activePop = totalPop;
            for (int count = 0;count<(activePop*4)/10;count ++) { // 40% will be crossed over
                typedef location<creatureType, parasiteType> *simpleLocPtr;
                
                // With layers, this is confined to the current layer (neighbours) because
                // in the selectBestCritters part we selected and copied into "newCreatures" and
                // "newParasites" the slots in the layers, so competitive previous layer creatures/parasites
                // have already had the chance to be promoted up a layer.
                
                // We can only choose from those in the neighbourhood who have the isParticipating flag set.
                
                
                int chosen= randomint(totalPop);
                // we need to find one of the "nodes" where we have a candidate that is partcipating
                // this is a bit quick and dirty as it assumes we will stuble on one quickly. 
                // query - what happens if only a tiny portion are participating, then we would be better sorting out the participants
                // and choosing one of them. (or counting them, rolling the "die" and choosing the rolled one.
                while (!theWorld[chosen]->creature->isParticipating()) {
                    chosen = randomint(totalPop);
                }
                // we have our node, now we need to find a participating mate.
                
                vector<simpleLocPtr> candidates;
                
                // so ideally I would have used something like copy_if, but I was having problems with the vector being too large and null stuff in it
                // this works exactly as needed.
                for (auto counter=theWorld[chosen]->neighbours.begin();counter < theWorld[chosen]->neighbours.end(); counter++) {
                    if ((*counter)->creature->isParticipating()) { candidates.push_back(*counter);  }
                }
                
                if (candidates.size() > 0) {
                    int mate = randomint(candidates.size());
                    //      Call the doCrossover in the creature,this replaces the critter contained
                    //      in the creature class with the results of the crossover (if any)
                    theWorld[chosen]->newCreature->doCrossover(*(candidates[mate]->newCreature));
               }
                //      rinse and repeat for parasites. 
                
                chosen= randomint(totalPop);
                // see above - what happens if only a tiny portion are participating.
                while (!theWorld[chosen]->creature->isParticipating()) {
                    chosen = randomint(totalPop);
                }
                
                candidates.clear();
                // grab a vector of the parasites with the isParticipating() flag.
                for (auto counter=theWorld[chosen]->neighbours.begin();counter < theWorld[chosen]->neighbours.end(); counter++) {
                    if ((*counter)->parasite->isParticipating()) candidates.push_back(*counter);
                }
                if (candidates.size() > 0) { // i.e. we have something to choose from - if there are none just ignore and carry on.
                    int mate = randomint(candidates.size()); // choose one.
                    theWorld[chosen]->newParasite->doCrossover(*(candidates[mate]->newParasite));
                }
            }
        }
        
        
        /************************************************************************
         doMutation
         
         Note: I have hard coded the chances. Seems that I should probably fix that.
         *************************************************************************/
        
        template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::doMutation() {
            for (int count = 0; count < (totalPop*2)/10;count++) { // 20% will be selected for mutation (with re-selection)
                int chosen = randomint(totalPop);
                theWorld[chosen]->newCreature->doMutation();
            }
            for (int count = 0;count < (totalPop/10);count ++) { // 10% will be selected for mutation (with re-selection)
                int chosen = randomint(totalPop);
                theWorld[chosen]->newParasite->doMutation(getLayerNo());
            }
            
            // An alternative way would have been to give each creature/parasite a chance to mutate and
            // call the following for each of them
            //    for_each(theWorld.begin(),theWorld.end(),checkForMutation<creatureType,parasiteType>());
            // But here we are following the Melanie Mitchell algorithm
        }
        
        
        
        
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
                // only update the location if the newCreature/newParasite is valid - we can call really valid in creature/parasite to do this
                // The point of the "reallyValid" is in case the creature has some other test for it to be valid, not just, say, in the case of a GE creature that it decodes properly.
                // The case in point which made me implelent this was when I was evolving networks, I wanted an additional test that at least one output and one input was connected when
                // the "network growth language" was executed. Since this a quite expensive operation, it was delayed until we knew we were really wanting that particular genome.
                if (p->newCreature->isReallyValid()) {
                    // Set the underlying pointers in a creature to point to those in newCreature - we dont need to copy the creature
                    p->creature->setCreature(p->newCreature);
                } else {
                    //				cout << "New Creature was invalid, not updating " << p->id << endl;
                }
                if (p->creature->isParticipating()) { // only do the age stuff if the creature is participating
                    if (maxGenerationAge < 0 ) p->creature->geneticAge++;
                    else if (p->creature->geneticAge++ > maxGenerationAge) {
                        // creature is too old to live!
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
                                    // of couse this doesn't need to be fatal and is quite easily handled, but the nature of GP/GE is that if I just through an error message and
                                    // carried on the chances are the msg wouldn't be seen.
                                    // Since this is a situation which really shouldn't occur a fatal error is the best way of making sure its noticed (and hopefully sorted).
                                    cout << "Fatal Error: UpdateLocation created a new creature as the once it copied was not really valid, but the new creature is not really valid!\n";
                                    exit(1);
                                }
                            }
                        }
                    }
                }
                if (p->newParasite->isReallyValid()) {
                    p->parasite->setParasite(p->newParasite);
                } else {
                    //				cout << "New Parasite was invalid, not updating " << p->id << endl;
                }
                if (p->parasite->isParticipating()) {
                    if (maxGenerationAge < 0) p->parasite->geneticAge++;
                    else if ( p->parasite->geneticAge++ > maxGenerationAge) {
                        if (previousLayer == NULL) {
                            p->parasite.reset(new parasiteType());
                            if (!p->parasite->isReallyValid()) {
                                cout << "Fatal Error: Update created a new parasite which is not really valid!\n";
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
            }
        };
        
        /************************************************************************
         checkAgeLayer
         
         Note: used with the update method, and find_if to check if any creature/parasite has
         an age at least as great as the maxAge the unary_function is created with.
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
         update
         *************************************************************************/
        
        template <class creatureType, class parasiteType> void spatialWorld<creatureType,parasiteType>::update() {
            if (isItAnExpandingSpatialWorld) { // only true for most senior layer
                if (generationLife!=-1) {
                    checkAgeLayer<creatureType,parasiteType> doAgeLayer(generationLife);
                    // In case I forget find_if returns as soon as it gets a hit (on doAgeLayer). So if its
                    // not equal to theWorld.end(), we have a creature that is at the maximum age limit for that layer
                    // which means its time to add a new Layer.
                    if (find_if(theWorld.begin(),theWorld.end(),doAgeLayer) != theWorld.end()) {
                        addBottomLayer();
                    }
                }
            }
            updateLocation<creatureType,parasiteType> doUpdate(generationLife,previousLayer);
            for_each(theWorld.begin(),theWorld.end(),doUpdate);
            // unlike docalc etc, we need to update this layer before we update the prevous layers, in case we wipe an earlier layer.
            if (previousLayer) previousLayer->update();
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
         Note: bit if a at the moment hack - but it scans the world getting each creature to calculate using its realScore method
         You might use this e.g. with a regression problem to get its score against ALL THE DATA rather than just the datapoints
         encapsulated in the parasites.
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
            char filenameToUse[200];
            if (strlen(fname) > 100) {
                cout << "Been asked to use the filename " << fname << " which will cause a buffer overflow, need to change the size of the buffer in spatial.h savePop\n";
                exit(1);
            }
            sprintf(filenameToUse,"%s.layer%d.pop",fname,getLayerNo());
            outFile.open(filenameToUse,ios::out);
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
            if (previousLayer) return previousLayer->savePop(fname);
            else return true;
        }
        
        
        /************************************************************************
         loadPop
         *************************************************************************/
        
        template<class creatureType,class parasiteType> bool spatialWorld<creatureType,parasiteType>::loadPop(const char *currFileName,int layerLevel) {
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
                theWorld[i]->parasite->loadParasite(fin,layerLevel);
                if (previousLayer) {
                    theWorld[i]->creature->geneticAge = previousLayer->generationLife+1;
                    theWorld[i]->parasite->geneticAge = previousLayer->generationLife+1;
                }
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
                theWorld[at]->neighbours.push_back(theWorld[total-x+i-1].get());
                theWorld[at]->neighbours.push_back(theWorld[total-x+i].get());
                theWorld[at]->neighbours.push_back(theWorld[total-x+i+1].get());
                theWorld[at]->neighbours.push_back(theWorld[at-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at].get());
                theWorld[at]->neighbours.push_back(theWorld[at+1].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x+1].get());
            }
            for (int i=1;i<x-1;i++) { // step along the middle
                for (int j=1;j<y-1;j++) {
                    int at =i+x*j;
                    theWorld[at]->locationNumber = at;
                    theWorld[at]->id = at+layerNo*total;
                    theWorld[at]->neighbours.push_back(theWorld[at-x-1].get());
                    theWorld[at]->neighbours.push_back(theWorld[at-x].get());
                    theWorld[at]->neighbours.push_back(theWorld[at-x+1].get());
                    theWorld[at]->neighbours.push_back(theWorld[at-1].get());
                    theWorld[at]->neighbours.push_back(theWorld[at].get());
                    theWorld[at]->neighbours.push_back(theWorld[at+1].get());
                    theWorld[at]->neighbours.push_back(theWorld[at+x-1].get());
                    theWorld[at]->neighbours.push_back(theWorld[at+x].get());
                    theWorld[at]->neighbours.push_back(theWorld[at+x+1].get());
                }
            }
            for (int j=1;j<y-1;j++) { // LHS
                int at = x*j;
				theWorld[at]->locationNumber = at;
                theWorld[at]->id = at+layerNo*total;
                theWorld[at]->neighbours.push_back(theWorld[at-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at-x].get());
                theWorld[at]->neighbours.push_back(theWorld[at-x+1].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at].get());
                theWorld[at]->neighbours.push_back(theWorld[at+1].get());
                theWorld[at]->neighbours.push_back(theWorld[at+2*x-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x+1].get());
            }
            for (int j=1;j<y-1;j++) { // RHS
                int at = x*j+x-1;
				theWorld[at]->locationNumber = at;
                theWorld[at]->id = at+layerNo*total;
                theWorld[at]->neighbours.push_back(theWorld[at-x-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at-x].get());
                theWorld[at]->neighbours.push_back(theWorld[at-2*x+1].get());
                theWorld[at]->neighbours.push_back(theWorld[at-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at].get());
                theWorld[at]->neighbours.push_back(theWorld[at-x+1].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at+x].get());
                theWorld[at]->neighbours.push_back(theWorld[at+1].get());
            }
            for (int j=1;j<x-1;j++) { // Bottom row
                int at = (y-1)*x+j;
				theWorld[at]->locationNumber = at;
                theWorld[at]->id = at+layerNo*total;
                theWorld[at]->neighbours.push_back(theWorld[at-x-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at-x].get());
                theWorld[at]->neighbours.push_back(theWorld[at-x+1].get());
                theWorld[at]->neighbours.push_back(theWorld[at-1].get());
                theWorld[at]->neighbours.push_back(theWorld[at].get());
                theWorld[at]->neighbours.push_back(theWorld[at+1].get());
                theWorld[at]->neighbours.push_back(theWorld[j-1].get());
                theWorld[at]->neighbours.push_back(theWorld[j].get());
                theWorld[at]->neighbours.push_back(theWorld[j+1].get());
            }
            // Now corner 0,0
            theWorld[0]->id = 0+layerNo*total;
            theWorld[0]->locationNumber = 0;
            theWorld[0]->neighbours.push_back(theWorld[total-1].get());
            theWorld[0]->neighbours.push_back(theWorld[total-x].get());
            theWorld[0]->neighbours.push_back(theWorld[total-x+1].get());
            theWorld[0]->neighbours.push_back(theWorld[x-1].get());
            theWorld[0]->neighbours.push_back(theWorld[0].get());
            theWorld[0]->neighbours.push_back(theWorld[1].get());
            theWorld[0]->neighbours.push_back(theWorld[2*x-1].get());
            theWorld[0]->neighbours.push_back(theWorld[x].get());
            theWorld[0]->neighbours.push_back(theWorld[x+1].get());
            // corner X,0 (which has value x-1 as we start at 0.
            theWorld[x-1]->id = x-1+layerNo*total;
            theWorld[x-1]->locationNumber = x-1;
            theWorld[x-1]->neighbours.push_back(theWorld[total-2].get());
            theWorld[x-1]->neighbours.push_back(theWorld[total-1].get());
            theWorld[x-1]->neighbours.push_back(theWorld[total-x].get());
            theWorld[x-1]->neighbours.push_back(theWorld[x-2].get());
            theWorld[x-1]->neighbours.push_back(theWorld[x-1].get());
            theWorld[x-1]->neighbours.push_back(theWorld[0].get());
            theWorld[x-1]->neighbours.push_back(theWorld[2*x-2].get());
            theWorld[x-1]->neighbours.push_back(theWorld[2*x-1].get());
            theWorld[x-1]->neighbours.push_back(theWorld[x].get());
            // corner 0,Y
            theWorld[total-x]->id = total-x+layerNo*total;
            theWorld[total-x]->locationNumber = total-x;
            theWorld[total-x]->neighbours.push_back(theWorld[total-x-1].get());
            theWorld[total-x]->neighbours.push_back(theWorld[total-2*x].get());
            theWorld[total-x]->neighbours.push_back(theWorld[total-2*x+1].get());
            theWorld[total-x]->neighbours.push_back(theWorld[total-1].get());
            theWorld[total-x]->neighbours.push_back(theWorld[total-x].get());
            theWorld[total-x]->neighbours.push_back(theWorld[total-x+1].get());
            theWorld[total-x]->neighbours.push_back(theWorld[x-1].get());
            theWorld[total-x]->neighbours.push_back(theWorld[0].get());
            theWorld[total-x]->neighbours.push_back(theWorld[1].get());
            // and last but not least corner x,y
            theWorld[total-1]->locationNumber = total-1;
            theWorld[total-1]->id = total-1+layerNo*total;
            theWorld[total-1]->neighbours.push_back(theWorld[total-x-2].get());
            theWorld[total-1]->neighbours.push_back(theWorld[total-x-1].get());
            theWorld[total-1]->neighbours.push_back(theWorld[total-2*x].get());
            theWorld[total-1]->neighbours.push_back(theWorld[total-2].get());
            theWorld[total-1]->neighbours.push_back(theWorld[total-1].get());
            theWorld[total-1]->neighbours.push_back(theWorld[total-x].get());
            theWorld[total-1]->neighbours.push_back(theWorld[x-2].get());
            theWorld[total-1]->neighbours.push_back(theWorld[x-1].get());
            theWorld[total-1]->neighbours.push_back(theWorld[0].get());
            
        }
        
#endif
