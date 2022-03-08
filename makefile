TARGET := trabalhocg
LINK := -lGL -lGLU -lglut
CFLAGS := -g -std=c++17
INCLUDE_PATHS := -Iinclude -Ilib
CXX := g++
IN_FILE := levels/arena_teste.svg

all: $(TARGET)

SRC = src/main.cpp \
	  src/Arena.cpp \
	  src/Axes.cpp \
	  src/Box.cpp \
	  src/Camera.cpp \
	  src/LightSource.cpp \
	  src/Plane.cpp \
	  src/Player.cpp \
	  src/Objects/Box.cpp \
	  src/Objects/Object.cpp \
	  src/Objects/Plane.cpp \
	  lib/tinyxml2.cpp

HEADERS = include/Arena.h \
		  include/Axes.h \
		  include/Box.h \
		  include/Camera.h \
		  include/LightSource.h \
		  include/Plane.h \
		  include/Player.h \
		  include/VectorMath.h \
		  include/Objects/Box.h \
	  	  include/Objects/Object.h \
	  	  include/Objects/Plane.h \
		  lib/tinyxml2.h

OBJECTS := $(SRC:%.cpp=build/%.o)

build/src/%.o: src/%.cpp $(HEADERS)
	@mkdir -p build/src/
	$(CXX) $(INCLUDE_PATHS) $(CFLAGS) -c -o $@ $<

build/src/Objects/%.o: src/Objects/%.cpp $(HEADERS)
	@mkdir -p build/src/Objects
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
