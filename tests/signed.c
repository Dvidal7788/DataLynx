#include <stdio.h>
#include <stdint.h>

// void myFunc(uint16_t x) {
//     printf("x: %d\n", x);
// }

int main(void)
{

    uint8_t x = -1;
    uint16_t y = -1;
    uint32_t z = -1;

    // printf("x: %d, y: %d, z: %d\n", x, y, z); /* This way, z prints SIGNED, evnthough its uint32_t */
    printf("x: %u, y: %u, z: %u\n", x, y, z); /* This way prints z correctly */

    if (z < 0) printf("Less than Zero\n");

    return 0;
}