%option noyywrap yylineno

%x stringState 
%x singleCommentState
%x multiCommentState

%{
#include<iostream> 
#include<stdio.h>
#include<stdlib.h>
#include<sstream>
#include<fstream>
#include<string>
#include "symboltable.cpp"
#include "y.tab.h"

using namespace std;

string str;
string strlexeme;
int line_no=1;
int line_count_inc=0;
int error_count=0;
extern SymbolTable * table;
extern ofstream logfile;
extern ofstream errorfile;
extern void printError(string text);



void passInfo(string name,string type)
{
      SymbolInfo *s= new  SymbolInfo(name,type);
      yylval.symbolinfo = s; //instead of typecasting
     // cout<<name<<"\t"<<type<<endl;
}

string getNormalChar(string str)
{
	string ch;
	ch=str[1];
	return ch;
}

string getEscapeChar(string str)
{
	string ch;
	switch(str[2])
	{
		case 'a':
		ch="\a";
		break;
		case 'b':
		ch="\b";
		break;
		case 'f':
		ch="\f";
		break;
		case 'n':
		ch="\r\n";
		break;
		case 'r':
		ch="\r";
		break;
		case 't':
		ch="\t";
		break;
		case '\\':
		ch="\\";
		break;
		case '\"':
		ch="\"";
		break;
		case '\'':
		ch="\'";
		break;
		case 'v':
		ch="\v";
		break;
		case '0':
		ch="\0";
		}
	return ch;
	
}

string getEscape(string str)
{
	string ch;
	switch(str[1])
	{
		case 'a':
		ch="\a";
		break;
		case 'b':
		ch="\b";
		break;
		case 'f':
		ch="\f";
		break;
		case 'n':
		ch="\r\n";
		break;
		case 'r':
		ch="\r";
		break;
		case 't':
		ch="\t";
		break;
		case '\\':
		ch="\\";
		break;
		case '\"':
		ch="\"";
		break;
		case '\'':
		ch="\'";
		break;
		case 'v':
		ch="\v";
		break;
		case '0':
		ch="\0";
		
		}
	return ch;
}


%}

AlphaNumeric [a-zA-Z0-9_]
Alphabet [a-zA-Z_]
digit [0-9]
digits {digit}+
newline "\n"|"\r\n"
whitespace [ \t\b]+
floatingpointnum {digits}?\.{digits} 
floatnum ({floatingpointnum}|{digits})([Ee][+-]?{digits})?

%%



if {return IF;}
for {return FOR;}
do {return DO;}
int {return INT;}
float {return FLOAT;}
void {return VOID;}
switch {return SWITCH;}
default {return DEFAULT;}
else {return ELSE;}
while {return WHILE;}
break {return BREAK;}
char {return CHAR;}
double {return DOUBLE;}
return {return RETURN;}
case {return CASE;}
continue {return CONTINUE;}
printf {return PRINTLN;}

{digits} {passInfo(yytext,"CONST_INT");
	return CONST_INT;}
{floatnum} {passInfo(yytext,"CONST_FLOAT");
	return CONST_FLOAT;}
{floatingpointnum}{floatingpointnum}+ {string st="Too many decimal points ";
					st= st+yytext;
					printError(st);
					passInfo(yytext,"WRONG_FLOAT");
					return WRONG_FLOAT;} 
({floatingpointnum}+|{digits})[\.]?([Ee][+-]?({digits}|{floatingpointnum}+))? {string st="Ill formed number ";
										st= st+yytext;
										printError(st);
										passInfo(yytext,"WRONG_FLOAT");
										return WRONG_FLOAT;} 



\'(\\[\\ntvb0rfa\'\"])\'  {}   
\'[^\'\n\\]\'  {}  
\'(\\[\\ntvb0rfa\'\"])[^\'\n]*{newline} {
					string st="Unterminated character ";
					st= st+yytext;
					printError(st);
					line_no++;}//UNFINISHED CHAR ERROR 

