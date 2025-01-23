#include <stdlib.h>
#include <stdio.h>

#include "db.h"

int main(int argc, char** argv) {
    //if (argc != 4) {
    //    printf("Bad amount of args\n");
    //    return 1;
    //}
    struct DB* database = malloc(sizeof(struct DB));
    if (database == NULL) {
        printf("Error mallocing memory\n");
        return 2;
    }
    buildDB(database, argv[1], argv[2], argv[3]);
    char buf[256] = "SELECT firstname FROM students";
    if (argc == 5) {
        strcpy(buf, argv[4]);
    }

    for (int i = 4; i < argc; i++) {
        strcpy(buf, argv[4]);
        executeInstruction(database, buf);
    }
}
