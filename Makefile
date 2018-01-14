CXX := g++
TARGET := carcassonne
TEST_TARGET := tests
CXXFLAGS := -Wall -std=c++11

SRCS := score_sheet.cpp tile_factory.cpp game_context.cpp region.cpp segment.cpp tile_position_map.cpp tile.cpp board.cpp game.cpp player.cpp
OBJS := $(SRCS:.cpp=.o)
HEADERS := ./*.hpp

GTEST_LIB_DIR := lib/googletest
GTEST_CPPFLAGS := -isystem $(GTEST_LIB_DIR)/include
GTEST_CXXFLAGS := -g -Wall -Wextra -pthread -std=c++11
GTEST_HEADERS := $(GTEST_LIB_DIR)/include/gtest/*.h \
                $(GTEST_LIB_DIR)/include/gtest/internal/*.h
GTEST_SRCS := $(GTEST_LIB_DIR)/src/*.cc $(GTEST_LIB_DIR)/src/*.h $(GTEST_HEADERS)

TEST_DIR := test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(notdir $(TEST_SRCS:.cpp=.o))

$(TARGET): main.o $(OBJS)
	$(CXX) -o $@ $^

# Builds libraries for Google Test
gtest-all.o: $(GTEST_SRCS)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_LIB_DIR) $(GTEST_CXXFLAGS) -c \
            $(GTEST_LIB_DIR)/src/gtest-all.cc

gtest_main.o: $(GTEST_SRCS)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_LIB_DIR) $(CXXFLAGS) -c \
            $(GTEST_LIB_DIR)/src/gtest_main.cc

gtest.a: gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds tests written with Google Test
test_objs: $(TEST_SRCS) $(HEADERS) $(GTEST_HEADES)
	@for test_src in $(TEST_SRCS); do\
	    $(CXX) $(GTEST_CPPFLAGS) -isystem ./ $(GTEST_CXXFLAGS) -c $$test_src || exit 1;\
	done

$(TEST_TARGET): test_objs $(OBJS) gtest_main.a
	$(CXX) $(GTEST_CPPFLAGS) $(GTEST_CXXFLAGS) -lpthread $(OBJS) $(TEST_OBJS) gtest_main.a -o $@
	./$@

clean:
	rm -f $(TARGET) tests *.a *.o
