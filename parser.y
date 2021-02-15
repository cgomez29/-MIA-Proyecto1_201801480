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

// main commands
%token <stringVal> mkdisk
%token <stringVal> rmdisk
%token <stringVal> fdisk

// Parameters
%token <stringVal> size
%token <stringVal> f
%token <stringVal> u
%token <stringVal> path
%token <stringVal> type
%token <stringVal> p_delete
%token <stringVal> name
%token <stringVal> add

// Values
%token <stringVal> v_integer
%token <stringVal> v_string
%token <stringVal> id
%token <stringVal> route

// for -u
%token <stringVal> k
%token <stringVal> m
%token <stringVal> b

// for -f
%token <stringVal> bf
%token <stringVal> ff
%token <stringVal> wf

// for type
%token <stringVal> p
%token <stringVal> e
%token <stringVal> l

// for delete
%token <stringVal> fast
%token <stringVal> full

// Symbols
%token <stringVal> equals

// Non Terminals

%type <node> Start
%type <node> Commands
%type <node> MKParams
%type <node> MKParam
%type <node> FParams
%type <node> FParam

%start Start

%%

Start 
    :  Commands      { root = $$; }
    ;

Commands
    : mkdisk MKParams               { $$ = new Node("MKDISK", ""); $$->add(*$2); }
    | rmdisk path equals route      { $$ = new Node("RMDISK", $4); }
    | rmdisk path equals v_string   { $$ = new Node("RMDISK", $4); }
    | fdisk FParams                 { $$ = new Node("FDISK", ""); $$->add(*$2); }
    ;
    
MKParams 
    : MKParams MKParam          { $$ = $1; $$->add(*$2); }
    | MKParam                   { $$ = new Node("PARAM", ""); $$->add(*$1); }
    ;

MKParam
    : size equals v_integer     { $$ =  new Node("SIZE", $3); }
    | u equals k                { $$ =  new Node("U", $3); }
    | u equals m                { $$ =  new Node("U", $3); }
    | f equals bf               { $$ =  new Node("F", $3); }
    | f equals ff               { $$ =  new Node("F", $3); }
    | f equals wf               { $$ =  new Node("F", $3); }
    | path equals route         { $$ =  new Node("PATH", $3); }
    | path equals v_string      { $$ =  new Node("PATH", $3); }
    ;

FParams 
    : FParams FParam { $$ = $1; $$->add(*$2); }
    | FParam         { $$ =  new Node ("PARAM", ""); $$->add(*$1); }
    ;

FParam 
    : size equals v_integer { $$ = new Node("SIZE", $3); }     
    | u equals b            { $$ = new Node("U", $3); } 
    | u equals k            { $$ = new Node("U", $3); } 
    | u equals m            { $$ = new Node("U", $3); } 
    | path equals route     { $$ = new Node("PATH", $3); } 
    | path equals v_string  { $$ = new Node("PATH", $3); } 
    | type equals p         { $$ = new Node("TYPE", $3); } 
    | type equals e         { $$ = new Node("TYPE", $3); } 
    | type equals l         { $$ = new Node("TYPE", $3); } 
    | f equals bf           { $$ =  new Node("F", $3); }
    | f equals ff           { $$ =  new Node("F", $3); }
    | f equals wf           { $$ =  new Node("F", $3); }
    | p_delete equals fast    { $$ =  new Node("DELETE", $3); }
    | p_delete equals full    { $$ =  new Node("DELETE", $3); }
    | name equals v_string  { $$ =  new Node("NAME", $3); }
    | name equals id        { $$ =  new Node("NAME", $3); }
    | add equals v_integer  { $$ =  new Node("ADD", $3); }
    ;