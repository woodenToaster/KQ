KQ_ROOT = /home/chris/Dev/kirpsquest
TEST_DIR = $(KQ_ROOT)/tests
SRC_DIR = $(KQ_ROOT)/src
INCLUDE_DIR = $(KQ_ROOT)/include
TEST = kq_test

WARNING_FLAGS = -O0 -Wall -Werror
EXEC = kirpsquest


CXX = g++
CPPFLAGS = -g $(WARNING_FLAGS) -I$(INCLUDE_DIR) `sdl2-config --cflags` -std=c++11
LIBS = `sdl2-config --libs` -lSDL_image 

all: $(SRC_DIR)/main.o
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $^ $(LIBS) -o $(EXEC)

main.o: $(SRC_DIR)/main.cpp
	$(CXX) -o $@ -c $< $(CPPFLAGS)

test:
	cd $(TEST_DIR); make && ./$(TEST)

clean:
	rm -f *.o *~ $(TEST_DIR)/test.o $(TEST_DIR)/*~ src/*.o src/*~ $(EXEC) $(TEST_DIR)/$(TEST)