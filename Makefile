CXX = g++
DEBUG = -g

INCLUDE_DIR = -I include -I lib/include
COMPILE_DIR = bin
LIB_PATH = ./lib/lib
GTEST_LIB = $(LIB_PATH)/libgtest_main.a -lgtest -lpthread
LIBS = $(GTEST_LIB)

all: test

TestCpp.o: src/TestCpp.cpp
	$(CXX) $(INCLUDE_DIR) $(DEBUG) -c -o $@ $<

TestBase64.o: src/TestBase64.cpp
	$(CXX) $(INCLUDE_DIR) $(DEBUG) -c -o $@ $<

test: TestCpp.o TestBase64.o
	$(CXX) $(INCLUDE_DIR) -L $(LIB_PATH) $(LIBS) $(DEBUG) -o $@ $^

.PHONY: all clean

clean:
	rm -f test *.o
    
