# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

# Directories
SRC_DIR = src
TEST_DIR = test
INCLUDE_DIR = include
BUILD_DIR = build

# Source Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Object Files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_OBJS = $(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)

# Executables
TEST_EXECS = $(TEST_OBJS:$(BUILD_DIR)/%.o=$(BUILD_DIR)/%)

# Create build directory if not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to compile source files into objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile test files into objects
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build test executables
$(BUILD_DIR)/%: $(BUILD_DIR)/%.o $(OBJS)
	$(CC) $^ -o $@

# Run all test executables
.PHONY: test
test: $(TEST_EXECS)
	@for exec in $(TEST_EXECS); do \
		echo "Running $$exec..."; \
		./$$exec; \
	done

# Clean build directory
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
