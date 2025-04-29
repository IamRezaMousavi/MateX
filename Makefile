PROJECT = MateX
CC = gcc
CXX = g++
CFLAGS = -O2 -Wall -Wextra -I./inc
CXXFLAGS = -O2 -Wall -Wextra -I./inc -std=c++17
LDFLAGS = 
LIBS = -lm -lbsd -lcrypt

SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
BIN_DIR = bin

TARGETS = $(BIN_DIR)/matex_server $(BIN_DIR)/matex_addplayer $(BIN_DIR)/makerank

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
	@mkdir -p $(BIN_DIR)


$(BIN_DIR)/matex_server: $(BUILD_DIR)/ficsmain.o $(filter-out $(BUILD_DIR)/fics_addplayer.o $(BUILD_DIR)/makerank.o, $(OBJS))
	$(E) "  LINK    " $@
	$(Q) $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

$(BIN_DIR)/matex_addplayer: $(BUILD_DIR)/fics_addplayer.o $(filter-out $(BUILD_DIR)/ficsmain.o $(BUILD_DIR)/makerank.o, $(OBJS))
	$(E) "  LINK    " $@
	$(Q) $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

$(BIN_DIR)/makerank: $(BUILD_DIR)/makerank.o $(filter-out $(BUILD_DIR)/ficsmain.o $(BUILD_DIR)/fics_addplayer.o, $(OBJS))
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
	$(RM) -r $(BUILD_DIR) $(BIN_DIR)

.PHONY: all prepare clean
