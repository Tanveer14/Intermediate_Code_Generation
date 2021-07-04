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
string codeStart=".MODEL SMALL\n.STACK 100H;initialize model and stack size\n";
string codePrint="\nPRINTLN PROC\nMOV DX,0\nPUSH DX\nMOV CX,1\nSHL CX,15\nAND CX,BX\nCMP CX,0\nJE START\nMOV DL,45\nMOV AH,2\nINT 21H\nNEG BX\nSTART:\nMOV AX,BX\nMOV DX,0\nMOV CX,10\nDIV CX\nMOV BX,AX\nADD DX,30H \nPUSH DX\nCMP BX,0\nJE EXIT\nJMP START\nEXIT:\nSTARTPRINT:\nPOP DX\nCMP DX,0\nJE ENDPRINT \nMOV AH,2\nINT 21H\nJMP STARTPRINT\nENDPRINT:MOV DL,13\nINT 21H\nMOV DL,10\nINT 21H\nRET  \nPRINTLN ENDP\n";

string codeDataDec=".DATA\n";

int labelCount=0;
int tempCount=0;



void addDeclaredVar(string varName)
{
	codeDataDec=codeDataDec+varName+table->currentScopeID()+" DW "+" ?; line no: "+to_string(line_no)+"\n";
	cout<<table->currentScopeID()<<": "<<line_no<<endl;
}

string newLabel()
{
	string lb="L"+to_string(labelCount);
	labelCount++;
	return lb;
}

string newTemp()
{
	string tmp="t"+to_string(tempCount);
	tempCount++;
	codeDataDec=codeDataDec+tmp+" DW "+" ?\n";
	return tmp;
}

void printASM(string code)
{
	codefile<<code<<endl;
}

vector<string> split(string str)
{
    vector<string> strings;
    string temp="";
    for(int i=0;i<str.size();i++)
    {
        if(str[i]==' '||str[i]==';'||str[i]==',')
        {
            if(temp!="")
            {
                strings.push_back(temp);
            }
            temp="";
        }
        else
        {
            temp+=str[i];

        }
    }

    if(temp!="") strings.push_back(temp);

    return strings;
}

void printOptimizedASM()
{
	ifstream infile;
	ofstream outfile;
	string infilename="code.asm";
	string outfilename="optimized_code.asm";
     string string_part1,string_part2;
    infile.open(infilename);
    outfile.open(outfilename);

     getline(infile,string_part1);
     outfile<<string_part1<<endl;

    while(!infile.eof())
    {

        getline(infile,string_part2);
       while(string_part2==""&&!infile.eof()){
		   outfile<<"\n"<<endl;
         getline(infile,string_part2);
       }


        vector<string> part1vect;
        vector<string> part2vect;
        part1vect=split(string_part1);
        part2vect=split(string_part2);
        if(part1vect.size()==0||part2vect.size()==0||!(part1vect.size()==0||part2vect.size()==0)&&part1vect[0]=="MOV"&&part2vect[0]=="MOV"&&((part1vect[1]==part2vect[2])&&(part1vect[2]==part2vect[1]) || (part1vect[1]==part2vect[1])&&(part1vect[2]==part2vect[2])))
        {

        }
        else{
            outfile<<string_part2<<endl;
        }
        string_part1=string_part2;

    }


    infile.close();
    outfile.close();
}



void yyerror(string text)
{
	error_count++;
	errorfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;
	
}

void printError(string text)
{
	error_count++;
	errorfile<<"Error at line "<<line_no<<": "<<text<<endl<<endl;

}

void printLogRule(string rule)
{
	//logfile<<"Line "<<line_no<<": "<<rule<<endl<<endl;
}

void printLogSymbol(SymbolInfo * symbol)
{
	//logfile<<symbol->getName()<<endl<<endl;

}


