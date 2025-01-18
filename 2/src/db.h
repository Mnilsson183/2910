#ifndef DB_H
#define DB_H

#include <string.h>
#include "table.h"

#define MAX_FILE_LENGTH 256
#define MAX_FILE_LINE_LENGTH 1024
#define TABLE_INT_SIZE 32

struct DB {
    Table* studentTable;
    Table* courseTable;
    Table* gradeTable;
};

struct DB* buildDB(struct DB* dataBase, const char* studentFilename, const char* courseFilename, const char* gradeFilename);

void helperParseStringToSep(char *dest, char** src);
int helperGetLines(unsigned int* size, const char* filename, char** lines);

#endif
