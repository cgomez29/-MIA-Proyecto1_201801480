%{

#include <stdio.h>
#include <string>
#include "scanner.h"
#include "Node.h"

extern int yylineno;
extern int columna;
extern char *yytext;
extern Node *root;


int yyerror(const char* msg){
        std::cout<< "Error sintáctico: " << msg << std::endl;
        return 0;
}

%}

%define parse.error verbose

%union {
char                stringVal[500];
class Node          *node;
}

// Principal commands
%token <stringVal> mkdisk

// Params
%token <stringVal> size
%token <stringVal> f
%token <stringVal> u
%token <stringVal> path

// Values
%token <stringVal> v_integer
%token <stringVal> v_string
%token <stringVal> id
%token <stringVal> route
%token <stringVal> k
%token <stringVal> m

// Symbols
%token <stringVal> equals


// Non Terminals

%type <node> Start
%type <node> Command
%type <node> MKSENTENCE
%type <node> MKParams

%start Start

%%

Start 
    :  Command      { root = $$; }
    ;

Command
    : mkdisk MKSENTENCE { $$ = new Node("MKDISK", ""); $$->add(*$2); }
    ;
    
MKSENTENCE :  MKSENTENCE MKParams               { $$ = $1; $$->add(*$2); }
        | MKParams                          { $$ = new Node("PARAM", ""); $$->add(*$1); }
    ;

MKParams
    : size equals v_integer     { $$ =  new Node("SIZE", $3); }
    | u equals k                { $$ =  new Node("K", $3); }
    | u equals m                { $$ =  new Node("M", $3); }
    | path equals v_string      { $$ =  new Node("PATH", $3); }
    ;