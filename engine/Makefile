CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 \
           -Iinclude \
           -Ilibs/boost_1_86_0 \
           -I/opt/homebrew/opt/websocketpp/include \
           -pthread

LDFLAGS = -Llibs/boost_1_86_0/install/lib -Wl,-rpath,libs/boost_1_86_0/install/lib -lboost_system -lpthread

SRC_DIR = src
OBJ_DIR = build

TARGET_MAIN = main

SRC_MAIN = $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_MAIN = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_MAIN))

all: $(TARGET_MAIN)

$(TARGET_MAIN): $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)/$(dir $*)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-main: $(TARGET_MAIN)
	./$(TARGET_MAIN)

clean:
	rm -rf $(OBJ_DIR) $(TARGET_MAIN)
