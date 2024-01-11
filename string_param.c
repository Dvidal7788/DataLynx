#include <stdio.h>

void func(char s[]);

int main(void) {

    func("hello");

    return 0;
}


void func(char s[]) {
    s[0] = '!';

    printf("\n%s\n\n", s);
}