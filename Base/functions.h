/*
 *  functions.h
 *  
 *
 *  Created by Robin Harper on 5/10/04.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef FUNCTION
#define FUNCTION
#include "params.h"

class function {
public:
	int expression[LENGTH_OF_EXPRESSION];
	int expLength;
	int fno; // the function number
	function();
	int params;
};
	
#endif
