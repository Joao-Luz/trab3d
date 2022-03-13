TARGET := trabalhocg
LINK := -lGL -lGLU -lglut
CFLAGS := -g -std=c++17
INCLUDE_PATHS := -Iinclude -Ilib
CXX := g++
IN_FILE := levels/arena_teste.svg

all: $(TARGET)

SRC = src/main.cpp \
	  src/Animation.cpp \
	  src/Arena.cpp \
	  src/Game.cpp \
	  src/Textures.cpp \
	  src/Objects/Axes.cpp \
	  src/Objects/Box.cpp \
	  src/Objects/Camera.cpp \
	  src/Objects/Character.cpp \
	  src/Objects/Enemy.cpp \
	  src/Objects/LightSource.cpp \
	  src/Objects/Object.cpp \
	  src/Objects/Plane.cpp \
	  src/Objects/Player.cpp \
	  src/Objects/Shot.cpp \
	  src/Objects/Text.cpp \
	  lib/tinyxml2.cpp

HEADERS = include/Animation.h \
		  include/Arena.h \
		  include/Game.h \
		  include/Textures.h \
		  include/VectorMath.h \
		  include/Objects/Axes.h \
		  include/Objects/Box.h \
		  include/Objects/Camera.h \
		  include/Objects/Character.h \
		  include/Objects/Enemy.h \
		  include/Objects/LightSource.h \
	  	  include/Objects/Object.h \
	  	  include/Objects/Plane.h \
		  include/Objects/Player.h \
		  include/Objects/Shot.h \
		  include/Objects/Text.h \
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

debug: $(TARGET)
	@valgrind ./$(TARGET) $(IN_FILE)

run: $(TARGET)
	@./$(TARGET) $(IN_FILE)
