%{%{
#include <iostream>
#include "parser.h"
Node *root;
%}


%option case-insensitive
%option noyywrap

/*Regular Expressions*/
v_integer ("-")?[0-9]+
v_id ([a-zA-Z_])[a-zA-Z0-9_]*
v_id2 ([a-zA-Z_]*)[a-zA-Z0-9_]*
route ("/"{v_id})+("/"{v_id}"."{v_id})
route2 ("/"{v_id})+("/"{v_id})
v_string ("\""[^"\""]*"\"")

%%

"#".*       {}

"mkdisk"   {return mkdisk;}
"rmdisk"   {return rmdisk;}
"fdisk"    {return fdisk;}
"mount"    {return mount;}
"unmount"  {return unmount;}
"mkfs"     {return mkfs;}
"rep"      {return rep;}
"exec"     {return exec;}
"login"    {return login;}
"mkdir"    {return mkdir;}
"pause"    {return pausev;}
"logout"    {return logout;}

"="         {return equals;}

"-size"     {return size;}
"-f"        {return f;}
"bf"        {return bf;}
"ff"        {return ff;}
"wf"        {return wf;}
"-u"        {return u;}
"m"         {return m;}
"k"         {return k;}
"b"         {return b;}
"-path"     {return path;}
"-type"     {return type;}
"p"     {return p;}
"e"     {return e;}
"l"     {return l;}
"-delete" {return p_delete;}
"fast"  {return fast;}
"full"  {return full;}
"-name" {return name;}
"-add"  {return add;}
"-id"  {return id;}
"-fs" {return fs;}
"2fs" {return dosfs;}
"3fs" {return tresfs;}
"-ruta" {return ruta;}
"-usr" {return usuario;}
"-usuario" {return usuario;}
"-pwd" {return password;}
"-password" {return password;}
"-p" {return p;}

{v_integer}      {strcpy(yylval.stringVal, yytext); return v_integer;}
{v_string}       {strcpy(yylval.stringVal, yytext); return v_string;}
{v_id}            {strcpy(yylval.stringVal, yytext); return v_id;}
{v_id2}            {strcpy(yylval.stringVal, yytext); return v_id2;}
{route}         {strcpy(yylval.stringVal, yytext); return route;}
{route2}         {strcpy(yylval.stringVal, yytext); return route2;}
(\s|\r|\t|\n)+   {}
[[:blank:]]     {}
.               {
                    if(yytext != "\r"){
                        std::cout << "Error : " << yytext << std::endl;
                    }
                }
%%