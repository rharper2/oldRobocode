/*
 *  funcinterpreter.h
 *  test
 *
 *  Created by Robin Harper on 7/10/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef INTERPBASE
#define INTERPBASE 

#include <vector>
#include <map>
#include <iostream>
#include "functions.h"
#include "params.h"

using namespace std;


class interpreterBase {
public:

          virtual double getScore(int *ex,vector<function> *fV) { return 0; }
          virtual double getScore(int *ex,vector<function> *fV,int &) { return getScore(ex,fV); }
          virtual double getAltScore(int *ex,vector<function> *fV) { return getScore(ex,fV);}
          virtual void displayMaps(ostream &,int,int*,vector<function> *fV) =0;
          virtual ~interpreterBase() {}
          virtual bool newGen() { return false;}
          virtual void newGenStarted() { }
       //   virtual void setTestCase(int) { }
		  virtual bool validate(int *ex,vector<function> *fV) { return true; } 
		  // typically the interpreter doesnt need to validate the grammatical expression
		  // but if, for example, you are building networks you might only clasify the
		  // critter as correct (and capable of evaluation) IF it connects an input and
		  // output node and can build a network.
protected:
          interpreterBase() {}; // all interpreters are singletons
private: 
 };

#endif

 
 
