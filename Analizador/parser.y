%{

#include <stdio.h>
#include <string>
#include "scanner.h"
#include "../Headers/Node.h"

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
%token <stringVal> mount
%token <stringVal> unmount

// Parameters
%token <stringVal> size
%token <stringVal> f
%token <stringVal> u
%token <stringVal> path
%token <stringVal> type
%token <stringVal> p_delete
%token <stringVal> name
%token <stringVal> add
%token <stringVal> id

// Values
%token <stringVal> v_integer
%token <stringVal> v_string
%token <stringVal> v_id
%token <stringVal> v_id2
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
%type <node> MOUNTParams
%type <node> MOUNTParam

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
    | mount MOUNTParams             { $$ = new Node("MOUNT", ""); $$->add(*$2); }
    | unmount id equals v_id2        { $$ = new Node("UNMOUNT", $4); }
    | unmount id equals v_string        { $$ = new Node("UNMOUNT", $4); }
    ;
    
MKParams 
    : MKParams MKParam          { $$ = $1; $$->add(*$2); }
    | MKParam                   { $$ = new Node("PARAM", ""); $$->add(*$1); }
    ;

MKParam
    : size equals v_integer     { $$ =  new Node("SIZE", $3); }
    | u equals k                { $$ =  new Node("U", "k"); }
    | u equals m                { $$ =  new Node("U", "m"); }
    | f equals bf               { $$ =  new Node("F", "bf"); }
    | f equals ff               { $$ =  new Node("F", "ff"); }
    | f equals wf               { $$ =  new Node("F", "wf"); }
    | path equals route         { $$ =  new Node("PATH", $3); }
    | path equals v_string      { $$ =  new Node("PATH", $3); }
    ;

FParams 
    : FParams FParam { $$ = $1; $$->add(*$2); }
    | FParam         { $$ =  new Node ("PARAM", ""); $$->add(*$1); }
    ;

FParam 
    : size equals v_integer { $$ = new Node("SIZE", $3); }     
    | u equals b            { $$ = new Node("U", "b"); }
    | u equals k            { $$ = new Node("U", "k"); }
    | u equals m            { $$ = new Node("U", "m"); }
    | path equals route     { $$ = new Node("PATH", $3); }
    | path equals v_string  { $$ = new Node("PATH", $3); } 
    | type equals p         { $$ = new Node("TYPE", "p"); }
    | type equals e         { $$ = new Node("TYPE", "e"); }
    | type equals l         { $$ = new Node("TYPE", "l"); }
    | f equals bf           { $$ =  new Node("F", "bf"); }
    | f equals ff           { $$ =  new Node("F", "ff"); }
    | f equals wf           { $$ =  new Node("F", "wf"); }
    | p_delete equals fast  { $$ =  new Node("DELETE", "fast"); }
    | p_delete equals full  { $$ =  new Node("DELETE", "full"); }
    | name equals v_string  { $$ =  new Node("NAME", $3); }
    | name equals v_id        { $$ =  new Node("NAME", $3); }
    | add equals v_integer  { $$ =  new Node("ADD", $3); }
    ;

MOUNTParams
    : MOUNTParams MOUNTParam
    | MOUNTParam
    ;

MOUNTParam
    : path equals route     { $$ = new Node("PATH", $3); }
    | path equals v_string  { $$ = new Node("PATH", $3); }
    | name equals v_string  { $$ =  new Node("NAME", $3); }
    | name equals v_id        { $$ =  new Node("NAME", $3); }
    ;
