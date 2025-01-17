#include <string.h>
#define studentCSV students.csv
#define courseCSV course.csv
#define gradesCSV grades.csv

#define MAX_FILE_LENGTH 256
#define MAX_FILE_LINE_LENGTH 1024

typedef struct {
    unsigned int id;
    char lastname[200];
    char firstName[50];
    char phoneNumber[16];      // tmp break this apart later
    char email[320];
} Student;

enum SEMESTERS {
    FALL,                       // assuming 2 semssters
    WINTER
};

typedef struct {
    char name[30];             // split to department and code
    enum SEMESTERS semster;
    int courseID;
} Course;

typedef struct {
    unsigned int gradeID;
    char lastname[200];
    char firstName[50];
    int gradeCount;
    int grades[256];        // -1 as na
} Grade;

struct DB {
    Student** studentTable;
    unsigned int studentTableSize;
    Course** courseTable;
    unsigned int courseTableSize;
    Grade** gradesTable;
    unsigned int gradesTableSize;
};

// these functions return malloced memory
Student** readStudentTable(unsigned int* studentsSize, const char* studnetFilename);
Course** readCourseTable(unsigned int* courseSize, const char* courseFilename);
Grade** readGradesTable(unsigned int* gradesSize, const char* gradeFilename);

struct DB* buildDB(struct DB* dataBase, const char* studentFilename, const char* courseFilename, const char* gradeFilename);
