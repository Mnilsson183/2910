#include "db.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void helperParseStringToSep(char *dest, char** src) {
    while((*src)[0] != ';' && (*src)[0] != '\0') {
        *dest = (*src)[0];
        ++dest;
        ++(*src);
    }
    *dest = '\0';
    ++(*src);
}

// assumes linesBuffer is inited and fully malloced
// returns an error value
// 1 = malloc error
int helperGetLines(unsigned int* size, const char* filename, char** lines) {
    *size = 0;
    FILE* studentFile = fopen(filename, "r");

    if (studentFile == NULL) {
        printf("Unable to read from file: %s\n", filename);
        return 1;
    }

    while(fgets(lines[*size], MAX_FILE_LINE_LENGTH, studentFile) != NULL) (*size)++;
    return 0;
}

// most of this db stuff is hard coded but a schema command would be able to build all of this
struct DB* buildDB(struct DB* dataBase, const char* studentFilename, const char* courseFilename, const char* gradeFilename) {

    Table* studentTable = malloc(sizeof(Table));
    if (studentTable == NULL) {
        printf("malloc error\n");
        return NULL;
    }

    char studentTableName[MAX_TABLE_NAME_SIZE + 1] = "students";
    char studentTableTypeTable[MAX_TABLE_COLUMNS] = {'u', 's', 's', 's', 's'};
    unsigned int studentTableMemoryTable[MAX_TABLE_COLUMNS] = {TABLE_INT_SIZE, 200, 50, 16, 320};
    char studentTableColumns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS] = {"id", "lastname", "firstname", "phonenumber", "email"};
    int studentTableNumberOfColumns = 5;

    buildTable(studentTable, studentTableName, studentTableTypeTable, studentTableMemoryTable, studentTableColumns, studentTableNumberOfColumns);

    int error = parseDataIntoTable(studentTable, studentFilename);
    if (error != 0) {
        printf("perseDataToTable error: %d\n", error);
        return NULL;
    }

    dataBase->tables[0] = studentTable;


    Table* courseTable = malloc(sizeof(Table));
    if (courseTable == NULL) {
        printf("malloc error\n");
        return NULL;
    }

    char courseTableName[MAX_TABLE_NAME_SIZE + 1] = "courses";
    char courseTableTypeTable[MAX_TABLE_COLUMNS] = {'s', 'u', 'u'};
    unsigned int courseTableMemoryTable[MAX_TABLE_COLUMNS] = {30, TABLE_INT_SIZE, TABLE_INT_SIZE};
    char courseTableColumns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS] = {"name", "semester", "courseid"};
    int courseTableNumberOfColumns = 3;

    buildTable(courseTable, courseTableName, courseTableTypeTable, courseTableMemoryTable, courseTableColumns, courseTableNumberOfColumns);

    error = parseDataIntoTable(courseTable, courseFilename);
    if (error != 0) {
        printf("perseDataToTable error: %d\n", error);
        return NULL;
    }

    dataBase->tables[1] = courseTable;

    Table* gradeTable = malloc(sizeof(Table));
    if (gradeTable == NULL) {
        printf("malloc error\n");
        return NULL;
    }

    char gradeTableName[MAX_TABLE_NAME_SIZE + 1] = "grades";
    char gradeTableTypeTable[MAX_TABLE_COLUMNS] = {'u', 's', 's', 'u', 'u', 'u', 'u'};
    unsigned int gradeTableMemoryTable[MAX_TABLE_COLUMNS] = {TABLE_INT_SIZE, 200, 50, TABLE_INT_SIZE, TABLE_INT_SIZE, TABLE_INT_SIZE, TABLE_INT_SIZE};
    char gradeTableColumns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS] = {"id", "lastname", "firstname", "g1", "g2", "g3", "g4"};
    int gradeTableNumberOfColumns = 7;

    buildTable(gradeTable, gradeTableName, gradeTableTypeTable, gradeTableMemoryTable, gradeTableColumns, gradeTableNumberOfColumns);

    error = parseDataIntoTable(gradeTable, gradeFilename);
    if (error != 0) {
        printf("perseDataToTable error: %d\n", error);
        return NULL;
    }

    dataBase->tables[2] = gradeTable;

    dataBase->tableCount = 3;


    for (unsigned int i = 0; i < dataBase->tableCount; i++) {
        printTable(dataBase->tables[i]);
    }

    return dataBase;
}

// 0 = false
// 1 = true
int wordcmpBool(char *s1, char *s2) {
    if (s1 == s2) return 1;
    if (s1 == NULL) return 0;
    if (s2 == NULL) return 0;

    while(1) {
        if (*s1 == '\0' && *s2 == '\0') return 1;
        if (*s1 == ' ' && *s2 == '\0') return 1;
        if (*s1 != *s2) return 0;
        ++s1;
        ++s2;
    }
}

unsigned long wordlen(char *s) {
    if (s == NULL) return 0;
    unsigned long length = 0;
    while(*s != '\0' && *s != ' ') {
        length++;
        ++s;
    }
    return length;
}

enum COMMANDS identifyInstruction(char * s) {
    if (wordcmpBool(s, "SELECT") == 1) return SELECT;
    if (wordcmpBool(s, "FROM") == 1) return FROM;
    if (wordcmpBool(s, "SORTED") == 1) return SORTED;
    return UNKNOWN;
}

