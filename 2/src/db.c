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

int max(int i1, int i2) {
    if (i1 > i2) return i1;
    else return i2;
}

int min(int i1, int i2) {
    if (i1 < i2) return i1;
    else return i2;
}

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
    char gradeTableColumns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS] = {"id", "lastname", "firstname", "grades", "grades", "grades", "grades"};
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

int getOff(unsigned int row, int memMapSize, unsigned int memMapOffset) {
    return memMapOffset + row * memMapSize;
}

char* getPtrToMemLoc(struct DBQueryResponse* response, int row, int column) {
    if (response->numberOfRows < row) return NULL;
    else if (row < 0) return NULL;
    else if (response->numberOfColumns < column) return NULL;
    else if (column < 0) return NULL;
    char *c = &response->memory[response->memoryTable[column] + row * response->memoryTableSize];

    return c;
}

void printDBResponse(struct DBQueryResponse* response) {

    // bugs with printing full table can be fixed with some more time

    int neededColumnSizes[response->numberOfColumns];
    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        neededColumnSizes[i] = strlen(response->columns[i]);
    }
    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        for (unsigned int j = 0; j < response->numberOfRows; j++) {
            char* mem = getPtrToMemLoc(response, j, i);
            neededColumnSizes[i] = max(neededColumnSizes[i], strlen(mem));
        }
    }

    // first line
    printf("\n+");
    for (int i = 0; i < response->numberOfColumns; i++) {
        for (int j = 0; j < strlen(response->columns[i]); j++) {
            printf("-");
        }
        printf("---");
    }
    printf("+\n|");

    //// column line
    //for (unsigned int i = 0; i < response->numberOfColumns; i++) {
    //    printf(" %s ", response->columns[i]);
    //    int len = strlen(response->columns[i]);
    //    for (int j = len; j < neededColumnSizes[i]; j++) {
    //        printf(" ");
    //    }
    //    printf("|");
    //}
    //printf("\n+");

    // column line
    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        printf(" %s ", response->columns[i]);
    }
    //printf("\n+");

    //// under column line
    //for (int i = 0; i < response->numberOfColumns; i++) {
    //    for (int j = 0; j < neededColumnSizes[i]; j++) {
    //        printf("-");
    //    }
    //    printf("--");
    //}
    //printf("+\n");

    printf("\n+");
    for (int i = 0; i < response->numberOfColumns; i++) {
        for (int j = 0; j < strlen(response->columns[i]); j++) {
            printf("-");
        }
        printf("---");
    }
    printf("+\n");

    //// print data segment
    //for (unsigned int i = 0 ; i < response->numberOfRows; i++) {
    //    printf("|");
    //    for (unsigned int j = 0; j < response->numberOfColumns; j++) {
    //        char* val = getPtrToMemLoc(response, i, j);
    //        int len = strlen(val);
    //        printf(" %-*s ", neededColumnSizes[j], val);
    //        fflush(stdout);
    //        for (int k = len; k < neededColumnSizes[k]; k++) {
    //            printf(" ");
    //        }
    //        printf("|");
    //    }
    //    printf("|\n");
    //}
    
    for (unsigned int i = 0 ; i < response->numberOfRows; i++) {
        for (unsigned int j = 0; j < response->numberOfColumns; j++) {
            char* val = getPtrToMemLoc(response, i, j);
            printf(" %s ", val);
        }
        printf("\n");
    }

    //// button under data
    //printf("+");
    //for (int i = 0; i < response->numberOfColumns; i++) {
    //    for (int j = 0; j < neededColumnSizes[i]; j++) {
    //        printf("-");
    //    }
    //    printf("--");
    //}
    //printf("+\n");

    printf("+");
    for (int i = 0; i < response->numberOfColumns; i++) {
        for (int j = 0; j < strlen(response->columns[i]); j++) {
            printf("-");
        }
        printf("---");
    }
    printf("+\n");
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

    printf("Columns names:\n");
    for (unsigned int i = 0; i < response->numberOfColumns; i++) {
        printf("%s ", response->columns[i]);
    }
    printf("\n");

    for (unsigned int i = 0; i < response->numberOfRows; i++) {
        for (unsigned int j = 0; j < response->numberOfColumns; j++) {

            int offset = response->memoryTable[j] + i * response->memoryTableSize;
            printf("%d ", offset);
        }
        printf("\n");

        for (unsigned int j = 0; j < response->numberOfColumns; j++) {

            int offset = response->memoryTable[j] + i * response->memoryTableSize;
            printf("%s|", &response->memory[offset]);
        }
        printf("\n");
    }

    printf("Raw memory:\n");
    int size = response->memoryTableSize * response->numberOfRows;
    for (int i = 0; i < size; i++) {
        printf("%c ", response->memory[i]);
        if (i % 20 == 0) printf("\n");
    }
    printf("\n");
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
    //printTable(database->tables[0]);
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
            //fullPrintDBResponse(response);
            q += size + 1;
            set = 1;
        } else if (wordcmpBool("SELECT", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 7;
            int size = wordcpy(buf1, q);
            response = selectInstructionFreeRemalloc(response, buf1);
            printDBResponse(response);
            //fullPrintDBResponse(response);
            return;

        } else if (wordcmpBool("SORTEDBY", q)) {

            if (response == NULL) goto selectNotCalled;

            q += 9;
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

                response = whereEqualsFreeRemalloc(response, buf1, buf2);
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

                response = toAddFreeRemalloc(response, buf2, buf1);
            } else {
                printf("Malformed statment: mo matching ADD to my TO\n    %s\n", query);
                return;
            }

            return;
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

            return;
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

            return;

        } else {
            printf("Cannot parse the following %s\n", q);
            return;
        }
        // just memleak for now
    }

