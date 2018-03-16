#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sodium.h>
#include "eval.h"
#include "type.h"
int64_t eval_sub(node* n);
int64_t eval_mathop(node* n);
int64_t eval_roll(node* n);
void sort(uint32_t* arr, int64_t s);
int s = -1;

int64_t eval(node* n){
    int64_t rv=eval_sub(n);
    printf(" = %ld %s\n", rv, (s!=-1)?(s?"(success)":"(failure)"):"");
    return rv;
}

int64_t eval_sub(node* n){
    switch(n->nt){
    case NT_SUCC:
        {}//there has to be an expression here. Because C.
        int64_t v = eval_sub(n->data.succ.intern);
        s=n->data.succ.lt?(n->data.succ.v>v):(n->data.succ.v<v);
        return v;
        break;
    case NT_VALUE:
        printf("%ld", n->data.value);
        return n->data.value;
    case NT_MATHOP:
        return eval_mathop(n);
    case NT_ROLL:
        return eval_roll(n);
    case NT_RMOD:
        printf("parse tree in invalid state. Thoth is bad at code.\n");
        exit(1);
    }
    printf("we are never supposed to get here. Thoth sucks.\n");
    exit(1);
}

int64_t eval_mathop(node* n){
    printf("(");
    int64_t l=eval_sub(n->data.mathop.left);
    printf(" %c ", n->data.mathop.op); 
    int64_t r=eval_sub(n->data.mathop.right);
    printf(")");
    switch(n->data.mathop.op){
    case '+':
        return l + r;
    case '-':
        return l - r;
    case '*':
        return l * r;
    }
    printf("How did... what? Wow, Thoth's code is garbage.\n");
    exit(1);
}

int64_t eval_roll(node* n){
    int64_t rv=0;
    uint32_t *rl=calloc(n->data.roll.n, sizeof(uint32_t));
    for(int64_t i=0; i<n->data.roll.n;i++)
        rl[i]=randombytes_uniform(n->data.roll.x)+1;
    int64_t as=n->data.roll.n;
    int64_t es=n->data.roll.n;
    int64_t srn=as;
    uint32_t* barr=NULL;
    int64_t bs=0;
    if(n->data.roll.mod!=NULL){
        rmod* mod=n->data.roll.mod;
        sort(rl, as);
        do{
            switch(mod->mt){
            case 'e':
                for(int64_t i=((es-srn)>=0?es-srn:0);i<es;i++)
                    if(mod->v==rl[i]) rl[i]=randombytes_uniform(n->data.roll.x)+1;
                break;
            case 'k':
                srn=mod->v;
                break;
            case 'u':
                es=mod->v;
                break;
            case '!':
                for(int64_t i=((es-srn)>=0?es-srn:0);i<es;i++){
                    if(mod->v==rl[i]){
                        barr=realloc(barr, sizeof(uint32_t)*++bs);
                        barr[bs-1]=randombytes_uniform(n->data.roll.x)+1;
                    }
                }
                break;
            }
        }while((mod=mod->next)!=NULL);
    }
    printf("(");
    for(int64_t i=0; i<as;i++){
        if(i>=es-srn&&i<es) rv+=rl[i];
        printf((i<(es-srn)||i>=es)?"~~%d~~":"%d", rl[i]);
        if(i!=(as-1)) printf(", ");
    }
    if(bs){
        printf(" - !(");
        for(int64_t i=0; i<bs;i++){
            rv+=barr[i];
            printf("%d", barr[i]);
            if(i!=(bs-1)) printf(", ");
        }
        printf(")");
    }
    printf(")");
    free(rl);
    if(barr) free(barr);
    return rv;
}

void sort(uint32_t* arr, int64_t s){
    //could I have written my own implementation of quicksort?
    //yes, but stealing it from Rosetta Code was faster.
    if (s<2) return;
    uint32_t pivot = arr[s/2];
    int64_t i, j;
    for (i=0,j=s-1;;i++,j--) {
        while (arr[i]<pivot) i++;
        while (arr[j]>pivot) j--;
        if (i>=j) break;
        uint32_t temp=arr[i];
        arr[i]=arr[j];
        arr[j]=temp;
    }
    sort(arr, i);
    sort(arr+i, s-i);
}
