#ifndef DB_H
#define DB_H

#include <string.h>
#include "table.h"

#define MAX_FILE_LENGTH 256
#define MAX_FILE_LINE_LENGTH 1024
#define TABLE_INT_SIZE 32
#define DB_DEFAULT_TABLE_MAX 3
#define MAX_RESPONSE_COLUMNS 16
#define MAX_CMD_LEN

struct DB {
    Table* tables[DB_DEFAULT_TABLE_MAX];
    unsigned int tableCount;
};

struct DBQueryResponse {
    unsigned int numberOfColumns;
    unsigned int numberOfRows;
    unsigned int memoryTableSize;
    char* memory;
    unsigned int memoryTable[MAX_RESPONSE_COLUMNS];
    char typeTable[MAX_TABLE_COLUMNS];                          // s for string i for int u for usign int
    char columns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_RESPONSE_COLUMNS];
};

struct DB* buildDB(struct DB* dataBase, const char* studentFilename, const char* courseFilename, const char* gradeFilename);
void executeInstruction(struct DB* database, char *query);
struct DBQueryResponse* buildResponseFromTableMalloc(struct DB* database, char *tablename);

struct DBQueryResponse* selectInstructionFreeRemalloc(struct DBQueryResponse* src, char *column);
void printDBResponse(struct DBQueryResponse* src);

void helperParseStringToSep(char *dest, char** src);
int helperGetLines(unsigned int* size, const char* filename, char** lines);

#endif