selectNotCalled:
    printf("FROM or TO instructions where never used what table am I using?\n");
    return;
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
            response->memory = malloc(size * sizeof(char));
            if (response->memory == NULL) {
                printf("Error malloc\n");
                return NULL;
            }

            memcpy(response->memory, table->memory, (response->numberOfRows * response->memoryTableSize) * sizeof(char));

            memcpy(response->memoryTable, table->memoryTable, MAX_TABLE_COLUMNS * sizeof(unsigned int));

            memcpy(response->typeTable, table->typeTable, MAX_TABLE_COLUMNS * sizeof(char));

            for (int i = 0; i < response->numberOfColumns; i++) {
                strcpy(response->columns[i], table->columnNames[i]);
            }

            for (unsigned int column = 0; column < table->numberOfColumns - 1; column++) {
                if (strcmp(table->columnNames[column], table->columnNames[column + 1]) == 1) {
                    strcpy(&response->groupedName, table->columnNames[column]);
                    response->grouped[response->groupedSize] = column;
                    response->groupedSize++;
                }
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

            response->memory = malloc((response->numberOfRows * response->numberOfColumns + 1) * sizeof(char));
            if (response->memory == NULL) {
                printf("Malloc error\n");
                return NULL;
            }

            for (unsigned int row = 0; row < src->numberOfRows; row++) {

                int memOffset = src->memoryTable[i] % src->memoryTableSize;

                int chars =  src->memoryTable[(i + 1) % src->numberOfColumns] - src->memoryTable[i];

                memcpy(&src->memory[row * src->memoryTableSize], &src->memory[src->memoryTable[memOffset * row]], chars * sizeof(char));
            }

            return response;

        } else if (i == src->numberOfColumns - 1) {

            printf("Unknown name of column: %s\n", column);
            return NULL;
        }
    }

    return NULL;
}

struct DBQueryResponse* badGlobalVar;
int badGlobalColumn;

int compareStrings(const void *a, const void *b) {
    int idxA = *(const int*)a;
    int idxB = *(const int*)b;
    int offset = badGlobalVar->memoryTable[badGlobalColumn];
    char* strA = &badGlobalVar->memory[offset + idxA * badGlobalVar->memoryTableSize];
    char* strB = &badGlobalVar->memory[offset + idxB * badGlobalVar->memoryTableSize];
    return strcmp(strA, strB);
}

int compareInts(const void *a, const void *b) {
    int idxA = *(const int*)a;
    int idxB = *(const int*)b;
    int offset = badGlobalVar->memoryTable[badGlobalColumn];
    int valA = atoi(&badGlobalVar->memory[offset + idxA * badGlobalVar->memoryTableSize]);
    int valB = atoi(&badGlobalVar->memory[offset + idxB * badGlobalVar->memoryTableSize]);
    return valA - valB;
}

