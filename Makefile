INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/cli.c $(SRC_DIR)/banner.c $(SRC_DIR)/scanner.c $(SRC_DIR)/config.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CC = gcc
CFLAGS = -Wall -Wextra -O2 -I$(INCLUDE_DIR)

all: scan

scan: $(OBJS) 
	$(CC) $(CFLAGS) -pthread -o scan $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -pthread -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: test_config

test_config: tests/test_config.c src/config.c 
	$(CC) -o test_config tests/test_config.c src/config.c \
		-Iinclude \
		-I$(shell brew --prefix check)/include \
		-L$(shell brew --prefix check)/lib \
		-lcheck

run_tests: test
	./test_config


clean:
	rm -rf $(BUILD_DIR) scan test_config