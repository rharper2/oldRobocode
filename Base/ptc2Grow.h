/*
 *  ptc2Grow.h
 *  ThesisCode
 *  Header file to implement classes
 *  Needed for the ptc2 style of tree growing
 * Sean Luke - Two Fast Tree-Creation Algorithms
 * IEEE Transactions on Evolutionary Computation, Vol 4, No 3. Sep 2000
 *
 *  Created by Robin on 30/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream> 
#ifndef ptc2GROW
#define ptc2GROW
#include <boost/shared_ptr.hpp>

using namespace std;

class parseTreeNode;

typedef boost::shared_ptr<parseTreeNode> parseTreePtr;

class parseTreeNode {

public:
	int value;
	int depth;
	long codonValue;
	bool codonUsed;
	parseTreePtr next;
	parseTreePtr expansion;
	
	parseTreeNode() { 
		value = -1; depth = 0; 
	}

};



#endif


