#include <stdio.h>

int main(void) {

    int num_bins = 10;

    int ex1 = 100;
    int ex2 = 200;
    int ex3 = 300;

    int answer = 0;

    switch (num_bins) {
        case 10:
            answer = ex1;
            break;
        case 20:
            answer = ex2;
            break;
        default:
            answer = 99;
            break;
    }

    printf("\n%d\n\n", answer);

    return 0;
}