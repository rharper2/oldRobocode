/*
 *  spatialCreatureBases.h
 *  GrammarTest
 *
 *  Created by Robin Harper on 25/08/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include "critter.h"

class spatialParasiteBase;

class spatialCreatureBase {
    crPtr critter;
public:
    spatialCreatureBase();
    spatialCreatureBase(bool create);
    virtual ~spatialCreatureBase() { }
    virtual void doCrossover(spatialCreatureBase &p2) = 0;
    virtual void doMutation()=0;
    virtual double getScore(spatialParasiteBase &p1) = 0;
	virtual void clearScores()=0;
	virtual bool isBetterThan(spatialCreatureBase *cr) = 0;
    virtual bool isBetterThan(double lhs,double rhs) = 0;
    virtual double realScore() { return 0.0; }
    virtual void setCreature(crPtr toset) { critter = toset; }
    virtual void makeCopyOfCreature(crPtr tocopy) {
			 cr_data *newCopy;
			 newCopy = new cr_data(*tocopy); // dereference the crPtr to refer to the cr_data
			 critter.reset(newCopy);
	}
    virtual crPtr getCreature() { return critter; }
    virtual void display() { if (critter) cout << critter->display(); }
	virtual void display(const char *);
};
    
class spatialParasiteBase {
    crPtr parasite;
public:
    spatialParasiteBase();
    spatialParasiteBase(bool create);
    virtual ~spatialParasiteBase() { } 
    virtual void doCrossover(spatialParasiteBase &p1) =0;
	virtual void doMutation()=0;
    virtual double getScore(spatialCreatureBase &p1) = 0;
	virtual void clearScores() = 0;
	virtual bool isBetterThan(spatialParasiteBase *para) = 0;
    virtual bool isBetterThan(double lhs,double rhs) = 0;
    virtual void setParasite(crPtr toset) { parasite = toset; }
    virtual void makeCopyOfParasite(crPtr tocopy) {
		cr_data *newCopy;
		newCopy = new cr_data(*tocopy);
		parasite.reset(newCopy);
	}
	virtual crPtr &getParasite() { return parasite; }
    virtual void display() { if (parasite) cout << parasite->display(); }
};
