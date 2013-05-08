//
//  javaCodesnips.java
//  
//
//  Created by robin on 9/09/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
package Gen380;
import  robocode.*;
import java.util.*;
import robocode.util.Utils ; 
public  class Critter11858 extends  AdvancedRobot 

#define MAXSTACK 100
{ 
    Stack<Double> globalStack1, globalStack2;
    
    
    //********************************
    //Some useful helper functions
    //********************************
    
    public  double  PDIV(  double  top  ,  double  bottom  ) 
    {  if  (  bottom  ==  0  )  return  1  ;  else  return  top  /  bottom  ; 
    }
    
    public  void  stackPush(  Stack  thestack  ,  double  item  ) 
    {  if (thestack.size() > MAXSTACK) thestack.remove(0); 
       thestack.push(  new  Double(  item  )  )  ; 
    }
    
    public  double  stackPop(  Stack  thestack  ) 
    {  if  (  thestack.empty()  )  return  0  ;  else  return  ( (Double) thestack.pop()).doubleValue()  ; 
    }
    
    public  double  stackPeek(  Stack  thestack  ) 
    {  if  (  thestack.empty()  )  return  0  ;  else  return  ( (Double) thestack.peek()).doubleValue()  ; 
    }

public void stackDUP(Stack thestack ) {
{if (thestack.size() > MAXSTACK) thestack.remove(0); 
if (theStack.size() == 0) return; theStack.push(,theStack.peek());
}

public void stackSWAP(Stack theStack) {
if (theStack.size() < 2) return;
double top = stackPop(theStack);
theStack.add(theStack.size()-2,top);

}

public void stackOVER(Stack theStack) {
if (thestack.size() > MAXSTACK) thestack.remove(0); 
if (theStack.size() > 1) {
theStack.add(get(theStack.size()-2))
}
}

public void stackNIP(Stack theStack) {
if (theStack.size() > 1) {
theStack.remove(theStack.size()-2);
}
}

public void stackTUCK(Stack theStack) {
if (thestack.size() > MAXSTACK) thestack.remove(0); 
if (theStack.size()>2) theStack.add(theStack.size()-2,theStack.peek());
}
    
public void stackROT(Stack theStack) {
if (theStack.size()>2) {
Object removed = (theStack.remove(theStack.size()-3));
theStack.push(new Double(removed));
}

public void stackRROT(Stack theStack) {
if (theStack.size()>2) {
Object removed = (theStack.remove(theStack.size()-1));
theStack.add(theStack.size()-2,removed);
}
}

public void stackPICKU(Stack theStack) {
if (theStack.size() > 1) {
    double index = stackPop(theStack);
index = theStack.size() - index;
if (index < 0) index = 0;
if (index > theStack.size() -1) index = theStack.size()-1;
theStack.push(theStack.get(index));

}


public void stack2DUP(Stack theStack) {
if (thestack.size() > MAXSTACK-1 ) {thestack.remove(0); thestack.remove(0); }
if (theStack.size() == 0) return; 
if (theStack.size() == 1) { theStack.push(theStack.peek());theStack.push(theStack.peek());}
else { theStack.push(theStack.get(theStack.size()-2);theStack.push(theStack.get(theStack.size()-2);}

}