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

Student* parseStudentEntry(Student* student, char* line) {
    if (student == NULL) return NULL;
    if (line == NULL) return NULL;

    char buf[128];

    // parse id
    helperParseStringToSep(&buf[0], &line);
    student->id = atoi(buf);

    // parse lastname
    helperParseStringToSep(student->lastname, &line);

    // parse firstname
    helperParseStringToSep(student->firstName, &line);

    // parse phonenumber
    helperParseStringToSep(student->phoneAddress, &line);

    // parse email
    helperParseStringToSep(student->email, &line);

    return student;
}

// this mallocs its own memory
Student** readStudentTable(unsigned int* studentsSize, const char* studentFilename) {

    char* lines[MAX_FILE_LENGTH];
    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        // rewrite as a block
        lines[i] = malloc(sizeof(char) * MAX_FILE_LINE_LENGTH);
    }

    // read lines to mem
    helperGetLines(studentsSize, studentFilename, lines);


    const int numStudents = *studentsSize;

    Student** studentTable = malloc(sizeof(Student*) * (numStudents + 1));
    if (studentTable == NULL) {
        printf("Unable to malloc\n");
        for (int i = 0; i < MAX_FILE_LENGTH; i++) {
            free(lines[i]);
        }
        return NULL;
    }

    for (int i = 0; i < numStudents; i++) {
        studentTable[i] = malloc(sizeof(Student));
        if (parseStudentEntry(studentTable[i], lines[i]) == NULL) {
            printf("Malformed student entry line %d\n", i);
        }
    }

    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        free(lines[i]);
    }
    return studentTable;
}

Course* parseCourseEntry(Course* course, char* line) {
    if (course == NULL) return NULL;
    if (line == NULL) return NULL;

    helperParseStringToSep(course->name, &line);

    char buf[128];
    helperParseStringToSep(buf, &line);
    course->semster = (strcmp(buf, "winter") == 0 ? WINTER : FALL);

    helperParseStringToSep(buf, &line);
    course->courseID = atoi(buf);

    return course;
}

// this mallocs its own memory
Course** readCourseTable(unsigned int* courseSize, const char* courseFilename) {

    char* lines[MAX_FILE_LENGTH];
    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        // rewrite as a block
        lines[i] = malloc(sizeof(char) * MAX_FILE_LINE_LENGTH);
    }

    // read lines to mem
    helperGetLines(courseSize, courseFilename, lines);

    const int numCourses = *courseSize;

    Course** courseTable = malloc(sizeof(Course*) * (numCourses + 1));
    if (courseTable == NULL) {
        printf("Unable to malloc\n");
        for (int i = 0; i < MAX_FILE_LENGTH; i++) {
            free(lines[i]);
        }
        return NULL;
    }

    for (int i = 0; i < numCourses; i++) {
        courseTable[i] = malloc(sizeof(Course));
        if (parseCourseEntry(courseTable[i], lines[i]) == NULL) {
            printf("Malformed course entry line %d\n", i);
        }
    }

    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        free(lines[i]);
    }
    return courseTable;
}


Grade* parseGradeEntry(Grade* grade, char* line) {
    if (grade == NULL) return NULL;
    if (line == NULL) return NULL;

    char buf[128];
    helperParseStringToSep(&buf[0], &line);
    grade->gradeID = atoi(buf);

    helperParseStringToSep(grade->lastname, &line);

    helperParseStringToSep(grade->firstName, &line);

    int index = 0;
    while (line[0] != '\0') {
        helperParseStringToSep(buf, &line);
        if (buf[0] == 'n' && buf[1] == 'a') {
            buf[0] = '-';
            buf[1] = '1';
            buf[2] = '\0';
        }
        grade->grades[index] = atoi(buf);
        index++;
    }
    grade->gradeCount = index;

    return grade;
}

// this mallocs its own memory
Grade** readGradesTable(unsigned int* gradesSize, const char* gradeFilename) {

    char* lines[MAX_FILE_LENGTH];
    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        // rewrite as a block
        lines[i] = malloc(sizeof(char) * MAX_FILE_LINE_LENGTH);
    }

    // read lines to mem
    helperGetLines(gradesSize, gradeFilename, lines);

    const int numGrades = *gradesSize;

    Grade** gradeTable = malloc(sizeof(Grade*) * (numGrades + 1));
    if (gradeTable == NULL) {
        printf("Unable to malloc\n");
        for (int i = 0; i < MAX_FILE_LENGTH; i++) {
            free(lines[i]);
        }
        return NULL;
    }

    for (int i = 0; i < numGrades; i++) {
        gradeTable[i] = malloc(sizeof(Course));
        if (parseGradeEntry(gradeTable[i], lines[i]) == NULL) {
            printf("Malformed grade entry line %d\n", i);
        }
    }

    for (int i = 0; i < MAX_FILE_LENGTH; i++) {
        free(lines[i]);
    }
    return gradeTable;
}

struct DB* buildDB(struct DB* dataBase, const char* studentFilename, const char* courseFilename, const char* gradeFilename) {
    dataBase->studentTable = readStudentTable(&dataBase->studentTableSize, studentFilename);
    dataBase->courseTable = readCourseTable(&dataBase->courseTableSize, courseFilename);
    dataBase->gradesTable = readGradesTable(&dataBase->gradesTableSize, gradeFilename);

    return dataBase;
}
