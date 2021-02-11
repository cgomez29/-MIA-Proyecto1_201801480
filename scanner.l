%{%{
#include <iostream>
#include "parser.h"
Node *root;
%}

%option case-insensitive
%option noyywrap

/*REGULAR EXPRESSIONS*/
v_integer ("-")?[0-9]+
id ([a-zA-Z_])[a-zA-Z0-9_]*
route ("/"{id})+("/"{id}"."{id})
v_string ("\""[^"\""]*"\"")

%%

"#".*       {}

"mkdisk"    { return mkdisk;}

"="             {return equals;}

"-size"     { return size; }
"-f"        { return f; }
"-u"        { return u; }
"-path"     { return path; }

"m"         { return m; }
"k"         { return k; }

{v_integer}      {strcpy(yylval.stringVal, yytext); return v_integer;}

{v_string}       {strcpy(yylval.stringVal, yytext); return v_string;}
{id}            {strcpy(yylval.stringVal, yytext); return idN;}
{route}         {strcpy(yylval.stringVal, yytext); return route;}
(\s|\r|\t|\n)+   {}
[[:blank:]]     {}
.               {
                    if(yytext != "\r"){
                        std::cout << "Error Léxico: " << yytext << std::endl;
                    }
                }

<<EOF>>     { return yyterminate(); }
%%