void endParse()
{
	/*
	table->printAllScope(logfile);
	logfile<<endl<<endl<<"Total lines: "<<line_no<<endl<<endl;
	logfile<<"Total errors: "<<error_count<<endl;
	*/
	
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
	   	if(s->getName()!="$") 
		{
			table->Insert(s); 
		
		}
	    	
	 }
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
		if(error_count<=0) 
		{
			printASM(codeStart+codeDataDec+"\n.CODE\n"+"\n"+codePrint+"\n"+$1->code+"\nEND MAIN");
			printOptimizedASM();
		}
		
		endParse();
	}
	;

program : program unit {
			$$=new SymbolInfo($1->getName()+"\n"+$2->getName(),"program");
			printLogRule("program : program unit");
			printLogSymbol($$);
			$$->setCode($1->code+"\n"+$2->code);
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
		$2->setIsDefined();
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
			//s->setIsDefined();
			if($1->getName()=="void") $$->setCode($2->getName()+" PROC\n"+$7->code+"\nRET\n"+$2->getName()+" ENDP\n");
			else $$->setCode($2->getName()+" PROC\n"+$7->code+"\n"+$2->getName()+" ENDP\n");
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
			if($2->getName()=="main")$$->setCode($2->getName()+" PROC\nMOV AX,@DATA\nMOV DS,AX"+$6->code+"\nMOV AH,4CH\nINT 21H\n"+$2->getName()+" ENDP\n");           
			else 
			{
				if($1->getName()=="void") $$->setCode($2->getName()+" PROC\n"+$6->code+"\nRET\n"+$2->getName()+" ENDP\n");
				else $$->setCode($2->getName()+" PROC\n"+$6->code+"\n"+$2->getName()+" ENDP\n");
				
			}
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

 		
compound_statement : LCURL {table->EnterScope(logfile);
			for(int i=0;i< list_of_parameters.size();i++)
			 {
				if(list_of_parameters[i]->getName()!="$") 
				{
					addDeclaredVar(list_of_parameters[i]->getName());//declare each variable in assembly code that is in the parameter of a defined function
					list_of_parameters[i]->setAssemblyVarName(list_of_parameters[i]->getName()+table->currentScopeID());
				}
					
			 }
					InsertParametersSymbolTable();
					} statements RCURL {
		    
 		    $$=new SymbolInfo("{"+$3->getName()+"}","compound_statement");

			string parameters="";
			for(int i=list_of_parameters.size()-1;i>=0;i--)//inserting parameters in function ID
			 {
			  parameters=parameters+"POP AX\nMOV "+list_of_parameters[i]->getAssemblyVarName()+",AX\n" ;
			 
			 }
			 if(parameters!="") parameters="POP BX\n"+parameters+"PUSH BX\n";
			$$->code=parameters+$3->code;
 		    printLogRule("compound_statement : LCURL statements RCURL");
 		    printLogSymbol($$);
 		    table->printAllScope(logfile);
	         table->ExitScope(logfile);
	            
 		    }
 		    
 		    | LCURL {table->EnterScope(logfile);
			InsertParametersSymbolTable();} RCURL {
				
 		    
 		    $$=new SymbolInfo("{}","compound_statement");
			for(int i=0;i< list_of_parameters.size();i++)
			 {
				if(list_of_parameters[i]->getName()!="$") 
				{
					addDeclaredVar(list_of_parameters[i]->getName());//declare each variable in assembly code that is in the parameter of a defined function
					list_of_parameters[i]->setAssemblyVarName(list_of_parameters[i]->getName()+table->currentScopeID());
				}
					
			 }
			string parameters="";
			for(int i=list_of_parameters.size()-1;i>=0;i--)//inserting parameters in function ID
			 {
			  parameters=parameters+"POP AX\nMOV "+list_of_parameters[i]->getAssemblyVarName()+",AX\n" ;
			 
			 }
			if(parameters!="") parameters="POP BX\n"+parameters+"PUSH BX\n";
			$$->code=parameters;
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
						$3->setAssemblyVarName($3->getName()+table->currentScopeID());
						InsertInDeclarationList($3);
						addDeclaredVar($3->getName());
						$$=new SymbolInfo($1->getName()+","+$3->getName(),"declaration_list");
						printLogRule("declaration_list : declaration_list COMMA ID");
						printLogSymbol($$);}
						
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD {
 		  							$3->setArray();
									$3->setAssemblyVarName($3->getName()+table->currentScopeID());
									codeDataDec+=$3->getAssemblyVarName()+" DW "+" "+$5->getName()+" DUP(?)\n";
 		  							//list_of_declared_vars.push_back($3);//array handling
 		  							InsertInDeclarationList($3);
 		  							$$=new SymbolInfo($1->getName()+","+$3->getName()+"["+$5->getName()+"]","declaration_list");
									printLogRule("declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
									printLogSymbol($$);}
									
 		  | ID {
 		
			$1->setAssemblyVarName($1->getName()+table->currentScopeID());
 		  	InsertInDeclarationList($1);
 		  	//add declared var
			addDeclaredVar($1->getName());//
 		  	printLogRule(" declaration_list : ID");
 		  	$$= new SymbolInfo();
		      	$$->makeCopy($1);
		      	printLogSymbol($1);}
 		  	
 		  | ID LTHIRD CONST_INT RTHIRD {
 		  				$1->setArray();
						$1->setAssemblyVarName($1->getName()+table->currentScopeID());
						
						codeDataDec+=$1->getAssemblyVarName()+" DW  "+$3->getName()+" DUP(?)\n";
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
			$$->code=";"+$1->getName()+"\n"+$1->code;
			$$->setName(";"+$1->getName());
			printLogSymbol($1);}
			
	   | statements statement {
	   $$=new SymbolInfo($1->getName()+"\n;"+$2->getName(),"declaration_list");
	   printLogRule("statements : statements statement");
	   printLogSymbol($$);
	   $$->code=$1->code+"\n"+";"+$2->getName()+"\n"+$2->code+"\n";
	   $$->setName(";"+$1->getName());
	   }
	   
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
	  string startLabel="startFor"+newLabel();
	  string endLabel="endFor"+newLabel();
	  $$->code=$3->code+"\n";
	  $$->code+=startLabel+":\n"+$4->code;
	  $$->code+="MOV AX,"+$4->getAssemblyVarName()+"\nCMP AX,0\n";
	  $$->code+="JE "+endLabel+"\n"+$7->code+"\n"+$5->code;
	  $$->code+="\nJMP "+startLabel+"\n"+endLabel+":\n";
	  }
	  
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE {
	  $$=new SymbolInfo("if("+$3->getName()+")"+$5->getName(),"declaration_list");
	  printLogRule("statement : IF LPAREN expression RPAREN statement");
	  printLogSymbol($$);
	  string label1=newLabel();
	  $$->code=$3->code+"MOV AX,"+$3->getAssemblyVarName()+"\n";
	  $$->code+="CMP AX,0\nJE "+label1+"\n"+$5->code+"\n"+label1+":;line no "+to_string(line_no)+"\n";
	  }
	  
	  | IF LPAREN expression RPAREN statement ELSE statement {
	  $$=new SymbolInfo("if("+$3->getName()+")"+$5->getName()+"else "+$7->getName(),"declaration_list");
	  printLogRule("statement : IF LPAREN expression RPAREN statement ELSE statement");
	  printLogSymbol($$);
	  string label1=newLabel();
	  string label2=newLabel();
	  
	  $$->code=$3->code+"MOV AX,"+$3->getAssemblyVarName()+"\n";
	  $$->code+="CMP AX,0\nJE "+label1+"\n"+$5->code+"\nJMP "+label2+"\n"+label1+":\n"+$7->code+"\n"+label2+":\n";
	  
	  
	  }
	  
	  | WHILE LPAREN expression RPAREN statement {
	  $$=new SymbolInfo("while("+$3->getName()+")"+$5->getName(),"declaration_list");
	  printLogRule("statement : WHILE LPAREN expression RPAREN statement");
	  printLogSymbol($$);
	  
	  string startLabel="startWhile"+newLabel();
	  string endLabel="endWhile"+newLabel();
	  $$->code=startLabel+":\n"+$3->code+"\nMOV AX,"+$3->getAssemblyVarName()+"\nCMP AX,0\n";
	  $$->code+="JE "+endLabel+"\n"+$5->code+"\nJMP "+startLabel+"\n"+endLabel+":\n";
	  }
	  
	  | PRINTLN LPAREN ID RPAREN SEMICOLON {
	  $$=new SymbolInfo("println("+$3->getName()+");","declaration_list");
	  printLogRule("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
	  
	  
	  SymbolInfo * s=table->lookUp($3->getName());
	  
	  if(s==nullptr) printError("Undeclared variable "+$3->getName());
	  else
	  {
	  	$$->setCode("\nMOV BX,"+s->getAssemblyVarName()+"\nCALL PRINTLN\n");
	  }
	  printLogSymbol($$);
	  }
	  
	  | RETURN expression SEMICOLON {
	  $$=new SymbolInfo("return "+$2->getName()+";","declaration_list");
	  $$->code=$2->code;
	  if(currentFunctionName != "main") $$->code+="\nPOP BX\nMOV AX,"+$2->getAssemblyVarName()+"\nPUSH AX\nPUSH BX\nRET\n";
	  printLogRule("statement : RETURN expression SEMICOLON");
	  printLogSymbol($$);}
	  ;
	  
expression_statement 	: SEMICOLON {
			$$=new SymbolInfo(";","expression_statement");
			printLogRule("expression_statement : SEMICOLON");
			printLogSymbol($$);}
					
			| expression SEMICOLON {
			$$=new SymbolInfo($1->getName()+";","expression_statement");
			$$->setCode($1->code);
			$$->setAssemblyVarName($1->getAssemblyVarName());
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
			
			 $$->code=$3->code;
			 $$->code+="\nMOV BX,"+$3->getAssemblyVarName()+"\nSAL BX,1;accessing array in based mode\n";
			 $$->setAssemblyVarName(s->getAssemblyVarName()+"[BX]");
			
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
	   $$->setCode($1->code+"\n"+$3->code+"\nMOV AX,"+$3->getAssemblyVarName()+"\nMOV "+$1->getAssemblyVarName()+",AX");
	   $$->setAssemblyVarName($1->getAssemblyVarName());//may need to change by adding new temporary variable
	   
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
		 string newTempVar= newTemp();
		 string newJumpLabel=newLabel();
		 $$->code=$1->code+"\n"+$3->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName();
					
		if($2->getName()=="&&"){
			$$->code+="\nMOV CX,0\nMOV "+newTempVar+",CX\nCMP AX,0\nJE "+newJumpLabel+"\nCMP BL,0 \nJE "+newJumpLabel+"\nMOV CX,1\nMOV "+newTempVar+",CX\n"+newJumpLabel+":\n";
		}
		else if($2->getName()=="||"){
			$$->code+="\nOR AX,BX\nMOV "+newTempVar+",0\nCMP AX,0\nJE "+newJumpLabel+"\nMOV "+newTempVar+",1\n"+newJumpLabel+":\n";
		}
		 $$->setAssemblyVarName(newTempVar);
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
		string newTempVar= newTemp();
		string newJumpLabel= newLabel();
		
		if($2->getName()=="<")$$->setCode($1->code+"\n"+$3->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName()+"\
		\nCMP AX,BX\nMOV "+newTempVar+",0\nJNL "+newJumpLabel+"\nMOV "+newTempVar+",1\n"+newJumpLabel+":\n");//add new code
		else if($2->getName()=="<=") $$->setCode($1->code+"\n"+$3->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName()+"\
		\nCMP AX,BX\nMOV "+newTempVar+",0\nJNLE "+newJumpLabel+"\nMOV "+newTempVar+",1\n"+newJumpLabel+":\n");//add new code
		else if($2->getName()==">") $$->setCode($1->code+"\n"+$3->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName()+"\
		\nCMP AX,BX\nMOV "+newTempVar+",0\nJNG "+newJumpLabel+"\nMOV "+newTempVar+",1\n"+newJumpLabel+":\n");//add new code
		else if($2->getName()==">=") $$->setCode($1->code+"\n"+$3->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName()+"\
		\nCMP AX,BX\nMOV "+newTempVar+",0\nJNGE "+newJumpLabel+"\nMOV "+newTempVar+",1\n"+newJumpLabel+":\n");//add new code
		else if($2->getName()=="==") $$->setCode($1->code+"\n"+$3->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName()+"\
		\nCMP AX,BX\nMOV "+newTempVar+",0\nJNE "+newJumpLabel+"\nMOV "+newTempVar+",1\n"+newJumpLabel+":\n");//add new code
		else $$->setCode($1->code+"\n"+$3->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName()+"\
		\nCMP AX,BX\nMOV "+newTempVar+",0\nJE "+newJumpLabel+"\nMOV "+newTempVar+",1\n"+newJumpLabel+":\n");//add new code
		
		$$->setAssemblyVarName(newTempVar);
		
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
		  
		  string newTempVar= newTemp(); //create a new temp var to keep the result
		  if($2->getName()=="+")
		  {
			
			  $$->code=$1->code+"\n"+$3->code;
			  $$->code+="\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName();
			  $$->code+="\nADD AX,BX\nMOV "+newTempVar+",AX\n";
			  
		  }
		  else 
		  {
			  $$->code=$1->code+"\n"+$3->code;
			  $$->code+="\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName();
			  $$->code+="\nSUB AX,BX\nMOV "+newTempVar+",AX\n";
			  
		  }
		  $$->setAssemblyVarName(newTempVar);
		  
		  
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
	 
	string newTempVar= newTemp(); //create a new temp var to keep the result
	
	if($2->getName()=="*")//add new code
	{
		$$->code=$1->code+"\n"+$3->code;
		$$->code+="\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName();
		$$->code+="\nIMUL BX\n\nMOV "+newTempVar+",AX\n";
		
	}
	else if($2->getName()=="/") 
	{
		
		$$->code=$1->code+"\n"+$3->code;
		$$->code+="\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName();
		$$->code+="\nMOV DX,0\nIDIV BX\nMOV "+newTempVar+", AX\n";
	}
	else {
		
		$$->code=$1->code+"\n"+$3->code;
		$$->code+="\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV BX,"+$3->getAssemblyVarName();
		$$->code+="\nMOV DX,0\nIDIV BX\nMOV "+newTempVar+", DX\n";
	} 
	
	$$->setAssemblyVarName(newTempVar);
     
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
		  $$->setAssemblyVarName($2->getAssemblyVarName());
		  if($1->getName()=="+")
		  {
			$$->code=$2->code;
			
				  
		  }
		  else
		  {
			  
			 string newTempVar=newTemp();
			 $$->code=$2->code;
			 $$->code+="\nMOV AX,"+$2->getAssemblyVarName();
			 $$->code+="\nNEG AX";
			 $$->code+="\nMOV "+newTempVar+",AX\n";
			 $$->setAssemblyVarName(newTempVar);
			//$$->setCode($2->code+"NEG "+$2->getAssemblyVarName()+"\n");
		  }
				

		  if($2->getDeclaredType()=="VOID")
		  {
		 	printError("Invalid operation on void type.");
		  }
		  printLogSymbol($$);
		  }
		  
		 | NOT unary_expression {//need this ?
		 $$=new SymbolInfo("!"+$2->getName(),"unary_expression");
		 printLogRule("unary_expression : NOT unary_expression");
		 $$->setDeclaredType("INT");
		 
		 if($2->getDeclaredType()=="VOID")
		 {
		 	printError("Invalid operation on void type.");
		 }else{
			 string newTempVar=newTemp();
			 string newJumpLabel=newLabel();
			 $$->code=$2->code+"\nMOV "+newTempVar+",1";
			 $$->code+="\nMOV AX,"+$2->getAssemblyVarName()+"\nCMP AX,0";
			 $$->code+="\nJE "+newJumpLabel;
			 $$->code+="\nMOV "+newTempVar+",0\n"+newJumpLabel+":\n";
			$$->setAssemblyVarName(newTempVar);
			 
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
						
						
					}
				}
			}
			else
			{
				printError("Total number of arguments mismatch in function "+s->getName());
				
				
				
			}
			$$->setDeclaredType(s->getDeclaredType());
			
			
			
			$$->code=$3->code+"\nCALL "+s->getName()+"\n";
			if(s->getDeclaredType()!="VOID"){
			string newTempVar= newTemp();
			$$->code+="POP AX\nMOV "+newTempVar+", AX\n";
			$$->setAssemblyVarName(newTempVar);
			}
			
		}
		else
		{
			printError(s->getName()+" not a function");
			$$->setDeclaredType("UNDECLARED");
		}
		
		//$$->setDeclaredType(s->getDeclaredType());
		
		//call $1->getName, pop the result and store in a new temporary variable
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
	$$->setCode($2->code);
	$$->setAssemblyVarName($2->getAssemblyVarName());
	printLogSymbol($$);}
	
	| CONST_INT {
	$1->setDeclaredType("INT");
	$1->setAssemblyVarName($1->getName());
	printLogRule("factor : CONST_INT");
	$$= new SymbolInfo();
	$$->makeCopy($1);
	printLogSymbol($$);}
	
	| CONST_FLOAT {
	$1->setDeclaredType("FLOAT");
	$1->setAssemblyVarName($1->getName());
	printLogRule("factor : CONST_FLOAT");
	$$= new SymbolInfo();
	$$->makeCopy($1);
	printLogSymbol($$);}
	
	| variable INCOP {
	$$=new SymbolInfo($1->getName()+"++","factor");
	printLogRule("factor : variable INCOP");
	string newTempVar=newTemp();
	$$->code=$1->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV "+newTempVar+",AX";
	$$->setAssemblyVarName(newTempVar);
	$$->code+="\nADD "+$1->getAssemblyVarName()+",1\n";
	$$->setDeclaredType($1->getDeclaredType());
	printLogSymbol($$);
	}
	
	| variable DECOP {
	$$=new SymbolInfo($1->getName()+"--","factor");
	printLogRule("factor : variable DECOP");
	string newTempVar=newTemp();
	$$->code=$1->code+"\nMOV AX,"+$1->getAssemblyVarName()+"\nMOV "+newTempVar+",AX";
	$$->setAssemblyVarName(newTempVar);
	$$->code+="\nSUB "+$1->getAssemblyVarName()+",1\n";
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
		//mov ah,0\n mov al,$3->getAssemblyVarName()\n push AX
		$$=new SymbolInfo($1->getName()+","+$3->getName(),"arguments");
		printLogRule("arguments : arguments COMMA logic_expression");
		InsertInArgumentList($3);
		printLogSymbol($$);
		
		$$->code=$1->code+$3->code;
		$$->code+="\nMOV AX,"+$3->getAssemblyVarName()+"\nPUSH AX\n";
		
		}
		
	      | logic_expression {
	      	printLogRule("arguments : logic_expression");
	      	$$= new SymbolInfo();
	      	$$->makeCopy($1);
	      	InsertInArgumentList($1);
	      	printLogSymbol($$);
			$$->code=$1->code;
			$$->code+="\nMOV AX,"+$1->getAssemblyVarName()+"\nPUSH AX\n";
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
	
	

	errorfile.open(errorfilename);
	codefile.open(codefilename);
	yyin=fin;
	
	yyparse();
	

	errorfile.close();
	codefile.close();
	
	fclose(fin);
	
	

	return 0;
}

