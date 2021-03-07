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
%token <stringVal> mkfs
%token <stringVal> rep

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
%token <stringVal> fs
%token <stringVal> ruta

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

// for fs
%token <stringVal> dosfs
%token <stringVal> tresfs

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
%type <node> MKFSParams
%type <node> MKFSParam
%type <node> REPParams
%type <node> REPParam

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
    | unmount id equals v_id2       { $$ = new Node("UNMOUNT", $4); }
    | unmount id equals v_string    { $$ = new Node("UNMOUNT", $4); }
    | mkfs MKFSParams               { $$ = new Node("MKFS", ""); $$->add(*$2); }
    | rep REPParams               { $$ = new Node("REP", ""); $$->add(*$2); }
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
    | name equals v_id      { $$ =  new Node("NAME", $3); }
    | add equals v_integer  { $$ =  new Node("ADD", $3); }
    ;

MOUNTParams
    : MOUNTParams MOUNTParam { $$ = $1; $$->add(*$2); }
    | MOUNTParam             { $$ =  new Node ("PARAM", ""); $$->add(*$1); }
    ;

MOUNTParam
    : path equals route     { $$ = new Node("PATH", $3); }
    | path equals v_string  { $$ = new Node("PATH", $3); }
    | name equals v_string  { $$ =  new Node("NAME", $3); }
    | name equals v_id       { $$ =  new Node("NAME", $3); }
    ;

MKFSParams
    : MKFSParams MKFSParam { $$ = $1; $$->add(*$2); }
    | MKFSParam            { $$ =  new Node ("PARAM", ""); $$->add(*$1); }
    ;

MKFSParam
    : id equals v_id2 { $$ = new Node("ID", $3); }
    | id equals v_string { $$ = new Node("ID", $3); }
    | type equals fast { $$ = new Node("TYPE", $3); }
    | type equals full { $$ = new Node("TYPE", $3); }
    | fs equals dosfs { $$ = new Node("FS", "2fs"); }
    | fs equals tresfs { $$ = new Node("FS", "3fs"); }
    ;

REPParams
    : REPParams REPParam { $$ = $1; $$->add(*$2); }
    | REPParam            { $$ =  new Node ("PARAM", ""); $$->add(*$1); }
    ;

REPParam
    : path equals route     { $$ = new Node("PATH", $3); }
    | path equals v_string  { $$ = new Node("PATH", $3); }
    | id equals v_id2 { $$ = new Node("ID", $3); }
    | id equals v_string { $$ = new Node("ID", $3); }
    | name equals v_id       { $$ =  new Node("NAME", $3); }
    | name equals v_string  { $$ =  new Node("NAME", $3); }
    | ruta equals v_id       { $$ =  new Node("RUTA", $3); }
    | ruta equals v_string  { $$ =  new Node("RUTA", $3); }
    ;