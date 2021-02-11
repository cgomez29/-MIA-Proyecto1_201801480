%{

#include <stdio.h>
#include <string>
#include "scanner.h"
#include "node.h"

extern int yylineno;
extern int columna;
extern char *yytext;

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
%token <stringVal> k
%token <stringVal> m

// Symbols
%token <stringVal> equals


// Non Terminals

%type <node> Start
%type <node> Command
%type <node> MKParams

%start Start

%%

Start 
    :  Command      { root = $$; }
    ;

Command
    : mkdisk MKParams { $$ = new Node("MKParams", ""); $$ -> add(*$1); }
    ;

MKParams
    : size equals v_integer     { $$ =  new Node("SIZE", $3); }
    | u equals k                { $$ =  new Node("K", $3); }
    | u equals m                { $$ =  new Node("M", $3); }
    | path equals v_string      { $$ =  new Node("PATH", $3); }
    ;