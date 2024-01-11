#include <stdio.h>

int main(void) {

    /* Although this seems to behave fine, it is undefined behavior. */

    int x = 9;

    const char *out = NULL;
    int *y_ptr = NULL;

    if (x > 100) {
        printf("NO\n");
    }
    else {
        const char *in = "THis is A String!?!?";
        out = in;
        int y = 899;
        y_ptr = &y;
    }

    (*y_ptr)++;
    printf("\n%s\n\n", out);
    printf("\n%s\n%d\n\n", out, *y_ptr);

    return 0;
}