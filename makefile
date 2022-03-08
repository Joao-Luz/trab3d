TARGET := trabalhocg
LINK := -lGL -lGLU -lglut
CFLAGS := -g -std=c++17
INCLUDE_PATHS := -Iinclude -Ilib
CXX := g++
IN_FILE := levels/arena_teste.svg

all: $(TARGET)

SRC = src/main.cpp src/Camera.cpp src/Player.cpp src/Box.cpp src/Axes.cpp src/Plane.cpp src/Arena.cpp src/LightSource.cpp lib/tinyxml2.cpp
HEADERS = include/Camera.h include/VectorMath.h include/Player.h include/Box.h include/Axes.h include/Plane.h include/Arena.h include/LightSource.h lib/tinyxml2.h

OBJECTS := $(SRC:%.cpp=build/%.o)

build/src/%.o: src/%.cpp $(HEADERS)
	@mkdir -p build/src/
	$(CXX) $(INCLUDE_PATHS) $(CFLAGS) -c -o $@ $<

build/lib/%.o: lib/%.cpp $(HEADERS)
	@mkdir -p build/lib/
	$(CXX) $(INCLUDE_PATHS) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS)
	$(CXX) $(INCLUDE_PATHS) $(OBJECTS) $(LINK) -o $@

clean:
	-rm -f -r build
	-rm -f *.o
	-rm -f $(TARGET)

run: $(TARGET)
	@./$(TARGET) $(IN_FILE)
