%{
#include<bits/stdc++.h>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include "symboltable.cpp"

using namespace std;

int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern int line_no;
extern int line_count_inc;
extern int error_count;
SymbolTable * table = new SymbolTable(30);
vector<SymbolInfo*> list_of_declared_vars; 
vector<SymbolInfo*> list_of_parameters; 
vector<SymbolInfo*> list_of_arguments;
ofstream logfile;
ofstream errorfile;
ofstream codefile;
string currentFunctionName;
string codeStart=".MODEL SMALL\n.STACK 100H;initialize model and stack size\n"
string codePrint="PRINTLN PROC;procedure for printing\nPUSH AH\nMOV AH, 2\nINT 21H\nPOP AH\nRET\nPRINTLN ENDP\n"
string codeDataDec=".DATA\n"

int labelCount=0;
int tempCount=0;

string newLabel()
{
	string lb="L";
	string t=to_string(labelCount);
	labelCount++;
	return lb;
}

string newTemp()
{
	string tmp="t"+to_string(labelCount);
	tempCount++;
	return tmp;
}

string println()
{
	return 
	//each time move the var to DL
}


void yyerror(string text)
{
	error_count++;
	errorfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
	logfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
}

void printError(string text)
{
	error_count++;
	errorfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
	logfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
}

void printLogRule(string rule)
{
	logfile<<"Line "<<line_no<<": "<<rule<<endl<<endl;
}


void printLogSymbol(SymbolInfo * symbol)
{
	logfile<<symbol->getName()<<endl<<endl;

}


void endParse()
{
	table->printAllScope(logfile);
	logfile<<endl<<endl<<"Total lines: "<<line_no<<endl<<endl;
	logfile<<"Total errors: "<<error_count<<endl;
	
}

string toUpper(string str)
{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
}


void InsertInDeclarationList(SymbolInfo * s)
{
	SymbolInfo * s1=new SymbolInfo();
	s1->makeCopy(s);
	if(table->lookUpCurrentScope(s->getName()) != nullptr)
	{
	printError("Multiple declaration of "+s->getName());
	return;
	}
	codeDataDec=codeDataDec+"DB "+s->getName()+"?\n"
	list_of_declared_vars.push_back(s1);
}


void InsertDeclaredVarsSymbolTable(string type_specifier)//for var declaration
{
	
	
	for(int i=0;i<list_of_declared_vars.size();i++)
	{
		SymbolInfo * s=new SymbolInfo();
		s->makeCopy(list_of_declared_vars[i]);
		s->setDeclaredType(toUpper(type_specifier));  
		if(type_specifier!="VOID")
		{
			if(!table->Insert(s)){
	    		printError("Multiple declaration of "+s->getName());
	    		}
	    		
		}
		else 
		{
		printError("Variable type cannot be void ");
		break;
		}
	    	
	}
	
	 list_of_declared_vars.clear();
	 
}

void InsertInParameterList(SymbolInfo * s)
{
	SymbolInfo * s1=new SymbolInfo();
	s1->makeCopy(s);
	for(int i=0;i<list_of_parameters.size();i++)
	{
		if(s1->getName()!="$" && list_of_parameters[i]->getName()==s1->getName()) 
		{
			printError("Multiple declaration of "+s1->getName()+" in parameter");
			return;
		}
	}
	list_of_parameters.push_back(s1);
}

void InsertParametersSymbolTable()//for function declaration
{
	for(int i=0;i< list_of_parameters.size();i++)
	 {
		SymbolInfo * s=new SymbolInfo();
		s->makeCopy(list_of_parameters[i]);
	   	if(s->getName()!="$") table->Insert(s); 
	    	
	 }
	 //list_of_parameters.clear();
}

void checkParameterError()//for function declaration
{
	for(int i=0;i< list_of_parameters.size();i++)
	 {
		
	   	if(list_of_parameters[i]->getName()=="$")
	   	{
	   		printError(to_string(i+1)+"th parameter's name not given in function definition of "+currentFunctionName);
	   	}
	 
	    	
	 }
	
}

void InsertInArgumentList(SymbolInfo * s)
{
	SymbolInfo * s1=new SymbolInfo();
	s1->makeCopy(s);
	if(s->getDeclaredType()=="VOID") 
	{
	printError("Invalid use of void expression");
	}
	list_of_arguments.push_back(s1);
}



%}

%union {SymbolInfo* symbolinfo;}

