#include "db.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct DBQueryResponse* fromColumnMalloc(struct DB* database, char *tablename);
struct DBQueryResponse* selectInstructionFreeRemalloc(struct DBQueryResponse* src, char *column);
struct DBQueryResponse* sortedByInstructionFreeRemalloc(struct DBQueryResponse* src, char *column);
struct DBQueryResponse* whereEqualsFreeRemalloc(struct DBQueryResponse* src, char* column, char* value);
struct DBQueryResponse* toAddFreeRemalloc(struct DBQueryResponse* src, char* values, char* column);
struct DBQueryResponse* updateToFreeRemalloc(struct DBQueryResponse* src, char* column, char* updatedValue);
struct DBQueryResponse* searchForFreeRemalloc(struct DBQueryResponse* src, char* column, char* value);
struct DBQueryResponse* averageElements(struct DBQueryResponse* src, char* column);

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

    return dataBase;
}

void fullPrintDBResponse(struct DBQueryResponse* response) {
    printf("Number of Columns: %d\n", response->numberOfColumns);
    printf("Number of Rows: %d\n", response->numberOfRows);
    printf("Memory Table Size: %d\n", response->memoryTableSize);

    printf("Memory Table offsets:\n");
    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        printf("%d ", response->memoryTable[i]);
    }
    printf("\n");

    printf("Column names:\n");
    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        printf("%s ", response->columns[i]);
    }
    printf("\n");


    printf("Columns names:\n");
    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        printf("%s ", response->columns[i]);
    }
    printf("\n");

    for (unsigned int i = 0; i < response->numberOfRows; i++) {

        for (unsigned int j = 0; j < response->numberOfColumns; j++) {

            int offset = response->memoryTable[j] + i * response->memoryTableSize;
            printf("%s|", &response->memory[offset]);
        }
        printf("\n");
    }
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
        if (*s1 == '\0' && *s2 == ' ') return 1;
        if (*s1 != *s2) return 0;
        ++s1;
        ++s2;
    }
}

