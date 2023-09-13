#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

    const char *text = "hello\n";

    char *s, *s2 = malloc(sizeof(char)*strlen(text)+1);

    strcpy(s2, text);
    // char *s2 = s;

    printf("%p %p\n", s, s2);


    return 0;
}