KQ_ROOT = /home/chris/Dev/kirpsquest
WARNING_FLAGS = -O0 -Wall -Werror
TEST_DIR = $(KQ_ROOT)/tests
TEST_INCLUDE_DIR = $(TEST_DIR)/include
TEST = kq_test

CXX = g++
CPPFLAGS = -g $(WARNING_FLAGS) -I$(TEST_INCLUDE_DIR) `sdl2-config --cflags` -std=c++11
LIBS = `sdl2-config --libs` -lSDL_image

all: test_main.o test.o
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $^ $(LIBS) -o $(TEST)

%.o: %.cpp $(TEST_INCLUDE_DIR)/catch.hpp
	$(CXX) -o $@ -c $< $(CPPFLAGS)

clean:
	rm -f *.o *~ $(TEST)