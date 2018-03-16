#ifndef TYPE_H
#define TYPE_H
#include <stdint.h>

typedef enum {NT_VALUE, NT_MATHOP, NT_ROLL, NT_RMOD, NT_SUCC} NTYPE;

struct node;

typedef struct rmod{
    char mt;
    int64_t v;
    struct rmod* next;
}rmod;

typedef struct {
    int64_t n;
    int32_t x;
    rmod* mod;
}roll;

typedef struct {
    char op;
    struct node* left;
    struct node* right;
}mathop;

typedef struct{
    int8_t lt;
    int64_t v;
    struct node* intern;
} succ;

typedef union {
    succ succ;
    rmod* rmod;
    roll roll;
    mathop mathop;
    int64_t value;
}nun;

typedef struct node {
    NTYPE nt;
    nun data;
}node;

void free_node(node* n);
void free_rmod(rmod* rmod);
node* new_value(int64_t v);
node* new_succ(int8_t lt, int64_t v, node* intern);
node* new_mathop(char op, node* left, node* right);
node* new_rmod(char mt, int64_t v, rmod* next);
node* new_roll(int64_t n, int64_t x, rmod* mod);
#endif
