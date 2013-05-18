CXX = g++
DEBUG = -g

INCLUDE_DIR = -I include -I lib/include
COMPILE_DIR = bin
LIB_PATH = ./lib/lib
GTEST_LIB = $(LIB_PATH)/libgtest_main.a $(LIB_PATH)/libgtest.a -lpthread
LIBS = $(GTEST_LIB)
CXXFLAGS = $(DEBUG)

SOURCE_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(SOURCE_FILES:.cpp=.o)

all: test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c -o $@ $<

test: $(OBJ_FILES)
	$(CXX) $(INCLUDE_DIR) -L $(LIB_PATH) $(LIBS) $(DEBUG) -o $@ $^

.PHONY: all clean

clean:
	rm -f test $(OBJ_FILES)
    
