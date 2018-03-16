#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "type.h"

void free_rmod (rmod* rmod){
    if(rmod->next)
        free_rmod(rmod->next);
    free(rmod);
}

void free_node(node* n){
    switch(n->nt){
    case NT_MATHOP:
        if((n->data).mathop.left)
            free((n->data).mathop.left);
        if((n->data).mathop.right)
            free((n->data).mathop.right);
        break;
    case NT_RMOD:
        if ((n->data).rmod)
            free_rmod(((n->data).rmod));
        break;
    }
    free(n);
}

node* new_value(int64_t v){
    node* rv = calloc(1, sizeof(node));
    rv->nt=NT_VALUE;
    (rv->data).value=v;
    return rv;
}

node* new_succ(int8_t lt, int64_t v, node* intern){
    node* rv = calloc(1, sizeof(node));
    rv->nt=NT_SUCC;
    (rv->data).succ.lt=lt;
    (rv->data).succ.v=v;
    (rv->data).succ.intern = intern;
    return rv;
}

node* new_mathop(char op, node* left, node* right){
    node* rv = calloc(1, sizeof(node));
    rv->nt=NT_MATHOP;
    (rv->data).mathop.op=op;
    (rv->data).mathop.left=left;
    (rv->data).mathop.right=right;
    return rv;
}

node* new_rmod(char mt, int64_t v, rmod* next){
    rmod* rm=calloc(1, sizeof(rmod));
    rm->mt=mt;
    rm->v=v;
    rm->next = next;
    node* rv = calloc(1, sizeof(node));
    rv->nt=NT_RMOD;
    (rv->data).rmod = rm;
    return rv;
}

node* new_roll(int64_t n, int64_t x, rmod* mod){
    if(x>0xffffffff){
        printf("Your die has too many sides. Talk to Thoth if you want the sordid details (TL;DR, he is lazy).\n");
        exit(0);
    }
    node* rv = calloc(1, sizeof(node));
    rv->nt=NT_ROLL;
    (rv->data).roll.n = n;
    (rv->data).roll.x = x;
    (rv->data).roll.mod = mod;
    return rv;
}
