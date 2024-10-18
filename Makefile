CXX = g++
CXXFLAGS = -std=c++14 -O3
SOURCES = main.cpp board.cpp pieces.cpp 
HEADERS = board.hpp pieces.hpp
BUILD_DIR = build/
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET = chess

all: $(BUILD_DIR) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.o: $(BUILD_DIR)/board.o
$(BUILD_DIR)/board.o: $(BUILD_DIR)/pieces.o

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
