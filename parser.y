%{
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "type.h"
#define YYSTYPE node*
void yyerror(const char *str){
    printf("I reject your reality and substitute my own!\n");
    exit(0);
}
int yywrap(){
        return 1;
} 
node* ntree;
uint64_t ntimes=1;

%}
%token NUM SUCC PMOP MOP RMOD
%left PMOP
%left MOP
%precedence RMOD
%nonassoc 'd'


%%
expr: head {ntree=$1;}
    | NUM 'r' head {ntree=$3; ntimes=$1->data.value; free_node($1);}
    ;
head: ops 
    | ops SUCC NUM {
                    $2->data.succ.intern=$1;
                    $2->data.succ.v=$3->data.value;
                    free_node($3);
                    $$=$2;
                   }
    ;
ops: NUM
| '(' ops ')' {$$=$2;}
   | ops PMOP ops {
                   $2->data.mathop.left=$1;
                   $2->data.mathop.right=$3;
                   $$=$2;
                  }
   | ops MOP ops  {
                   $2->data.mathop.left=$1;
                   $2->data.mathop.right=$3;
                   $$=$2;
                  }
   | NUM 'd' NUM  {
                   $$=new_roll($1->data.value, $3->data.value, NULL);
                   free_node($1);
                   free_node($3);
                  }
   | NUM 'd' NUM rmods {
                        $$=new_roll($1->data.value,\
                                    $3->data.value, $4->data.rmod);
                        ($4->data).rmod=NULL;
                        free_node($1);
                        free_node($3);
                        free_node($4);
                       }
   ;
rmods: rmod 
     | rmod rmods {
                   $1->data.rmod->next=$2->data.rmod;
                   $2->data.rmod=NULL;
                   free_node($2);
                   $$=$1;
                  }
     ;
rmod: RMOD NUM {
                $1->data.rmod->v=$2->data.value;
                free_node($2);
                $$=$1;
               }
    ;
%%
