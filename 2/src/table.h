#ifndef TABLE_H
#define TABLE_H

#define MAX_TABLE_COLUMNS 128
#define MAX_TABLE_NAME_SIZE 32
#define MAX_TABLE_COLUMN_NAME_SIZE 32

typedef struct {
    const char name[MAX_TABLE_NAME_SIZE + 1];
    char* memory;
    unsigned int memoryTable[MAX_TABLE_COLUMNS];
    unsigned int memoryTableSize;
    char typeTable[MAX_TABLE_COLUMNS];                                  // s for string i for int u for usign int
    char columnNames[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS];
    unsigned int numberOfColumns;
} Table;

struct TableEntry {
    char* memory;
    unsigned int memoryTable[MAX_TABLE_COLUMNS];
    char typeTable[MAX_TABLE_COLUMNS];                                  // s for string i for int u for usign int
    char columnNames[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS];
    unsigned int numberOfColumns;
};

Table* buildTable(Table* table, char name[MAX_TABLE_NAME_SIZE + 1], char typeTable[MAX_TABLE_COLUMNS], unsigned int memoryTable[MAX_TABLE_COLUMNS], char columns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS], int numberOfColumns);

int parseDataIntoTable(Table* table, const char* filename);

void printTable(Table* table);

#endif
