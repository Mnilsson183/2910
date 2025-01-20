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
        if (lines[i] == NULL) {
            printf("malloc error\n");
            return 1;
        }
    }

    unsigned int lineCount = 0;
    helperGetLines(&lineCount, filename, lines);

    table->numberOfRows = lineCount;

    int neededSpaceCount = lineCount * table->memoryTableSize;

    table->memory = malloc(neededSpaceCount + 1);
    if (table->memory == NULL) {
        for (int i = 0; i < MAX_FILE_LENGTH; i++) {
            free(lines[i]);
        }
            printf("malloc error\n");
            return 1;
    }

    char buf[128];

    for (unsigned int row = 0; row < lineCount; row++) {

        char *s = lines[row];

        for (unsigned int column = 0; column < table->numberOfColumns; column++) {

            int bufSize = 0;
            int offset = table->memoryTable[column] + row * table->memoryTableSize;

            while(*s != '\0' && *s != ';') {

                buf[bufSize] = *s;
                ++bufSize;
                ++s;
            }
            ++s;

            memcpy(&table->memory[offset], buf, bufSize);
        }
    }

    table->memory[neededSpaceCount + 1] = '\0';

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

    for (unsigned int i = 0; i < table->numberOfRows; i++) {

        for (unsigned int j = 0; j < table->numberOfColumns; j++) {

            int offset = table->memoryTable[j] + i * table->memoryTableSize;
            printf("%s ", &table->memory[offset]);
        }
        printf("\n");
    }
}
