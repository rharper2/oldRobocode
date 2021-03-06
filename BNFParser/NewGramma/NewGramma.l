%{
/*********************************************************************/
/*         Altered by Robin Harper, to allow a different syntax      */
/*         and to implement the keywords needed for DDF              */
/*         Also different look up tables for enum support            */
/*		   Last Update August 2010. 								 */
/*                                                                   */
/*                                                                   */
/*          Gramma v0.63 - programmed by Miguel Nicolau              */
/*                                                                   */
/*         Gramma.l - written with AT&T lex specification            */
/*                                                                   */
/*********************************************************************/
#include "y.tab.h"
#include <string.h>
#define FIRSTTERMINAL 1
#define FIRSTNONTERMINAL 1001
#define FIRSTDUMMYNONTERMINAL 5001
int state = NONTERMINAL;
int pass = 1;
int nonterminal = FIRSTNONTERMINAL;
int terminal = FIRSTTERMINAL;
int dummyNonTerminal = FIRSTDUMMYNONTERMINAL;

/*struct element;*/

typedef struct smallelement{
  struct element *value;
  struct smallelement *next;
  /* the following are used to try and determine which branches
     of the grammar are expansionary or terminate the expansion */
  int willTerminate,mightTerminate,mightExpand,willExpand;
} selem;

typedef struct element{
    char *symbolName;
	char *symbolRepresentation;
    int code, type, rtype;
    int willTerminate,mightTerminate, mightExpand,willExpand;

    selem *chain,/* chain of RHS elements */
        *last;/* the last of those elements (to help insertion) */
	int total;/* number of RHS elements following this NONTERMINAL */
    struct element *next;
  } elem;

elem *header=NULL,/* the first element of the symbol table */
	 *lastNT = NULL, /* the last non terminal in the symbol table */
     *footer=NULL,/* the last element of the symbol table */
     *left=NULL,/* the left-side element of the currently parsed rule */
     *right=NULL;/* the current symbol in the RHS of the rule (work variable) */
int type;
int lineNo;

elem *addSymbol(char *text, char *repr,int type);
void addRightHandSide(elem *left, elem *right);
elem *retrieveSymbolByName(char *text,int *type);
int addUnique(char *);

%}

%s FIRST SECOND

%%
[\t ]+   
^[\t ]*\n
\n     {
	//			state = NONTERMINAL;
				if(pass>1){
					lineNo++;
//					return CF;
                }
             }
(%%\n)       {
				if(++pass == 2){ /* temporary token to end grammar definition */
					fseek(yyin, 0, 0);
					state = NONTERMINAL;
					BEGIN SECOND;
					lineNo = 1;
				}
				else
                return 0;
             }


<INITIAL>^[^ \n\t]+:	{ 		 // In the first pass scan spot all the expansion and note them as non-terminals
									yytext[strlen(yytext)-1]=0; // strip off the colon
									addSymbol(yytext,yytext,NONTERMINAL);
						}

<SECOND>^[^ \n\t]+:     {   // ok we have matched the beginning of a new expansion.
							yytext[strlen(yytext)-1]=0; // strip off the colon
							left = retrieveSymbolByName(yytext,&type);
							state = TERMINAL;
							return NONTERMINAL;	
						}	

<SECOND>[\(\)\{\}\[\];] {
							if(pass > 1){
								if(state != NONTERMINAL){
									right = addSymbol(yytext,yytext,TERMINAL);
									addRightHandSide(left,right);
//									printf("Just added *%s*",yytext);
									return right->type;
								}
								else{
									printf("Error with token \"%s\" on line %d.\n",yytext,lineNo);
                                }
                            }
                        }
<SECOND>(([^:| \t\n])|("|"{2}))+ {
//	printf("yytext = %s\n",yytext);
	if((pass == 1) && (state == NONTERMINAL)){
		printf("STOPSTOPSTOPSTOPSTOPSTOPSTOPSTOP*******************************\n");
		addSymbol(yytext,yytext,state);
		state = TERMINAL;
	}
	else if(pass > 1){
		right = addSymbol(yytext,yytext,TERMINAL);
		if(state != NONTERMINAL){
			/* add the symbol to the RHS of the rule */
			addRightHandSide(left,right);
//			printf("Just added *%s* to the rhs",yytext);
			/*printf("Sending %i\n",right->type);*/
			return TERMINAL;
		}
		else{/* NONTERMINAL state */
			left = retrieveSymbolByName(yytext,&type);
			state = TERMINAL;
//			printf("\nJust adding *%s* as a LHS: ",yytext);
			/*printf("Sending %i\n",left->type);*/
			printf("I really shouldn't be here.\n");
			return left->type;
		}
	}
}
<SECOND>\|           {
						if(pass > 1){
							/*printf("Sending %s\n",yytext);*/
							addRightHandSide(left,NULL);/* Insert a separator */
//							printf("Adding a separator\n");
							return yytext[0];
						}
					  }
			/* [ \t]+        ;*/
.            {
				if(pass > 1) {
//					printf("Default to %s\n",yytext);
					return yytext[0];}
			  }
	
%%

void makeupper(char *txt) {
	// printf("Called with %s\n",txt);
	while(*txt) {
		if (*txt>='a' && *txt <= 'z') {
		//	printf("Would change %c\n",*txt); 
			*txt = *txt - ('a' - 'A');
		}
		txt++;
	}
}


