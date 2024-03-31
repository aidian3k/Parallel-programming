CC = gcc

SRC_DIR = src
INCLUDES_DIR = includes
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(wildcard $(SRC_DIR)/$(INCLUDES_DIR)/*.h)

TARGET = c4

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

without_concurency:
	$(CC) -o without_concurency without_concurency/without_concurency.c

gen:
	$(CC) -o gen generator/gen.c

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

