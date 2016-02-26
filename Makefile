CXX = g++
DEBUG = -g

GTEST_DIR ?= /tmp/no_gtest
INCLUDE_DIR = -I include -I lib/include -I $(GTEST_DIR)/include
COMPILE_DIR = bin
LIB_PATH = ./lib/lib
#GTEST_LIB = $(LIB_PATH)/libgtest_main.a $(LIB_PATH)/libgtest.a -lpthread
LIBS = -pthread
CXXFLAGS = $(DEBUG)

SOURCE_FILES = $(wildcard src/T*.cpp)
OBJ_FILES = $(SOURCE_FILES:.cpp=.o)
GTEST_OBJ_FILES = gtest_main.o gtest-all.o

all: test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIR) -c -o $@ $<

test: $(GTEST_OBJ_FILES) $(OBJ_FILES)
	$(CXX) $(INCLUDE_DIR) -L $(LIB_PATH) $(LIBS) $(DEBUG) -o $@ $^

test_multi_index: test_multi_index.cpp
	$(CXX) $(INCLUDE_DIR) -L $(LIB_PATH) -lboost_system -lboost_timer -lrt -O3 -o $@ $^

gtest-all.o: ${GTEST_DIR}/src/gtest-all.cc $(GTEST_DIR)
	$(CXX) -isystem ${GTEST_DIR}/include -I ${GTEST_DIR} -pthread -c $<

gtest_main.o: ${GTEST_DIR}/src/gtest_main.cc $(GTEST_DIR)
	$(CXX) -isystem ${GTEST_DIR}/include -I ${GTEST_DIR} -pthread -c $<

.PHONY: all clean

clean:
	rm -f test $(OBJ_FILES) $(GTEST_OBJ_FILES)
    
