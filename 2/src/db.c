#include "db.h"
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

    parseDataIntoTable(studentTable, studentFilename);

    dataBase->studentTable = studentTable;


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

    parseDataIntoTable(courseTable, courseFilename);

    dataBase->courseTable = courseTable;

    Table* gradeTable = malloc(sizeof(Table));
    if (gradeTable == NULL) {
        printf("malloc error\n");
        return NULL;
    }

    char gradeTableName[MAX_TABLE_NAME_SIZE + 1] = "courses";
    char gradeTableTypeTable[MAX_TABLE_COLUMNS] = {'s', 'u', 'u'};
    unsigned int gradeTableMemoryTable[MAX_TABLE_COLUMNS] = {30, TABLE_INT_SIZE, TABLE_INT_SIZE};
    char gradeTableColumns[MAX_TABLE_COLUMN_NAME_SIZE + 1][MAX_TABLE_COLUMNS] = {"name", "semester", "courseid"};
    int gradeTableNumberOfColumns = 3;

    buildTable(gradeTable, gradeTableName, gradeTableTypeTable, gradeTableMemoryTable, gradeTableColumns, gradeTableNumberOfColumns);

    parseDataIntoTable(gradeTable, gradeFilename);

    dataBase->gradeTable = gradeTable;

    return dataBase;
}
