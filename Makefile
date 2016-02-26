CXX = g++
DEBUG = -g

INCLUDE_DIR = -I include -I lib/include
COMPILE_DIR = bin
LIB_PATH = ./lib/lib
GTEST_LIB = $(LIB_PATH)/libgtest_main.a $(LIB_PATH)/libgtest.a -lpthread
LIBS = $(GTEST_LIB)
CXXFLAGS = $(DEBUG)

SOURCE_FILES = $(wildcard src/T*.cpp)
OBJ_FILES = $(SOURCE_FILES:.cpp=.o)

all: test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c -o $@ $<

test: $(OBJ_FILES)
	$(CXX) $(INCLUDE_DIR) -L $(LIB_PATH) $(LIBS) $(DEBUG) -o $@ $^

test_multi_index: test_multi_index.cpp
	$(CXX) $(INCLUDE_DIR) -L $(LIB_PATH) -lboost_system -lboost_timer -lrt -O3 -o $@ $^

.PHONY: all clean

clean:
	rm -f test $(OBJ_FILES)
    
