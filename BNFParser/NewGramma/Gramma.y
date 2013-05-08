
%{
	/*********************************************************************/
	// 
	//		Function Gramma y - programmed by Robin Harper (robinh@cse.unsw.edu.au)
	//	    rharper2@mac.com
	//
	//	    Copyright Robin Harper, August 2010.
	//      Its been amended to provide support for DDFs
	//      Ramped H&H and ptc2 (which require some guesswork as to which 
	//      parts of the grammar expand and which terminate)
	//      Enums are now used instead of numbers.
	//		Version 0.3 (of the dynamic grammar form) August 2010
	//
	//	    Based on an original file by:
	//          Gramma v0.63 - programmed by Miguel Nicolau              
	//                                                                   
	//        Gramma.y - written with AT&T yacc specification            
	//                                                                   
	/*********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h> /* Some systems require this */
	%}
%token TERMINAL NONTERMINAL CF DUMMYNONTERMINAL
%%
setOfRules      :rule
|setOfRules rule
;
rule            :NONTERMINAL transformation 
;
transformation  :setOfSymbols
|transformation '|' setOfSymbols
;
setOfSymbols    :TERMINAL setOfSymbols
|TERMINAL
;
%%

typedef struct smallelement{
	struct element *value;
	struct smallelement *next;
	int willTerminate,mightTerminate, mightExpand,willExpand;
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

extern FILE *yyin;
extern int symbolsTok(int init, char *text, int *c, int *t, int *total,char *text2);
extern int rhsTok(int init, char *text, int *c,char *text2);
extern int lineNo;
extern char *yytext;
extern int retrieveCodeByName(char *text);
extern elem *changeSymbolText(char *text,char *repr);
extern elem *addSymbol(char *text,char *repr,int type);
extern int chainContains(char *text, int match);
extern int enumSymbolTable(int,char*,int*);
extern int addUnique(char *text);
extern elem *retrieveSymbolByName(char *text,int *type);
extern elem *symbolsTokAt;
void suball(char*,char*,char*);
extern elem *header;
extern selem *branchrhsAt;

int DYNAMICGRAMMAR;


void identifyGrammarRules() {
    selem *s,*branchHead;
    elem *p;
    int changed,found,nonTerms;
    int hasTerminatingBranch,hasExpandingBranch,hasUnknownBranch;
    int funcCode;
	int defunCode;
	int paramCode;
    p = header;
    funcCode = retrieveCodeByName("FUNC");
    if (DYNAMICGRAMMAR && (funcCode == -1)) printf("No functions appear to have been defined\n");
	defunCode = retrieveCodeByName("DEFUN");
	if (DYNAMICGRAMMAR && (defunCode == -1)) printf("No function definition appears to have been allowed e.g. DEFUN\n");
	paramCode = retrieveCodeByName("PARAM");
	if (DYNAMICGRAMMAR && (paramCode == -1)) printf("No PARAM keyword appears to have been used.\n");
	// set the rtype of all terminals to TERMINAL
	while (p) {
        if (p->type == TERMINAL) {
            if (p->code == defunCode || p->code == paramCode) {
				// bit of common theme here, keywords are nonterminals.
                p->rtype = NONTERMINAL;
            } else {
                p->rtype = TERMINAL;
                p->willTerminate = 1;
			}
        }
        else p->rtype = NONTERMINAL;
        p = p->next;
    }
    // identify any non-terminals that only expand to terminals
    do {
        changed = 0;
        p = header;
        while (p) {
            if (p->rtype == NONTERMINAL && (p->code!= funcCode)) {
                s = p->chain;
                while (s)  {
                    found = 1;
                    //if (s->value && ((s->value->rtype != TERMINAL) || (s->value->code == funcCode))) {
                    if (s->value && (s->value->rtype != TERMINAL) ) {
                        found = 0;
                        break;
                    }
                    s = s->next;
                }
                if (found) {
                    p->rtype = TERMINAL;
                    p->willTerminate = 1;
                    changed = 1;
                }
            }
            p =p->next;
		}
    } while(changed); 
	// i.e. until we did a full scan and didnt set anything else to TERMINAL
    // So far so good - now we know what non-terminals are really terminals.
    // We now need to identify the following 
    // for each branch of an expansion we need to know if 
    // It 1) leads to a terminal (willTerminate == 1) // Note started to set with Terminal Identification
    // 2) If it might lead to a terminal (mightTerminate == 1)
    // 3) If it might lead to an expansion (mightExpand == 1)
    // And numbers 2 and 3 are  not mutually exclusive!
    p = header;
    changed = 0;
    while (p) {
        if (p->rtype != TERMINAL) {
            hasTerminatingBranch = 0;
            hasExpandingBranch = 0;
            hasUnknownBranch = 0;
            s = p->chain;
            while (s) {
                // s is now at the beginning of an expansion
                nonTerms = 0;
                branchHead = s;
                while (s && s->value) { // s->value is null when we get to the end of a branch
				    if (s->value->rtype != TERMINAL) {
						nonTerms++;
                    }
					/* // removed, turns out its best to view functions as terminals (I think)
					 if (s->value->code == funcCode) {
					 nonTerms+=2; // ie definitely an expansion by our definition
					 }
					 */
			        if (s->value->chain && s->value->chain->willExpand) {
                        nonTerms+=2;
                    }
                    s = s->next;
                } // we have reached the end of one potential expansion of p
                // if nonTerms == 0 it means that expansion only has Terminals
                if (nonTerms == 0) {
                    branchHead->willTerminate = 1;
                    hasTerminatingBranch = 1;
                    //printf("Terminal branch in %s, branch starts %s, will Terminate set\n",p->symbolName,branchHead->value->symbolName);
                }
                else if (nonTerms >1) {
                    // The branch has more than one non-terminal in it
                    // we will therefore assume it might expand our expression
                    // remember if any of the non-terminals automatically leads
                    // to a terminal, the rtype will also be terminal
                    branchHead->willExpand = 1;
                    hasExpandingBranch = 1;
                    //printf("Expanding branch in %s, branch starts %s, will Expand set\n",p->symbolName,branchHead->value->symbolName);
                } else {
                    branchHead->mightTerminate = 1;
                    branchHead->mightExpand = 1;
                    hasUnknownBranch = 1;
                    //printf("Unknown branch in %s, branch starts %s, mightTerminate and mightExpand set\n",p->symbolName,branchHead->value->symbolName);
                }
                if (s) s=s->next;
            }
            // we have done all the branches now to set the flags
            if (hasTerminatingBranch){
                if (!hasExpandingBranch && !hasUnknownBranch) {
                    p->willTerminate = 1; // Odd that I am here I think
                    //printf("Nonterminal %s will terminate\n",p->symbolName);
                } else {
                    p->mightTerminate = 1;
					//printf("Nonterminal %s might terminate\n",p->symbolName);
                }
            }
            if (hasExpandingBranch) {
                if (!hasTerminatingBranch && !hasUnknownBranch) {
                    p->willExpand = 1;
                    //printf("NonTerminal %s will expand\n",p->symbolName);
                } else {
                    p->mightExpand = 1;
                    //printf("NonTerminal %s might expand\n",p->symbolName);
                }
            }
            if (hasUnknownBranch) {
                p->mightTerminate = 1;
                p->mightExpand = 1;
				//printf("NonTerminal %s has unknown branch, might Terminate might Expand\n",p->symbolName);
            }
        }
        p=p->next;
    }
    
}    



int main(int argc, char **argv){
	char text[25555], text2[25555], output[204855], tmp[25555], *ptr;
	char ctext[25555];
	char gname[25555];
	int code, code2, type, total, caseIndex, index, result;
    int functodo,paramstodo;
    int growcount;
	FILE *f,*in;
	int defunCode, paramCode, funcCode, puseCode,funcBodyTypeCode,cvalCode; 
	int commaCode, funcParTypeCode, oFB,cFB;
	int place,willExpand,mightExpand;
	int defunThisBranch,paramsThisBranch,funcsThisBranch,cvalThisBranch;
	int nonTerminal;
	int forceAdvance;
	int terminateElse;
	elem *lhs,*rhs;
    selem *s;
	DYNAMICGRAMMAR = 1;
	if(argc!=2){
		printf("Function GE grammar parser v0.3\nUsage: %s <file>\n",argv[0]);
		printf("Output will be generated to file <name of grammar>.cpp\n");
		printf("Generates a dynamic grammar from the bnf file, which should ");
		printf("slot into the Dynamic GE system.\n");
		return 0;
    }
	if (!strstr(argv[1],".bnf")) {
		printf("Grammar file must end in .bnf\n");
		return 0;
	}
	yyin = fopen(argv[1],"r");
	strcpy(tmp,argv[1]);
	(*strstr(tmp,".bnf"))=0;
	strcpy(gname,tmp); /* This will become the grammar name */
	strcat(tmp,".h");
	f = fopen(tmp,"w");
	in = fopen("grammar.template.h","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	strcpy(tmp,gname);
	strcat(tmp,".cpp");
	f = fopen(tmp,"w");
	
	
	in = fopen("grammar.template.p1","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	fprintf(f,"/***************************************************************/\n");
	fprintf(f,"// File generated from grammar included in file:\n");
	fprintf(f,"// %s\n",argv[1]);
	fprintf(f,"/***************************************************************/\n");
	while(fgets(output,120,yyin)){
		fprintf(f,"// %s",output);
	}
	fprintf(f,"/***************************************************************/\n");
    fseek(yyin,0,0);
    addSymbol("code","CODE",NONTERMINAL);
    addSymbol(",","COMMA",TERMINAL);    
    addSymbol("(","oRB",TERMINAL);      
    addSymbol(")","cRB",TERMINAL);      
    addSymbol("[","oSB",TERMINAL);	    
    addSymbol("]","cSB",TERMINAL);	    
    addSymbol("DEFUN","DEFUN",TERMINAL); 
    addSymbol("*(","oFB",TERMINAL);     
    addSymbol(")*","cFB",TERMINAL);     
    addSymbol("{","oCB",TERMINAL);      
    addSymbol("}","cCB",TERMINAL);      
    addSymbol("*{","BEGINEXEC",TERMINAL); 
    addSymbol("}*","ENDEXEC",TERMINAL); 
    addSymbol("+","PLUS",TERMINAL);     
    addSymbol("-","MINUS",TERMINAL); 
    addSymbol("*","MULT",TERMINAL);   
    addSymbol("%","DIV",TERMINAL);    
    addSymbol(";","SEMICOLON",TERMINAL); 
    addSymbol(":","COLON",TERMINAL);
    addSymbol(">","GREATER",TERMINAL);
    addSymbol("<","LESS",TERMINAL); 
    addSymbol("==","EEQUAL",TERMINAL);
    addSymbol("=","EQUAL",TERMINAL);  
    addSymbol("0","ZERO",TERMINAL);		
    addSymbol("1","ONE",TERMINAL);		
    addSymbol("2","TWO",TERMINAL);		
    addSymbol("3","THREE",TERMINAL);	
    addSymbol("4","FOUR",TERMINAL);		
    addSymbol("5","FIVE",TERMINAL);		
    addSymbol("6","SIX",TERMINAL);		
    addSymbol("7","SEVEN",TERMINAL);	
    addSymbol("8","EIGHT",TERMINAL);	
    addSymbol("9","NINE",TERMINAL);		
    addSymbol("NOP","NOP",TERMINAL);
    addSymbol("BEPOPTREE","BEPOPTREE",TERMINAL);
    addSymbol("FUNCPLACEHOLDER","FUNCPLACEHOLDER",TERMINAL);
    addSymbol("PUSEPLACEHOLDER","PUSEPLACEHOLDER",TERMINAL);
    //addSymbol("CVAL","CVAL",TERMINAL);
	do{
		yyparse();
	}   while(!feof(yyin));
	
    if (retrieveSymbolByName("CVAL",&type)) {
        if (type == NONTERMINAL) {
            printf("CVAL is a special keyword it stands for: store the next Codon VALue. Its primary use is where you might need to manipulate numbers.\n");
            printf("One of the problems with just using straight grammar eg Number: DIGIT DIGIT DIGIT, is that would take three codons to store\n");
            printf("And might bias searches since a random cut in the bit stream makes it more likely that a number would be found rather than say an\n");
            printf("expression expansion, so by storing a CVAL you can cut down the number of codons needed, in addition the parse tree crossovers implemented know\n");
            printf("they can ignore CVALs when looking for branches to swap (in theory). So the upshot of this is CVAL must be a terminal, and you are using\n");
            printf("it on the left hand side of a grammar rule, so you will just have to change the name of your rule. If you are not worried about numbers just change\n");
            printf("the name of your rule, if you want to use numbers and are confused look in the regression grammars and interpreters provided to see how\n");
            printf("to use it effectively.\n");
            exit(1);
        }
    }
	
	if (addUnique("POPTREE") == 0) {
		printf("You must not use POPTREE, this is a reserved word.\n");
		exit(1);
	}
	if (addUnique("POPTREED") == 0) {
		printf("You must not use POPTREED, this is a reserved word.\n");
		exit(1);
	}
	cvalCode = retrieveCodeByName("CVAL");

  	if ( (funcCode = retrieveCodeByName("FUNC")) == -1) {
		printf("No FUNC was found, I am assuming that the grammar is not a DYNAMIC GRAMMAR - if you are trying to write a grammar which uses");
		printf(" the dynamically defined function protocols you must use the FUNC keyword (to indicate where function calls can be made.\n");
		DYNAMICGRAMMAR = 0;
	} else {
		DYNAMICGRAMMAR = 1;
		// we are going to rewrite FUNC as _FUNC which allows us to break out all the funky FUNC code and 
		// removes the anal problems if FUNC appeared embedded in a chain (and "expansion")
		if (!changeSymbolText("FUNC","_DOFUNC")) {
			printf("PROBLEM changing the FUNC text ot _FUNC, not sure why as I have already checked that func is defined.\n");
			exit(1);
		}
		// Make a dummy rule _DOFUNC -> _FUNC
		
		lhs = addSymbol("_DOFUNC","_DOFUNC",NONTERMINAL);
		rhs = addSymbol("_FUNC","_FUNC",TERMINAL);
		addRightHandSide(lhs,rhs);
		funcCode = rhs->code;
		if (funcCode == -1) {
			printf("Strange error in trying to rename FUNC as _FUNC and to add _FUNC as an expansion.\n");
			exit(1);
		}
	}
	
	if ( (paramCode = retrieveCodeByName("PARAMS")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("PARAMS token not declared, you need to provide a rule to construct function parameters called PARAMS e.g. PARAMS: PARAM | PARAM PARAMS\n");
			printf("Use NOP if you want no paramaters declared e.g. PARAMS: NOP | PARAM PARAMS or even PARAMS: NOP | SOMEPARAMS and SOMEPARAMS: PARAM | PARAM PARAMS would work\n");
			exit(1);
		}
	}
	if ( (puseCode = retrieveCodeByName("PUSE")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("PUSE token not declared, you need to provide a rule which tells us when we can refer to parameter within a function call - that is when the paramater can be used by the function.\n");
			exit(1);
		}
	} else {
		if (DYNAMICGRAMMAR) { // only rewrite PUSE if we doing the dynamically defined function thingy
			// same with PUSE -> _PUSE
			if (!changeSymbolText("PUSE","_DOPUSE")) {
				printf("Problem changing the representation of PUSE to _PUSE\n");
				exit(1);
			}
			lhs = addSymbol("_DOPUSE","_DOPUSE",NONTERMINAL);
			rhs = addSymbol("_PUSE","_PUSE",TERMINAL);
			addRightHandSide(lhs,rhs);
			puseCode = rhs->code;
			if (puseCode == -1) {
				printf("Strange error in rewriting puse as _puse\n");
				exit(1);
			}
		}
	}  
	if ( (funcParTypeCode = retrieveCodeByName("FPARS")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("You need to define a rule for FPARS, to tell me how the paramaters passed to a function are formed e.g. FPARS: Integer\n");
			exit(1);
		}
	} 
	if ( (funcBodyTypeCode = retrieveCodeByName("FUNCBODY")) == -1) {
		if (DYNAMICGRAMMAR) {
			printf("You need to define a rule for FUNCBODY, to tell me what can appear in the body of a funcation e.g FUNCBODY: line | line FUNCBODY\n");
			exit(1);
		}
	}
	if (retrieveCodeByName("DFVAL") == -1) {
        if (DYNAMICGRAMMAR) {
			printf("You need to define DFVAL - which is the grammar expansion you want ");
			printf("to use if I am going to try and call a function but no functions have been defined! Some examples might be defining DFVAL to be an action or a x value.\n");
			exit(1);
		}
    }
	if (retrieveCodeByName("DPVAL") == -1) {
        if (DYNAMICGRAMMAR) {
			printf("You need to define DPVAL - which is the grammar expansion you want ");
			printf("to use if I am going to try and call a function but no functions have been defined! Some examples might be defining DPVAL to be a NOP or a x value.\n");
			exit(1);
		}
    }
	// fprintf(f,"enum { DEFUN = %d, PARAMS = %d, FUNC = %d, PUSE = %d, FPARS = %d, FUNCBODY = %d};\n",defunCode,paramCode,funcCode,puseCode,funcParTypeCode,funcBodyTypeCode);
	in = fopen("grammar.template.p2","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	identifyGrammarRules();
	
	fprintf(f,"\tint expr_t = curr_expr[*curr];\n");
	fprintf(f,"\tswitch(curr_expr[*curr])\n\t\t{\n");
	if (symbolsTok(1, text, &code, &type, &total,text2)) {
		do {
			defunThisBranch = paramsThisBranch = funcsThisBranch = cvalThisBranch = 0;
			forceAdvance = 0;
			fprintf(f,"\t\tcase(%s):{ /* %s */\n",text2,text);
			strcpy(ctext,text); // just copy the text somewhere safe for use at the end
			
            if (( chainContains(text,cvalCode))) {
                cvalThisBranch = 1;
            }
			
			if ( ( place = chainContains(text,defunCode))) {
				fprintf(f,"\t\t\tdivisor = %i;\n",total);    
				if (place == total) fprintf(f,"\t\t\tif (!AllowDefun) divisor--;\n");
				else {
					printf("Ideally your DEFUN *( )* should be on the last branch of a particular expansion.\n");
					printf("This allows the ability to define functions to be easily switched off e.g. not allowing function definitions whilst ");
					printf("defining functions. If your grammar doesn't allow that kind of recursion (ie. PARAMS and FVALUE are defined ");
					printf("in such a way that they cannot encounter a DEFUN - then you can blithely ignore this warning. \n");
					printf("However if your grammar would allow a DEFUN to be encountered whilst ");
					printf("expanding the definition of a function (a function definition within a function), then I am not sure what the results will be.\n");
				}
				defunThisBranch = 1;
			}
			if ( DYNAMICGRAMMAR && (place = chainContains(text,funcCode))) {
				printf("Found the chain %s, which contains code %d\n",text,funcCode);
				funcsThisBranch=1;
				strcpy(tmp,"_");
				strcat(tmp,text2);
				addSymbol(tmp,tmp,DUMMYNONTERMINAL);
				fprintf(f,"\t\t\tif (doingPTC2) {\n");
				fprintf(f,"\t\t\tcurr_expr[0]=FUNCPLACEHOLDER;\n");
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\tif (expandPTC2) {\n");
				fprintf(f,"\t\t\t\t//cout << \"Hit a dofun with expandPTC2 true\";\n");
				fprintf(f,"\t\t\t\tballoonExpansion++;\n");
				fprintf(f,"\t\t\t\tgrowing=true;\n");
				fprintf(f,"\t\t\t\tmaxDepth=2;\n");
				fprintf(f,"\t\t\t\tmaxGrow = false;\n");
				fprintf(f,"\t\t\t\tif (balloonExpansion == 1) {\n");
				fprintf(f,"\t\t\t\t\tdepth = 1; // so we allow them to expand a bit but not too much\n");
				fprintf(f,"\t\t\t\t\tlastCodonPos = codonPos-1; // prime the pump so to speak\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t// need to think what happens here with recursion\n");
				
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+1]=curr_expr[i];\n");
				fprintf(f,"\t\t\t\t*length = *length + 1;\n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=%s;	// And no functions happen to have been defined. \n",tmp);
				fprintf(f,"\t\t\t\tcurr_expr[*curr+1]=BEPOPTREE;\n");
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\t curr_expr[*curr]=%s;\n",tmp);
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\tcase(%s):{ /* %s */\n",tmp,tmp);
				fprintf(f,"\t\t\texpr_t=%s;\n",text2);
			}	   
			
			
			if (DYNAMICGRAMMAR &&  (place = chainContains(text,puseCode))) {
				paramsThisBranch = 1;
				strcpy(tmp,"_");
				strcat(tmp,text2);
				addSymbol(tmp,tmp,DUMMYNONTERMINAL);
				fprintf(f,"\t\t\tif (doingPTC2) {\n");
				fprintf(f,"\t\t\tcurr_expr[0]=PUSEPLACEHOLDER;\n");
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\tif (expandPTC2) {\n");
				fprintf(f,"\t\t\t\t//cout << \"Hit a dopuse with expandPTC2 true\";\n");
				fprintf(f,"\t\t\t\tballoonExpansion++;\n");
				fprintf(f,"\t\t\t\tgrowing=true;\n");
				fprintf(f,"\t\t\t\tmaxDepth=2;\n");
				fprintf(f,"\t\t\t\tmaxGrow = false;\n");
				fprintf(f,"\t\t\t\tif (balloonExpansion == 1) {\n");
				fprintf(f,"\t\t\t\t\tdepth = 1; // so we allow them to expand a bit but not too much\n");
				fprintf(f,"\t\t\t\t\tlastCodonPos = codonPos-1; // prime the pump so to speak\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t// need to think what happens here with recursion\n");
				
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+1]=curr_expr[i];\n");
				fprintf(f,"\t\t\t\t*length = *length + 1;\n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=%s;	// And no functions happen to have been defined. \n",tmp);
				fprintf(f,"\t\t\t\tcurr_expr[*curr+1]=BEPOPTREE;\n");
				fprintf(f,"\t\t\t\t//print_grammar_individual(cout,curr_expr,*length);\n");
				fprintf(f,"\t\t\t\t//cout << endl;\n");
				fprintf(f,"\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t}\n");
				fprintf(f,"\t\t\t curr_expr[*curr]=%s;\n",tmp);
				fprintf(f,"\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t}\n");
				fprintf(f,"\t\tcase(%s):{ /* %s */\n",tmp,tmp);
				fprintf(f,"\t\t\texpr_t=%s;\n",text2);
			}	   
			
			terminateElse = 0;
			if (funcsThisBranch && paramsThisBranch) {
				printf("Odd the grammar was changed to move FUNC to _FUNC and PUSE to _PUSE and to have each of them\n");
				printf("in a seperate expansion, but I appear to have found both of them in the same expansion\n");
				exit(1);
			}
			if (paramsThisBranch) { 
				fprintf(f,"\t\t\tdivisor = %i;\n",total-1); // we have two "defaults defined" so subtract two.
				fprintf(f,"\t\t\tif (AllowParams) divisor = divisor + noOfParams;\n");
				fprintf(f,"\t\t\tif (divisor == 0) {\t\t// This deals with the situation where no other members of the branch \n");
				index = 2;
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",index-1);
				fprintf(f,"\t\t\t\t*length = *length + %i;\n",index-1);
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=DPVAL;\t// And no functions/params happen to have been defined. \n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr+%i]=POPTREE;\n",index-1);
				fprintf(f,"\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t} else {\n"); // set terminate else so we put a } after the switch statement.
				terminateElse = 1;// NOTE This means we need a } at the end of the switch statement
				total = total -2;
				forceAdvance = 1; // we have used this codon evem though just now there were no paramaters to choose from
			}
			if (funcsThisBranch) {
				fprintf(f,"\t\t\tdivisor = %i;\n",total-1);    
				fprintf(f,"\t\t\tif (functionNos>0) divisor = divisor + functionNos;\n");
				fprintf(f,"\t\t\tif (divisor == 0) {\t\t// This deals with the situation where no other members of the branch \n");
				index = 2;
				fprintf(f,"\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",index-1);
				fprintf(f,"\t\t\t\t*length = *length + %i;\n",index-1);
				fprintf(f,"\t\t\t\tcurr_expr[*curr]=DFVAL;\t// And no functions happen to have been defined. \n");
				fprintf(f,"\t\t\t\tcurr_expr[*curr+%i]=POPTREE;\n",index-1);
				fprintf(f,"\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t} else { \n"); 
				terminateElse = 1;// NOTE This means we need a } at the end of the switch statement
				total = total -1; /* We automatically define this part, regardless of grammar */
				forceAdvance= 1; // we have used this codon, we must advance even though just now there was no choice of branch
			}
			
			
			if(!(defunThisBranch || funcsThisBranch || paramsThisBranch)) 
				fprintf(f,"\t\t\tdivisor = %i;\n",total);    
			
			//    going to change so that we always insert the growing call, have changed the function to take care
			//    of situation where we might want to terminate but there are only grow options and
			//    vice-versa.
			
			//  if (! (symbolsTokAt->willTerminate || symbolsTokAt->willExpand)) { // so there is a choice here. 
			fprintf(f,"\t\t\tif (growing) {\n");
			fprintf(f,"\t\t\t\tnonTerminals.clear();\n\t\t\t\tterminals.clear();\n");
			growcount =0;
			s = symbolsTokAt->chain;
			functodo = paramstodo = 0;
			while (s) {
				if (s && s->value) {
					if (s->value->code == funcCode) {
						functodo = 1;
					} else if (s->value->code == puseCode) {
						paramstodo =1;
					} else {
						if (s->willTerminate || s->mightTerminate) {
							fprintf(f,"\t\t\t\tterminals.push_back(%d);\n",growcount);
						}
						if (s->willExpand || s->mightExpand) {
							fprintf(f,"\t\t\t\tnonTerminals.push_back(%d);\n",growcount);
						}
						growcount++;
					}
					while (s && s->value) s = s->next; // move to end of branch
					//                     growcount++; // expand branch count
				}
				if (s) s = s->next; // if more branches move to start of next one
			}
			if (functodo || paramstodo) {
				if (functodo) {
					fprintf(f,"\t\t\t\tfor (int i=0;i<functionNos;i++) {\n");
					fprintf(f,"\t\t\t\t\tterminals.push_back(%d+i);\n",growcount);
					fprintf(f,"\t\t\t\t}\n");
				}
				if (paramstodo) {
					fprintf(f,"\t\t\t\tif (AllowParams) {\n");
					fprintf(f,"\t\t\t\t\tfor (int i=0;i<noOfParams;i++) {\n");
					if (functodo) fprintf(f,"\t\t\t\t\t\tterminals.push_back(%d + functionNos + i);\n",growcount);
					else fprintf(f,"\t\t\t\t\t\tterminals.push_back(%d+ i);\n",growcount);
					fprintf(f,"\t\t\t\t\t}\n");
					fprintf(f,"\t\t\t\t}\n");
				}
			}
			fprintf(f,"\t\t\t\tchangeCodonsForGrow(divisor);\n");
			fprintf(f,"\t\t\t\tcurrentCodon = (terminating?0:codons[codonPos]) & CODONMASK;\n");
			fprintf(f,"\t\t\t}\n");
			//    }
            fprintf(f,"\t\t\tswitch(currentCodon%%divisor){\n");
			index = 0;
			nonTerminal =0;
			strcpy(output,"");
			for (caseIndex = 0; caseIndex < total; caseIndex++) {
				if(caseIndex) {
					rhsTok(0,text,&code2,text2);
					//	rhsTok(0,text,&code2);
				}
				else {
					rhsTok(1,text,&code2,text2);
					//	rhsTok(1,text,&code2);
				}
				fprintf(f,"\t\t\t\tcase(%i):{\n",caseIndex);
				do {
                    willExpand = branchrhsAt->willExpand;
					mightExpand = branchrhsAt->mightExpand;
					sprintf(tmp,"\t\t\t\t\tcurr_expr[*curr+%i]=%s; //%s\n",index,text2,text);
					if (code2>1000) nonTerminal++;
					index++;
					if(strlen(output))
						strcat(output,tmp);
					else
						strcpy(output,tmp);
				} while ((result = rhsTok(0,text,&code2,text2)) == 1);
				if (total>1 || forceAdvance) index++; //if (nonTerminal>0) index++;
				if (index > 1) {
					fprintf(f,"\t\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
					fprintf(f,"\t\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",index-1);
					fprintf(f,"\t\t\t\t\t*length = *length + %i;\n",index-1);
					
				}
				fprintf(f,"%s",output);
				if ((total>1) || forceAdvance) { // used to be nonTerminal>0) 
					//- note forceAdvance as we will be reading codon regardless if only one
					if (willExpand || mightExpand) {
						fprintf(f,"\t\t\t\t\tcurr_expr[*curr+%i]=POPTREED;\n",index-1);
						fprintf(f,"\t\t\t\t\tif (growing) depth++;\n");
                    }
				    else fprintf(f,"\t\t\t\t\tcurr_expr[*curr+%i]=POPTREE;\n",index-1);
					fprintf(f,"\t\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				}
				fprintf(f,"\t\t\t\t\tbreak;\n\t\t\t\t\t}\n");
				index = 0;
				strcpy(output,"");
			}
            if (funcsThisBranch) {
				fprintf(f,"\t\t\t\tdefault:{ // We are going to call a function\n");
				fprintf(f,"\t\t\t\t\tif (functionNos == 0) {\n");
				fprintf(f,"\t\t\t\t\t\tcout << \"Error trying to call a non-extant function\\n\";\n");
				fprintf(f,"\t\t\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t\tint function = (currentCodon%%divisor) - (divisor - functionNos); // work out which function to call\n");
				fprintf(f,"\t\t\t\t\tint fPars = (*funcVector)[function].params;\n");
				fprintf(f,"\t\t\t\t\tint shift = (fPars == 0?3:fPars*2 +2); // need to shift by a number dependent on the paramaters of the function\n");
				fprintf(f,"\t\t\t\t\tfor (int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\t\tcurr_expr[i+shift]=curr_expr[i];\n");
				fprintf(f,"\t\t\t\t\t*length = *length+shift;\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+0]=500+function;\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+1]=OFB; //  *( = %i\n",oFB);
				fprintf(f,"\t\t\t\t\tif (fPars > 0) {\n");
				fprintf(f,"\t\t\t\t\t\tcurr_expr[*curr+2]=FPARS; // FPARS = %i\n",funcParTypeCode);
				fprintf(f,"\t\t\t\t\t\tfor (int j=0;j<fPars-1;j++) {\n");
				fprintf(f,"\t\t\t\t\t\t\tcurr_expr[*curr+3+2*j]=COMMA; // , = %i\n",commaCode);
				fprintf(f,"\t\t\t\t\t\t\tcurr_expr[*curr+4+2*j]=FPARS; // FPARS = %i\n",funcParTypeCode);
				fprintf(f,"\t\t\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t\t}\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+shift-1]=CFB; // )* = %i\n",cFB);
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+shift]=POPTREE;\n");
				fprintf(f,"\t\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t\t\tbreak;\n\t\t\t\t\t} // of default branch\n");
			} else if (paramsThisBranch) {
				fprintf(f,"\t\t\t\tdefault: { // We are using a paramater\n");
				fprintf(f,"\t\t\t\t\tfor(int i=*length-1;i>*curr;i--)\n");
				fprintf(f,"\t\t\t\t\t\tcurr_expr[i+%i]=curr_expr[i];\n",1);
				fprintf(f,"\t\t\t\t\t*length = *length + %i;\n",1);
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+0] = 400 + currentCodon%%divisor - (divisor-noOfParams);\n");
				fprintf(f,"\t\t\t\t\tcurr_expr[*curr+1]=POPTREE;\n");
				fprintf(f,"\t\t\t\t\tif (ExtractingGrammarInfo) stack.push_back(codonPos);\n");
				fprintf(f,"\t\t\t\t\tbreak;\n");
				fprintf(f,"\t\t\t\t\t} // of default \n");
			} 
     		fprintf(f,"\t\t\t}//of switch\n");
			if (terminateElse) fprintf(f,"\t\t\t}\n");
			
			if(total>1 || forceAdvance) {
				fprintf(f,"\t\t\tif (!terminating && ExtractingGrammarInfo)\n");
				fprintf(f,"\t\t\t\tcodons[codonPos] = ( (codons[codonPos] & FCODONMASK) | ((expr_t - 1000) << CODON));\n");
				fprintf(f,"\t\t\tcodonPos = codonPos + 1; // query if this should be incremented if terminating?\n");
				forceAdvance = 0;
			}
			fprintf(f,"\t\t\tbreak;\n\t\t\t}//of case %s\n",ctext);
		} while((symbolsTok(0, text, &code, &type, &total,text2))&&(type==NONTERMINAL));
        }
	fprintf(f,"\t\tdefault:{ \n\t\t\t*curr = *curr + 1;\n");
	fprintf(f,"\t\t\t}\n\t\t}//Of switch\n\t}\n");
	fprintf(f,"/***************************************************************/\n");
	fprintf(f,"void %s::print_grammar_individual(ostream& output_stream,int curr_expr[],int length){\n",gname);
	fprintf(f,"\tfor(int i=0;i<length;i++)\n\t\tswitch(curr_expr[i]){\n");
	do{/* Since we've parsed all the NONTERMINALs, the remaining symbols are TERMINALs */
		strcpy(tmp,text);
		if((ptr=strtok(tmp,"\""))){
			/* Search for '"', and replace with '\"' */
			strcpy(text,ptr);
			while((ptr=strtok(NULL,"\""))){
				strcat(text,"\\\"");
				strcat(text,ptr);
			}
		}
		if (code == cvalCode) fprintf(f,"\t\tcase %s: { output_stream << \" %s :\"; i++ ; output_stream << curr_expr[i] << \": \"; break;}\n",text2,text);
		else fprintf(f,"\t\tcase %s: {output_stream << \" %s \";break;}\n",text2,text);
	} while(symbolsTok(0, text, &code, &type, &total,text2));
	fprintf(f,"\t\tdefault: {\n");
	fprintf(f,"\t\t\tif (curr_expr[i]>=400 && curr_expr[i]<500) {\n");
	fprintf(f,"\t\t\t\toutput_stream << \"P\" << curr_expr[i]-400;\n");
	fprintf(f,"\t\t\t} else if (curr_expr[i]>=500 && curr_expr[i]<600) {\n");
	fprintf(f,"\t\t\t\toutput_stream << \"F\" << curr_expr[i]-500;\n");
	fprintf(f,"\t\t\t} else {\n\t\t\t\toutput_stream << \" UNKNOWN(\" << curr_expr[i] << \") \";\n\t\t\t}\n");
	fprintf(f,"\t\t\tbreak;\n\t\t\t} // of default branch\n");
	fprintf(f,"\t\t}\n\toutput_stream << endl;\n}\n");
	fclose(f);
	fclose(yyin);
	strcpy(tmp,gname);
	strcat(tmp,".h");
	f = fopen(tmp,"w");	
	in = fopen("grammar.template.h","r");
	while(fgets(output,120,in)) {
		suball(output,"GRAMMARNAME",gname);
		fprintf(f,"%s",output);
	}
	fclose(in);
	if (enumSymbolTable(1,tmp,&code)) { // the one resest the table
		fprintf(f,"\tenum {\t%s = %d",tmp,code);
		while (enumSymbolTable(0,tmp,&code)) {
			fprintf(f,",\n\t\t%s = %d",tmp,code);
		}
		fprintf(f,"};\n}\n\n\n#endif\n");
	}
	
	fclose(f);
	
	if (enumSymbolTable(1,tmp,&code)) { // the one resest the table
		if (code < 1000) printf("%%token %s\n",tmp);
		while (enumSymbolTable(0,tmp,&code)) {
			if (code < 1000) printf("%%token %s\n",tmp);
		}
	}
	if (symbolsTok(1,text,&code,&type,&total,text2)) 
		do {
			strcpy(output,text);
			strcat(output,":");	
			//printf("// %s\n",output);
			printf("%s: \n\t",text2);
			
			for (caseIndex = 0;caseIndex < total;caseIndex++) {
				strcpy(output,"");
				if (caseIndex) rhsTok(0,text,&code2,text2);
				else rhsTok(1,text,&code2,text2);
				do { 
					printf("%s ",text2);
					strcat(output,text);
					strcat(output," ");
				} while (rhsTok(0,text,&code2,text2) ==1);
				if (caseIndex !=total-1) printf("|");
				//printf("\t//%s\n\t",output);
				printf("\n\t");
				
			}
			printf("\n");
		}
	while(symbolsTok(0,text,&code,&type,&total,text2) && type==NONTERMINAL);
	return 1;
}

int yyerror(char *s){
	fprintf(stderr,"Oops...\nYou have an error on line %i!\n",lineNo);
	if(yytext)
		fprintf(stderr,"Token \"%s\" is out of place or not recognized...\n",yytext);
	exit(0);
}


void suball(char *output,char *from,char *to) {
	char *tmp,*tmp2;
	char buff[256];
	strcpy(buff,output);
	tmp2 = buff;
	tmp = strstr(tmp2,from);
	*output = 0;
	while (tmp) {
		*tmp =0;
		strcat(output,tmp2);
		strcat(output,to);
		tmp2=tmp+strlen(from);
		tmp = strstr(tmp2,from);
	}
	strcat(output,tmp2);
}