struct DBQueryResponse* sortedByInstructionFreeRemalloc(struct DBQueryResponse* src, char *column) {
    if (!src || !column) return NULL;

    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (!response) {
        printf("malloc error building response table\n");
        return NULL;
    }

    int colIndex = -1;
    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        if (strcmp(column, src->columns[i]) == 0) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        printf("Unknown name of column: '%s'\n", column);
        free(response);
        return NULL;
    }

    char type = src->typeTable[colIndex];
    int offset = src->memoryTable[colIndex];

    int* rowIndices = malloc(src->numberOfRows * sizeof(int));
    if (!rowIndices) {
        printf("malloc error for sorting indices\n");
        free(response);
        return NULL;
    }

    for (int i = 0; i < src->numberOfRows; i++) {
        rowIndices[i] = i;  // = 0, 1, 2, 3 to size
    }

    badGlobalVar = src;
    badGlobalColumn = src->memoryTable[colIndex];

    if (type == 's') {
        qsort(rowIndices, src->numberOfRows, sizeof(int), &compareStrings);
    } else { 
        qsort(rowIndices, src->numberOfRows, sizeof(int), &compareInts);
    }

    response->numberOfColumns = src->numberOfColumns;
    response->numberOfRows = src->numberOfRows;
    response->memoryTableSize = src->memoryTableSize;

    for (int col = 0; col < src->numberOfColumns; col++) {
        strcpy(response->columns[col], src->columns[col]);
        response->memoryTable[col] = src->memoryTable[col];
    }

    response->memory = malloc(response->numberOfRows * response->memoryTableSize * sizeof(char));
    if (!response->memory) {
        printf("Error mallocing memory\n");
        free(rowIndices);
        free(response);
        return NULL;
    }

    for (int i = 0; i < src->numberOfRows; i++) {
        int destIndex = i * response->memoryTableSize;
        int srcIndex = rowIndices[i] * src->memoryTableSize;
        memcpy(&response->memory[destIndex], &src->memory[srcIndex], response->memoryTableSize * sizeof(char));
    }

    free(rowIndices);
    return response;
}

struct DBQueryResponse* whereEqualsFreeRemalloc(struct DBQueryResponse* src, char* column, char* value) {

    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (response == NULL) {
        printf("malloc error building response table\n");
        return NULL;
    }

    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        if (strcmp(column, src->columns[i]) == 0) {

            int offset = src->memoryTable[i];

            int stays[src->numberOfRows];
            int staysCount = 0;

            for (int row = 0; row < src->numberOfRows; row++) {
                char *rcvalue = &src->memory[offset + row * src->memoryTableSize];
                if (strcmp(rcvalue, value) == 0) {
                    stays[row] = 1;
                    staysCount++;
                }
                else stays[row] = 0;
            }

            if (staysCount == 0) {
                free(response);
                return NULL;
            }

            response->numberOfColumns = src->numberOfColumns;

            response->numberOfRows = staysCount;

            response->memoryTableSize = src->memoryTableSize;

            for (int column = 0; column < src->numberOfColumns; column++) {
                strcpy(response->columns[column], src->columns[column]);
                response->memoryTable[column] = src->memoryTable[column];
            }

            response->memory = malloc(response->numberOfRows * response->memoryTableSize * sizeof(char));
            if (response->memory == NULL) {
                printf("Error mallocing memory");
                return NULL;
            }

            int index = 0;
            for (int row = 0; row < src->numberOfRows; row++) {
                if (stays[row] == 1) {
                    int destIndex = index * response->memoryTableSize;
                    int srcIndex = row * response->memoryTableSize;
                    memcpy(&response->memory[destIndex], &src->memory[srcIndex], response->memoryTableSize * sizeof(char));
                    ++index;
                }
            }

            return response;

        } else if (i == src->numberOfColumns - 1) {

            printf("Unknown name of column: %s\n", column);
            return NULL;
        }
    }
    return NULL;
}

struct DBQueryResponse* toAddFreeRemalloc(struct DBQueryResponse* src, char* values, char* column) {
    return NULL;
}