void executeInstruction(struct DB* database, char *query) {
    // pointers to the first letter of each word
    char* wordPtrs[128];
    int wordPtrIndex = 0;

    char* q = query;

    while (*q != '\0') {
        if (*q == ' ' && q[1] != '\0') {
            wordPtrs[wordPtrIndex] = q+1;
            ++wordPtrIndex;
        }
        ++q;
    }

    // make the index point to the last word
    --wordPtrIndex;

    char buf[MAX_TABLE_COLUMN_NAME_SIZE + 1];

    // this is a query
    if (identifyInstruction(wordPtrs[wordPtrIndex - 1]) == FROM) {
        struct DBQueryResponse *response = buildResponseFromTableMalloc(database, wordPtrs[wordPtrIndex]);
        wordPtrIndex = wordPtrIndex - 2;

        enum COMMANDS command;

        // start from the very last word and work inwards
        while (wordPtrIndex >= 0) {
            command = identifyInstruction(wordPtrs[wordPtrIndex]);

            if (command == UNKNOWN) {

                unsigned int wordlength = wordlen(wordPtrs[wordPtrIndex]);
                memcpy(buf, wordPtrs[wordPtrIndex], wordlength);
                buf[wordlength + 1] = '\0';

            } else if (command == SELECT) {

                response = selectInstructionFreeRemalloc(response, buf);

                if (response == NULL) {

                    printf("malformed select statment: %s", buf);
                    free(response);
                    return;
                }
            }

            --wordPtrIndex;
            free(response);
        }

    } else {
        printf("not implimented non query\n");
        return;
    }
}

void printDBResponse(struct DBQueryResponse* src) {
    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        printf("%s ", src->columns[i]);
    }
    printf("\n");

    char buf[128];

    unsigned int row = 0;
    char *s = src->memory;
    while(*s != '\0') {

        for (unsigned int i = 0; i < src->numberOfColumns; i++) {
            if (src->typeTable[i] == 's') {
                int indexStart = src->memoryTable[i] + row * src->memoryTableSize;
                int indexNextStart = i == src->numberOfColumns - 1 
                    ? src->memoryTable[0] + (row + 1) *src->memoryTableSize 
                    : src->memoryTable[i + 1] + row * src->memoryTableSize;

                memcpy(buf, &src->memory[indexStart], indexNextStart - indexStart);
                buf[indexNextStart - indexStart + 1] = '\0';
                printf("%s ", buf);
            } else if (src->typeTable[i] == 'u') {
                printf("not implimented");
                return;
            } else if (src->typeTable[i] == 'i') {
                printf("not implimented");
                return;

            }
        }
        printf("\n");
        row++;
    }
}


struct DBQueryResponse* buildResponseFromTableMalloc(struct DB* database, char *tablename) {

    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (response == NULL) {
        printf("malloc error building responsetable\n");
        return NULL;
    }

    for (unsigned int i = 0; i < database->tableCount; i++) {
        if (strcmp(tablename, database->tables[i]->name) == 0) {

            Table* table = database->tables[i];

            int size = strlen(table->memory);
            response->memory = malloc(size);
            if (response->memory == NULL) {
                printf("Error malloc\n");
                return NULL;
            }
            strcpy(response->memory, table->memory);

            memcpy(response->memoryTable, table->memoryTable, MAX_RESPONSE_COLUMNS);

            memcpy(response->typeTable, table->typeTable, MAX_TABLE_COLUMNS);

            for (int i = 0; i < MAX_TABLE_COLUMNS; i++) {
                strcpy(response->columns[i], table->columnNames[i]);
            }

            response->numberOfColumns = table->numberOfColumns;

            printf("Hello\n");
            return response;

        } else if (i == database->tableCount - 1) {

            printf("Unrecognized table name: %s\n", tablename);
            return NULL;
        }
    }
    return NULL;
}


struct DBQueryResponse* selectInstructionFreeRemalloc(struct DBQueryResponse* src, char *column) {

    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (response == NULL) {
        printf("malloc erroor building responsetable\n");
        return NULL;
    }

    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        if (strcmp(column, src->columns[i]) == 0) {

            // hardcoded tmp values only able to select one colun

            response->numberOfColumns = 1;

            response->memoryTable[0] = 0;

            response->typeTable[0] = src->typeTable[i];

            strcpy(response->columns[0], src->columns[i]);

            // write memory
            int index = 0;
            int memoryIndex = 0;

            int indexDiff = i == src->numberOfColumns - 1
                ? src->memoryTable[i]
                : src->memoryTable[i+1] - src->memoryTable[i];

            while(src->memory[index] != '\0') {
                if (index % src->memoryTableSize == src->memoryTable[i]) {
                    memcpy(&response->memory[memoryIndex], &src->memory[index], indexDiff);
                    index += indexDiff;
                    memoryIndex += indexDiff;
                }
                ++index;
            }
            response->memory[indexDiff + 1] = '\0';

            free(src->memory);
            free(src);

            return response;

        } else if (i == src->numberOfColumns - 1) {

            printf("Unknown name of column: %s\n", column);
            return NULL;
        }
    }

    return NULL;
}
