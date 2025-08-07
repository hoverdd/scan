INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/argc.c $(SRC_DIR)/banner.c $(SRC_DIR)/scanner.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

CC = gcc
CFLAGS = -Wall -Wextra -O2 -I$(INCLUDE_DIR)

all: scan

scan: $(OBJS) 
	$(CC) $(CFLAGS) -o scan $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


clean:
	rm -rf $(BUILD_DIR) scan