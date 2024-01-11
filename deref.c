#include <stdio.h>

int main(void)
{
    char *s = NULL;
    printf("TEST: %s\n", s);

    *s = 'f';
    printf("TEST: %s\n", s);
    return 0;
}