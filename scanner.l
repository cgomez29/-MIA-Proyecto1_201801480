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
route ("/"{v_id})+("/"{v_id}"."{v_id})
v_string ("\""[^"\""]*"\"")

%%

"#".*       {}

"mkdisk"    {return mkdisk;}
"rmdisk"    {return rmdisk;}
"fdisk"    {return fdisk;}
"mount"    {return mount;}

"="             {return equals;}

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

{v_integer}      {strcpy(yylval.stringVal, yytext); return v_integer;}
{v_string}       {strcpy(yylval.stringVal, yytext); return v_string;}
{v_id}            {strcpy(yylval.stringVal, yytext); return v_id;}
{route}         {strcpy(yylval.stringVal, yytext); return route;}
(\s|\r|\t|\n)+   {}
[[:blank:]]     {}
.               {
                    if(yytext != "\r"){
                        std::cout << "Error Léxico: " << yytext << std::endl;
                    }
                }
%%