struct DBQueryResponse* updateToFreeRemalloc(struct DBQueryResponse* src, char* column, char* updatedValue) {
    // replaces FROM attaches a ponter to each line that when the query is finsihed updates the parent table

    return NULL;
}

int search(char* haystack, char* needle) {

    int frontWildcard = *needle == '*';
    int backwildcard = 0;

    char* n = needle;
    while(*(n+1) != '\0') ++n;

    backwildcard = *n == '*';

    if (frontWildcard == 0 && backwildcard == 0) return strstr(haystack, needle) != NULL;

    char buf[strlen(needle) - frontWildcard - backwildcard + 1];

    n = needle;
    int index = 0;
    while(*n != '\0') {
        if (*n != '*') {
            buf[index] = *n;
            index++;
        }
        ++n;
    }
    buf[index] = '\0';

    char* found = strstr(haystack, buf);
    if (found == NULL) return 0;

    if (frontWildcard == 0) {
        if (found == haystack) return 0;
        else return 1;
    } else { //back wild card
        found += strlen(needle) + 1;
        if (*found != '\0') {
            return 0;
        } else {
            return 1;
        }

    }
}

struct DBQueryResponse* searchForFreeRemalloc(struct DBQueryResponse* src, char* column, char* value) {

    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (response == NULL) {
        printf("malloc error building response table\n");
        return NULL;
    }

    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        if (strcmp(column, src->columns[i]) == 0) {

            int offset = src->memoryTable[i];

            int stays[src->numberOfRows];
            int staysCount = 0;

            for (int row = 0; row < src->numberOfRows; row++) {
                char *rcvalue = &src->memory[offset + row * src->memoryTableSize];
                int searcResult = search(rcvalue, value);
                if (searcResult == 1) {
                    stays[row] = 1;
                    staysCount++;
                } else stays[row] = 0;
            }

            if (staysCount == 0) {
                free(response);
                return NULL;
            }

            response->numberOfColumns = src->numberOfColumns;

            response->numberOfRows = staysCount;

            response->memoryTableSize = src->memoryTableSize;

            for (int column = 0; column < src->numberOfColumns; column++) {
                strcpy(response->columns[column], src->columns[column]);
                response->memoryTable[column] = src->memoryTable[column];
            }

            response->memory = malloc(response->numberOfRows * response->memoryTableSize * sizeof(char));
            if (response->memory == NULL) {
                printf("Error mallocing memory");
                return NULL;
            }

            int index = 0;
            for (int row = 0; row < src->numberOfRows; row++) {
                if (stays[row] == 1) {
                    int destIndex = index * response->memoryTableSize;
                    int srcIndex = row * response->memoryTableSize;
                    memcpy(&response->memory[destIndex], &src->memory[srcIndex], response->memoryTableSize * sizeof(char));
                    ++index;
                }
            }

            return response;

        } else if (i == src->numberOfColumns - 1) {

            printf("Unknown name of column: %s\n", column);
            return NULL;
        }
    }
    return NULL;
    return NULL;
}

struct DBQueryResponse* averageElements(struct DBQueryResponse* src, char* column) {
    struct DBQueryResponse* response = malloc(sizeof(struct DBQueryResponse));
    if (response == NULL) {
        printf("malloc error building response table\n");
        return NULL;
    }

    int colIndex = -1;
    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        if (strcmp(column, src->columns[i]) == 0) {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1) {
        
    }

    if (colIndex == -1) {
        printf("Unknown name of column: '%s'\n", column);
        free(response);
        return NULL;
    }


    for (unsigned int i = 0; i < src->numberOfColumns; i++) {
        if (strcmp(column, src->columns[i]) == 0) {

            if (src->typeTable[i] == 's') {
                printf("Attempt to average a non integer column");
                return NULL;
            }

            int memMapSize = src->memoryTableSize;
            int memMapOffset = src->memoryTable[i];

            int sum = 0;
            for (unsigned int i = 0; i < src->numberOfRows; i++) {
                sum += atoi(&src->memory[getOff(i, memMapSize, memMapOffset)]);
            }

            printf("Average: %f\n", 1.00 * sum / src->numberOfColumns);
        }
    }
    return NULL;
}
