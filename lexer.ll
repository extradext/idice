%{
#include <stdlib.h>
#include <stdio.h>
#include "type.h"
#define YYSTYPE node*
#include "y.tab.h"

extern node* ntree;
extern uint64_t ntimes;
%}

%%
[0-9]+  {yylval=new_value(strtoll(yytext, NULL, 10));
        return NUM;}
[<>]    {yylval=new_succ(yytext[0]=='<', 0, NULL); return SUCC;}
[+\-*]  {yylval=new_mathop(yytext[0], NULL, NULL);return yytext[0]=='*'?MOP:PMOP;}
[!eku]  {yylval=new_rmod(yytext[0], 0, NULL); return RMOD;}
#.*     {printf("%s =>", yytext);}
.       {return yytext[0];}
[\t ]+ ;
^\/[A-Za-z]*[\t ]+ ;
%%

void parse(char* str){
    YY_BUFFER_STATE bs = yy_scan_string(str);
    yyparse();
    yy_delete_buffer(bs);
}