%token<symbolinfo> IF FOR DO INT FLOAT VOID SWITCH DEFAULT ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE CONST_INT CONST_FLOAT CONST_CHAR INCOP ADDOP MULOP RELOP ASSIGNOP LOGICOP NOT LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA SEMICOLON ID STRING COMMENT PRINTLN DECOP WRONG_FLOAT
%type<symbolinfo> declaration_list var_declaration type_specifier unit program parameter_list func_declaration func_definition compound_statement statement statements expression_statement expression variable logic_expression rel_expression simple_expression term unary_expression factor argument_list arguments

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/*
%left 
%right

%nonassoc 

*/
%%

start : program
	{	
		
		printLogRule("start : program");
		endParse();
	}
	;

program : program unit {
			$$=new SymbolInfo($1->getName()+"\n"+$2->getName(),"program");
			printLogRule("program : program unit");
			printLogSymbol($$);
			}
			
	| unit {
		printLogRule("program : unit");
		printLogSymbol($1);
		$$= new SymbolInfo();
		$$->makeCopy($1);
		}
		
	;
	
unit : var_declaration {
			
			printLogRule("unit : var_declaration");
			printLogSymbol($1);
			$$= new SymbolInfo();
		      	$$->makeCopy($1);
			}
			
     | func_declaration {printLogRule("unit : func_declaration");
     			printLogSymbol($1);
     			$$= new SymbolInfo();
		      	$$->makeCopy($1);}
     
     | func_definition {printLogRule("unit : func_definition");
     			printLogSymbol($1);
     			$$= new SymbolInfo();
		      	$$->makeCopy($1);}
     
     ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN {
		SymbolInfo * func=table->lookUp($2->getName());
		if(func!=nullptr)
			{
				printError("Multiple declaration of "+func->getName());
			}
			
		else
			{
				$2->setFunction();
				$2->setDeclaredType(toUpper($1->getName())); 
				SymbolInfo * s= new SymbolInfo();
				s->makeCopy($2);
				for(auto var : list_of_parameters)//inserting parameters in function ID
				 {
				   s->insertParameter(var); 
				 }
				table->Insert(s);
			}
		} SEMICOLON {
		
		list_of_parameters.clear();
		 $$=new SymbolInfo($1->getName()+" "+$2->getName()+"("+$4->getName()+");","func_declaration");
		 printLogRule("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
		 printLogSymbol($$);}
		  
		| type_specifier ID LPAREN RPAREN {
		SymbolInfo * func=table->lookUp($2->getName());
		if(func!=nullptr)
			{
				printError("Multiple declaration of "+func->getName());
			}
			
		else
			{
				$2->setFunction();
				$2->setDeclaredType(toUpper($1->getName())); 
				SymbolInfo * s= new SymbolInfo();
				s->makeCopy($2);
				table->Insert(s);
			}
		} SEMICOLON {
		
		$$=new SymbolInfo($1->getName()+" "+$2->getName()+"();","func_declaration");
		printLogRule("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
		printLogSymbol($$);}
		
		|type_specifier ID LPAREN parameter_list RPAREN error{}
		|type_specifier ID LPAREN RPAREN error{}
		
		
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN {

		SymbolInfo * func=table->lookUp($2->getName());
		currentFunctionName=$2->getName();
		checkParameterError();
		
		if(func!=nullptr)
		{
			if(func->getIsFunction())
			{
				if(func->getIsDefined())
				{
					printError("Multiple definition of function "+func->getName());
				}
				else
				{
					vector<SymbolInfo *> declaredParameterList=func->getParameters();
					if(toUpper($1->getName())!=func->getDeclaredType())
					{
						printError("Return type mismatch with function declaration in function "+func->getName());
					}
					if(list_of_parameters.size()!=declaredParameterList.size())
					{
						printError("Total number of arguments mismatch with declaration in function "+func->getName());
					}
					else
					{
						for(int i=0;i<declaredParameterList.size();i++)
						{	
							//cout<<declaredParameterList[i]->getDeclaredType()<<endl;
							if(declaredParameterList[i]->getDeclaredType()!=list_of_parameters[i]->getDeclaredType())
							{
								printError((i+1)+"th parameter mismatch in function "+func->getName());
								break;
							}
						}
					}
				
				}

			}
			else
			{
				printError("Multiple declaration of "+func->getName());
			}
		}
		
		else{
		$2->setFunction();
		$2->setDeclaredType(toUpper($1->getName()));
		SymbolInfo * s= new SymbolInfo();
		s->makeCopy($2);
		for(auto var : list_of_parameters)//inserting parameters in function ID
		 {
		   s->insertParameter(var); 
		 
		 }
		 
		table->Insert(s);		
		}


		
		} compound_statement {
		//inside compound statement, list of parameters inserted in scope
		list_of_parameters.clear();
		$$=new SymbolInfo($1->getName()+" "+$2->getName()+"("+$4->getName()+")"+$7->getName(),"func_definition");
		printLogRule("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
		SymbolInfo * s= table->lookUp($2->getName());
		if(s!=nullptr && s->getIsFunction())
		{
			s->setIsDefined();
		}
		printLogSymbol($$);
		}
		
		| type_specifier ID LPAREN RPAREN {
		
		SymbolInfo * func=table->lookUp($2->getName());
		currentFunctionName=$2->getName();
		
		if(func!=nullptr)
		{
			if(func->getIsFunction())
			{
				if(func->getIsDefined())
				{
					printError("Multiple definition of function "+func->getName());
				}
				else
				{
					vector<SymbolInfo *> declaredParameterList=func->getParameters();
					if(toUpper($1->getName())!=func->getDeclaredType())
					{
						printError("Function return type didn't match with declaration");
					}
					if(list_of_parameters.size()!=declaredParameterList.size())
					{
						printError("Total number of arguments mismatch with declaration in function "+func->getName());
					}				
				}

			}
			else
			{
				printError("Multiple declaration of "+func->getName());
			}
		}
		
		else{
		$2->setFunction();
		$2->setDeclaredType(toUpper($1->getName())); 
		SymbolInfo * s= new SymbolInfo();
		s->makeCopy($2);
		table->Insert(s);
		}
		
		} compound_statement {		    
		$$=new SymbolInfo($1->getName()+" "+$2->getName()+"()"+$6->getName(),"func_definition");
		printLogRule("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
		printLogSymbol($$);
		SymbolInfo * s= table->lookUp($2->getName());
		if(s!=nullptr && s->getIsFunction())
		{
			s->setIsDefined();
		}
		}
		
 		;				


parameter_list  : parameter_list COMMA type_specifier ID {
		
		$4->setDeclaredType(toUpper($3->getName()));
		//list_of_parameters.push_back($4);
		InsertInParameterList($4);
		$$=new SymbolInfo($1->getName()+","+$3->getName()+" "+$4->getName(),"parameter_list");
		printLogRule("parameter_list  : parameter_list COMMA type_specifier ID");
		printLogSymbol($$);}
		
		| parameter_list COMMA type_specifier {//need to insert in parameter_list
		
		$$=new SymbolInfo($1->getName()+","+$3->getName(),"parameter_list");
		printLogRule("parameter_list  : parameter_list COMMA type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper($3->getName())); 
		InsertInParameterList(s);
		printLogSymbol($$);
		}
		
 		| type_specifier ID {
		$2->setDeclaredType(toUpper($1->getName()));//correction needed
		//list_of_parameters.push_back($2);
		InsertInParameterList($2);
 		$$=new SymbolInfo($1->getName()+" "+$2->getName(),"parameter_list");
 		printLogRule("parameter_list  : type_specifier ID");
 		printLogSymbol($$);}
 		
		| type_specifier {//need to insert in parameter_list
		
		printLogRule("parameter_list  : type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper($1->getName())); 
		InsertInParameterList(s);
		$$=new SymbolInfo();
		$$->makeCopy($1);
		printLogSymbol($1);
		}
		
		| type_specifier error {
		
		printLogRule("parameter_list  : type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper($1->getName())); 
		InsertInParameterList(s);
		$$=new SymbolInfo();
		$$->makeCopy($1);
		printLogSymbol($1);

		yyclearin;
		}
		
		| parameter_list COMMA type_specifier error{
		
		$$=new SymbolInfo($1->getName()+","+$3->getName(),"parameter_list");
		printLogRule("parameter_list  : parameter_list COMMA type_specifier");
		SymbolInfo * s= new SymbolInfo("$","type_specifier");
		s->setDeclaredType(toUpper($3->getName())); 
		InsertInParameterList(s);
		printLogSymbol($$);
	
		yyclearin;
		}
		
 		;

 		
compound_statement : LCURL {table->EnterScope(logfile);InsertParametersSymbolTable();} statements RCURL {
		    
 		    $$=new SymbolInfo("{\n"+$3->getName()+"\n}\n","compound_statement");
 		    printLogRule("compound_statement : LCURL statements RCURL");
 		    printLogSymbol($$);
 		    table->printAllScope(logfile);
	            table->ExitScope(logfile);
	            
 		    }
 		    
 		    | LCURL {table->EnterScope(logfile);InsertParametersSymbolTable();} RCURL {
 		    
 		    $$=new SymbolInfo("{\n}\n","compound_statement");
 		    printLogRule("compound_statement : LCURL RCURL");
 		    printLogSymbol($$);
 		    table->printAllScope(logfile);
	            table->ExitScope(logfile);

				
	            
 		    }
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON {
								
								//string str=$1->getName();
								//transform(str.begin(), str.end(), str.begin(), ::toupper);//this may cause problem
								
								$$=new SymbolInfo($1->getName()+" "+$2->getName()+";","var_declaration");
								printLogRule("var_declaration : type_specifier declaration_list SEMICOLON");
								InsertDeclaredVarsSymbolTable(toUpper($1->getName()));
		
							printLogSymbol($$);
								}
		/*						
		| type_specifier declaration_list error {
								
								//string str=$1->getName();
								//transform(str.begin(), str.end(), str.begin(), ::toupper);//this may cause problem
								
								$$=new SymbolInfo($1->getName()+" "+$2->getName(),"var_declaration");
								printLogRule("var_declaration : type_specifier declaration_list error");
								InsertDeclaredVarsSymbolTable(toUpper($1->getName()));
		
								printLogSymbol($$);
								
								}
								*/

 		 ;
 		 
type_specifier	: INT {
			$$=new SymbolInfo("int","type_specifier");
			printLogRule("type_specifier : INT");
			printLogSymbol($$);}
			
 		| FLOAT {
			$$=new SymbolInfo("float","type_specifier");
			printLogRule("type_specifier : FLOAT");
			printLogSymbol($$);}
			
 		| VOID {
			$$=new SymbolInfo("void","type_specifier");
			printLogRule("type_specifier : VOID");
			printLogSymbol($$);}
			
 		;
 		
 		///SOME INSERTIONS HERE
declaration_list : declaration_list COMMA ID {
						//list_of_declared_vars.push_back($3);
						InsertInDeclarationList($3);
						$$=new SymbolInfo($1->getName()+","+$3->getName(),"declaration_list");
						printLogRule("declaration_list : declaration_list COMMA ID");
						printLogSymbol($$);}
						
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD {
 		  							$3->setArray();
 		  							//list_of_declared_vars.push_back($3);//array handling
 		  							InsertInDeclarationList($3);
 		  							$$=new SymbolInfo($1->getName()+","+$3->getName()+"["+$5->getName()+"]","declaration_list");
									printLogRule("declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
									printLogSymbol($$);}
									
 		  | ID {
 		  	//list_of_declared_vars.push_back($1);
 		  	InsertInDeclarationList($1);
 		  	printLogRule(" declaration_list : ID");
 		  	$$= new SymbolInfo();
		      	$$->makeCopy($1);
		      	printLogSymbol($1);}
 		  	
 		  | ID LTHIRD CONST_INT RTHIRD {
 		  				$1->setArray();
 		  				//list_of_declared_vars.push_back($1);//array handling
 		  				InsertInDeclarationList($1);
 		  				$$=new SymbolInfo($1->getName()+"["+$3->getName()+"]","declaration_list");
						printLogRule("declaration_list : ID LTHIRD CONST_INT RTHIRD");
						printLogSymbol($$);}

 		  | declaration_list error COMMA ID{
						
						
						InsertInDeclarationList($4);
						$$=new SymbolInfo($1->getName()+","+$4->getName(),"declaration_list");
						printLogRule("declaration_list : declaration_list COMMA ID");
						printLogSymbol($$);
						}				
 		  ;
 		  
statements : statement {
			printLogRule("statements : statement");
			$$= new SymbolInfo();
		      	$$->makeCopy($1);
		      	printLogSymbol($1);}
			
	   | statements statement {
	   $$=new SymbolInfo($1->getName()+"\n"+$2->getName(),"declaration_list");
	   printLogRule("statements : statements statement");
	   printLogSymbol($$);}
	   
	   | statements error {
		
		
	
		
	   }
	   
	   | error {
	   	
	   }
	 
	   
	   ;
	   
statement : var_declaration {
				//$$->setName($1->getName());
				printLogRule("statement : var_declaration");
				$$= new SymbolInfo();
		      		$$->makeCopy($1);
		      		printLogSymbol($1);
			     }
	  | func_declaration {
	  
	  			printLogRule("statement : func_declaration");
				$$= new SymbolInfo();
		      		$$->makeCopy($1);
		      		printLogSymbol($1);
		      		printError("A function declared inside a function");
	  
	  			}
	  | func_definition {
	  
	  			printLogRule("statement : func_definition");
				$$= new SymbolInfo();
		      		$$->makeCopy($1);
		      		printLogSymbol($1);
		      		printError("A function defined inside a function");
	  
	  			}
			     
	  | expression_statement {
	  			printLogRule("statement : expression_statement");
	  			$$= new SymbolInfo();
		      		$$->makeCopy($1);
		      		printLogSymbol($1);}
	  			
	  | compound_statement {
	  			printLogRule("statement : compound_statement");
	  			$$= new SymbolInfo();
		      		$$->makeCopy($1);
		      		printLogSymbol($1);}
	  			
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement {
	  $$=new SymbolInfo("for("+$3->getName()+$4->getName()+$5->getName()+")"+$7->getName(),"declaration_list");
	  printLogRule("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
	  printLogSymbol($$);
	  }
	  
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE {
	  $$=new SymbolInfo("if("+$3->getName()+")"+$5->getName(),"declaration_list");
	  printLogRule("statement : IF LPAREN expression RPAREN statement");
	  printLogSymbol($$);}
	  
	  | IF LPAREN expression RPAREN statement ELSE statement {
	  $$=new SymbolInfo("if("+$3->getName()+")"+$5->getName()+"else\n"+$7->getName(),"declaration_list");
	  printLogRule("statement : IF LPAREN expression RPAREN statement ELSE statement");
	  printLogSymbol($$);}
	  
	  | WHILE LPAREN expression RPAREN statement {
	  $$=new SymbolInfo("while("+$3->getName()+")"+$5->getName(),"declaration_list");
	  printLogRule("statement : WHILE LPAREN expression RPAREN statement");
	  printLogSymbol($$);}
	  
	  | PRINTLN LPAREN ID RPAREN SEMICOLON {
	  $$=new SymbolInfo("printf("+$3->getName()+");","declaration_list");
	  printLogRule("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
	  
	  
	  SymbolInfo * s=table->lookUp($3->getName());
	  if(s==nullptr) printError("Undeclared variable "+$3->getName());
	  printLogSymbol($$);
	  }
	  
	  | RETURN expression SEMICOLON {
	  $$=new SymbolInfo("return "+$2->getName()+";","declaration_list");
	  printLogRule("statement : RETURN expression SEMICOLON");
	  printLogSymbol($$);}
	  ;
	  
expression_statement 	: SEMICOLON {
			$$=new SymbolInfo(";","expression_statement");
			printLogRule("expression_statement : SEMICOLON");
			printLogSymbol($$);}
					
			| expression SEMICOLON {
			$$=new SymbolInfo($1->getName()+";","expression_statement");
			printLogRule("expression_statement : expression SEMICOLON");
			printLogSymbol($$);}
			
		
			
		
			
			;
	  
variable : ID {
		printLogRule("variable : ID");
		
		SymbolInfo * s=table->lookUp($1->getName());
		
		if(s != nullptr)
		{
			$$= new SymbolInfo();
			$$->makeCopy(s);
			if(s->getIsFunction())
			{
				printError("Type mismatch, "+s->getName()+" is a function");//if I use a function name in place of a variable
				 $$->setDeclaredType("UNDECLARED"); 
			}
			
			else if(s->getIsArray())
			{
				printError("Type mismatch, "+s->getName()+" is an array");//if I use an array name in place of a variable
				 $$->setDeclaredType("UNDECLARED"); 
			}
			
			
		}
		else
		{
			printError("Undeclared variable "+$1->getName());
			$$= new SymbolInfo();
		      	$$->makeCopy($1);
			$$->setDeclaredType("UNDECLARED");//problem may arise
		}
		
		printLogSymbol($1);
		
		}
		
	 | ID LTHIRD expression RTHIRD {
	 
	 $$=new SymbolInfo($1->getName()+"["+$3->getName()+"]","variable");
	 printLogRule("variable : ID LTHIRD expression RTHIRD");
	 
	 SymbolInfo * s=table->lookUp($1->getName());
 	
	if(s != nullptr)
	{
		
		if(s->getIsFunction())//if I use function name in place of an array
		{
			printError("Type mismatch, "+s->getName()+" is a function");
		}
			
		else if(s->getIsArray())//correct case
		{
			
			//$$->setDeclaredType(s->getDeclaredType());
			if($3->getDeclaredType()!="INT") {printError("Expression inside third brackets not an integer");}
			$$->setDeclaredType(s->getDeclaredType());
			
		}
		
		else //if I use a non-array variable in place of an array
		{
			printError(s->getName()+" not an array");//this
		}
	}
	else
	{
		printError("Undeclared variable "+$1->getName());
		$$= new SymbolInfo();
		$$->makeCopy($1);
		$$->setDeclaredType("UNDECLARED");//problem may arise
	}
		
	
	printLogSymbol($$);	
	 
	 }
	 
	 ;
	 
expression : logic_expression {
	    printLogRule("expression : logic_expression");
	    $$= new SymbolInfo();
	    $$->makeCopy($1);
	    printLogSymbol($$);}
	    
	   | variable ASSIGNOP logic_expression {
	   $$=new SymbolInfo($1->getName()+"="+$3->getName(),"expression");
	   printLogRule("expression : variable ASSIGNOP logic_expression");
	   //check type mismatch
	   
	   if($1->getDeclaredType()=="VOID" || $3->getDeclaredType()=="VOID")
	  {
	  	printError("Void used in expression ");
	  }
	else if($1->getDeclaredType()=="INT" && $3->getDeclaredType()=="FLOAT")
	   {
	   	printError("Type Mismatch");
	   }
	   
	   
	   $$->setDeclaredType($1->getDeclaredType());
	   
	   printLogSymbol($$);
	   
	   
	   } 
	   


	  
	   ;
			
logic_expression : rel_expression {
		  printLogRule("logic_expression : rel_expression");
		  $$= new SymbolInfo();
		  $$->makeCopy($1);
		  printLogSymbol($$);}
		  
		 | rel_expression LOGICOP rel_expression {
		 $$=new SymbolInfo($1->getName()+$2->getName()+$3->getName(),"logic_expression");
		 printLogRule("logic_expression : rel_expression LOGICOP rel_expression");
		 $$->setDeclaredType("INT");
		 if($1->getDeclaredType()=="VOID" ) 
		  {
		  
		  printError("Void used in expression ");
		  }
		  else if($3->getDeclaredType()=="VOID")
		  {
	
		  printError("Void used in expression ");
		  }
		  
		  printLogSymbol($$);
		 }
		 
		 ;
			
rel_expression : simple_expression {
		printLogRule("rel_expression : simple_expression");
		$$= new SymbolInfo();
		$$->makeCopy($1);
		printLogSymbol($$);}
		
		| simple_expression RELOP simple_expression	{
		$$=new SymbolInfo($1->getName()+$2->getName()+$3->getName(),"rel_expression");
		printLogRule("rel_expression : simple_expression RELOP simple_expression");
		$$->setDeclaredType("INT");
		 if($1->getDeclaredType()=="VOID" ) 
		  {
		  
		  printError("Void used in expression ");
		  }
		  else if($3->getDeclaredType()=="VOID")
		  {
	
		  printError("Void used in expression ");
		  }
		  
		  printLogSymbol($$);
		}
		
		;
				
simple_expression : term {
		   $$= new SymbolInfo();
		   $$->makeCopy($1);
		   printLogRule("simple_expression : term");
		   printLogSymbol($$);}
		   
		  | simple_expression ADDOP term {
		  $$=new SymbolInfo($1->getName()+$2->getName()+$3->getName(),"simple_expression");
		  printLogRule("simple_expression : simple_expression ADDOP term");
		  
		  if($1->getDeclaredType()=="INT" && $3->getDeclaredType()=="INT") $$->setDeclaredType("INT");
		  else if($1->getDeclaredType()=="VOID" ) 
		  {
		  $$->setDeclaredType($3->getDeclaredType());
		  printError("Void used in expression ");
		  }
		  else if($3->getDeclaredType()=="VOID")
		  {
		  $$->setDeclaredType($1->getDeclaredType());
		  printError("Void used in expression ");
		  }
		  else if($1->getDeclaredType()=="UNDECLARED")
		  {
		  	$$->setDeclaredType($3->getDeclaredType());
		  }
		  else if($3->getDeclaredType()=="UNDECLARED")
		  {
		  	$$->setDeclaredType($1->getDeclaredType());
		  }
     		  else if($1->getDeclaredType()=="FLOAT" || $3->getDeclaredType()=="FLOAT") $$->setDeclaredType("FLOAT");
     		  
     		  printLogSymbol($$);
		  }
		  ;
					
term :	unary_expression {
      printLogRule("term : unary_expression");
      $$= new SymbolInfo();
      $$->makeCopy($1);
      printLogSymbol($$);}
      
     |  term MULOP unary_expression {
     $$=new SymbolInfo($1->getName()+$2->getName()+$3->getName(),"term");
     printLogRule("term : term MULOP unary_expression");
     
     if($2->getName()=="%")
     {
     	    if($3->getName()=="0") printError("Modulus by Zero ");
	    if($1->getDeclaredType()=="VOID" ) 
	     {
		 $$->setDeclaredType($3->getDeclaredType());
		 printError("Void used in expression ");
	     }
	    else if($3->getDeclaredType()=="VOID")
	     {
		 $$->setDeclaredType($1->getDeclaredType());
		 printError("Void used in expression ");
	     }
	  else if($1->getDeclaredType()=="UNDECLARED")
	  {
	  	$$->setDeclaredType($3->getDeclaredType());
	  }
	  else if($3->getDeclaredType()=="UNDECLARED")
	  {
	  	$$->setDeclaredType($1->getDeclaredType());
	  }
	    else if($1->getDeclaredType()!="INT" || $3->getDeclaredType()!="INT")
	     {
	     	printError("Non-Integer operand on modulus operator");
	     }
	     $$->setDeclaredType("INT");
     	
     }
     else
     {
     	if($2->getName()=="/" && $3->getName()=="0") printError("Divide by Zero ");
     	if($1->getDeclaredType()=="INT" && $3->getDeclaredType()=="INT") $$->setDeclaredType("INT");
     	else if($1->getDeclaredType()=="VOID" ) 
	  {
	  $$->setDeclaredType($3->getDeclaredType());
	  printError("Void used in expression ");
	  }
	else if($3->getDeclaredType()=="VOID")
	  {
	  $$->setDeclaredType($1->getDeclaredType());
	  printError("Void used in expression ");
	  }
     	else if($1->getDeclaredType()=="FLOAT" || $3->getDeclaredType()=="FLOAT") $$->setDeclaredType("FLOAT");
     }
     
     printLogSymbol($$);
     
     
     }
     
     ;

unary_expression : ADDOP unary_expression  {
		  $$=new SymbolInfo($1->getName()+$2->getName(),"unary_expression");
		  printLogRule("unary_expression : ADDOP unary_expression");
		  $$->setDeclaredType($2->getDeclaredType());
		  if($2->getDeclaredType()=="VOID")
		  {
		 	printError("Invalid operation on void type.");
		  }
		  printLogSymbol($$);
		  }
		  
		 | NOT unary_expression {
		 $$=new SymbolInfo("!"+$2->getName(),"unary_expression");
		 printLogRule("unary_expression : NOT unary_expression");
		 $$->setDeclaredType("INT");
		 
		 if($2->getDeclaredType()=="VOID")
		 {
		 	printError("Invalid operation on void type.");
		 }
		 
		 printLogSymbol($$);
		 }
		 
		 | factor {
		 printLogRule("unary_expression : factor");
		 $$= new SymbolInfo();
		 $$->makeCopy($1);
		 
		 printLogSymbol($$);}
		 
		 ;
	
factor	: variable {
	printLogRule("factor: variable");
	$$= new SymbolInfo();
	$$->makeCopy($1);
	printLogSymbol($$);}
	
	| ID LPAREN argument_list RPAREN {//function call
	//get the function variable from symbolTable
	
	$$=new SymbolInfo($1->getName()+"("+$3->getName()+")","factor");
	printLogRule("factor : ID LPAREN argument_list RPAREN");
	
	SymbolInfo * s= table->lookUp($1->getName());
	if(s!=nullptr)
	{
		if(s->getIsFunction())
		{
			$$->setFunction();
			$$->setDeclaredType(s->getDeclaredType());
			vector<SymbolInfo*> parameters= s->getParameters();
			
			if(!s->getIsDefined())
			{
				printError("Undefined reference to '"+s->getName()+"'");
			}
			
			if(parameters.size()==list_of_arguments.size())
			{
				for(int i=0;i<list_of_arguments.size();i++)
				{
					if(list_of_arguments[i]->getDeclaredType()=="FLOAT" && parameters[i]->getDeclaredType()=="INT")
					{
						
						printError(to_string(i+1)+"th argument mismatch in function "+s->getName());
						break;
						//cout<<list_of_arguments[i]->getDeclaredType()<<" "<<parameters[i]->getDeclaredType()<<endl;
						
					}
				}
			}
			else
			{
				printError("Total number of arguments mismatch in function "+s->getName());
				
				
				
			}
			$$->setDeclaredType(s->getDeclaredType());
		}
		else
		{
			printError(s->getName()+" not a function");
			$$->setDeclaredType("UNDECLARED");
		}
		
		//$$->setDeclaredType(s->getDeclaredType());
	}
	else 
	{
		printError("Undeclared function "+$1->getName());
		$$->setDeclaredType("UNDECLARED");
	}
	
	list_of_arguments.clear();
	printLogSymbol($$);
	;}
	
	
	| LPAREN expression RPAREN {
	$$=new SymbolInfo("("+$2->getName()+")","factor");
	printLogRule("factor : LPAREN expression RPAREN");
	$$->setDeclaredType($2->getDeclaredType());
	printLogSymbol($$);}
	
	| CONST_INT {
	$1->setDeclaredType("INT");
	printLogRule("factor : CONST_INT");
	$$= new SymbolInfo();
	$$->makeCopy($1);
	printLogSymbol($$);}
	
	| CONST_FLOAT {
	$1->setDeclaredType("FLOAT");
	printLogRule("factor : CONST_FLOAT");
	$$= new SymbolInfo();
	$$->makeCopy($1);
	printLogSymbol($$);}
	
	| variable INCOP {
	$$=new SymbolInfo($1->getName()+"++","factor");
	printLogRule("factor : variable INCOP");
	
	$$->setDeclaredType($1->getDeclaredType());
	printLogSymbol($$);
	}
	
	| variable DECOP {
	$$=new SymbolInfo($1->getName()+"--","factor");
	printLogRule("factor : variable DECOP");
	
	$$->setDeclaredType($1->getDeclaredType());
	printLogSymbol($$);}


	| WRONG_FLOAT {
	$1->setDeclaredType("WRONG_FLOAT");
	printLogRule("factor : WRONG_FLOAT");
	$$= new SymbolInfo();
	$$->makeCopy($1);
	printLogSymbol($$);
	
	}
	
	
	;
	
argument_list : arguments{
			  printLogRule("argument_list : arguments");
		      	$$= new SymbolInfo();
		      	$$->makeCopy($1);			  
			printLogSymbol($$); 
			  }
			  
			  |{
			  SymbolInfo * s=new SymbolInfo("","argument_list");
			  printLogRule("argument_list : ");
			  
			  $$=s;
			  printLogSymbol($$);
			  }
			  
			  ;
	
arguments : arguments COMMA logic_expression {
		$$=new SymbolInfo($1->getName()+","+$3->getName(),"arguments");
		printLogRule("arguments : arguments COMMA logic_expression");
		//list_of_arguments.push_back($3);
		InsertInArgumentList($3);
		printLogSymbol($$);
		
		}
		
	      | logic_expression {
	      	printLogRule("arguments : logic_expression");
	      	$$= new SymbolInfo();
	      	$$->makeCopy($1);
	      	InsertInArgumentList($1);
	      	printLogSymbol($$);
	      	//list_of_arguments.push_back();
	      	}
	      	
	      ;


%%
int main(int argc,char *argv[])
{

	if(argc<2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	
	
	
	string logfilename="log.txt";
	string errorfilename="error.txt";
	string codefilename="code.asm";
	
	
	logfile.open(logfilename);
	errorfile.open(errorfilename);
	codefile.open(codefilename);
	yyin=fin;
	
	yyparse();
	
	logfile.close();
	errorfile.close();
	codefile.close();
	
	fclose(fin);
	
	

	return 0;
}

