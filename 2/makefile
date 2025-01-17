CC=gcc
CFLAGS=-Wall -Werror -Wextra -g

BUILD_DIR=build
SRC_DIR=src

EXECUTABLE=$(BUILD_DIR)/a.out

OBJ_FILES=$(BUILD_DIR)/main.o $(BUILD_DIR)/db.o

$(EXECUTABLE): $(OBJ_FILES) | $(BUILD_DIR)
	$(CC) $(OBJ_FILES) -o $(EXECUTABLE)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c -c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/db.o: $(SRC_DIR)/db.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/db.c -c -o $(BUILD_DIR)/db.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)/*

run:
	./$(EXECUTABLE) students.csv courses.csv grades.csv

check:
	cppcheck $(SRC_DIR)/main.c $(SRC_DIR)/db.c --check-level=exhaustive

unsafe:
	$(CC) -g $(SRC_DIR)/main.c -c -o $(BUILD_DIR)/main.o
	$(CC) -g $(SRC_DIR)/db.c -c -o $(BUILD_DIR)/db.o
	$(CC) $(OBJ_FILES) -o $(EXECUTABLE)
