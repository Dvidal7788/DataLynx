#include <stdio.h>
#include <stdlib.h>

int main(void){

    char *s = malloc(sizeof(char)*10);


    char *s2 = s;

    printf("%p\n%p\n", s, s2);

    free(s);
    s = NULL; /* setting string to NULL protects against double free */
    free(s);


    // FILE *file = fopen("csv/staff.csv", "r");

    // fclose(file);
    // file = NULL;
    // fclose(file); /* Even if you set file pointer to NULL, it will still give valgrind errors (although not double free) */



    return 0;
}