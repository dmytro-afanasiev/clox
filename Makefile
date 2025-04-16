CFLAGS := -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter
MODE := debug
TARGET := clox
SOURCE_DIR := .

ifeq ($(MODE),debug)
	CFLAGS += -O0 -DDEBUG -g
	BUILD_DIR := build/debug
else
	CFLAGS += -O3 -flto
	BUILD_DIR := build/release
endif


HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS := $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))


clean:
	rm -rf build/


run: $(BUILD_DIR)/$(TARGET)
	@$^


$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@



$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@



.PHONY: clean run
.DEFAULT_GOAL := $(BUILD_DIR)/$(TARGET)
