CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -fsanitize=undefined

BUILD_DIR=build
SRC_DIR=src

EXECUTABLE=$(BUILD_DIR)/a.out
EXEWFILES=$(EXECUTABLE) students.csv courses.csv grades.csv

OBJ_FILES=$(BUILD_DIR)/main.o $(BUILD_DIR)/db.o $(BUILD_DIR)/table.o

$(EXECUTABLE): $(OBJ_FILES) | $(BUILD_DIR)
	$(CC) $(OBJ_FILES) -o $(EXECUTABLE)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c -c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/db.o: $(SRC_DIR)/db.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/db.c -c -o $(BUILD_DIR)/db.o

$(BUILD_DIR)/table.o: $(SRC_DIR)/table.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/table.c -c -o $(BUILD_DIR)/table.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)/*

run:
	./$(EXECUTABLE) students.csv courses.csv grades.csv

check:
	cppcheck $(SRC_DIR)/main.c $(SRC_DIR)/db.c $(SRC_DIR)/table.c --check-level=exhaustive

unsafe:
	$(CC) -g $(SRC_DIR)/main.c -c -o $(BUILD_DIR)/main.o
	$(CC) -g $(SRC_DIR)/db.c -c -o $(BUILD_DIR)/db.o
	$(CC) -g $(SRC_DIR)/table.c -c -o $(BUILD_DIR)/table.o
	$(CC) $(OBJ_FILES) -o $(EXECUTABLE)
test:
	@echo '1) output original list of all students'
	./$(EXEWFILES) "FROM students SELECT *"

	@echo '2) output list of all students sorted by name (in alphabetic order and vice versa'
	./$(EXEWFILES) "FROM students SORTEDBY firstname SELECT *"

	@echo '3) output original list of all courses'
	./$(EXEWFILES) "FROM courses SELECT *"

	@echo '4) output list of all courses for the specific semester (specified by the user)'
	./$(EXEWFILES) "FROM courses WHERE semester EQUALS winter SELECT *"

	@echo '5) output list of all courses for the specific semester sorted by course name (in alphabetic order and vice versa)'
	./$(EXEWFILES) "FROM courses WHERE semester EQUALS winter SORTEDBY name SELECT *"

	@echo '6) add new student, add new course, add a grade for the course (user enters students last name, course code, and grade);'
	./$(EXEWFILES) "TO students ADD {6,hamad,jehad,123123123,jehad@hamad.com}"

	@echo '7) update student info (user enters student id);'
	./$(EXEWFILES) "FROM students WHERE id EQUALS 2 UPDATE firstname TO 'alex'"

	@echo '8) search for course by name (successful result: full course info);'
	./$(EXEWFILES) "FROM courses SEARCH name FOR CS2020 SELECT *"

	@echo '9) search for course by code (successful result: full course info);'
	./$(EXEWFILES) "FROM courses SEARCH name FOR CS2910 SELECT *"


	@echo '10) search for student info by last name (successful result: full student info);'
	./$(EXEWFILES) "FROM students SEARCH lastname FOR Smith SELECT *"

	@echo '11) search for student info by last phone number (successful result full student info);'
	./$(EXEWFILES) "FROM students SEARCH phonenumber FOR *2 SELECT *"

	@echo '12) output list of all courses taken by student (user enters student last name) and grades'
	./$(EXEWFILES) "FROM courses WHERE lastname EQUALS Smith SELECT *"


	@echo '13) output list of all courses taken by student (user enters student last name) and grades'
	./$(EXEWFILES) "FROM grades WHERE lastname EQUALS Smith SELECT *"


	@echo '14) calculate average grade for a specific student (user enters student last name)'
	./$(EXEWFILES) "FROM grades WHERE lastname EQUALS Smith AVERAGE grades"

	@echo '15) calculate the average grade for a specific student for a specific term (user enters students last name and term)'
	./$(EXEWFILES) "FROM grades WHERE lastname EQUALS Smith WHERE semester EQUALS winter AVERAGE grades"

	@echo '16) calculate average grade for specific courses (user enters course name).'
	./$(EXEWFILES) "FROM grades WHERE name EQUALS CS2910 AVERAGE grades"
