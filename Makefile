CXX = g++
TARGET = carcassonne
TEST = unittest
CXXFLAGS = -Wall -std=c++11
SRCS = score_sheet.cpp tile_factory.cpp game_context.cpp region.cpp segment.cpp tile_position_map.cpp tile.cpp board.cpp game.cpp player.cpp
MAIN_SRCS = main.cpp $(SRCS)
TEST_SRCS = unit_tests.cpp test_util.cpp $(SRCS)
OBJS := $(MAIN_SRCS:.cpp=.o)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

GTEST_LIB_DIR = lib/googletest
GTEST_CPPFLAGS += -isystem $(GTEST_LIB_DIR)/include
GTEST_CXXFLAGS += -g -Wall -Wextra -pthread
GTEST_HEADERS = $(GTEST_LIB_DIR)/include/gtest/*.h \
                $(GTEST_LIB_DIR)/include/gtest/internal/*.h
GTEST_SRCS = $(GTEST_LIB_DIR)/src/*.cc $(GTEST_LIB_DIR)/src/*.h $(GTEST_HEADERS)

TEST_DIR = test

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

$(TEST): $(TEST_OBJS)
	$(CXX) -o $@ $(TEST_OBJS)
	./$@

# Builds libraries for Google Test
gtest-all.o : $(GTEST_SRCS)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_LIB_DIR) $(GTEST_CXXFLAGS) -c \
            $(GTEST_LIB_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_LIB_DIR) $(CXXFLAGS) -c \
            $(GTEST_LIB_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds tests using Google Test
sample_unittest.o : $(TEST_DIR)/sample_unittest.cpp $(GTEST_HEADERS)
	$(CXX) $(GTEST_CPPFLAGS) $(GTEST_CXXFLAGS) -c $(TEST_DIR)/sample_unittest.cpp

sample_unittest : sample_unittest.o gtest_main.a
	$(CXX) $(GTEST_CPPFLAGS) $(GTEST_CXXFLAGS) -lpthread $^ -o $@

clean:
	rm -f $(TARGET) $(TEST) $(OBJS) $(TEST_OBJS) gtest.a gtest_main.a *.o
