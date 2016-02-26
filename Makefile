CXX = g++
DEBUG = -g

GTEST_DIR ?= /tmp/no_gtest
BOOST_DIR ?= /tmp/no_boost
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

gtest-all.o: ${GTEST_DIR}/src/gtest-all.cc $(GTEST_DIR)
	$(CXX) -isystem ${GTEST_DIR}/include -I ${GTEST_DIR} -pthread -c $<

gtest_main.o: ${GTEST_DIR}/src/gtest_main.cc $(GTEST_DIR)
	$(CXX) -isystem ${GTEST_DIR}/include -I ${GTEST_DIR} -pthread -c $<

test_multi_index: src/test_multi_index.cpp $(BOOST_DIR)
	$(CXX) -O3 -I include -I $(BOOST_DIR)/include/ -o $@ -L $(BOOST_DIR)/lib $< -lpthread -lboost_system -lboost_date_time -lrt

.PHONY: all clean

clean:
	rm -f test $(OBJ_FILES) $(GTEST_OBJ_FILES)
    
