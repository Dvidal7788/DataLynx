#include <stdio.h>
#include <string.h>

int main(void) {

    char s[1] = {'\0'};
    char s2[1] = {'\0'};

    printf("strcmp %d\n", strcmp(s, s2));

    return 0;
}