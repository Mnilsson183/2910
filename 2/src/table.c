#include "table.h"
#include "db.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table* buildTable(Table* table, char name[MAX_TABLE_NAME_SIZE + 1], char typeTable[MAX_TABLE_COLUMNS], unsigned int memoryTable[MAX_TABLE_COLUMNS], char columns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS], int numberOfColumns) {
    // write the table name
    memcpy((void *)table->name, name, MAX_TABLE_NAME_SIZE + 1);

    // number of filled columns
    table->numberOfColumns = numberOfColumns;

    // the given types of the data
    memcpy((void *)table->typeTable, typeTable, MAX_TABLE_COLUMNS);

    // write column names
    for (int i = 0; i < numberOfColumns; i++) {
        memcpy(table->columnNames[i], columns[i], MAX_TABLE_COLUMN_NAME_SIZE + 1);
    }

    int prev = 0;
    for (int i = 0; i < numberOfColumns; i++) {
        table->memoryTable[i] = prev;
        prev = prev + memoryTable[i];
    }
    table->memoryTableSize = prev;

    return table;
}


int parseDataIntoTable(Table* table, const char* filename) {
    char* lines[MAX_FILE_LENGTH];
    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        // rewrite as a block
        lines[i] = malloc(sizeof(char) * MAX_FILE_LINE_LENGTH);
    }

    unsigned int lineCount = 0;
    helperGetLines(&lineCount, filename, lines);

    // get a large temperary buffer
    char* tmpBuffer = malloc(MAX_FILE_LENGTH * MAX_FILE_LINE_LENGTH * sizeof(char));
    if (tmpBuffer == NULL) {
        printf("malloc error parsing data to tables");
        for (int i = 0; i < MAX_FILE_LENGTH; i++) {
            free(lines[i]);
        }
        return 1;
    }

    // find needed size and copy to tmp buffer
    char* s = NULL;
    char* tmpBufferPtr = tmpBuffer;
    int neededSpaceCount = 0;
    for (unsigned int i = 0; i < lineCount; i++) {
        s = lines[i];
        while(*s != '\0') {
            if (*s != ';') {
                neededSpaceCount++;
                *tmpBufferPtr = *s;
            }
            s++;
        }
    }

    // get a correctly sized area
    table->memory = malloc(neededSpaceCount * sizeof(char));
    if (table->memory == NULL) {
        printf("malloc error parsing data to tables");
        for (int i = 0; i < MAX_FILE_LENGTH; i++) {
            free(lines[i]);
        }
        free(tmpBuffer);
        return 1;
    }

    // copy tmp buffer to correctly sized memory
    memcpy(table->memory, tmpBuffer, neededSpaceCount);

    table->memory[neededSpaceCount] = '\0';

    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        free(lines[i]);
    }
    return 0;
}

void printTable(Table* table) {

    for (unsigned int i = 0; i < table->numberOfColumns; i++) {
        printf("%s ", table->columnNames[i]);
    }
    printf("\n");

    char buf[128];

    unsigned int row = 0;
    char *s = table->memory;
    while(*s != '\0') {

        for (unsigned int i = 0; i < table->numberOfColumns; i++) {
            if (table->typeTable[i] == 's') {
                printf("not implimented");
                return;
            } else if (table->typeTable[i] == 'u') {
                printf("not implimented");
                return;
            } else if (table->typeTable[i] == 'i') {
                printf("not implimented");
                return;
            }
        }
        printf("\n");
        row++;
    }
}
