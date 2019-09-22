# Constants

## Program
PROGRAM ?= gamebook-engine

## Compiler and linker
CXX ?= g++
CXX_FLAGS ?= -std=c++14 -Wall -pedantic -Wextra -Wno-long-long -O0 -g -ggdb
LDD_FLAGS ?= -lmenu -lncurses -ltinfo

## System commands
MKDIR ?= mkdir
MKDIR_FLAGS ?= -p
RM ?= rm
RM_FLAGS ?= -r

## Project directories
BUILD_DIR ?= build
DOCS_DIR ?= doc
SRC_DIR ?= src

## Other files
SAVE_FILE ?= .save

## Header files
HEADER_FILES ?= $(SRC_DIR)/Creature.hpp $(SRC_DIR)/Entity.hpp \
	$(SRC_DIR)/Inventory.hpp $(SRC_DIR)/io.hpp $(SRC_DIR)/Item.hpp \
	$(SRC_DIR)/Location.hpp	$(SRC_DIR)/Option.hpp $(SRC_DIR)/Player.hpp \
	$(SRC_DIR)/util.hpp $(SRC_DIR)/World.hpp

## Object files
OBJECT_FILES ?= ${BUILD_DIR}/Creature.o ${BUILD_DIR}/Entity.o \
	${BUILD_DIR}/Inventory.o ${BUILD_DIR}/io.o ${BUILD_DIR}/Item.o \
	${BUILD_DIR}/Location.o ${BUILD_DIR}/main.o ${BUILD_DIR}/Option.o \
	${BUILD_DIR}/Player.o ${BUILD_DIR}/util.o ${BUILD_DIR}/World.o


# Make commands

.PHONY: all
all: compile doc

.PHONY: compile
compile: $(PROGRAM)

.PHONY: run
run:
	./$(PROGRAM)

.PHONY: clean
clean:
	$(RM) $(RM_FLAGS) $(PROGRAM) $(BUILD_DIR) $(DOCS_DIR) ${SAVE_FILE}

.PHONY: doc
doc: $(SRC_DIR)/main.cpp $(HEADER_FILES)
	doxygen Doxyfile


# Rules

## Build directory
$(BUILD_DIR):
	$(MKDIR) $(MKDIR_FLAGS) $(BUILD_DIR)

## Program executable
$(PROGRAM): $(OBJECT_FILES) | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $^ $(LDD_FLAGS) -o $@

## Modules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) -c $< -o $@ $(LDD_FLAGS)


# Dependencies (check with `g++ -MM src/*.cpp`)

$(BUILD_DIR)/Creature.o: $(SRC_DIR)/Creature.cpp $(SRC_DIR)/Creature.hpp \
	$(SRC_DIR)/Entity.hpp $(SRC_DIR)/Inventory.hpp $(SRC_DIR)/Item.hpp \
	$(SRC_DIR)/util.hpp
$(BUILD_DIR)/Entity.o: $(SRC_DIR)/Entity.cpp $(SRC_DIR)/Entity.hpp \
	$(SRC_DIR)/Inventory.hpp $(SRC_DIR)/Item.hpp
$(BUILD_DIR)/Inventory.o: $(SRC_DIR)/Inventory.cpp $(SRC_DIR)/Inventory.hpp \
	$(SRC_DIR)/Item.hpp $(SRC_DIR)/io.hpp
$(BUILD_DIR)/io.o: $(SRC_DIR)/io.cpp $(SRC_DIR)/io.hpp
$(BUILD_DIR)/Item.o: $(SRC_DIR)/Item.cpp $(SRC_DIR)/Item.hpp
$(BUILD_DIR)/Location.o: $(SRC_DIR)/Location.cpp $(SRC_DIR)/Location.hpp \
	$(SRC_DIR)/Creature.hpp $(SRC_DIR)/Entity.hpp $(SRC_DIR)/Inventory.hpp \
	$(SRC_DIR)/Item.hpp $(SRC_DIR)/Option.hpp
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/Creature.hpp \
	$(SRC_DIR)/Entity.hpp $(SRC_DIR)/Inventory.hpp $(SRC_DIR)/Item.hpp \
	$(SRC_DIR)/Location.hpp $(SRC_DIR)/Option.hpp $(SRC_DIR)/Player.hpp \
	$(SRC_DIR)/World.hpp $(SRC_DIR)/io.hpp
$(BUILD_DIR)/Option.o: $(SRC_DIR)/Option.cpp $(SRC_DIR)/Option.hpp
$(BUILD_DIR)/Player.o: $(SRC_DIR)/Player.cpp $(SRC_DIR)/Player.hpp \
	$(SRC_DIR)/Entity.hpp $(SRC_DIR)/Inventory.hpp $(SRC_DIR)/Item.hpp \
	$(SRC_DIR)/io.hpp $(SRC_DIR)/util.hpp
$(BUILD_DIR)/util.o: $(SRC_DIR)/util.cpp $(SRC_DIR)/util.hpp
$(BUILD_DIR)/World.o: $(SRC_DIR)/World.cpp $(SRC_DIR)/World.hpp \
	$(SRC_DIR)/Location.hpp $(SRC_DIR)/Creature.hpp $(SRC_DIR)/Entity.hpp \
	$(SRC_DIR)/Inventory.hpp $(SRC_DIR)/Item.hpp $(SRC_DIR)/Option.hpp \
	$(SRC_DIR)/Player.hpp $(SRC_DIR)/io.hpp $(SRC_DIR)/util.hpp
