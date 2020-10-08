SRC_FILES = main.c
CC_FLAGS = -Wall -Wextra -g -std=c11 -o
CC = gcc

TARGET = vmc
BIN_DIR = ~/bin

all:
	$(CC) $(SRC_FILES) $(CC_FLAGS) $(TARGET)

clean:
	rm $(TARGET) && cd $(BIN_DIR) && rm $(TARGET)

release:
	$(CC) $(SRC_FILES) $(CC_FLAGS) $(TARGET) && cp $(TARGET) $(BIN_DIR)