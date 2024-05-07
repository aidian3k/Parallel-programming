CC = mpicc

SRC_DIR = src
INCLUDES_DIR = includes
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(wildcard $(SRC_DIR)/$(INCLUDES_DIR)/*.h)

TARGET = c6

nwd:
	$(CC) -o nwd $(SRC_DIR)/main-nwd.c

pipe:
	$(CC) -o pipe $(SRC_DIR)/main-pipe.c

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)