elem *addSymbol(char *text, char *repr,int type){
  elem *p;
  int bidon;

 // printf("Adding %s as %s , type %d\n",text,repr,type);

  if((p = retrieveSymbolByName(text,&bidon)))/* Already exists */
	{ 
//		printf("Already there\n");  
  		return p;/* treat it as a look up */
  	}
  //printf("ok\n");
  p = (elem *)malloc(sizeof(elem));
  p->symbolName = (char *)malloc(strlen(text)+1);
  p->symbolRepresentation = (char *)malloc(strlen(repr) +1);
  p->willTerminate = p->mightTerminate = p->mightExpand = 0;
  strcpy(p->symbolName,text);
  strcpy(p->symbolRepresentation,repr);
  makeupper(p->symbolRepresentation);
  if(type==NONTERMINAL) {
	  p->code = nonterminal++;
//	  printf("Giving it code %d\n",p->code);
  }	else if (type == TERMINAL) {
	  p->code = terminal++;
//	  printf("Gviing it code %d\n",p->code);
  } else {
	p->code = dummyNonTerminal++;
  }
  p->type = type;
  p->chain = NULL;
  p->last = NULL;
  p->total = 1;
  p->next = NULL;
  if (type == NONTERMINAL) {
	  if (!lastNT) {
//		 printf("Adding first NT\n");
		  p->next = header;
		  header = p;
		  footer = p;
	  } else {
//		printf("Adding another NT\n");
		  p->next = lastNT->next;
		  lastNT->next = p;
		  if (footer == lastNT) footer = p;
	  }
	  lastNT=p;
  } else {
	  if(!footer)/* first element */
		  header = p;
	  else
		  footer->next = p;
	  footer = p;
  }
//  enumSymbolTable(1,NULL,NULL);
//  while (enumSymbolTable(0,NULL,NULL)) ;
  return p;
}

elem *changeSymbolText(char *text,char *repr){
  elem *p;
  int bidon;
  if (! (p = retrieveSymbolByName(text,&bidon))) {
	return 0; // doesn't exist, so cant change it this is an error
   }
	
   // free(p->symbolRepresentation);
   p->symbolRepresentation = (char *)malloc(strlen(repr)+1);
   strcpy(p->symbolRepresentation,repr);
   return p;
}

void addRightHandSide(elem *left, elem *right){
  selem *p;

  p = (selem*)malloc(sizeof(selem));
  p->value = right;
  p->willTerminate = p->mightTerminate = p->mightExpand = p->willExpand = 0;
  p->next = NULL;
  if (!left->last)/* first RHS element */
    left->chain = p;
  else
    left->last->next = p;
  left->last = p;
  if(!right)
    left->total++;
}

elem *retrieveSymbolByName(char *text, int *type){
  elem *p = header;

// printf("Retrieving *%s*\n",text);

  for(; p; p = p->next){
    if(!strcmp(p->symbolName,text)){/* found it */
//       printf("Its name was %s and its representation was %s\n",p->symbolName,p->symbolRepresentation); 
      *type = p->type;
      return p;
      }
    }
  return NULL;
}

int retrieveCodeByName(char *text) {
	elem *p = header;
	for (; p;p=p->next) {
		if (!strcmp(p->symbolName,text)) { /* found it */
			return p->code;
		}
	}
	return -1;
}

elem *symbolsTokAt;
selem *branchrhsAt;

int symbolsTok(int init, char *text, int *c, int *t, int *total, char *text2){
  static elem *p;

// printf("sTok *%s*\n",text);

  if(init) {
    symbolsTokAt= header;
    p = header;
  } else {
    p = p->next;
    symbolsTokAt=p;
  }
  if(p == NULL)
    return 0;/* Reached the end of the symbol table */
  strcpy(text,p->symbolName);
  strcpy(text2,p->symbolRepresentation);
  *c = p->code;
  *t = p->type;
  *total = p->total;
  return 1;/* Operation successful */
}

int rhsTok(int init, char *text, int *c,char *text2){
  static elem *p;
  static selem *s;
  int bidon;

// printf("RHSTOK *%s*\n",text);

  if(init)
    if((p = retrieveSymbolByName(text,&bidon))) {
      s = p->chain;
      branchrhsAt = s;
    }
    else
      return 0;
  else
    s = s->next;
  if(!p)
    return 0;/* Reached the end of the rhs symbols */
  if(!s){
    p = NULL;/* So that on next call we will return 0 */
    return 2;/* Warn that the current transformation is over */
    }
  if(!s->value)/* This is a separator element */
    {   
        branchrhsAt = s->next;
        return 2;/* Warn that the current transformation is over */
    }
  strcpy(text,s->value->symbolName);
  strcpy(text2,s->value->symbolRepresentation);
  *c = s->value->code;
  return 1;/* Operation successful */
}

int chainContains(char *text,int match) {
    elem *p;
    selem *s;
    int bidon;
    int indexCount;

    indexCount = 1;
    if ( (p=retrieveSymbolByName(text,&bidon))) {
	   //printf("retreived it: %s\n",text);
       s = p->chain;
    }
	else return 0;
    while (s) {
      //printf("in while s loop\n");	
	if (!s->value) indexCount++;
	else if (s->value->code == match) return indexCount;
	s = s->next;
   }
   return 0;
}

int enumSymbolTable(int restart,char *representation,int *code) {
	static elem *p;
	if (restart) 
		p =header;
	if (p==NULL) return 0;
	strcpy(representation,p->symbolRepresentation);
	*code=p->code;
//	printf("%s = %d\n",p->symbolRepresentation,p->code);
	p=p->next;
	return 1;
}

int addUnique(char *text){
	elem *p;
	int bidon;
	
	if((p = retrieveSymbolByName(text,&bidon)))/* Already exists */
		return 0;/* this is an error*/
	addSymbol(text,text,TERMINAL);
	return 1;
}
	
