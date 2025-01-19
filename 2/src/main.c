#include <stdlib.h>
#include <stdio.h>

#include "db.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Bad amount of args");
        return 1;
    }
    printf("running\n");
    struct DB* database = malloc(sizeof(struct DB));
    if (database == NULL) {
        printf("Error mallocing memory\n");
        return 2;
    }
    buildDB(database, argv[1], argv[2], argv[3]);
    char buf[256] = "SELECT firstname FROM students";

    executeInstruction(database, buf);
}