int wordcpy(char* dest, const char* src) {
    if (dest == NULL) return -1;
    if (dest == NULL) return -1;

    int written = 0;
    while(*src != ' ' && *src != '\0') {
        *dest = *src;
        ++dest;
        ++src;
        ++written;
    }
    *dest = '\0';
    return written;
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

void executeInstruction(struct DB* database, char *query) {
    struct DBQueryResponse* response = NULL;
    int set = 0;
    char *q = query;
    char buf1[128];
    char buf2[128];
    printf("query: %s\n", query);
    while(q != NULL) {

        if (response == NULL && set == 1) {
            printf("Empty table\n");
            return;
        }

        //if (response != NULL) printDBResponse(response);

        if (wordcmpBool("FROM", q)) {
            if (response != NULL) {
                printf("You cannot have two FROM statments in one query\n");
                return;
            }
            // put q to the next word
            q += 5;
            int size = wordcpy(buf1, q);
            response = fromColumnMalloc(database, buf1);
            q += size + 1;
            set = 1;
        } else if (wordcmpBool("SELECT", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 7;
            int size = wordcpy(buf1, q);
            response = selectInstructionFreeRemalloc(response, buf1);
            printDBResponse(response);
            return;

        } else if (wordcmpBool("SORTEDBY", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 8;
            int size = wordcpy(buf1, q);

            response = sortedByInstructionFreeRemalloc(response, buf1);
            q += size + 1;

        } else if (wordcmpBool("WHERE", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 6;
            int size = wordcpy(buf1, q);

            q += size + 1;
            if (wordcmpBool("EQUALS", q)) {
                q += 7;
                size = wordcpy(buf2, q);
                q += size + 1;

                whereEqualsFreeRemalloc(response, buf1, buf2);
            } else {
                printf("Malformed statment: mo matching EQUALS to my WHERE\n    %s\n", query);
                return;
            }

        } else if (wordcmpBool("TO", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 3;
            int size = wordcpy(buf1, q);
            q += size + 1;

            if (wordcmpBool("ADD", q)) {
                q += 4;
                size = wordcpy(buf2, q);
                q += size + 1;

                toAddFreeRemalloc(response, buf2, buf1);
            } else {
                printf("Malformed statment: mo matching ADD to my TO\n    %s\n", query);
                return;
            }

            // add
        } else if (wordcmpBool("UPDATE", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 7;
            int size = wordcpy(buf1, q);
            q += size + 1;

            if (wordcmpBool("TO", q)) {
                q += 3;
                size = wordcpy(buf2, q);
                q += size + 1;

                response = updateToFreeRemalloc(response, buf1, buf2);
            } else {
                printf("Malformed statment: mo matching TO to my UPDATE\n    %s\n", query);
                return;
            }
            // to
        } else if (wordcmpBool("SEARCH", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 7;
            int size = wordcpy(buf1, q);
            q += size + 1;

            if (wordcmpBool("FOR", q)) {
                q += 4;
                size = wordcpy(buf2, q);
                q += size + 1;

                response = searchForFreeRemalloc(response, buf1, buf2);
            } else {
                printf("Malformed statment: mo matching FOR to my SEARCH\n    %s\n", query);
                return;
            }

            // for
        } else if (wordcmpBool("AVERAGE", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 8;
            int size = wordcpy(buf1, q);

            response = sortedByInstructionFreeRemalloc(response, buf1);
            q += size + 1;

        } else {
            printf("Cannot parse the following %s\n", q);
            return;
        }
    }


selectNotCalled:
    printf("FROM instruction was never used what table am I using?\n");
    return;
}

void printDBResponse(struct DBQueryResponse* response) {


    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        printf("%s ", response->columns[i]);
    }
    printf("\n");
    printf("\n");

    for (unsigned int i = 0; i < response->numberOfRows; i++) {

        for (unsigned int j = 0; j < response->numberOfColumns; j++) {

            int offset = response->memoryTable[j] + i * response->memoryTableSize;
            printf("%s ", &response->memory[offset]);
        }
        printf("\n");
    }
}



struct DBQueryResponse* fromColumnMalloc(struct DB* database, char *tablename) {

    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (response == NULL) {
        printf("malloc error building responsetable\n");
        return NULL;
    }

    for (unsigned int i = 0; i < database->tableCount; i++) {

        if (strcmp(tablename, database->tables[i]->name) == 0) {

            Table* table = database->tables[i];

            response->numberOfColumns = table->numberOfColumns;

            response->numberOfRows = table->numberOfRows;

            response->memoryTableSize = table->memoryTableSize;

            int size = response->numberOfColumns * response->numberOfRows + 1;
            response->memory = malloc(size);
            if (response->memory == NULL) {
                printf("Error malloc\n");
                return NULL;
            }

            memcpy(response->memory, table->memory, (response->numberOfRows * response->memoryTableSize) * sizeof(char));

            memcpy(response->memoryTable, table->memoryTable, MAX_TABLE_COLUMNS * sizeof(unsigned int));

            memcpy(response->typeTable, table->typeTable, MAX_TABLE_COLUMNS * sizeof(unsigned int));

            for (int i = 0; i < MAX_TABLE_COLUMNS; i++) {
                strcpy(response->columns[i], table->columnNames[i]);
            }

            return response;

        } else if (i == database->tableCount - 1) {

            printf("Unrecognized table name: %s\n", tablename);
            return NULL;
        }
    }
    return NULL;
}


struct DBQueryResponse* selectInstructionFreeRemalloc(struct DBQueryResponse* src, char *column) {

    if (column[0] == '*') return src;
    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (response == NULL) {
        printf("malloc error building response table\n");
        return NULL;
    }

    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        if (strcmp(column, src->columns[i]) == 0) {

            // hardcoded tmp values only able to select one colun
            response->numberOfColumns = 1;
            response->numberOfRows = src->numberOfRows;

            response->memoryTable[0] = 0;

            response->typeTable[0] = src->typeTable[i];

            strcpy(response->columns[0], src->columns[i]);

            response->memory = malloc(response->numberOfRows * response->numberOfColumns + 1);
            if (response->memory == NULL) {
                printf("Malloc error\n");
                return NULL;
            }

            for (unsigned int row = 0; row < src->numberOfRows; row++) {

                int memOffset = src->memoryTable[i] % src->memoryTableSize;

                int chars =  src->memoryTable[(i + 1) % src->numberOfColumns] - src->memoryTable[i];

                memcpy(&src->memory[row * src->memoryTableSize], &src->memory[src->memoryTable[memOffset * row]], chars * sizeof(char));
            }

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

struct DBQueryResponse* sortedByInstructionFreeRemalloc(struct DBQueryResponse* src, char *column) {
    return NULL;
}

struct DBQueryResponse* whereEqualsFreeRemalloc(struct DBQueryResponse* src, char* column, char* value) {
    return NULL;
}

struct DBQueryResponse* toAddFreeRemalloc(struct DBQueryResponse* src, char* values, char* column) {
    return NULL;
}

struct DBQueryResponse* updateToFreeRemalloc(struct DBQueryResponse* src, char* column, char* updatedValue) {
    return NULL;
}

struct DBQueryResponse* searchForFreeRemalloc(struct DBQueryResponse* src, char* column, char* value) {
    return NULL;
}

struct DBQueryResponse* averageElements(struct DBQueryResponse* src, char* column) {
    return NULL;
}
