# Directory for configuration files
CONFIGDIR=Config

# Default config file
defaultConfigFile=c1.config


# Compiler to use
CC=g++

# Flags to pass to the compiler
CFLAGS=-c -Wall
# -c: compile into object files
# -Wall: enable all warnings

# Include directory
CPPFLAGS := -Iinclude

# Debug flags
DEBUG_FLAGS=-DDEBUG -g -O0
# -DDEBUG: Define DEBUG for compiled files
# -g: enable debug symbols
# -O0: disable optimization

# Directory structure
SRC_DIR=./source
INC_DIR=./include
OBJ_DIR=./output

# Source files
SRCS=$(wildcard $(SRC_DIR)/*.cpp)

# Header files
INCS=$(wildcard $(INC_DIR)/*.h)

# Object files
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Name of the executable to generate
EXEC=netsim

# Name of the debug executable to generate
EXEC_D=netsim_d

# Default target
all: clean_if_debug_exists $(EXEC)

# Rule to clean if debug executable exists
.PHONY: clean_if_debug_exists
clean_if_debug_exists:
	@if [ -f $(EXEC_D) ]; then $(MAKE) clean; fi

# Rule to build the regular executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

# Rule to build object files without debug flags
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

# Rule to build the debug executable
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean_if_release_exists $(EXEC_D)

# Rule to clean if release executable exists
.PHONY: clean_if_release_exists
clean_if_release_exists:
	@if [ -f $(EXEC) ]; then $(MAKE) clean; fi

# Rule to build object files with debug flags
$(EXEC_D): $(OBJS)
	$(CC) $(OBJS) -o $@
	./$(EXEC_D) $(CONFIGDIR)/$(defaultConfigFile)

# Clean up all object files and the executables
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC) $(EXEC_D)

.PHONY: debug

# Rule to run the executable with the default config file
run: $(EXEC)
	./$(EXEC) $(CONFIGDIR)/$(defaultConfigFile)

