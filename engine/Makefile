CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = build

TARGET_MAIN = main
TARGET_TEST = testapp

SRC_MAIN = $(shell find $(SRC_DIR) -name '*.cpp')
SRC_TEST = $(shell find $(TEST_DIR) -name '*.cpp')

OBJ_MAIN = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_MAIN))
OBJ_TEST = $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_TEST))

all: $(TARGET_MAIN)

$(TARGET_MAIN): $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TARGET_TEST): $(OBJ_TEST) $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)/$(dir $*)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)/$(dir $*)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-main: $(TARGET_MAIN)
	./$(TARGET_MAIN)

run-test: $(TARGET_TEST)
	./$(TARGET_TEST)

clean:
	rm -rf $(OBJ_DIR) $(TARGET_MAIN) $(TARGET_TEST)
