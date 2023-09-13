#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool func(char *s) {
    if (s[0] == 'h') return true;
    return false;
}

typedef struct myObj {
    bool (*func)(char *s);
} myObj;

int main(void) {

    bool (*func_ptr)(char *s);

    // func_ptr = &func;
    myObj myObjInstance;
    
    // printf("HERE\n");
    // printf("HERE\n");
    // printf("HERE\n");
    // printf("HERE\n");

    // printf("starts with 's': %d\n", (*func_ptr)("David"));


    return 0;
}