#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    // int len = 4;
    // char s[len+1];
    // char *src = "1234567890";

    // strncpy(s, src, len);

    char *s = malloc(sizeof(char)*10);

    s[0] = '\0';

    // printf("%s\n", s);

    free(s);

    return 0;
}