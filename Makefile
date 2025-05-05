PROJECT = matex
CC = gcc
CXX = g++
CFLAGS = -O2 -Wall -Wextra -I./inc
CXXFLAGS = -O2 -Wall -Wextra -I./inc -std=c++17
LDFLAGS = 
LIBS = 

SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

TARGETS = $(BUILD_DIR)/matex

C_SRCS = $(wildcard $(SRC_DIR)/*.c)
CPP_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
HDRS = $(wildcard $(INC_DIR)/*.h $(INC_DIR)/*.hpp)

C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))
CPP_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))
OBJS = $(C_OBJS) $(CPP_OBJS)

Q = @
E = @echo
RM ?= @rm -f

all: prepare $(TARGETS)

prepare:
	@mkdir -p $(BUILD_DIR)


$(BUILD_DIR)/matex: $(BUILD_DIR)/main.o $(OBJS)
	$(E) "  LINK    " $@
	$(Q) $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS)
	$(E) "  CC      " $@
	$(Q) $(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS)
	$(E) "  CCX      " $@
	$(Q) $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(E) "  CLEAN"
	$(Q) $(RM) -r $(BUILD_DIR)

.PHONY: all prepare clean