\'\' {
	string st="Empty character constant error ";
	st= st+yytext;
	printError(st);} //EMPTY CHAR CONST ERROR 
(\'[^\'\n]*{newline}) {
			string st= "Unterminated character ";
			st= st+yytext;
			printError(st);
			line_no++;} //
\'[^\'\n][^\'\n]+\' {
			string st= "Multi character constant error";
			st= st+yytext;
			printError(st);
}  //MULTI CONSTANT ERROR 


"++" {return INCOP;}//INCOP
"--" {return DECOP;}
[+-] {passInfo(yytext,"ADDOP");return ADDOP;}//ADDOP
[*/%] {passInfo(yytext,"MULOP");return MULOP;}//MULOP

"<"|"<="|">"|">="|"=="|"!=" {passInfo(yytext,"RELOP");return RELOP;}//RELOP

"=" {return ASSIGNOP;}//ASSIGNOP

"&&"|"||" {passInfo(yytext,"LOGICOP");return LOGICOP;}//LOGICOP
"!" {return NOT;}//NOT
"(" {return LPAREN;}//LPAREN
")" {return RPAREN;}//RPAREN
"{" {return LCURL;}//LCURL
"}" {return RCURL;}//RCURL
"[" {return LTHIRD;}//LTHIRD
"]" {return RTHIRD;}//RTHIRD
"," {return COMMA;}//COMMA
";" {/*cout<<"semicolon\n";*/return SEMICOLON;}//SEMICOLON

{Alphabet}{AlphaNumeric}* {passInfo(yytext,"ID");return ID;}
({digits}|{floatingpointnum}+)[\.]?{Alphabet}({Alphabet}|{digits}|{floatingpointnum})*[.]? {string st="Invalid prefix on ID or invalid suffix on Number ";
												st= st+yytext;
												printError(st);
												passInfo(yytext,"ID");
												return ID;}



"\"" {BEGIN stringState;str="";strlexeme="";str=str+yytext;strlexeme=strlexeme+yytext;line_count_inc=0;}//make string empty before entering
<stringState>"\"" {str=str+yytext;strlexeme=strlexeme+yytext;
		   line_no+=line_count_inc;BEGIN INITIAL;}
<stringState>\\{newline} {line_count_inc++;strlexeme=strlexeme+yytext;}//only newline allowed to make continuation
<stringState>\\[\\ntvb0rfa\"\'] {str=str+getEscape(yytext);strlexeme=strlexeme+yytext;}
<stringState>\\ {strlexeme=strlexeme+yytext;str=str+yytext;}
<stringState>{newline} {
			string st="Unterminated String ";
			st= st+yytext;
			printError(st);
			line_count_inc++;line_no+=line_count_inc;BEGIN INITIAL;}
<stringState>. {str=str+yytext;strlexeme=strlexeme+yytext;}

"//" {BEGIN singleCommentState;strlexeme="//";line_count_inc=0;}
<singleCommentState>\\{newline} {line_count_inc++;strlexeme=strlexeme+yytext;}
<singleCommentState>{newline} {line_count_inc++;line_no+=line_count_inc;BEGIN INITIAL;}
<singleCommentState>. {strlexeme=strlexeme+yytext;}

"/*" {BEGIN multiCommentState;strlexeme="/*";line_count_inc=0;}
<multiCommentState>"*/" {strlexeme=strlexeme+yytext;
			line_no+=line_count_inc; BEGIN INITIAL;return COMMENT;}
<multiCommentState>{newline} {line_count_inc++;strlexeme=strlexeme+yytext;}
<multiCommentState><<EOF>> {
				line_no+=line_count_inc;
				string st="Unterminated Comment ";
				st= st+yytext;
				printError(st);
				return 0;}
<multiCommentState>. {strlexeme=strlexeme+yytext;}

{newline} {line_no++;}

{whitespace} {}//WS

<<EOF>> {return 0;}
. {
	string st="Unrecognized character ";
	st= st+yytext;
	printError(st);}

