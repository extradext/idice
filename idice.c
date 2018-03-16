#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>
#include "type.h"
#include "eval.h"
extern node* ntree;
extern uint64_t ntimes;
void parse(char* str);

int main(int argc, char** argv){
    if (sodium_init() == -1) {
        printf("The salt has turned against us! Run!\n");
        exit(1);
    }
    if(argc<2){
        printf("No can do, boss.\n");
        exit(1);
    }
    parse(argv[1]);
    for(uint64_t i=0;i<ntimes;i++){
        eval(ntree);
    }
    free_node(ntree);
    return 0;
}
